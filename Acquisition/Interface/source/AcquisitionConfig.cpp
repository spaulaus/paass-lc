#include "AcquisitionConfig.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Display.h"

using namespace std;

AcquisitionConfig::AcquisitionConfig() {
    // Set-up valid configuration keys if they don't exist yet
    if (validConfigKeys_.empty()) {
        // module files
        validConfigKeys_.insert("ModuleType");
        validConfigKeys_.insert("ModuleBaseDir");
        validConfigKeys_.insert("SpFpgaFile");
        validConfigKeys_.insert("TrigFpgaFile");
        validConfigKeys_.insert("ComFpgaFile");
        validConfigKeys_.insert("DspConfFile");
        validConfigKeys_.insert("DspVarFile");
        // global files
        validConfigKeys_.insert("PixieBaseDir");
        validConfigKeys_.insert("DspSetFile");
        validConfigKeys_.insert("DspWorkingSetFile");
        validConfigKeys_.insert("ListModeFile");
        validConfigKeys_.insert("SlotFile");
        validConfigKeys_.insert("CrateConfig");
    }
}

bool AcquisitionConfig::ReadConfigFile(const char *fn) {
    ifstream in(fn);
    if (!in)
        return false;

    stringbuf tag, value;
    string line;

    std::cout << "Reading Pixie Configuration\n";

    //Loop over lines in config file
    bool error = false;
    bool newModule = false;
    string moduleType = "";
    while (std::getline(in, line)) {
        //Get a string stream of current line
        std::istringstream lineStream(line);
        //If the line leads with a '#' we ignore it.
        if (lineStream.peek() == '#') continue;

        //Extract the tag and value
        std::string tag, value;
        if ((lineStream >> tag >> value)) {
            //Check if tag is recognized
            if (validConfigKeys_.find(tag) == validConfigKeys_.end()) {
                cout << "Unrecognized tag " << Display::WarningStr(tag)
                     << " in PixieInterface configuration file." << endl;
            }

            //Parse the ModuleType tag.
            //Moule type is expected as with the following three items ##b, ###m, rev#
            if (tag == "ModuleType") {

                moduleType = ParseModuleTypeTag(value);

                std::cout << "Module Type: ";

                //If we have multiple entires for one type we throw an error.
                if (configStrings_.find(moduleType) != configStrings_.end()) {
                    error = true;

                    std::cout << Display::ErrorStr(moduleType) << "\n";

                    std::cout << Display::ErrorStr()
                              << " Duplicate module type information found for "
                              << moduleType << "!\n";
                    std::cout
                            << "        Remove or comment out tags to be ignored.\n";

                    moduleType = "ignored_" + moduleType;
                } else { std::cout << Display::InfoStr(moduleType) << "\n"; }

                newModule = true;
            }

                //Store configuration
            else if (tag == "SpFpgaFile" || tag == "ComFpgaFile" ||
                     tag == "DspConfFile" || tag == "DspVarFile" ||
                     tag == "TrigFpgaFile" || tag == "ModuleBaseDir") {
                if (moduleType == "") {
                    moduleType = "default";
                    std::cout << "Module Type: " << Display::InfoStr(moduleType) << "\n";
                }
                if (newModule && tag != "ModuleBaseDir") {
                    std::cout << " PixieBaseDir\t"
                              << configStrings_["global"]["PixieBaseDir"]
                              << "\n";
                }
                newModule = false;
                if ((configStrings_[moduleType][tag]).empty()) {
                    error = true;

                    std::cout << " " << Display::ErrorStr(tag) << "\t" << value << endl;

                    std::cout << Display::ErrorStr() << " Duplicate " << tag
                              << " specified for " << moduleType << "!\n";
                    std::cout
                            << "        Remove or comment out tags to be ignored.\n";

                    tag = "ignored_" + tag;
                } else {
                    std::cout << " " << tag << "\t" << value << endl;
                }
                configStrings_[moduleType][tag] = ConfigFileName(moduleType,
                                                                value);
            } else {
                std::cout << " " << tag << "\t" << value << endl;
                configStrings_["global"][tag] = ConfigFileName("global", value);
            }

            //Check if BaseDir is defined differently then in the environment
            if (tag == "PixieBaseDir") {
                // check if this matches the environment PXI_ROOT if it is set
                if (getenv("PXI_ROOT") != nullptr) {
                    if (value != string(getenv("PXI_ROOT"))) {
                        cout << Display::WarningStr("This does not match the value of PXI_ROOT set in the environment")
                             << endl;
                    }
                }
            }
        }
    }

    if (error) return false;

    return true;
}

std::string AcquisitionConfig::ParseModuleTypeTag(std::string value) {
    std::string moduleType = "invalid";
    int adcRes = -1, msps = -1;
    string revision = "";

    string adcResStr = "";
    size_t adcResLocEnd = value.find('b');
    if (adcResLocEnd != string::npos) {
        size_t adcResLocBegin = value.find_last_not_of("0123456789",
                                                       adcResLocEnd - 1);
        if (adcResLocBegin == string::npos) adcResLocBegin = 0;
        else adcResLocBegin++;
        adcResStr = value.substr(adcResLocBegin, adcResLocEnd - adcResLocBegin);
    }

    if (adcResStr.empty()) {
        std::cout << Display::ErrorStr() << " Invalid ModuleType, ADC resolution not specified: '"
                  << Display::InfoStr(value) << "'.\n";
    } else {
        try { adcRes = std::stoi(adcResStr); }
        catch (const std::invalid_argument &ia) {
            std::cout << Display::ErrorStr() << " Invalid ADC resolution: '" << value << "' (" << adcResStr << ")\n";
        }
    }

    string mspsStr = "";
    size_t mspsLocEnd = value.find('m');
    if (mspsLocEnd != string::npos) {
        size_t mspsLocBegin = value.find_last_not_of("0123456789",
                                                     mspsLocEnd - 1);
        if (mspsLocBegin == string::npos) mspsLocBegin = 0;
        else mspsLocBegin++;
        mspsStr = value.substr(mspsLocBegin, mspsLocEnd - mspsLocBegin);
    }

    if (mspsStr.empty()) {
        std::cout << Display::ErrorStr() << " Invalid ModuleType, sample rate not specified: '"
                  << Display::InfoStr(value) << "'.\n";
    } else {
        try { msps = std::stoi(mspsStr); }
        catch (const std::invalid_argument &ia) {
            std::cout << Display::ErrorStr() << " Invalid sample rate: '" << value << "' (" << mspsStr << ")\n";
        }
    }

    size_t revLoc = value.find("rev");
    string revStr = "not specified";
    if (revLoc != string::npos) revStr = value.substr(revLoc + 3, 1);
    if (revStr.length() == 1) {
        revision = revStr;
    } else {
        std::cout << Display::ErrorStr() << " Invalid Revision: '" << value << "' (" << revStr << ")\n";
    }

    if (adcRes > 0 && msps > 0 && revision != "") {
        stringstream moduleTypeStream;
        moduleTypeStream << adcRes << "b" << msps << "m-rev" << revision;
        moduleType = moduleTypeStream.str();
    }

    return moduleType;

}

string AcquisitionConfig::ConfigFileName(const string &type, const string &str) {
    //If the file name starts with a '.' or a '/' then we assume the BaseDir should be ignored.
    if (str[0] == '.' || str[0] == '/') return str;

    //Try to determine correct BaseDir.
    string baseDir;
    //If the file is a global type we use PixieBaseDir
    if (type == "global") baseDir = configStrings_["global"]["PixieBaseDir"];
        //Otherwise we try the ModuleBaseDir for the specified type and then the PixieBaseDir
    else {
        baseDir = configStrings_[type]["ModuleBaseDir"];
        if (baseDir.empty()) baseDir = configStrings_["global"]["PixieBaseDir"];
    }
    //No success so we assume they want the local directory.
    if (baseDir.empty()) baseDir = ".";

    //Return the appended string.
    return baseDir + '/' + str;
}

std::string AcquisitionConfig::Get(std::string moduleType, std::string tag) {
    return configStrings_[moduleType][tag];
}

bool AcquisitionConfig::HasModuleConfig(const std::string &moduleType) {
    return configStrings_.find(moduleType) != configStrings_.end();
}