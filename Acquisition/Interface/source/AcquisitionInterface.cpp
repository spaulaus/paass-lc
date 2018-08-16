#include "AcquisitionInterface.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Display.h"

using namespace std;

AcquisitionInterface::AcquisitionInterface(const char *cfgFile) : lock_("PixieInterface") {
    Display::SetColorTerm();
    if (!config_.ReadConfigFile(cfgFile)) {
        cout << Display::ErrorStr() << " Unable to read configuration file: '" << cfgFile << "'\n"
             << Display::InfoStr() << " Are the configuration files in the running directory?\n"
             << "Auto-configured files are available in " << INSTALL_PREFIX << "\n";
        exit(EXIT_FAILURE);
    }
    numberOfChannels_ = Pixie16::maximumNumberOfChannels;
}