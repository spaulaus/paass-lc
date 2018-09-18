///@file EmulatedInterface.cpp
///@brief Implementation for the emulated pixie interface for Poll2, we'll use a combination of the XIA offline API
// and custom work. The XIA API does not allow us to actually generate / display data.
///@authors S. V. Paulauskas and K. Smith
///@date March 24, 2018
#include <EmulatedInterface.hpp>

#include <Display.h>

#include <pixie16app_export.h>

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

using namespace std;

EmulatedInterface::EmulatedInterface() : PixieInterface("") {
    //Does nothing fun right now
}

EmulatedInterface::EmulatedInterface(const char *cfgFile/* = ""*/) : PixieInterface(cfgFile) {
    //Does nothing fun right now.
}

EmulatedInterface::~EmulatedInterface() = default;

bool EmulatedInterface::AcquireTraces(int mod) {
    return false;
}

bool EmulatedInterface::AdjustOffsets(unsigned short mod) {
    return false;
}

unsigned long EmulatedInterface::CheckFIFOWords(unsigned short mod) {
    return 0;
}

bool EmulatedInterface::CheckRunStatus(short mod) {
    return true;
}

bool EmulatedInterface::EndRun(short mod) {
    return true;
}

bool EmulatedInterface::Init() {
    Display::LeaderPrint("Initializing Pixie");
    retval_ = Pixie16InitSystem(config_.GetNumberOfModules(), &(config_.GetSlotMapAsVector(0)[0]), true);
    return true;
}

bool EmulatedInterface::ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose) {
    return true;
}

bool EmulatedInterface::ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch) {
    static const unsigned int nrolls = 10000;
    static std::default_random_engine generator;
    std::normal_distribution<double> distribution(sz*0.5, sz*0.001);

    for (unsigned int i = 0; i < nrolls; ++i) {
        int number = (int)round(distribution(generator));
        if (number >= 0.0)
            ++hist[number];
    }
    return false;
}

bool EmulatedInterface::ReadSglChanPar(const char *name, double &val, int mod, int chan) {
    return true;
}

bool EmulatedInterface::ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan) {
    return true;
}

bool EmulatedInterface::ReadSglModPar(const char *name, Pixie16::word_t &val, int mod) {
    return true;
}

bool EmulatedInterface::RemovePresetRunLength(int mod) {
    return true;
}

bool EmulatedInterface::StartHistogramRun(short mod, unsigned short mode) {
    return true;
}

bool EmulatedInterface::StartListModeRun(short mod, unsigned short listMode, unsigned short runMode) {
    return true;
}

bool EmulatedInterface::SaveDSPParameters(const char *fn) {
    return true;
}

bool EmulatedInterface::WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval) {
    return true;
}

bool EmulatedInterface::WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval) {
    return true;
}

bool EmulatedInterface::GetStatistics(unsigned short mod) {
    return true;
}

double EmulatedInterface::GetInputCountRate(int mod, int chan) {
    return 0;
}

double EmulatedInterface::GetOutputCountRate(int mod, int chan) {
    return 0;
}
