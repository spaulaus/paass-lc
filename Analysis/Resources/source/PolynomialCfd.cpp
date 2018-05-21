/// @file PolynomialCfd.cpp
/// @brief Timing method that calculates the timing using a Polynomial based CFD.
/// @author C. R. Thornsberry and S. V. Paulauskas
/// @date December 6, 2016
#include "PolynomialCfd.hpp"

#include "HelperFunctions.hpp"
#include "TimingConfiguration.hpp"

using namespace std;

PolynomialCfd::PolynomialCfd() = default;
PolynomialCfd::~PolynomialCfd() = default;

/// Perform CFD analysis on the waveform.
double PolynomialCfd::CalculatePhase(const std::vector<double> &data, const TimingConfiguration &cfg,
                                     const std::pair<unsigned int, double> &max,
                                     const std::pair<double, double> baseline) {
    if (data.size() == 0)
        throw range_error("PolynomialCfd::CalculatePhase - The data vector was empty!");
    if (data.size() < max.first)
        throw range_error("PolynomialCfd::CalculatePhase - The maximum position is larger than the size of the data vector.");

    double phase = std::numeric_limits<int>::min();
    double multiplier = 1.;

    vector<double> fitCoefficients;
    for (unsigned int cfdIndex = max.first; cfdIndex > 0; cfdIndex--) {
        if (data[cfdIndex - 1] < cfg.GetFraction() && data[cfdIndex] >= cfg.GetFraction()) {
            // Fit the rise of the trace to a 2nd order polynomial.
            fitCoefficients = Polynomial::CalculatePoly2(data, cfdIndex - 1).second;

            // Calculate the phase of the trace.
            if (fitCoefficients[2] > 1)
                multiplier = -1.;

            phase = (-fitCoefficients[1] + multiplier * sqrt(fitCoefficients[1] * fitCoefficients[1]
                    - 4 * fitCoefficients[2] * (fitCoefficients[0] - cfg.GetFraction())))
                    / (2 * fitCoefficients[2]);

            break;
        }
    }
    return phase;
}