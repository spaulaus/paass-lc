/// @file Mca.cpp
/// @brief Implementation of the base Mca class.
/// @author K. Miernik, K. Smith, C. R. Thornsberry, and S. V. Paulauskas
/// @date September 11, 2018
#include <Mca.hpp>

#include <AcquisitionInterface.hpp>
#include <Display.h>
#include <Utility.h>

#include <iostream>
#include <iomanip>
#include <thread>

Mca::Mca(AcquisitionInterface *pif) : startTime_(std::chrono::steady_clock::now()), pif_(pif) {}

Mca::~Mca() = default;

///@TODO This calculation needs revisited in the event that we have multiple crates.
unsigned int Mca::CalculateHistogramId(const unsigned int &crate, const unsigned int &module, const unsigned int &channel) {
    return crate * pif_->GetConfiguration().GetNumberOfModules() + module * Pixie16::maximumNumberOfChannels + channel;
}

void Mca::Flush() {}

double Mca::GetRunTimeInSeconds() {
    stopTime_ = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double>>(stopTime_ - startTime_).count();
}

bool Mca::IsOpen() { return isOpen_; }

bool Mca::OpenFile(const char *basename) { return false; }

void Mca::Run(const float &duration) {
    pif_->StartHistogramRun();

    startTime_ = std::chrono::steady_clock::now();

    while (true) {
        if(GetRunTimeInSeconds() > duration && duration != 0)
            break;

        if (duration > 0.0 && GetRunTimeInSeconds() >= duration)
            break;

        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "|" << std::fixed << std::setprecision(2) << GetRunTimeInSeconds() << " s |\r" << std::flush;

        if (!Step()) {
            std::cout << Display::ErrorStr("Run TERMINATED") << std::endl;
            break;
        }
    }

    pif_->EndRun();

    std::cout << std::endl;
    Display::LeaderPrint("Run finished");
    std::cout << Display::OkayStr() << std::endl;
    Display::LeaderPrint("Total running time:");
    std::cout << std::fixed << std::setprecision(2) << GetRunTimeInSeconds() << " s" << std::endl;

    std::cout.unsetf(std::ios_base::floatfield);
    std::cout.precision(6);
}

bool Mca::Step() {
    if (!pif_ || !pif_->CheckRunStatus())
        return false;

    for (unsigned short mod = 0; mod < pif_->GetConfiguration().GetNumberOfModules(); mod++)
        for (unsigned short ch = 0; ch < pif_->GetConfiguration().GetNumberOfChannels(); ch++)
            StoreData(mod, ch);

    Flush();

    stopTime_ = std::chrono::steady_clock::now();

    return true;
}

bool Mca::StoreData(const unsigned short &mod, const unsigned short &ch) { return true; }