///@file XiaCfd.cpp
///@brief Approximation of the CFD algorithm implemented by XIA LLC on the Pixie-16 modules
///@author S. V. Paulauskas
///@date July 22, 2011
///@date Rewritten May 13, 2018
#include "XiaCfd.hpp"

#include "HelperFunctions.hpp"
#include "TimingConfiguration.hpp"

#include <algorithm>
#include <stdexcept>

using namespace std;

double XiaCfd::CalculatePhase(const std::vector<double> &data, const TimingConfiguration &cfg) {
    if (data.empty())
        throw range_error("XiaCfd::CalculatePhase - The data vector was empty!");

    cfd_.clear();
    cfd_.resize(data.size(), 0.0);

    auto filter = Filtering::TrapezoidalFilter(data, cfg.GetLength(), cfg.GetGap());
    for (unsigned int i = 0; i < filter.size() - cfg.GetDelay(); i++)
        cfd_[i + cfg.GetDelay()] = filter[i + cfg.GetDelay()] - filter[i] / pow(2, cfg.GetFraction() + 1);

    for (auto i = max_element(cfd_.begin(), cfd_.end()) - cfd_.begin(); i <= min_element(cfd_.begin(), cfd_.end()) - cfd_.begin(); i++)
        if (cfd_.at(i) <= 0.0)
            return cfd_.at(i - 1) / (cfd_.at(i - 1) + fabs(cfd_.at(i)));
    return 0.0;
}