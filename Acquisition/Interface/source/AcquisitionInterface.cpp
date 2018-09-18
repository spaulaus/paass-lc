/// @file AcquisitionInterface.cpp
/// @brief
/// @author S. V. Paulauskas, K. Smith
/// @date September 17, 2018
#include <AcquisitionInterface.hpp>

#include <Display.h>

#include <iostream>

using namespace std;

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

bool AcquisitionInterface::Boot(int mode, bool useWorkingSetFile) {}
