/// @file EmulatedInterface.cpp
/// @brief Implementation for the emulated pixie interface for Poll2, we'll use a combination of the XIA offline API
/// and custom work. The XIA API does not allow us to actually generate / display data.
/// @authors S. V. Paulauskas and K. Smith
/// @date March 24, 2018
#include <EmulatedInterface.hpp>

#include <Display.h>
#include <UnitTestSampleData.hpp>

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
    std::cout << "EmulatedInterface::AcquireTraces - Acquiring traces for Module " << mod << std::endl;
    return true;
}

bool EmulatedInterface::AdjustOffsets(unsigned short mod) {
    const char * name = "VOFFSET";
    for (unsigned int i = 0; i < config_.GetNumberOfChannels(); i++)
        SetParameterValue(GenerateParameterKey(name, 0, mod, i), 8008135);
    return true;
}

unsigned long EmulatedInterface::CheckFIFOWords(unsigned short mod) {
    return unittest_encoded_data::R30474_250::rawHeader.size() * numberOfTriggersInFifo_;
}

bool EmulatedInterface::CheckRunStatus(short mod) {
    return true;
}

bool EmulatedInterface::EndRun(short mod) {
    return true;
}

template<typename T>
bool EmulatedInterface::FillBuffer(T *buf, const unsigned long &bufSize, const std::vector<unsigned int> &filler) {
    double offset = 0;
    double counter = 1;
    for(unsigned int i = 0; i < bufSize; i++) {
        if(i >= filler.size() * counter) {
            offset = filler.size() * counter;
            counter++;
        }

        try {
            buf[i] = (T)filler.at((unsigned int)(i - offset));
        } catch (std::out_of_range &outOfRange) {
            std::cout << "EmulatedInterface::FillBuffer - Attempted to access out of range index " << i - offset << "\n";
            return false;
        }
    }
    return true;
}

std::string EmulatedInterface::GenerateParameterKey(const char *name, const int &crate, const int &mod, const int &chan) {
    return string(name) + "-" + std::to_string(crate) + "-" + std::to_string(mod) + "-" + std::to_string(chan);
}

double EmulatedInterface::GetInputCountRate(int mod, int chan) {
    return 100;
}

double EmulatedInterface::GetOutputCountRate(int mod, int chan) {
    return 100;
}

double EmulatedInterface::GetParameterValue(const std::string &key) {
    if(parameterValues_.find(key) == parameterValues_.end())
        SetParameterValue(key, 8008135);

    return parameterValues_.find(key)->second;
}

bool EmulatedInterface::GetStatistics(unsigned short mod) {
    return true;
}

bool EmulatedInterface::Init() {
    Display::LeaderPrint("Initializing Pixie");
    retval_ = Pixie16InitSystem(config_.GetNumberOfModules(), &(config_.GetSlotMapAsVector(0)[0]), true);
    numberOfTriggersInFifo_ = 3;
    return true;
}

bool EmulatedInterface::ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose) {
    return FillBuffer(buf, nWords, unittest_encoded_data::R30474_250::rawHeader);
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
    return true;
}

bool EmulatedInterface::ReadSglChanPar(const char *name, double &val, int mod, int chan) {
    val = GetParameterValue(GenerateParameterKey(name, 0, mod, chan));
    return true;
}

bool EmulatedInterface::ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan) {
    return FillBuffer(buf, sz, unittest_trace_variables::trace);
}

bool EmulatedInterface::ReadSglModPar(const char *name, Pixie16::word_t &val, int mod) {
    val = (unsigned int)GetParameterValue(GenerateParameterKey(name, 0, mod, 0));
    return true;
}

bool EmulatedInterface::RemovePresetRunLength(int mod) {
    Display::LeaderPrint("Removing preset run length");
    return true;
}

bool EmulatedInterface::SaveDSPParameters(const char *fn) {
    std::cout << "EmulatedInterface::SaveDSPParameters - DSP Parameters saved successfully." << std::endl;
    return true;
}

void EmulatedInterface::SetParameterValue(const std::string &key, const double &value) {
    if(parameterValues_.find(key) == parameterValues_.end())
        parameterValues_.emplace(std::make_pair(key, 8008135));

    parameterValues_.find(key)->second = value;
}

bool EmulatedInterface::StartHistogramRun(short mod, unsigned short mode) {
    Display::LeaderPrint("Starting an MCA run!");
    return true;
}

bool EmulatedInterface::StartListModeRun(short mod, unsigned short listMode, unsigned short runMode) {
    Display::LeaderPrint("Starting list mode run!");
    return true;
}

bool EmulatedInterface::WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval) {
    if(pval)
        *pval = GetParameterValue(GenerateParameterKey(name, 0, mod, chan));
    SetParameterValue(GenerateParameterKey(name, 0, mod, chan), val);
    return true;
}

bool EmulatedInterface::WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval) {
    if(pval)
        *pval = (unsigned int)GetParameterValue(GenerateParameterKey(name, 0, mod, 0));
    SetParameterValue(GenerateParameterKey(name, 0, mod, 0), val);
    return true;
}