/// @file MCA.cpp
/// @brief Implementation of the base MCA class.
/// @author K. Miernik, K. Smith, C. R. Thornsberry, and S. V. Paulauskas
/// @date September 11, 2018
#include "MCA.h"

#include "AcquisitionInterface.hpp"
#include "Display.h"
#include "Utility.h"

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <MCA.h>


MCA::MCA(AcquisitionInterface *pif) : pif_(pif) {}

MCA::MCA(PixieInterface *pif) {}

MCA::MCA(EmulatedInterface *pif) {}

MCA::~MCA() = default;

double MCA::GetRunTimeInSeconds() {
    stopTime_ = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double>>(stopTime_ - startTime_).count();
}

void MCA::Run(const float &duration, const bool *stop) {
    //Start the pixie histogram
    pif_->StartHistogramRun();

    startTime_ = std::chrono::steady_clock::now();

    while (true) {
        if(GetRunTimeInSeconds() > duration)
            break;

        if (stop != nullptr && *stop)
            break;
        if (duration > 0.0 && GetRunTimeInSeconds() >= duration)
            break;

        sleep(2);

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

bool MCA::Step() {
    if (!pif_ || !pif_->CheckRunStatus())
        return false;

    for (int mod = 0; mod < pif_->GetConfiguration().GetNumberOfModules(); mod++)
        for (unsigned int ch = 0; ch < pif_->GetConfiguration().GetNumberOfChannels(); ch++)
            StoreData(mod, ch);

    Flush();

    stopTime_ = std::chrono::steady_clock::now();

    return true;
}

bool MCA::IsOpen() { return isOpen_; }

bool MCA::StoreData(int mod, int ch) {}

bool MCA::OpenFile(const char *basename) {}

void MCA::Flush() {}
