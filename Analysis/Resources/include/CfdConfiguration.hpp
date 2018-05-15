/// @file CfdConfiguration.hpp
/// @brief
/// @author S. V. Paulauskas
/// @date May 15, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License

#ifndef PAASS_LC_CFDCONFIGURATION_HPP
#define PAASS_LC_CFDCONFIGURATION_HPP

///@brief Configuration settings for the CFDs. We use this class because it has more transparency than an STL
/// tuple, set, map, etc.We can extend this class to handle more variation if necessary. We need to have mixed
/// data types, which is not possible with a map.
class CfdConfiguration {
public:
    /// Default Constructor
    CfdConfiguration();

    /// Default Destructor
    ~CfdConfiguration();

    ///@returns delay_
    unsigned int GetDelay();

    ///@returns fraction_
    double GetFraction();

    ///@returns gap_
    unsigned int GetGap();

    ///@returns length_
    unsigned int GetLength();

    ///Sets delay_
    ///@param[in] a : The value for delay_
    void SetDelay(const unsigned int &a);

    ///Sets fraction_
    ///@param[in] a : The value for fraction_
    void SetFraction(const double &a);

    ///Sets gap_
    ///@param[in] a : The value for gap_
    void SetGap(const unsigned int &a);

    ///Sets length_
    ///@param[in] a : The value for length_
    void SetLength(const unsigned int &a);

private:
    unsigned int delay_; //!< The delay to delay the CFD
    double fraction_; //!< The fraction used to scale the CFD
    unsigned int gap_; //!< The trapezoidal filter gap for XIA CFDs
    unsigned int length_; //!< The trapezoidal filter length parameter for XIA CFDs
};

#endif //PAASS_LC_CFDCONFIGURATION_HPP