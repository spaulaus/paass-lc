///@file EmulatedInterface.cpp
///@brief Implementation for the emulated pixie interface for Poll2
///@authors S. V. Paulauskas and K. Smith
///@date March 24, 2018
#include "EmulatedInterface.hpp"

#include <iomanip>
#include <iostream>

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
    return false;
}

unsigned long EmulatedInterface::CheckFIFOWords(unsigned short mod) {
    return 0;
}

bool EmulatedInterface::CheckRunStatus(short mod) {
    return false;
}

bool EmulatedInterface::EndRun(short mod) {
    return false;
}

bool EmulatedInterface::Init(bool offlineMode) {
    return true;
}

bool EmulatedInterface::ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose) {
    return false;
}

bool EmulatedInterface::ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch) {
    return false;
}

bool EmulatedInterface::ReadSlotConfig(const char *slotCfgFile) {
    numberOfModules_ = Pixie16::maximumNumberOfModulesPerCrate;
    for (int i = 0; i < numberOfModules_; i++)
        slotMap_[i] = 2 + i;

    cout << "EmulatedPixieInterface::ReadSlotDefinition - Initialzed system with " << numberOfModules_ << " modules.\n";

    for (int i = 0; i < numberOfModules_; i++)
        cout << "||  M  S ";
    cout << "|" << endl << "  ";

    for (int i = 0; i < numberOfModules_; i++)
        cout << "|| " << setw(2) << i << " " << setw(2) << slotMap_[i] << " ";
    cout << "|" << endl;

    return true;
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
    return false;
}

bool EmulatedInterface::StartHistogramRun(short mod, unsigned short mode) {
    return false;
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