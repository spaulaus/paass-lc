///@file TraditionalCfd.cpp
///@brief Traditional CFD implemented digitally, similar behavior to a NIM Module.
///@author S. Padgett and S. V. Paulauskas
///@date July 22, 2011
#include "TraditionalCfd.hpp"

#include "HelperFunctions.hpp"
#include "TimingConfiguration.hpp"

using namespace std;

double TraditionalCfd::CalculatePhase(const std::vector<double> &data, const TimingConfiguration &cfg) {
    if (data.empty())
        throw range_error("TraditionalCfd::CalculatePhase - The data vector was empty!");

    cfd_.clear();
    for (unsigned int i = 0; i < data.size() - cfg.GetDelay(); i++)
        cfd_.push_back(cfg.GetDelay() * (data[i] - data[i + cfg.GetDelay()]));

    pair<double, double> xyBelowZero(0, 0);
    pair<double, double> xyAboveZero(0, 0);

    auto cfdMinPosition = min_element(cfd_.begin(), cfd_.end()) - cfd_.begin();
    auto cfdMaxPosition = max_element(cfd_.begin(), cfd_.end()) - cfd_.begin();

    for (signed long i = cfdMinPosition; i < cfdMaxPosition; i++) {
        if(cfd_.at(i) > 0) {
            xyBelowZero.first = i - 1;
            xyBelowZero.second = cfd_.at(i - 1);
            xyAboveZero.first = i;
            xyAboveZero.second = cfd_.at(i);
            break;
        }
    }

    double slope = Polynomial::CalculateSlope(xyBelowZero, xyAboveZero);

    return - Polynomial::CalculateYIntercept(xyAboveZero, slope) / slope;
}

vector<double> TraditionalCfd::GetCfd() { return cfd_; }