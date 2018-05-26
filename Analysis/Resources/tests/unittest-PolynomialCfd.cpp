///@file unittest-PolynomialCfd.cpp
///@author S. V. Paulauskas
///@date December 12, 2016
#include "PolynomialCfd.hpp"
#include "TimingConfiguration.hpp"
#include "UnitTestSampleData.hpp"

#include <UnitTest++.h>

#include <stdexcept>

using namespace std;
using namespace unittest_trace_variables;
using namespace unittest_cfd_variables;


TEST_FIXTURE(PolynomialCfd, TestPolynomialCfd) {
    TimingConfiguration cfg;
    cfg.SetFraction(polynomial::fraction);
    cfg.SetDelay(polynomial::delay);

    CHECK_THROW(CalculatePhase(empty_vector_double, cfg, max_pair, baseline_pair), range_error);
    CHECK_THROW(CalculatePhase(trace_sans_baseline, cfg, make_pair(trace_sans_baseline.size() + 3, 100),
                               baseline_pair), range_error);
    CHECK_CLOSE(polynomial::phase, CalculatePhase(trace_sans_baseline, cfg, extrapolated_maximum_pair, baseline_pair), 5);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}