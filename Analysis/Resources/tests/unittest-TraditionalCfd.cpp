///@file unittest-TraditionalCfd.cpp
///@author S. V. Paulauskas
///@date December 12, 2016
#include "TimingConfiguration.hpp"

#include "TraditionalCfd.hpp"
#include "UnitTestSampleData.hpp"

#include <UnitTest++.h>

#include <stdexcept>

using namespace std;
using namespace unittest_trace_variables;
using namespace unittest_cfd_variables::traditional;

TEST_FIXTURE(TraditionalCfd, TestTraditionalCfd) {
    TimingConfiguration cfg;
    cfg.SetFraction(fraction);
    cfg.SetDelay(delay);
    CHECK_THROW(CalculatePhase(empty_vector_double, cfg), range_error);
    CHECK_CLOSE(phase, CalculatePhase(trace_sans_baseline, cfg), 0.001);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}