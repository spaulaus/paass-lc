///@file StringManipulationFunctions.hpp
///@brief A collection of functions to aid in string manipulations.
///@author S. V. Paulauskas
///@date February 09, 2017
#ifndef PAASS_STRINGMANIPULATIONFUNCTIONS_HPP
#define PAASS_STRINGMANIPULATIONFUNCTIONS_HPP

#include <algorithm>
#include <iomanip>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cmath>

namespace StringManipulation {
    ///Converts string to bool (True, true, 1 and False, false, 0) are accepted; throws an exception if not successful. Notice
    /// tolower will work only with ascii, not with utf-8, but shouldn't be a problem for true and false words.
    ///@param [in] s : String to convert to bool
    ///@return A bool from the input string
    inline bool StringToBool(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s == "true" || s == "1" || s == "yes")
            return true;
        else if (s == "false" || s == "0" || s == "no")
            return false;
        else {
            std::stringstream ss;
            ss << "strings::to_bool: Could not convert string '" << s << "' to bool" << std::endl;
            throw std::invalid_argument(ss.str());
        }
    }

    ///Tokenizes the string, splitting it on a given delimiter, which are removed from returned vector of tokens.
    ///@param [in] str : The string to break up
    ///@param [in] delimiter : character to break up on
    ///@return The vector of tokens
    inline std::vector<std::string> TokenizeString(const std::string &str, const std::string &delimiter) {
        std::vector<std::string> tokenized;
        size_t current, next = -1;
        do {
            current = next + 1;
            next = str.find_first_of(delimiter, current);
            tokenized.push_back(str.substr(current, next - current));
        } while (next != std::string::npos);
        return tokenized;
    }

    ///Takes a bool value and turns it into Yes or No
    ///@param[in] val : The bool that we want to convert to Yes/No
    ///@return Yes if val is true and No otherwise
    inline std::string BoolToString(const bool &val) {
        if(val)
            return "Yes";
        return "No";
    }

    ///Prepends or Appends the provided delimiter to a string until it's the specified length
    ///@param[in] message : The string that we want to pad
    ///@param[in] delimiter : What we want to pad the string with
    ///@param[in] length : How long the resulting string should be
    ///@param[in] isPrepended : If we should prepend or append to the string, defaults to Prepend
    ///@return The message that's been prepended/appended with the delimiter to the desired length.
    inline std::string PadString(const std::string &message, const std::string &delimiter, const unsigned int &length,
                                 const bool &isPrepended = true) {
        if(message.size() > length)
            throw std::length_error("StringManipulation::PadString - The provided string (" + message + ") is longer "
                                           "than the desired length (" + std::to_string(length) + ")!!");
        if(delimiter.size() != 1)
            throw std::invalid_argument("StringManipulation::PadString - The delimiter must be a single character.");

        if(isPrepended)
            return std::string(length - message.size(), delimiter[0]) + message;
        return message + std::string(length - message.size(), delimiter[0]);
    }

    ///Reformats file sizes from bytes to human readable formats. Similar to the -h flag for ls.
    ///@param[in] size : The number of bytes that we have to reformat
    ///@return The number of bytes formatted in a human readable format.
    inline std::string FormatHumanReadableSizes(const double &size) {
        unsigned int power = std::log10(size);
        std::stringstream output;
        output << std::setprecision(3);
        if (power >= 9)
            output << size / std::pow(1024,3) << " GB";
        else if (power >= 6)
            output << size / std::pow(1024,2) << " MB";
        else if (power >= 3)
            output << size / 1024 << " kB";
        else
            output << size << " B";
        return output.str();
    }

    /// IsNumeric: Check if an input string is strictly numeric. NOTE : This will not work on numbers using
    /// scientific notation!!
    /// @param[in] input_ : String to check.
    /// @param[in] prefix_ : String to print before the error message is printed.
    /// @param[in] msg_ : Error message to print if the value is not numeric.
    /// @return True if the string is strictly numeric and false otherwise.
    inline bool IsNumeric(const std::string &input){
        return std::regex_match(input, std::regex( ( "((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?" ) ) );
    }

    /// Removes spaces from the provided string. We modify the string directly so nothing to return.
    /// @param[in] input : The string that we want to remove spaces from.
    inline void RemoveSpaces(std::string &input) {
        input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
        input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
    }

    /// Converts all letters in a string to lower case. We modify the string directly so nothing to return.
    /// @param[in] input : The string that we want to convert.
    inline void ToLower(std::string &input) {
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    }
}

#endif //PAASS_STRINGMANIPULATIONFUNCTIONS_HPP
