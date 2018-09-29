/// @file AcquisitionInterface.cpp
/// @brief Abstracted interface to the Hardware API. Focuses on Pixie related commands at the moment.
/// @author S. V. Paulauskas, K. Smith
/// @date December 9, 2017
#include <AcquisitionInterface.hpp>

#include <Display.h>

#include <iostream>

using namespace std;

AcquisitionInterface::~AcquisitionInterface() = default;

AcquisitionInterface::AcquisitionInterface(const char *cfgFile) : lock_("PixieInterface") {
    try {
        config_.ReadConfiguration(cfgFile);
    } catch (std::invalid_argument &invalidArgument) {
        cout << Display::ErrorStr() << "AcquisitionInterface::AcquisitionInterface - Caught invalid argument while "
             << "loading the configuration file." << endl << invalidArgument.what()
             << Display::InfoStr() << " Are the configuration files in the running directory?\n"
             << "Auto-configured files are available in " << INSTALL_PREFIX << "\n";
        throw invalidArgument;
    }
}

bool AcquisitionInterface::Boot(int mode, bool useWorkingSetFile) {
    return false;
}

bool AcquisitionInterface::Init() {
    return false;
}

bool AcquisitionInterface::WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval) {
    return false;
}

bool AcquisitionInterface::ReadSglModPar(const char *name, Pixie16::word_t &val, int mod) {
    return false;
}

void AcquisitionInterface::PrintSglModPar(const char *name, int mod, Pixie16::word_t *prev) {

}

bool AcquisitionInterface::WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval) {
    return false;
}

bool AcquisitionInterface::ReadSglChanPar(const char *name, double &val, int mod, int chan) {
    return false;
}

void AcquisitionInterface::PrintSglChanPar(const char *name, int mod, int chan, double *prev) {

}

bool AcquisitionInterface::SaveDSPParameters(const char *fn) {
    return false;
}

bool AcquisitionInterface::AcquireTraces(int mod) {
    return false;
}

double AcquisitionInterface::GetInputCountRate(int mod, int chan) {
    return 0;
}

double AcquisitionInterface::GetOutputCountRate(int mod, int chan) {
    return 0;
}

bool AcquisitionInterface::ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan) {
    return false;
}

bool AcquisitionInterface::AdjustOffsets(unsigned short mod) {
    return false;
}

bool AcquisitionInterface::StartHistogramRun(short mod, unsigned short mode) {
    return false;
}

bool AcquisitionInterface::StartListModeRun(short mod, unsigned short listMode, unsigned short runMode) {
    return false;
}

bool AcquisitionInterface::CheckRunStatus(short mod) {
    return false;
}

bool AcquisitionInterface::EndRun(short mod) {
    return false;
}

bool
AcquisitionInterface::ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch) {
    return false;
}

bool AcquisitionInterface::RemovePresetRunLength(int mod) {
    return false;
}

AcquisitionConfig AcquisitionInterface::GetConfiguration() const {
    return config_;
}

bool AcquisitionInterface::GetModuleInfo(const unsigned short &mod, unsigned short *rev, unsigned int *serNum,
                                         unsigned short *adcBits, unsigned short *adcMsps) {
    return false;
}

bool AcquisitionInterface::GetStatistics(unsigned short mod) {
    return false;
}

unsigned long AcquisitionInterface::CheckFIFOWords(unsigned short mod) {
    return 0;
}

bool AcquisitionInterface::ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose) {
    return false;
}
