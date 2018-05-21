/// @file TimingConfiguration.hpp
/// @brief
/// @author S. V. Paulauskas
/// @date May 15, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License

#ifndef PAASS_LC_TIMINGCONFIGURATION_HPP
#define PAASS_LC_TIMINGCONFIGURATION_HPP

///@brief Configuration settings for the CFDs. We use this class because it has more transparency than an STL
/// tuple, set, map, etc.We can extend this class to handle more variation if necessary. We need to have mixed
/// data types, which is not possible with a map.
class TimingConfiguration {
public:
    /// Default Constructor
    TimingConfiguration();

    /// Default Destructor
    ~TimingConfiguration();

    ///@returns beta_;
    double GetBeta() const;

    ///@returns delay_
    unsigned int GetDelay() const;

    ///@returns fraction_
    double GetFraction() const;

    ///@returns gamma_
    double GetGamma() const;

    ///@returns gap_
    unsigned int GetGap() const;

    ///@returns length_
    unsigned int GetLength() const;

    ///@returns qdc_
    double GetQdc() const;

    ///@returns true if isFastSiPm_ is true
    bool IsFastSiPm() const;

    ///Sets beta_
    ///@param[in] a : The value for beta_
    void SetBeta(const double &a);

    ///Sets delay_
    ///@param[in] a : The value for delay_
    void SetDelay(const unsigned int &a);

    ///Sets fraction_
    ///@param[in] a : The value for fraction_
    void SetFraction(const double &a);

    ///Sets gamma_
    ///@param[in] a : The value for gamma_
    void SetGamma(const double &a);

    ///Sets gap_
    ///@param[in] a : The value for gap_
    void SetGap(const unsigned int &a);

    ///Sets the isFastSiPm_ flag
    ///@param[in] a : The value that we are going to set
    void SetIsFastSiPm(const bool &a);

    ///Sets length_
    ///@param[in] a : The value for length_
    void SetLength(const unsigned int &a);

    /// Sets the QDC that we want to set
    /// \param[in] a the qdc of the waveform for the fit
    void SetQdc(const double &a);

private:
    double beta_; //!< The beta value used for fitting
    unsigned int delay_; //!< The delay to delay the CFD
    double fraction_; //!< The fraction used to scale the CFD
    double gamma_; //!< The gamma value used for fitting
    unsigned int gap_; //!< The trapezoidal filter gap for XIA CFDs
    bool isFastSiPm_; //!< True if we want to analyze signals from SiPM fast outputs
    unsigned int length_; //!< The trapezoidal filter length parameter for XIA CFDs
    double qdc_;//!< qdc of the waveform being fitted
};

#endif //PAASS_LC_TIMINGCONFIGURATION_HPP