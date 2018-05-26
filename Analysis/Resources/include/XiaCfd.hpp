///@file TraditionalCfd.hpp
///@brief Approximation of the CFD algorithm implemented by XIA LLC on the Pixie-16 modules
///@author S. V. Paulauskas
///@date July 22, 2011
#ifndef PAASS_LC_XIACFD_HPP
#define PAASS_LC_XIACFD_HPP

#include "TimingDriver.hpp"

class TimingConfiguration;

/// A class that approximates the XIA LLC CFD. This algorithm works directly on the trace and doesn't have a CFD
/// scaling factor. The method used to calculate the zero crossing point is identical.
class XiaCfd : public TimingDriver {
public:
    XiaCfd() {};

    ~XiaCfd() {};

    /// Calculates the phase using an approximated XIA CFD method.
    /// @param[in] pars : A pair containing (fraction, delay)
    double CalculatePhase(const std::vector<double> &data, const TimingConfiguration &cfg);

    std::vector<double> GetCfd();

private:
    std::vector<double> cfd_;
};

#endif //PAASS_LC_XIACFD_HPP