#ifndef ACQUISTIONCONFIG_HPP
#define ACQUISTIONCONFIG_HPP

#include <map>
#include <set>

class AcquisitionConfig {
public:
    AcquisitionConfig();

    bool ReadConfigFile(const char *cfgFile);

    std::string Get(std::string moduleType, std::string tag);

private:
    /// @brief Parses the input from configuration file for the ModuleType tag.
    std::string ParseModuleTypeTag(std::string value);

    /// @brief Convert a configuration string to be relative to PixieBaseDir unless it begins with a .
    std::string ConfigFileName(const std::string &type, const std::string &str);

    std::set<std::string> validConfigKeys;
    std::map<std::string, std::map<std::string, std::string>> configStrings;
};

#endif //ACQUISTIONCONFIG_HPP