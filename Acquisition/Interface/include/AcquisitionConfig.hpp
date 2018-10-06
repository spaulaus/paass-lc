/// @file AcquisitionConfig.hpp
/// @brief Reads in and handles the acquisiton configuration file.
/// @author S. V. Paulauskas and K. Smith
/// @date August 11, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas.
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#ifndef ACQUISTIONCONFIG_HPP
#define ACQUISTIONCONFIG_HPP

#include "pugixml.hpp"

#include <map>
#include <set>
#include <vector>

///@TODO : Make this an abstract base class so that we can read / configure more than just Pixie modules.
class AcquisitionConfig {
public:
    /// The default constructor. It generates the list of valid nodes that the configuration cares about. Valid nodes
    /// are PixieBaseDir, DspSetFile, DspWorkingSetFile, Firmware, ModuleType, ModuleBaseDir, SpFpgaFile, ComFpgaFile,
    /// DspConfFile, DspVarFile, SlotDefinition, Pxisys
    AcquisitionConfig();

    /// Reads in a configuration file, processes the nodes. 
    /// @param[in] fileName : The file name (including path) of the configuration file
    /// @throw invalid_argument if PixieBaseDir is empty
    /// @throw invalid_argument if it encounters a node not listed validNodes_ (see constructor documentation)
    /// @throw invalid_argument if a node is empty
    /// @throw invalid_argument if it encounters a duplicate node.
    void ReadConfiguration(const std::string &fileName, const bool &shouldVerifyFiles = true);

    /// Get the value of a parameter from teh config.
    /// @param[in] moduleType : The type of module that you want to find the configuration for
    /// @param[in] tag : The specific tag that you want the value for.
    /// @return the value of the tag for the specified module type
    std::string Get(const std::string &moduleType, const std::string &tag);

    /// @return True if the provided module type was found
    /// @param[in] moduleType : The module type that we want to confirm exists.
    bool HasModuleConfig(const std::string &moduleType);

    /// Verifies that the files and paths generated during ReadConfiguration actually exist. Helps the user verify
    /// that the configuration file is valid.
    /// @throws invalid_argument if the files couldn't be located with the provided paths.
    void VerifyFiles();

    /// @return the number of modules read in from the configuration
    unsigned short GetNumberOfModules() const;

    /// @return the number of channels read in from the configuration
    unsigned short GetNumberOfChannels() const;

    /// @returns the slot maps read in from the configuration
    std::map<unsigned int, std::map<short, short> > GetSlotMaps() const;

    std::vector<short unsigned int> GetSlotMapAsVector(const unsigned int &crateNum) const;

    unsigned int GetSlotNumber(const unsigned int &crateNumber, const unsigned int &moduleNumber);
private:
    unsigned short numberOfModules_; //!< The number of modules defined in the config.
    unsigned short numberOfChannels_; //!< The number of channels in a Pixie16 module.

    /// Parses the input from configuration file for the ModuleType tag.
    /// @param[in] parent : The ModuleType node that we're going to be parsing.
    /// @return The module type in the following format : <bit resolution>b<sampling frequency>m-<revision>. Ex. 12b250m-revf
    std::string ParseModuleTypeNode(const pugi::xml_node &parent);

    /// Parses the firmware node by calling ParseModuleTypeNode to find the module type, then processing the files
    /// and paths.
    /// @param[in] node : The Firmware node that we want to parse
    /// @throw invalid_argument if the module type is duplicated
    /// @throw invalid_argument if any child is empty
    /// @throw invalid_argument if any child is duplicated
    void ParseFirmwareNode(const pugi::xml_node &node);

    /// Parses the slot definition node and ensures that the user didn't make any silly mistakes
    /// @param[in] node : The SlotDefinition node that we will parse.
    /// @throw invalid_argument if no pxisys path is specified
    /// @throw invalid_argument if more than one pxisys path is specified
    /// @throw invalid_argument if any child is duplicated
    void ParseSlotDefinitionNode(const pugi::xml_node &node);

    /// Determines the maximum number of modules for a crate based on the pxisys file name. We assume that
    /// pxisys.ini will always refer to a 14 slot crate.
    /// @param[in] pxisysPath : the pxisys file that we're goign to examine.
    /// @return the number of modules parsed from the file name.
    unsigned int DetermineMaximumNumberOfModules(const std::string &pxisysPath);

    /// Convert a configuration string to be relative to PixieBaseDir or ModuleBaseDir unless it begins with a . or /
    /// @param[in] type : The type of configuration we want, usually either global or moduleType
    /// @param[in] fileName : the name of the tile that we need to append the file path to.
    /// @return the generated file path.
    std::string ConfigFileName(const std::string &type, const std::string &fileName);

    /// Generates a message to throw if we found a duplicate entry
    /// @param[in] methodName : The name of the method that found the duplicate
    /// @param[in] duplicateName : The name of the duplicate entry.
    /// @return The message that we'll be throwing.
    std::string DuplicateEntryMessage(const std::string &methodName, const std::string &duplicateName);

    std::set<std::string> validNodes_; //!< Contains a list of nodes that we are expecting.
    std::map<std::string, std::map<std::string, std::string> > configStrings_; //!< Map containing the configuration parameters
    std::map<unsigned int, std::map<short, short> > slotMap_; //!< The slot maps parsed from the config
    std::map<unsigned int, std::string> pxisysMap_; //!< Map containing the pxisys paths for the crates.
};

#endif //ACQUISTIONCONFIG_HPP