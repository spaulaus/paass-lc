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

MCA::MCA(AcquisitionInterface *pif) : _pif(pif) {}

MCA::MCA(PixieInterface *pif) {}

MCA::MCA(EmulatedInterface *pif) {}

///Return the length of time the MCA has been running.
double MCA::GetRunTime() {
    stopTime_ = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double>>(stopTime_ - startTime_).count();
}

/**The MCA is initialized and run for the specified duration or until a
 * stop command is received. At specific intervals the MCA output is
 * updated via MCA::StoreData(). Will continue until external bool (stop)
 * is set to false. If this pointer is set to NULL, will continue uninterrupted.
 *
 * \param[in] duration Amount of time to run the MCA.
 * \param[in] stop External boolean flag for stop run command.
 */
void MCA::Run(float duration, bool *stop) {
    //Start the pixie histogram
    _pif->StartHistogramRun();

    startTime_ = std::chrono::steady_clock::now();

    while (true) {
        if(GetRunTime() > duration)
            break;

        if (stop != nullptr && *stop)
            break;
        if (duration > 0.0 && GetRunTime() >= duration)
            break;

        sleep(2);

        std::cout << "|" << std::fixed << std::setprecision(2) << GetRunTime() << " s |\r" << std::flush;

        if (!Step()) {
            std::cout << Display::ErrorStr("Run TERMINATED") << std::endl;
            break;
        }
    }

    _pif->EndRun();

    std::cout << std::endl;
    Display::LeaderPrint("Run finished");
    std::cout << Display::OkayStr() << std::endl;
    Display::LeaderPrint("Total running time:");
    std::cout << std::fixed << std::setprecision(2) << GetRunTime() << " s"
              << std::endl;

    std::cout.unsetf(std::ios_base::floatfield);
    std::cout.precision(6);
}

bool MCA::Step() {
    if (!_pif || !_pif->CheckRunStatus())
        return false;

    for (int mod = 0; mod < _pif->GetConfiguration().GetNumberOfModules(); mod++)
        for (unsigned int ch = 0; ch < _pif->GetConfiguration().GetNumberOfChannels(); ch++)
            StoreData(mod, ch);

    Flush();

    stopTime_ = std::chrono::steady_clock::now();

    return true;
}
