/// @file RootFitter.hpp
/// @brief Class to handle fitting traces using ROOT
/// @author S. V. Paulauskas
/// @date December 18, 2016
#ifndef _PIXIESUITE_ROOTFITTER_HPP_
#define _PIXIESUITE_ROOTFITTER_HPP_

#include "TimingDriver.hpp"

class TF1;
class VandleTimingFunction;
class TimingConfiguration;

class RootFitter : public TimingDriver {
public:
    RootFitter();

    ~RootFitter();

    /// Perform fitting analysis using ROOT
    double CalculatePhase(const std::vector<double> &data, const TimingConfiguration &cfg,
                          const std::pair<unsigned int, double> &maxInfo, std::pair<double, double> baseline);

private:
    TF1 *func_;
    VandleTimingFunction *vandleTimingFunction_;
};


#endif //#ifndef _PIXIESUITE_ROOTFITTER_HPP_
