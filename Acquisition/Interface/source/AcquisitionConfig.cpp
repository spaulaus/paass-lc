/// @file AcquisitionConfig.cpp
/// @brief Implementation for AcquisitionConfig class, which reads configuration file defining modules.
/// @author K. Smith, S. V. Paulauskas
/// @date August 11, 2018
#include "AcquisitionConfig.hpp"

#include "Display.h"
#include "HelperFunctions.hpp"
#include "StringManipulationFunctions.hpp"
#include "XmlInterface.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

AcquisitionConfig::AcquisitionConfig() {
    validNodes_.insert("PixieBaseDir");
    validNodes_.insert("DspSetFile");
    validNodes_.insert("DspWorkingSetFile");

    validNodes_.insert("Firmware");
    validNodes_.insert("ModuleType");
    validNodes_.insert("ModuleBaseDir");
    validNodes_.insert("SpFpgaFile");
    validNodes_.insert("ComFpgaFile");
    validNodes_.insert("DspConfFile");
    validNodes_.insert("DspVarFile");

    validNodes_.insert("SlotDefinition");
    validNodes_.insert("Pxisys");
}

void AcquisitionConfig::ReadConfiguration(const std::string &fileName) {
    pugi::xml_node root = XmlInterface::get(fileName.c_str())->GetDocument()->child("Configuration");

    configStrings_["global"]["PixieBaseDir"] = root.child("PixieBaseDir").text().as_string("");
    if(Get("global", "PixieBaseDir").empty())
        throw std::invalid_argument("AcquisitionConfig::ReadConfiguration : Missing <PixieBaseDir> node!");

    for (const auto &node : root.children()) {
        string tag = node.name();

        if (validNodes_.find(tag) == validNodes_.end())
            throw std::invalid_argument("AcquisitionConfig::ReadConfiguration - Invalid node named <" + tag +">!");

        if (tag == "PixieBaseDir")
          continue;
        else if (tag == "Firmware")
            ParseFirmwareNode(node);
        else if (tag == "SlotDefinition")
            ParseSlotDefinitionNode(node);
        else {
            string value = node.text().as_string("");
            if(value.empty())
                throw invalid_argument("AcquisitionConfig::ReadConfiguration : Missing value for <" + tag + "> node.");

            if(!(configStrings_["global"][tag]).empty())
                throw invalid_argument(DuplicateEntryMessage("ReadConfiguration", tag));
            else {
                StringManipulation::RemoveSpaces(value);
                configStrings_["global"][tag] = ConfigFileName("global", value);
            }
        }
    }
}

void AcquisitionConfig::VerifyFiles() {
    for(const auto &pair : pxisysMap_)
        if(!FileManagement::DoesFileExistPosix(pair.second))
            throw std::invalid_argument("AcquisitionConfig::ReadConfiguration - The file, " + pair.second
                                        + ", does not exist ! Check the file name and path!");

    for(const auto &pair : configStrings_)
        for(const auto &pair1 : pair.second)
            if(pair1.first != "ModuleBaseDir" || pair1.first != "PixieBaseDir")
                if(!FileManagement::DoesFileExistPosix(pair1.second))
                    throw std::invalid_argument("AcquisitionConfig::ReadConfiguration - The file, " + pair1.second
                                                + ", does not exist ! Check the file name and path!");
}

void AcquisitionConfig::ParseFirmwareNode(const pugi::xml_node &node) {
    std::string moduleType = ParseModuleTypeNode(node.child("ModuleType"));

    if (configStrings_.find(moduleType) != configStrings_.end())
        throw invalid_argument(DuplicateEntryMessage("ParseFirmwareNode", moduleType));

    for (const auto &child: node.children()) {
        string tag = child.name();

        if (tag == "ModuleType")
            continue;

        string value = child.text().as_string("");
        if (value.empty())
            throw invalid_argument("AcquisitionConfig::ParseFirmwareNode : Missing value for " + tag
                                   + ". Please provide a value so that we can continue!");

        if (!Get(moduleType, tag).empty())
            throw invalid_argument(DuplicateEntryMessage("ParseFirmwareNode", tag));
        else {
            StringManipulation::RemoveSpaces(value);
            configStrings_[moduleType][tag] = ConfigFileName(moduleType, value);
        }
    }
}

void AcquisitionConfig::ParseSlotDefinitionNode(const pugi::xml_node &node) {
    for (const auto &crates : node.children("Crate")) {
        auto crateNumber = crates.attribute("number").as_uint();
        auto pxisysPath = (string)crates.child("Pxisys").text().as_string("");
        StringManipulation::RemoveSpaces(pxisysPath);

        if(pxisysPath.empty())
            throw std::invalid_argument("AcquisitionConfig::ParseSlotDefinitionNode - No Pxisys path found for Crate "
                                        + std::to_string(crateNumber) + "! Provide one!");
        else if(pxisysPath.find_first_of(';') != string::npos)
            throw std::invalid_argument("AcquisitionConfig::ParseSlotDefinitionNode - Found more than one pxisys "
                                        "defined for Crate " + std::to_string(crateNumber) + "! Choose ONE!");
        else
            pxisysMap_[crateNumber] = ConfigFileName("global", pxisysPath);

        auto maximumNumberOfModuleSlotsInCrate = DetermineMaximumNumberOfModules(pxisysPath);

        unsigned int numberOfModulesInCrate = 0;
        for(const auto &modules : crates.children("Module")) {
            unsigned int slot = modules.attribute("slot").as_uint(std::numeric_limits<unsigned int>::max());
            unsigned int number = modules.attribute("number").as_uint(std::numeric_limits<unsigned int>::max());

            if(numberOfModulesInCrate > maximumNumberOfModuleSlotsInCrate)
                throw std::invalid_argument("AcquisitionConfig::ParseSlotDefinitionNode - Crate "
                + std::to_string(crateNumber) + " has too many modules defined. It has room for "
                + std::to_string(maximumNumberOfModuleSlotsInCrate) + " modules!!");

            slotMap_[crateNumber][slot] = number;

            numberOfModulesInCrate++;
        }
    }
}

unsigned int AcquisitionConfig::DetermineMaximumNumberOfModules(const std::string &pxisysPath) {
    auto name = pxisysPath.substr(pxisysPath.find_last_of("/") + 1);
    smatch m;

    if(std::regex_search (name, m, regex("([0-9]+)")))
        return (unsigned int)stoi(m[0].str()) - 1; // Note : A 14 slot crate holds 13 modules, 8 slot holds 7.

    //Magic number warning!! The default pxisys.ini file shipped from XIA is for a 14 slot Wiener crate. That crate
    // has 14 slots. Modules start at slot 2. The maximum number of available modules is 13.
    return 13;
}

std::string AcquisitionConfig::DuplicateEntryMessage(const std::string &methodName, const std::string &duplicateName) {
    return "AcquisitionConfig::" + methodName + " : Duplicate entry found for "
           + duplicateName + ". Rename or remove one of the entries.";
}

std::string AcquisitionConfig::ParseModuleTypeNode(const pugi::xml_node &parent) {
    if(parent.empty())
        throw std::invalid_argument("AcquisitionConfig::ParseModuleTypeNode : We received an empty <ModuleType> node!");

    string moduleType = (string) parent.child("BitResolution").text().as_string(" bit-missing ") + "b"
                        + (string) parent.child("SamplingFrequency").text().as_string(" freq-missing ") + "m-rev"
                        + (string) parent.child("Revision").text().as_string(" rev-missing ");

    if (moduleType.find("bit-missing") != string::npos)
        throw invalid_argument("AcquisitionConfig::ParseModuleTypeNode : <ModuleType> is missing its <BitResolution> node.");
    if (moduleType.find("freq-missing") != string::npos)
        throw invalid_argument("AcquisitionConfig::ParseModuleTypeNode : <ModuleType> is missing its <SamplingFrequency> node.");
    if (moduleType.find("rev-missing") != string::npos)
        throw invalid_argument("AcquisitionConfig::ParseModuleTypeNode : <ModuleType> is missing its <Revision> node.");

    StringManipulation::RemoveSpaces(moduleType);
    StringManipulation::ToLower(moduleType);

    return moduleType;
}

string AcquisitionConfig::ConfigFileName(const string &type, const string &str) {
    //If the file name starts with a '.' or a '/' then we assume the BaseDir should be ignored.
    if (str[0] == '.' || str[0] == '/')
        return str;

    string baseDir;
    if (type == "global") //If the file is a global type we use PixieBaseDir
        baseDir = Get("global", "PixieBaseDir");
    else { //Otherwise we try the ModuleBaseDir for the specified type and then the PixieBaseDir
        baseDir = Get(type, "ModuleBaseDir");
        if (baseDir.empty())
            baseDir = Get("global", "PixieBaseDir");
    }

    //No success so we assume they want the local directory.
    if (baseDir.empty())
        baseDir = ".";

    //Return the appended string.
    return baseDir + '/' + str;
}

std::string AcquisitionConfig::Get(const std::string &moduleType, const std::string &tag) {
    return configStrings_[moduleType][tag];
}

bool AcquisitionConfig::HasModuleConfig(const std::string &moduleType) {
    return configStrings_.find(moduleType) != configStrings_.end();
}