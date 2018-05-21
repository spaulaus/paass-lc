/// @file unittest-RootFitter.cpp
/// @brief Unit tests for the RootFitter class
/// @author S. V. Paulauskas
/// @date December 18, 2016
#include "RootFitter.hpp"
#include "TimingConfiguration.hpp"
#include "UnitTestSampleData.hpp"

#include <UnitTest++.h>

#include <iostream>
#include <stdexcept>

using namespace std;
using namespace unittest_trace_variables;
using namespace unittest_fit_variables;

TEST_FIXTURE(RootFitter, TestRootFitter) {
    TimingConfiguration cfg;
    cfg.SetBeta(pmt::beta);
    cfg.SetGamma(pmt::gamma);

    CHECK_THROW(CalculatePhase(empty_vector_double, cfg, max_pair, baseline_pair), range_error);

    cfg.SetQdc(waveform_qdc);
    CHECK_CLOSE(-0.581124, CalculatePhase(waveform, cfg, max_pair, baseline_pair), 1.);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}