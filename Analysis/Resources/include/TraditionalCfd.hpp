///@file TraditionalCfd.hpp
///@brief Traditional CFD implemented digitally
///@author S. V. Paulauskas
///@date July 22, 2011

#ifndef PIXIESUITE_TRADITIONALCFD_HPP
#define PIXIESUITE_TRADITIONALCFD_HPP

#include "TimingDriver.hpp"

class TraditionalCfd : public TimingDriver {
public:
    /// Default constructor
    TraditionalCfd() {};

    /// Default destructor
    ~TraditionalCfd() {};

    /// Calculates the phase using a Traditional CFD method.
    /// @param[in] pars : A pair containing (fraction, delay)
    double CalculatePhase(const std::vector<double> &data, const std::pair<double, double> &pars,
                          const std::pair<unsigned int, double> &max, const std::pair<double, double> baseline);
    std::vector<double> GetCfd();
private:
    std::vector<double> cfd_;
};
#endif //PIXIESUITE_TRADITIONALCFD_HPP
