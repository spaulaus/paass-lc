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

bool AcquisitionInterface::ReadSlotConfig(const char *slotF) {
    char restOfLine[CONFIG_LINE_LENGTH];

    if (slotF == NULL)
        slotF = config_.Get("global", "SlotFile").c_str();

    ifstream in(slotF);

    if (!in) {
        cout << Display::ErrorStr("Error opening slot definition file: ")
             << Display::ErrorStr(slotF) << endl;
        exit(EXIT_FAILURE);
    }
    stringstream line;

    in >> numberOfModules_;
    in.getline(restOfLine, CONFIG_LINE_LENGTH, '\n');

    if (numberOfModules_ > Pixie16::maximumNumberOfModulesPerCrate) {
        cout << Display::ErrorStr("Too many cards") << " : " << numberOfModules_ << " > "
             << Pixie16::maximumNumberOfModulesPerCrate << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numberOfModules_; i++) {
        in >> slotMap_[i];
        in.getline(restOfLine, CONFIG_LINE_LENGTH, '\n');
        if (!in.good()) {
            cout << Display::ErrorStr("Error reading slot definition file.") << endl;
            exit(EXIT_FAILURE);
        }
    }

    cout << "Slot definition from " << Display::InfoStr(slotF) << endl;
    cout << "  System with " << numberOfModules_ << " cards" << endl;
    cout << "  ";

    for (int i = 0; i < numberOfModules_; i++)
        cout << "||  M  S ";
    cout << "|" << endl << "  ";

    for (int i = 0; i < numberOfModules_; i++)
        cout << "|| " << setw(2) << i << " " << setw(2) << slotMap_[i] << " ";
    cout << "|" << endl;
    in.close();

    return true;
}