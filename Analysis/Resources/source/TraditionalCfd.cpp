///@file TraditionalCfd.cpp
///@brief Traditional CFD implemented digitally, similar behavior to a NIM Module.
///@author S. V. Paulauskas
///@date July 22, 2011
#include "TraditionalCfd.hpp"

#include "HelperFunctions.hpp"

using namespace std;

double TraditionalCfd::CalculatePhase(const std::vector<double> &data, const std::pair<double, double> &pars,
                                      const std::pair<unsigned int, double> &max,
                                      const std::pair<double, double> baseline) {
    if (data.size() == 0)
        throw range_error("PolynomialCfd::CalculatePhase - The data vector was empty!");

    double fraction = pars.first;
    auto delay = (unsigned int) pars.second;

    for (unsigned int i = 0; i < data.size() - delay; i++)
        cfd_.push_back(fraction * (data[i] - data[i + delay]));

    pair<double, double> xyBelowZero(0, 0);
    pair<double, double> xyAboveZero(0, 0);

    auto cfdMinPosition = int(min_element(cfd_.begin(), cfd_.end()) - cfd_.begin());
    auto cfdMaxPosition = int(max_element(cfd_.begin(), cfd_.end()) - cfd_.begin());

    for (int i = cfdMinPosition; i < cfdMaxPosition; i++) {
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