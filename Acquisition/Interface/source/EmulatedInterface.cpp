///@file EmulatedInterface.cpp
///@brief Implementation for the emulated pixie interface for Poll2
///@authors S. V. Paulauskas and K. Smith
///@date March 24, 2018
#include "EmulatedInterface.hpp"

#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

EmulatedInterface::EmulatedInterface() : AcquisitionInterface("") {
    //Does nothing fun right now
}

EmulatedInterface::EmulatedInterface(const char *cfgFile/* = ""*/) : AcquisitionInterface(cfgFile) {
    //Does nothing fun right now.
}

EmulatedInterface::~EmulatedInterface() = default;

bool EmulatedInterface::AcquireTraces(int mod) {
    return false;
}

bool EmulatedInterface::AdjustOffsets(unsigned short mod) {
    return false;
}

bool EmulatedInterface::Boot(Interface::BootType mode, bool useWorkingSetFile) {
    return true;
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
bool EmulatedInterface::Init(bool offlineMode) {
    return true;
}

bool EmulatedInterface::ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose) {
    return false;
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
    return false;
}

bool EmulatedInterface::ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod,
                                         unsigned short chan) {
    return false;
}

bool EmulatedInterface::ReadSglModPar(const char *name, Pixie16::word_t &val, int mod) {
    return false;
}

bool EmulatedInterface::RemovePresetRunLength(int mod) {
    return true;
}

bool EmulatedInterface::StartHistogramRun(short mod, unsigned short mode) {
    return true;
}

bool EmulatedInterface::StartListModeRun(short mod, unsigned short listMode, unsigned short runMode) {
    return false;
}

bool EmulatedInterface::SaveDSPParameters(const char *fn) {
    return false;
}

bool EmulatedInterface::WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval) {
    return false;
}

bool EmulatedInterface::WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval) {
    return false;
}