///@authors D. Miller, C. R. Thornsberry, K. Smith
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <cstdlib>
#include <cstring>

#include <sys/time.h>

#include "pixie16app_export.h"

#include "Display.h"
#include "PixieInterface.h"

using namespace std;
using namespace Display;


set <string> PixieInterface::validConfigKeys;

// some simple histogram functions
PixieInterface::Histogram::Histogram() : error(NO_ERROR) {
    bzero(data, sizeof(data));
}

PixieInterface::Histogram::Histogram(const PixieInterface::Histogram &x) {
    memcpy(data, x.data, sizeof(data));
}

const PixieInterface::Histogram &
PixieInterface::Histogram::operator=(const PixieInterface::Histogram &right) {
    memcpy(data, right.data, sizeof(data));

    return *this;
}

PixieInterface::Histogram
PixieInterface::Histogram::operator+(const PixieInterface::Histogram &right) {
    Histogram x(*this);

    for (size_t i = 0; i < HISTO_SIZE; i++) {
        x.data[i] += right.data[i];
    }

    return x;
}

PixieInterface::Histogram
PixieInterface::Histogram::operator-(const PixieInterface::Histogram &right) {
    Histogram x(*this);

    for (size_t i = 0; i < HISTO_SIZE; i++) {
        if (x.data[i] < right.data[i]) {
            x.data[i] = 0;
            error = ERROR_SUBTRACT;
        } else {
            x.data[i] -= right.data[i];
        }
    }

    return x;
}

const PixieInterface::Histogram &
PixieInterface::Histogram::operator+=(const PixieInterface::Histogram &right) {
    return (*this = *this + right);
}

const PixieInterface::Histogram &
PixieInterface::Histogram::operator-=(const PixieInterface::Histogram &right) {
    return (*this = *this - right);
}

bool PixieInterface::Histogram::Read(PixieInterface &pif, unsigned int mod,
                                     unsigned int ch) {
    if (pif.ReadHistogram(data, PixieInterface::HISTO_SIZE, mod, ch))
        return true;
    error = ERROR_READ;
    return false;
}

bool PixieInterface::Histogram::Write(ofstream &out) {
    out.write((char *) data, sizeof(data));

    if (!out.good()) {
        error = ERROR_WRITE;
        return false;
    }
    return true;
}

PixieInterface::PixieInterface(const char *fn) :
    AcqInterface(fn)
{
    //Overwrite the default path 'pxisys.ini' with the one specified in the scan file.
    PCISysIniFile = config.Get("global", "CrateConfig").c_str();
}

PixieInterface::~PixieInterface() {
    if (!doneInit)
        return;

    if (CheckRunStatus())
        EndRun();

    LeaderPrint("Closing Pixie interface");

    retval = Pixie16ExitSystem(numberCards);
    CheckError();
}


bool PixieInterface::Init(bool offlineMode) {
    LeaderPrint("Initializing Pixie");

    retval = Pixie16InitSystem(numberCards, slotMap, offlineMode);
    doneInit = !CheckError(true);

    return doneInit;
}

bool PixieInterface::Boot(BootType mode, bool useWorkingSetFile) {
    switch(mode) {
        case BootType::MCA:
            Boot(PixieInterface::DownloadParameters |
                 PixieInterface::ProgramFPGA |
                 PixieInterface::SetDAC, true);
            break;
    }
}
bool PixieInterface::Boot(int mode, bool useWorkingSetFile) {
    string &setFile = useWorkingSetFile ?
                      config.Get("global", "DspWorkingSetFile")
                                        : config.Get("global", "DspSetFile");

    LeaderPrint("Boot Configuration");

    //Loop through each module and determine its type.
    //We also check if the modules are all the same. If not we set multiConf to true.
    bool multiConf = false;
    std::vector<std::string> moduleTypes;
    for (int mod = 0; mod < numberCards; mod++) {
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
        for (int mod = 0; mod < numberCards; mod++) {
            if (configStrings.find(moduleTypes.at(mod)) ==
                configStrings.end()) {
                std::cout << ErrorStr()
                          << " Configuration not defined for type "
                          << moduleTypes.at(mod) << " (mod " << mod << ")\n";
                error = true;
            }
        }
        if (error) return false;

        for (int i = 0; i < numberCards; i++) {
            retval = Pixie16BootModule(
                    &config.Get(moduleTypes.at(i), "ComFpgaFile")[0],
                    &config.Get(moduleTypes.at(i), "SpFpgaFile")[0],
                    &config.Get(moduleTypes.at(i), "TrigFpgaFile")[0],
                    &config.Get(moduleTypes.at(i), "DspConfFile")[0],
                    &setFile[0],
                    &config.Get(moduleTypes.at(i), "DspVarFile")[0],
                    i, mode);

            stringstream leader;
            leader << "Booting Pixie (" << moduleTypes.at(i) << ") Module "
                   << i;
            LeaderPrint(leader.str());
            goodBoot = (goodBoot && !CheckError(true));
        }
    } else {
        //Break the leader print for the boot configuration status.
        cout << InfoStr("[SINGLE]") << "\n";

        //Determine if we need to use "default" type.
        string moduleType = moduleTypes.front();
        if (configStrings.find(moduleType) == configStrings.end()) {
            moduleType = "default";
            if (configStrings.find(moduleType) == configStrings.end()) {
                std::cout << ErrorStr() << " Config not defined for type "
                          << moduleTypes.back() << "\n";
                return false;
            }
        }
        //std::cout << "Booting all modules as type " << InfoStr(moduleType) << "\n";

        // boot all at once
        retval = Pixie16BootModule(&config.Get(moduleType, "ComFpgaFile")[0],
                                   &config.Get(moduleType, "SpFpgaFile")[0],
                                   &config.Get(moduleType, "TrigFpgaFile")[0],
                                   &config.Get(moduleType, "DspConfFile")[0],
                                   &setFile[0],
                                   &config.Get(moduleType, "DspVarFile")[0],
                                   numberCards, mode);

        stringstream leader;
        leader << "Booting Pixie (" << moduleType << ")";
        LeaderPrint(leader.str());
        goodBoot = !CheckError(true);
    }

    cout << "  Used set file: " << InfoStr(setFile) << endl;

    LeaderPrint("Checking SlotIDs");

    bool hadError = false;
    bool updated = false;

    word_t val;

    for (int i = 0; i < numberCards; i++) {
        if (!ReadSglModPar("SlotID", val, i))
            hadError = true;
        if (val != slotMap[i]) {
            updated = true;
            if (!WriteSglModPar("SlotID", slotMap[i], i))
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

bool PixieInterface::WriteSglModPar(const char *name, word_t val, int mod,
                                    word_t &pval = nullptr) {

    strncpy(tmpName, name, nameSize);

    if (!pval) {
        Pixie16ReadSglModPar(tmpName, &pval, mod);
    }
    retval = Pixie16WriteSglModPar(tmpName, val, mod);
    if (retval < 0) {
        cout << "Error writing module parameter " << WarningStr(name)
             << " for module " << mod << endl;
        return false;
    }
    return true;
}

bool PixieInterface::ReadSglModPar(const char *name, word_t &val, int mod) {
    strncpy(tmpName, name, nameSize);

    retval = Pixie16ReadSglModPar(tmpName, &val, mod);
    if (retval < 0) {
        cout << "Error reading module parameter " << WarningStr(name)
             << " for module " << mod << endl;
        return false;
    }
    return true;
}

void PixieInterface::PrintSglModPar(const char *name, int mod, word_t *prev=nullptr) {
    word_t val;

    strncpy(tmpName, name, nameSize);

    if (ReadSglModPar(tmpName, val, mod)) {
        cout.unsetf(ios_base::floatfield);
        cout << "  MOD " << setw(2) << mod << "  " 
             << setw(15) << name << "  "
             << setprecision(6);
        if (prev) {
            cout << setprecision(6) << prev << " -> ";
        }
        cout << val << endl;
    }
}

bool PixieInterface::WriteSglChanPar(const char *name, double val, int mod,
                                     int chan) {
    double dummy;
    return WriteSglChanPar(name, val, mod, chan, dummy);
}

bool
PixieInterface::WriteSglChanPar(const char *name, double val, int mod, int chan,
                                double &pval) {
    strncpy(tmpName, name, nameSize);

    Pixie16ReadSglChanPar(tmpName, &pval, mod, chan);
    retval = Pixie16WriteSglChanPar(tmpName, val, mod, chan);
    if (retval < 0) {
        cout << "Error writing channel parameter " << WarningStr(name)
             << " for module " << mod << ", channel " << chan << endl;
        return false;
    }
    return true;
}

bool PixieInterface::ReadSglChanPar(const char *name, double &pval, int mod,
                                    int chan) {
    strncpy(tmpName, name, nameSize);

    retval = Pixie16ReadSglChanPar(tmpName, &pval, mod, chan);
    if (retval < 0) {
        cout << "Error reading channel parameter " << WarningStr(name)
             << " for module " << mod << ", channel " << chan << endl;
        return false;
    }
    return true;
}

void PixieInterface::PrintSglChanPar(const char *name, int mod, int chan) {
    double val;
    strncpy(tmpName, name, nameSize);

    if (ReadSglChanPar(tmpName, val, mod, chan)) {
        cout.unsetf(ios_base::floatfield);
        cout << "  MOD " << setw(2) << mod << "  CHAN " << setw(2) << chan
             << "  " << setw(15) << name << "  " << setprecision(6) << val
             << endl;
    }
}

void PixieInterface::PrintSglChanPar(const char *name, int mod, int chan,
                                     double prev) {
    double val;
    strncpy(tmpName, name, nameSize);

    if (ReadSglChanPar(tmpName, val, mod, chan)) {
        cout.unsetf(ios_base::floatfield);
        cout << "  MOD " << setw(2) << mod << "  CHAN " << setw(2) << chan
             << "  " << setw(15) << name << "  " << setprecision(6) << prev
             << " -> " << val << endl;
    }
}

bool PixieInterface::SaveDSPParameters(const char *fn) {
    if (fn == NULL)
        fn = &config.Get("global", "DspWorkingSetFile")[0];
    strncpy(tmpName, fn, nameSize);

    LeaderPrint("Writing DSP parameters");

    retval = Pixie16SaveDSPParametersToFile(tmpName);
    return !CheckError();
}

bool PixieInterface::AcquireTraces(int mod) {
    retval = Pixie16AcquireADCTrace(mod);

    if (retval < 0) {
        cout << ErrorStr("Error acquiring ADC traces from module ") << mod
             << endl;
        return false;
    }

    return true;
}

bool PixieInterface::ReadSglChanTrace(unsigned short *buf, unsigned long sz,
                                      unsigned short mod, unsigned short chan) {
    if (sz > TRACE_LENGTH) {
        cout << ErrorStr("Trace length too large.") << endl;
        return false;
    }

    retval = Pixie16ReadSglChanADCTrace(buf, sz, mod, chan);

    if (retval < 0) {
        cout << ErrorStr("Error reading trace in module ") << mod << endl;
        return false;
    }

    return true;
}

bool PixieInterface::GetStatistics(unsigned short mod) {
    retval = Pixie16ReadStatisticsFromModule(statistics, mod);

    if (retval < 0) {
        cout << WarningStr("Error reading statistics from module ") << mod
             << endl;
        return false;
    }

    return true;
}

double PixieInterface::GetInputCountRate(int mod, int chan) {
    return Pixie16ComputeInputCountRate(statistics, mod, chan);
}

double PixieInterface::GetOutputCountRate(int mod, int chan) {
    return Pixie16ComputeOutputCountRate(statistics, mod, chan);
}

double PixieInterface::GetLiveTime(int mod, int chan) {
    return Pixie16ComputeLiveTime(statistics, mod, chan);
}

double PixieInterface::GetRealTime(int mod) {
    return Pixie16ComputeRealTime(statistics, mod);
}

double PixieInterface::GetProcessedEvents(int mod) {
    return Pixie16ComputeProcessedEvents(statistics, mod);
}

bool PixieInterface::StartHistogramRun(unsigned short mode) {
    LeaderPrint("Starting histogram run");
    retval = Pixie16StartHistogramRun(numberCards, mode);

    return !CheckError();
}

bool
PixieInterface::StartHistogramRun(unsigned short mod, unsigned short mode) {
    retval = Pixie16StartHistogramRun(mod, mode);

    if (retval < 0) {
        cout << ErrorStr("Error starting histogram run in module ") << mod
             << endl;
        exit(EXIT_FAILURE);
    }
    return true;
}

bool PixieInterface::StartListModeRun(unsigned short listMode,
                                      unsigned short runMode) {
    LeaderPrint("Starting list mode run");
    retval = Pixie16StartListModeRun(numberCards, listMode, runMode);

    return !CheckError();
}

bool PixieInterface::StartListModeRun(unsigned short mod,
                                      unsigned short listMode,
                                      unsigned short runMode) {
    retval = Pixie16StartListModeRun(mod, listMode, runMode);

    if (retval < 0) {
        cout << ErrorStr("Error starting list mode run in module ") << mod
             << endl;
        exit(EXIT_FAILURE);
    }

    return true;
}

bool PixieInterface::CheckRunStatus() {
    for (int mod = 0; mod < numberCards; mod++) {
        if (!CheckRunStatus(mod))
            return false;
    }

    return true;
}

bool PixieInterface::CheckRunStatus(int mod) {
    retval = Pixie16CheckRunStatus(mod);

    if (retval < 0) {
        cout << WarningStr("Error checking run status in module ") << mod
             << endl;
    }

    return (retval == 1);
}


// only Rev. D has the external FIFO
#ifdef PIF_FIFO
unsigned long PixieInterface::CheckFIFOWords(unsigned short mod)
{
  // word_t nWords;
  unsigned int nWords;

  retval = Pixie16CheckExternalFIFOStatus(&nWords, mod);

  if (retval < 0) {
    cout << WarningStr("Error checking FIFO status in module ") << mod << endl;
    return 0;
  }
 
    return nWords + extraWords[mod].size();
}

bool PixieInterface::ReadFIFOWords(word_t *buf, unsigned long nWords,
                   unsigned short mod, bool verbose)
{
    unsigned long availWords = CheckFIFOWords(mod);

    if (verbose) {
        std::cout << "mod " << mod << " nWords " << nWords;
        std::cout << " extraWords[mod].size " << extraWords[mod].size();
    }
    if (nWords < MIN_FIFO_READ + extraWords[mod].size()) {
        if (nWords > extraWords[mod].size()) {
            word_t minibuf[MIN_FIFO_READ];

            if (availWords < MIN_FIFO_READ) {
                std::cout << Display::ErrorStr() << " Not enough words available in module " << mod << "'s FIFO for read! (" << availWords << "/" << MIN_FIFO_READ << ")\n";
                return false;
            }
            retval = Pixie16ReadDataFromExternalFIFO(minibuf, MIN_FIFO_READ, mod);

            if (retval < 0) {
                cout << WarningStr("Error reading words from FIFO in module ") << mod << " retVal " << retval << endl;
                return false;
            }
            for (int i=0;i<MIN_FIFO_READ;i++) extraWords[mod].push(minibuf[i]);
        }
    }
    if (verbose) std::cout << " " << extraWords[mod].size();

    size_t wordsAdded;
    for (wordsAdded = 0;wordsAdded<nWords && !extraWords[mod].empty();++wordsAdded) {
        *buf++ = extraWords[mod].front();
        extraWords[mod].pop();
    }
    if (verbose) std::cout << " " << extraWords[mod].size();

    if (nWords <= wordsAdded) {
        std::cout <<std::endl;
        return true;
    }
    nWords -= wordsAdded;

    if (verbose) std::cout << " nWords " << nWords << std::endl;

    if (availWords < nWords) {
        std::cout << Display::ErrorStr() << " Not enough words available in module " << mod << "'s FIFO for read! (" << availWords << "/" << nWords << ")\n";
        return false;
    }
    retval = Pixie16ReadDataFromExternalFIFO(buf, nWords, mod);

    if (retval < 0) {
        cout << WarningStr("Error reading words from FIFO in module ") << mod << " retVal " << retval << endl;
        return false;
    }

    return true;
}

#endif // Rev. D FIFO access

bool PixieInterface::EndRun() {
    bool b = true;

    LeaderPrint("Ending run");

    for (int mod = 0; mod < numberCards; mod++)
        if (!EndRun(mod))
            b = false;

    if (!b) {
        cout << ErrorStr() << endl;
    } else {
        cout << OkayStr() << endl;
    }

    return b;
}

bool PixieInterface::EndRun(int mod) {
    retval = Pixie16EndRun(mod);

    if (retval < 0) {
        cout << WarningStr("Failed to end run in module ") << mod << endl;
        return false;
    }

    return true;
}

bool PixieInterface::RemovePresetRunLength(int mod) {
    strncpy(tmpName, "HOST_RT_PRESET", nameSize);

    unsigned long bigVal = Decimal2IEEEFloating(99999);

    LeaderPrint("Removing preset run length");

    if (!WriteSglModPar(tmpName, bigVal, mod)) {
        cout << ErrorStr() << endl;
        return false;
    }
    cout << OkayStr() << endl;
    return true;
}

bool PixieInterface::ReadHistogram(word_t *hist, unsigned long sz,
                                   unsigned short mod, unsigned short ch) {
    if (sz > MAX_HISTOGRAM_LENGTH) {
        cout << ErrorStr("Histogram length is too large.") << endl;
        return false;
    }

    retval = Pixie16ReadHistogramFromModule(hist, sz, mod, ch);

    if (retval < 0) {
        cout << ErrorStr("Failed to get histogram data from module ") << mod
             << endl;
        return false;
    }
    return true;
}

bool PixieInterface::AdjustOffsets(unsigned short mod) {
    LeaderPrint("Adjusting Offsets");
    retval = Pixie16AdjustOffsets(mod);

    return !CheckError();
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

// ### PRIVATE FUNCTIONS BELOW ### //
bool PixieInterface::ToggleChannelBit(int mod, int chan, const char *parameter,
                                      int bit) {
    double dval;

    ReadSglChanPar(parameter, dval, mod, chan);
    unsigned int ival = (int) dval;
    ival ^= (1 << bit);
    dval = ival;

    return WriteSglChanPar(parameter, dval, mod, chan);
}

string PixieInterface::ConfigFileName(const string &type, const string &str) {
    //If the file name starts with a '.' or a '/' then we assume the BaseDir should be ignored.
    if (str[0] == '.' || str[0] == '/') return str;

    //Try to determine correct BaseDir.
    string baseDir;
    //If the file is a global type we use PixieBaseDir
    if (type == "global") baseDir = config.Get("global", "PixieBaseDir");
        //Otherwise we try the ModuleBaseDir for the specified type and then the PixieBaseDir
    else {
        baseDir = config.Get(type, "ModuleBaseDir");
        if (baseDir.empty()) baseDir = config.Get("global", "PixieBaseDir");
    }
    //No success so we assume they want the local directory.
    if (baseDir.empty()) baseDir = ".";

    //Return the appended string.
    return baseDir + '/' + str;
}

bool PixieInterface::CheckError(bool exitOnError) const {
    if (StatusPrint(retval < 0) && exitOnError) {
        exit(EXIT_FAILURE); // or do something else
    }

    return (retval < 0);
}

bool
PixieInterface::GetModuleInfo(const unsigned short &mod, unsigned short *rev,
                              unsigned int *serNum, unsigned short *adcBits,
                              unsigned short *adcMsps) {
    //Return false if error code provided.
    return (Pixie16ReadModuleInfo(mod, rev, serNum, adcBits, adcMsps) == 0);
}

// vim: expandtab tabstop=4 shiftwidth=4 softtabstop=4 autoindent
