///@file unittest-HelperFunctions.cpp
///@author S. V. Paulauskas
///@date December 12, 2016
#include <UnitTest++.h>

#include "HelperFunctions.hpp"
#include "UnitTestSampleData.hpp"

using namespace std;
using namespace unittest_trace_variables;
using namespace unittest_helper_functions;

TEST(TestTrapezoidalFiltering) {
    CHECK_THROW(Filtering::TrapezoidalFilter(empty_vector_double, 0, 0), invalid_argument);
    CHECK_THROW(Filtering::TrapezoidalFilter(trace_sans_baseline, trace_sans_baseline.size() + 10, 2), invalid_argument);
    CHECK_THROW(Filtering::TrapezoidalFilter(trace_sans_baseline, 4, trace_sans_baseline.size() + 10), invalid_argument);
    CHECK_ARRAY_CLOSE(filteredTrace, Filtering::TrapezoidalFilter(trace_sans_baseline, length, gap),
                      filteredTrace.size(), 0.1);
}

TEST(TestCalculateSlopeAndIntercept) {
    auto result = Polynomial::CalculateSlope(xy1, xy2);
    CHECK_EQUAL(slope, result);
    CHECK_EQUAL(intercept, Polynomial::CalculateYIntercept(xy1, slope));
}

///This tests that the TraceFunctions::CalculateBaseline function works as
/// expected. This also verifies the Statistics functions CalculateAverage
/// and CalculateStandardDeviation
TEST(TestCalculateBaseline) {
    CHECK_THROW(TraceFunctions::CalculateBaseline(trace, make_pair(0, 1)), range_error);
    CHECK_THROW(TraceFunctions::CalculateBaseline(empty_vector_uint, make_pair(0, 16)), range_error);
    CHECK_THROW(TraceFunctions::CalculateBaseline(trace, make_pair(17, 1)), range_error);
    CHECK_THROW(TraceFunctions::CalculateBaseline(trace, make_pair(0, trace.size() + 100)), range_error);

    pair<double, double> result = TraceFunctions::CalculateBaseline(trace, make_pair(0, 70));
    CHECK_CLOSE(baseline, result.first, 1e-7);
    CHECK_CLOSE(standard_deviation, result.second, 1e-9);
}

TEST(TestFindMaxiumum) {
    CHECK_THROW(TraceFunctions::FindMaximum(empty_vector_uint, trace_delay), range_error);
    CHECK_THROW(TraceFunctions::FindMaximum(trace, trace.size() + 100), range_error);
    CHECK_THROW(TraceFunctions::FindMaximum(trace, 5), range_error);
    CHECK_THROW(TraceFunctions::FindMaximum(const_vector_uint, trace_delay), range_error);

    pair<unsigned int, double> result = TraceFunctions::FindMaximum(trace, trace_delay);
    CHECK_EQUAL(max_position, result.first);
    CHECK_EQUAL(maximum_value, result.second);
}


TEST(TestFindLeadingEdge) {
    CHECK_THROW(TraceFunctions::FindLeadingEdge(trace, bad_fraction, max_pair), range_error);
    CHECK_THROW(TraceFunctions::FindLeadingEdge(empty_vector_uint, leading_edge_fraction, max_pair), range_error);
    CHECK_THROW(TraceFunctions::FindLeadingEdge(trace, leading_edge_fraction, make_pair(trace.size() + 10, 3.)),
                range_error);

    ///@TODO We still need to fix this function so that it works properly
    CHECK_EQUAL(leading_edge_position, TraceFunctions::FindLeadingEdge(trace, leading_edge_fraction, max_pair));
}

TEST(TestCalculatePoly3) {
    CHECK_THROW(Polynomial::CalculatePoly3(empty_vector_uint, 0), range_error);

    pair<double, vector<double> > result = Polynomial::CalculatePoly3(poly3_data, 0);

    CHECK_ARRAY_CLOSE(poly3_coeffs, result.second, poly3_coeffs.size(), 1e-6);
    CHECK_CLOSE(extrapolated_maximum, result.first, 1e-6);
}

//For determination of the extrapolated maximum value of the trace. This trace
// favors the left side since f(max+1) is less than f(max - 1).
TEST(TestExtrapolateMaximum) {
    CHECK_THROW(TraceFunctions::ExtrapolateMaximum(empty_vector_uint, max_pair), range_error);

    pair<double, vector<double> > result = TraceFunctions::ExtrapolateMaximum(trace, max_pair);

    CHECK_CLOSE(extrapolated_maximum, result.first, 1e-6);
    CHECK_ARRAY_CLOSE(extrapolated_max_coeffs, result.second, extrapolated_max_coeffs.size(), 1e-3);
}

TEST(TestCalculatePoly2) {
    CHECK_THROW(Polynomial::CalculatePoly2(empty_vector_uint, 0), range_error);

    pair<double, vector<double> > result = Polynomial::CalculatePoly2(poly2_data, 0);

    CHECK_ARRAY_CLOSE(poly2_coeffs, result.second, poly2_coeffs.size(), 1e-3);
    CHECK_CLOSE(poly2_val, result.first, 1e-4);
}

TEST(TestCalculateIntegral) {
    CHECK_THROW(Statistics::CalculateIntegral(empty_vector_uint), range_error);
    CHECK_EQUAL(integral, Statistics::CalculateIntegral(integration_data));
}

TEST(TestCalculateQdc) {
    CHECK_THROW(TraceFunctions::CalculateQdc(empty_vector_uint, make_pair(0, 4)), range_error);
    CHECK_THROW(TraceFunctions::CalculateQdc(trace, make_pair(0, trace.size() + 10)), range_error);
    CHECK_THROW(TraceFunctions::CalculateQdc(trace, make_pair(1000, 0)), range_error);
    CHECK_EQUAL(integration_qdc, TraceFunctions::CalculateQdc(integration_data, qdc_pair));
}

TEST(TestCalculateTailRatio) {
    CHECK_THROW(TraceFunctions::CalculateTailRatio(empty_vector_uint, make_pair(0, 4), 100.0), range_error);
    CHECK_THROW(TraceFunctions::CalculateTailRatio(trace, make_pair(0, trace.size() + 10), 100.0), range_error);
    CHECK_THROW(TraceFunctions::CalculateTailRatio(trace, make_pair(0, 4), 0.0), range_error);

    double qdc = TraceFunctions::CalculateQdc(trace, make_pair(70, 91));
    pair<unsigned int, unsigned int> range(80, 91);
    double result = TraceFunctions::CalculateTailRatio(trace, range, qdc);
    CHECK_CLOSE(tail_ratio, result, 1e-6);
}

TEST(TestIeeeFloatingOperations) {
    unsigned int input = 1164725159;
    double expected = 3780.7283;
    CHECK_CLOSE(expected, IeeeStandards::IeeeFloatingToDecimal(input), 1e-4);
    CHECK_CLOSE(input, IeeeStandards::DecimalToIeeeFloating(expected), 1);
    CHECK_CLOSE((unsigned)1034818683, IeeeStandards::DecimalToIeeeFloating(0.085), 1);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}
