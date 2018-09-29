/// @file PixieInterface.cpp
/// @brief
/// @author S. V. Paulauskas, D. Miller, C. R. Thornsberry, K. Smith
/// @date January 2010
#include <PixieInterface.h>

#include <pixie16app_export.h>
#include <Display.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <cstdlib>
#include <cstring>

#include <sys/time.h>

using namespace std;
using namespace Display;

PixieInterface::PixieInterface(const char *fn) : AcquisitionInterface(fn) {
    //Overwrite the default path 'pxisys.ini' with the one specified in the scan file.
    PCISysIniFile = config_.Get("global", "CrateConfig").c_str();
}

PixieInterface::~PixieInterface() {
    if (!doneInit_)
        return;

    if (CheckRunStatus())
        EndRun();

    LeaderPrint("Closing Pixie interface");

    retval_ = Pixie16ExitSystem(config_.GetNumberOfModules());
    CheckError();
}

bool PixieInterface::AcquireTraces(int mod) {
    retval_ = Pixie16AcquireADCTrace(mod);

    if (retval_ < 0) {
        cout << ErrorStr("Error acquiring ADC traces from module ") << mod << endl;
        return false;
    }

    return true;
}

bool PixieInterface::AdjustOffsets(unsigned short mod) {
    LeaderPrint("Adjusting Offsets");
    retval_ = Pixie16AdjustOffsets(mod);

    return !CheckError();
}

bool PixieInterface::Boot(int mode, bool useWorkingSetFile) {
    string setFile;
    if(useWorkingSetFile)
        setFile = config_.Get("global", "DspWorkingSetFile");
    else
        setFile = config_.Get("global", "DspSetFile");

    LeaderPrint("Boot Configuration");

    //Loop through each module and determine its type.
    //We also check if the modules are all the same. If not we set multiConf to true.
    bool multiConf = false;
    std::vector<std::string> moduleTypes;
    for (int mod = 0; mod < config_.GetNumberOfModules(); mod++) {
        unsigned short rev, adcBits, adcMsps;
        unsigned int serNum;
        GetModuleInfo(mod, &rev, &serNum, &adcBits, &adcMsps);

        stringstream moduleType;
        moduleType << adcBits << "b" << adcMsps << "m";
        moduleType << "-rev" << (char) (97 + rev - 10);

        if (mod > 0 && moduleType.str() != moduleTypes.back()) multiConf = true;
        moduleTypes.push_back(moduleType.str());
    }

    bool goodBoot = true;

    if (multiConf) {
        // must proceed through boot module by module

        //Break the leader print for the boot configuration status.
        cout << InfoStr("[MULTI]") << "\n";

        //Check that all module types are valid.
        bool error = false;
        for (int mod = 0; mod < config_.GetNumberOfModules(); mod++) {
            if ( !config_.HasModuleConfig(moduleTypes.at(mod)) ) {
                std::cout << ErrorStr() << " Configuration not defined for type " << moduleTypes.at(mod) << " (mod "
                          << mod << ")\n";
                error = true;
            }
        }
        if (error) return false;

        for (int i = 0; i < config_.GetNumberOfModules(); i++) {
            retval_ = Pixie16BootModule(
                    &config_.Get(moduleTypes.at(i), "ComFpgaFile")[0],
                    &config_.Get(moduleTypes.at(i), "SpFpgaFile")[0],
                    &config_.Get(moduleTypes.at(i), "TrigFpgaFile")[0],
                    &config_.Get(moduleTypes.at(i), "DspConfFile")[0],
                    &setFile[0],
                    &config_.Get(moduleTypes.at(i), "DspVarFile")[0],
                    i, mode);

            stringstream leader;
            leader << "Booting Pixie (" << moduleTypes.at(i) << ") Module " << i;
            LeaderPrint(leader.str());
            goodBoot = (goodBoot && !CheckError(true));
        }
    } else {
        //Break the leader print for the boot configuration status.
        cout << InfoStr("[SINGLE]") << "\n";

        //Determine if we need to use "default" type.
        string moduleType = moduleTypes.front();
        if (!config_.HasModuleConfig(moduleType)) {
            moduleType = "default";
            if (!config_.HasModuleConfig(moduleType)) {
                std::cout << ErrorStr() << " Config not defined for type " << moduleTypes.back() << "\n";
                return false;
            }
        }

        // boot all at once
        retval_ = Pixie16BootModule(&config_.Get(moduleType, "ComFpgaFile")[0], &config_.Get(moduleType, "SpFpgaFile")[0],
                                    &config_.Get(moduleType, "TrigFpgaFile")[0], &config_.Get(moduleType, "DspConfFile")[0],
                                    &setFile[0], &config_.Get(moduleType, "DspVarFile")[0], config_.GetNumberOfModules(), mode);

        stringstream leader;
        leader << "Booting Pixie (" << moduleType << ")";
        LeaderPrint(leader.str());
        goodBoot = !CheckError(true);
    }

    cout << "  Used set file: " << InfoStr(setFile) << endl;

    LeaderPrint("Checking SlotIDs");

    bool hadError = false;
    bool updated = false;

    unsigned int val;

    for (int i = 0; i < config_.GetNumberOfModules(); i++) {
        if (!ReadSglModPar("SlotID", val, i))
            hadError = true;
        if (val != (unsigned int)config_.GetSlotMaps()[0][i]) {
            updated = true;
            if (!WriteSglModPar("SlotID", config_.GetSlotMaps()[0][i], i))
                hadError = true;
        }
    }

    if (hadError)
        cout << ErrorStr() << endl;
    else if (updated)
        cout << WarningStr("[UPDATED]") << endl;
    else
        cout << OkayStr() << endl;

    return goodBoot && !hadError;
}

///@TODO This needs to throw not exit
bool PixieInterface::CheckError(bool exitOnError) const {
    if (StatusPrint(retval_ < 0) && exitOnError)
        exit(EXIT_FAILURE); // or do something else

    return (retval_ < 0);
}

bool PixieInterface::CheckRunStatus(short mod) {
    bool error = false;
    if (mod >= 0)
        error = Pixie16CheckRunStatus(mod);
    else {
        for (mod = 0; mod < config_.GetNumberOfModules(); mod++) {
            if (!Pixie16CheckRunStatus(mod)) {
                error = true;
                break;
            }
        }
    }

    if (error)
        cout << WarningStr("Error checking run status in module ") << mod;

    return error;
}

bool PixieInterface::EndRun(short mod) {
    bool b = true;

    LeaderPrint("Ending run");

    if (mod >= 0)
        b = Pixie16EndRun(mod);
    else {
        for (mod = 0; mod < config_.GetNumberOfModules(); mod++)
            if (!EndRun(mod)) b = false;
    }

    if (!b)
        cout << ErrorStr() << endl << WarningStr("Failed to end run in module ") << mod << endl;
    else
        cout << OkayStr() << endl;

    return b;
}

double PixieInterface::GetInputCountRate(int mod, int chan) {
    return Pixie16ComputeInputCountRate(statistics_, mod, chan);
}

double PixieInterface::GetLiveTime(int mod, int chan) {
    return Pixie16ComputeLiveTime(statistics_, mod, chan);
}

bool PixieInterface::GetModuleInfo(const unsigned short &mod, unsigned short *rev, unsigned int *serNum,
                                   unsigned short *adcBits, unsigned short *adcMsps) {
    //Return false if error code provided.
    return (Pixie16ReadModuleInfo(mod, rev, serNum, adcBits, adcMsps) == 0);
}

double PixieInterface::GetOutputCountRate(int mod, int chan) {
    return Pixie16ComputeOutputCountRate(statistics_, mod, chan);
}

double PixieInterface::GetProcessedEvents(int mod) {
    return Pixie16ComputeProcessedEvents(statistics_, mod);
}

double PixieInterface::GetRealTime(int mod) {
    return Pixie16ComputeRealTime(statistics_, mod);
}

bool PixieInterface::GetStatistics(unsigned short mod) {
    retval_ = Pixie16ReadStatisticsFromModule(statistics_, mod);

    if (retval_ < 0) {
        cout << WarningStr("Error reading statistics from module ") << mod
             << endl;
        return false;
    }

    return true;
}

bool PixieInterface::Init() {
    LeaderPrint("Initializing Pixie");

    retval_ = Pixie16InitSystem(config_.GetNumberOfModules(), &(config_.GetSlotMapAsVector(0)[0]), false);
    doneInit_ = !CheckError(true);

    return doneInit_;
}

void PixieInterface::PrintSglChanPar(const char *name, int mod, int chan, double *prev) {
    double val;
    strncpy(tmpName_, name, nameSize_);

    if (ReadSglChanPar(tmpName_, val, mod, chan)) {
        cout.unsetf(ios_base::floatfield);
        cout << "  MOD " << setw(2) << mod << "  CHAN " << setw(2) << chan
             << "  " << setw(15) << name << " ";
        if (prev) {
            cout << setprecision(8) << *prev << " -> ";
        }
        cout << setprecision(8) << val << endl;
    }
}

void PixieInterface::PrintSglModPar(const char *name, int mod, Pixie16::word_t *prev) {
    Pixie16::word_t val;

    strncpy(tmpName_, name, nameSize_);

    if (ReadSglModPar(tmpName_, val, mod)) {
        cout.unsetf(ios_base::floatfield);
        cout << "  MOD " << setw(2) << mod << "  " << setw(15) << name << "  ";
        if (prev)
            cout << setprecision(6) << *prev << " -> ";
        cout << setprecision(6) << val << endl;
    }
}

bool PixieInterface::ReadHistogram(Pixie16::word_t *hist, unsigned long sz,
                                   unsigned short mod, unsigned short ch) {
    if (sz > MAX_HISTOGRAM_LENGTH) {
        cout << ErrorStr("Histogram length is too large.") << endl;
        return false;
    }

    retval_ = Pixie16ReadHistogramFromModule(hist, sz, mod, ch);

    if (retval_ < 0) {
        cout << ErrorStr("Failed to get histogram data from module ") << mod
             << endl;
        return false;
    }
    return true;
}

bool PixieInterface::ReadSglChanPar(const char *name, double &pval, int mod, int chan) {
    strncpy(tmpName_, name, nameSize_);

    retval_ = Pixie16ReadSglChanPar(tmpName_, &pval, mod, chan);
    if (retval_ < 0) {
        cout << "Error reading channel parameter " << WarningStr(name)
             << " for module " << mod << ", channel " << chan << endl;
        return false;
    }
    return true;
}

bool PixieInterface::ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan) {
    if (sz > TRACE_LENGTH) {
        cout << ErrorStr("Trace length too large.") << endl;
        return false;
    }

    retval_ = Pixie16ReadSglChanADCTrace(buf, sz, mod, chan);

    if (retval_ < 0) {
        cout << ErrorStr("Error reading trace in module ") << mod << endl;
        return false;
    }

    return true;
}

bool PixieInterface::ReadSglModPar(const char *name, Pixie16::word_t &val, int mod) {
    strncpy(tmpName_, name, nameSize_);

    retval_ = Pixie16ReadSglModPar(tmpName_, &val, mod);
    if (retval_ < 0) {
        cout << "Error reading module parameter " << WarningStr(name) << " for module " << mod << endl;
        return false;
    }
    return true;
}

bool PixieInterface::RemovePresetRunLength(int mod) {
    strncpy(tmpName_, "HOST_RT_PRESET", nameSize_);

    unsigned long bigVal = Decimal2IEEEFloating(99999);

    LeaderPrint("Removing preset run length");

    if (!WriteSglModPar(tmpName_, bigVal, mod)) {
        cout << ErrorStr() << endl;
        return false;
    }
    cout << OkayStr() << endl;
    return true;
}

bool PixieInterface::SaveDSPParameters(const char *fn) {
    if (fn == NULL)
        fn = &config_.Get("global", "DspWorkingSetFile")[0];
    strncpy(tmpName_, fn, nameSize_);

    LeaderPrint("Writing DSP parameters");

    retval_ = Pixie16SaveDSPParametersToFile(tmpName_);
    return !CheckError();
}

bool PixieInterface::StartHistogramRun(short mod, unsigned short mode) {
    LeaderPrint("Starting histogram run");
    if (mod < 0) mod = config_.GetNumberOfModules();
    retval_= Pixie16StartHistogramRun(mod, mode);

    return !CheckError();
}

bool PixieInterface::StartListModeRun(short mod, unsigned short listMode, unsigned short runMode) {
    LeaderPrint("Starting list mode run");
    if (mod < 0) mod = config_.GetNumberOfModules();
    retval_ = Pixie16StartListModeRun(mod, listMode, runMode);

    return !CheckError();
}

bool PixieInterface::ToggleChannelBit(int mod, int chan, const char *parameter, int bit) {
    double dval;

    ReadSglChanPar(parameter, dval, mod, chan);
    unsigned int ival = (int) dval;
    ival ^= (1 << bit);
    dval = ival;

    return WriteSglChanPar(parameter, dval, mod, chan);
}

bool PixieInterface::ToggleGain(int mod, int chan) {
    return ToggleChannelBit(mod, chan, "CHANNEL_CSRA", CCSRA_ENARELAY);
}

bool PixieInterface::ToggleGood(int mod, int chan) {
    return ToggleChannelBit(mod, chan, "CHANNEL_CSRA", CCSRA_GOOD);
}

bool PixieInterface::TogglePolarity(int mod, int chan) {
    return ToggleChannelBit(mod, chan, "CHANNEL_CSRA", CCSRA_POLARITY);
}

bool PixieInterface::WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval) {
    strncpy(tmpName_, name, nameSize_);

    if (!pval)
        Pixie16ReadSglChanPar(tmpName_, pval, mod, chan);
    retval_ = Pixie16WriteSglChanPar(tmpName_, val, mod, chan);
    if (retval_ < 0) {
        cout << "Error writing channel parameter " << WarningStr(name) << " for module " << mod << ", channel "
             << chan << endl;
        return false;
    }
    return true;
}

bool PixieInterface::WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval) {
    strncpy(tmpName_, name, nameSize_);

    if (!pval)
        Pixie16ReadSglModPar(tmpName_, pval, mod);
    retval_ = Pixie16WriteSglModPar(tmpName_, val, mod);
    if (retval_ < 0) {
        cout << "Error writing module parameter " << WarningStr(name) << " for module " << mod << endl;
        return false;
    }
    return true;
}

// only Rev. D has the external FIFO
#ifdef PIF_FIFO

unsigned long PixieInterface::CheckFIFOWords(unsigned short mod) {
    unsigned int nWords;

    retval_ = Pixie16CheckExternalFIFOStatus(&nWords, mod);

    if (retval_ < 0) {
        cout << WarningStr("Error checking FIFO status in module ") << mod << endl;
        return 0;
    }

    return nWords + extraWords_[mod].size();
}

bool PixieInterface::ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose) {
    unsigned long availWords = CheckFIFOWords(mod);

    if (verbose) {
        std::cout << "mod " << mod << " nWords " << nWords;
        std::cout << " extraWords[mod].size " << extraWords_[mod].size();
    }

    if (nWords < MIN_FIFO_READ + extraWords_[mod].size()) {
        if (nWords > extraWords_[mod].size()) {
            Pixie16::word_t minibuf[MIN_FIFO_READ];

            if (availWords < MIN_FIFO_READ) {
                std::cout << Display::ErrorStr() << " Not enough words available in module " << mod
                          << "'s FIFO for read! (" << availWords << "/" << MIN_FIFO_READ << ")\n";
                return false;
            }
            retval_ = Pixie16ReadDataFromExternalFIFO(minibuf, MIN_FIFO_READ, mod);

            if (retval_ < 0) {
                cout << WarningStr("Error reading words from FIFO in module ") << mod << " retVal " << retval_ << endl;
                return false;
            }
            for (int i = 0; i < MIN_FIFO_READ; i++) extraWords_[mod].push(minibuf[i]);
        }
    }

    if (verbose) std::cout << " " << extraWords_[mod].size();

    size_t wordsAdded;
    for (wordsAdded = 0; wordsAdded < nWords && !extraWords_[mod].empty(); ++wordsAdded) {
        *buf++ = extraWords_[mod].front();
        extraWords_[mod].pop();
    }

    if (verbose) std::cout << " " << extraWords_[mod].size();

    if (nWords <= wordsAdded) {
        std::cout << std::endl;
        return true;
    }
    nWords -= wordsAdded;

    if (verbose) std::cout << " nWords " << nWords << std::endl;

    if (availWords < nWords) {
        std::cout << Display::ErrorStr() << " Not enough words available in module " << mod << "'s FIFO for read! ("
                  << availWords << "/" << nWords << ")\n";
        return false;
    }
    retval_ = Pixie16ReadDataFromExternalFIFO(buf, nWords, mod);

    if (retval_ < 0) {
        cout << WarningStr("Error reading words from FIFO in module ") << mod << " retVal " << retval_ << endl;
        return false;
    }

    return true;
}
#endif // Rev. D FIFO access

#ifdef PIF_CATCHER
bool PixieInterface::ToggleCatcherBit(int mod, int chan)
{
  CatcherMessage();
  return ToggleChannelBit(mod, chan, "CHANNEL_CSRA", CCSRA_CATCHER);
}

bool PixieInterface::TogglePileupBit(int mod, int chan)
{
  return ToggleChannelBit(mod, chan, "CHANNEL_CSRA", CCSRA_PILEUP);
}

bool PixieInterface::ToggleTraceCapture(int mod, int chan)
{
  return ToggleChannelBit(mod, chan, "CHANNEL_CSRA", CCSRA_TRACEENA);
}

bool PixieInterface::SetProtonCatcherMode(int mod, int chan, CatcherModes mode)
{
  double dval;

  CatcherMessage();

  ReadSglChanPar("CHANNEL_CSRA", &dval, mod, chan);
  unsigned int ival = (int)dval;

  switch (mode) {
  case PC_STANDARD:
  case PC_HYBRID:
    ival = APP32_ClrBit(CCSRA_PILEUP, ival);
    break;
  case PC_REJECT:
  case PC_ACCEPT:
    ival = APP32_SetBit(CCSRA_PILEUP, ival);
    break;
  }
  switch (mode) {
  case PC_STANDARD:
  case PC_REJECT:
    ival = APP32_ClrBit(CCSRA_CATCHER, ival);
    break;
  case PC_HYBRID:
  case PC_ACCEPT:
    ival = APP32_SetBit(CCSRA_CATCHER, ival);
    break;
  }
  dval = ival;

  return WriteSglChanPar("CHANNEL_CSRA", dval, mod, chan);
}

void PixieInterface::CatcherMessage(void)
{
  // Only do this message once per program execution
  static bool threwMessage = false;

  if (false)
    cout << WarningStr("Altering firmware dependent bit") << endl;
  threwMessage = true;
}
#endif