#include "AcqInterface.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Display.h"

using namespace std;

AcqInterface::AcqInterface(const char *cfgFile) : 
    lock("PixieInterface") 
{
    Display::SetColorTerm();
    if (!config.ReadConfigFile(cfgFile)) {
        cout << Display::ErrorStr()
             << " Unable to read configuration file: '" << cfgFile << "'\n";
        cout << Display::InfoStr()
             << " Are the configuration files in the running directory?\n";
        cout << "Autoconfigured files are avaialable in "
             << INSTALL_PREFIX << "\n";
        exit(EXIT_FAILURE);
    }
}


bool AcqInterface::ReadSlotConfig(const char *slotF) {
    char restOfLine[CONFIG_LINE_LENGTH];

    if (slotF == NULL)
        slotF = config.Get("global", "SlotFile").c_str();

    ifstream in(slotF);

    if (!in) {
        cout << Display::ErrorStr("Error opening slot definition file: ")
             << Display::ErrorStr(slotF) << endl;
        exit(EXIT_FAILURE);
    }
    stringstream line;

    in >> numberCards;
    in.getline(restOfLine, CONFIG_LINE_LENGTH, '\n');

    if (numberCards > MAX_MODULES) {
        cout << Display::ErrorStr("Too many cards") << " : " << numberCards << " > "
             << MAX_MODULES << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numberCards; i++) {
        in >> slotMap[i];
        in.getline(restOfLine, CONFIG_LINE_LENGTH, '\n');
        if (!in.good()) {
            cout << Display::ErrorStr("Error reading slot definition file.") << endl;
            exit(EXIT_FAILURE);
        }
    }

    cout << "Slot definition from " << Display::InfoStr(slotF) << endl;
    cout << "  System with " << numberCards << " cards" << endl;
    cout << "  ";

    for (int i = 0; i < numberCards; i++) {
        cout << "||  M  S ";
    }
    cout << "|" << endl << "  ";

    for (int i = 0; i < numberCards; i++) {
        cout << "|| " << setw(2) << i << " " << setw(2) << slotMap[i] << " ";
    }
    cout << "|" << endl;
    in.close();

    return true;
}



// vim: expandtab tabstop=4 shiftwidth=4 softtabstop=4 autoindent
