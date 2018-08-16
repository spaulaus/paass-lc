#ifndef ACQUISTIONCONFIG_HPP
#define ACQUISTIONCONFIG_HPP

#include <map>
#include <set>
#include <pugixml.hpp>

class AcquisitionConfig {
public:
    AcquisitionConfig();

    void ReadConfiguration(const std::string &fileName);

    std::string Get(const std::string &moduleType, const std::string &tag);

    bool HasModuleConfig(const std::string &moduleType);

    void VerifyFiles();

    unsigned short GetNumberOfModules(void) const { return numberOfModules_; };

    unsigned short GetNumberOfChannels(void) const { return numberOfChannels_; }
private:
    unsigned short numberOfModules_;
    unsigned short numberOfChannels_;

    /// @brief Parses the input from configuration file for the ModuleType tag.
    std::string ParseModuleTypeNode(const pugi::xml_node &parent);

    void ParseFirmwareNode(const pugi::xml_node &node);

    void ParseSlotDefinitionNode(const pugi::xml_node &node);

    unsigned int DetermineMaximumNumberOfModules(const std::string &pxisysPath);

    /// @brief Convert a configuration string to be relative to PixieBaseDir unless it begins with a .
    std::string ConfigFileName(const std::string &type, const std::string &str);

    std::string DuplicateEntryMessage(const std::string &methodName, const std::string &duplicateName);

    std::set<std::string> validNodes_;
    std::map<std::string, std::map<std::string, std::string> > configStrings_;
    std::map<unsigned int, std::map<unsigned int, unsigned int> > slotMap_;
    std::map<unsigned int, std::string> pxisysMap_;
};

#endif //ACQUISTIONCONFIG_HPP