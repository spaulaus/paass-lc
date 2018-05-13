///@file unittest-TraditionalCfd.cpp
///@author S. V. Paulauskas
///@date December 12, 2016
#include "TraditionalCfd.hpp"
#include "UnitTestSampleData.hpp"

#include <UnitTest++.h>

#include <stdexcept>

using namespace std;
using namespace unittest_trace_variables;
using namespace unittest_cfd_variables;

TEST_FIXTURE(TraditionalCfd, TestTraditionalCfd) {
    CHECK_THROW(CalculatePhase(empty_vector_double, cfdTestPars, max_pair, baseline_pair), range_error);
    //CHECK_CLOSE(traditionalCfdPhase, CalculatePhase(trace_sans_baseline, cfdTestPars, max_pair, baseline_pair), 0
    // .001);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}