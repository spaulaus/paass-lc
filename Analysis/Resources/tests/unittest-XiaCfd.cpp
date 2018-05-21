///@file unittest-XiaCfd.cpp
///@author S. V. Paulauskas
///@date May 13, 2018
#include "XiaCfd.hpp"

#include "TimingConfiguration.hpp"
#include "UnitTestSampleData.hpp"

#include <UnitTest++.h>

#include <stdexcept>

using namespace std;
using namespace unittest_trace_variables;
using namespace unittest_cfd_variables::xia;

TEST_FIXTURE(XiaCfd, TestXiaCfd) {
    TimingConfiguration cfg;
    cfg.SetFraction(fraction);
    cfg.SetDelay(delay);
    cfg.SetGap(gap);
    cfg.SetLength(length);

    CHECK_THROW(CalculatePhase(empty_vector_double, cfg), range_error);
    CHECK_CLOSE(phase, CalculatePhase(trace_sans_baseline, cfg), 0.001);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}