///\file unittest-GslFitter.cpp
///\brief A small code to test the functionality of the FitDriver
///\author S. V. Paulauskas
///\date August 8, 2016
#include "GslFitter.hpp"

#include "UnitTestSampleData.hpp"

#include <UnitTest++.h>

#include <iostream>
#include <stdexcept>

using namespace std;
using namespace unittest_fit_variables;

TEST_FIXTURE(GslFitter, TestDataVectorValidation) {
    using namespace unittest_trace_variables;
    CHECK_THROW(CalculatePhase(empty_vector_double, pmtParameters, max_pair, baseline_pair), range_error);
}

TEST_FIXTURE(GslFitter, TestPmtFitting) {
    using namespace unittest_trace_variables;
    SetQdc(waveform_qdc);
    CHECK_CLOSE(phase, CalculatePhase(waveform, pmtParameters, max_pair, baseline_pair), 0.5);
}

TEST_FIXTURE(GslFitter, TestGaussianFitting) {
    using namespace unittest_gaussian_trace;
    SetQdc(qdc);
    SetIsFastSiPm(true);
    CHECK_CLOSE(phase, CalculatePhase(waveform, gaussianParameters, maxPair, baselinePair), 0.1);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}