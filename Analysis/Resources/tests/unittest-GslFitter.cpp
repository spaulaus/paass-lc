///\file unittest-GslFitter.cpp
///\brief A small code to test the functionality of the FitDriver
///\author S. V. Paulauskas
///\date August 8, 2016
#include "GslFitter.hpp"

#include "TimingConfiguration.hpp"
#include "UnitTestSampleData.hpp"

#include <UnitTest++.h>

#include <iostream>
#include <stdexcept>

using namespace std;
using namespace unittest_fit_variables;
using namespace unittest_trace_variables;

TEST_FIXTURE(GslFitter, TestPmtFitting) {
    TimingConfiguration cfg;
    cfg.SetBeta(pmt::beta);
    cfg.SetGamma(pmt::gamma);
    cfg.SetQdc(waveform_qdc);
    cfg.SetIsFastSiPm(false);

    CHECK_THROW(CalculatePhase(empty_vector_double, cfg, max_pair, baseline_pair), range_error);
    CHECK_CLOSE(pmt::phase, CalculatePhase(waveform, cfg, max_pair, baseline_pair), 0.5);
}

TEST_FIXTURE(GslFitter, TestGaussianFitting) {
    TimingConfiguration cfg;
    cfg.SetBeta(gaussian::beta);
    cfg.SetGamma(gaussian::gamma);
    cfg.SetQdc(unittest_gaussian_trace::qdc);
    cfg.SetIsFastSiPm(true);
    CHECK_CLOSE(gaussian::phase, CalculatePhase(unittest_gaussian_trace::waveform, cfg, max_pair, baseline_pair), 0.1);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}