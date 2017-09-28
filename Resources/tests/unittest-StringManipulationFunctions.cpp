///@file unittest-StringManipulationFunctions.cpp
///@brief A unit tests for StringManipulationFunctions header functions
///@author S. V. Paulauskas
///@date February 09, 2017
#include <UnitTest++.h>

#include "StringManipulationFunctions.hpp"

using namespace std;
using namespace StringManipulation;

TEST(TestStringTokenizer) {
    string input = "We tokenize on spaces.";
    string delim = " ";
    vector<string> expected = { "We", "tokenize", "on", "spaces." };
    CHECK_ARRAY_EQUAL(expected, TokenizeString(input, delim), expected.size());
}

TEST(TestStringToBool) {
    CHECK(!StringToBool("false"));
    CHECK(!StringToBool("0"));

    CHECK(StringToBool("true"));
    CHECK(StringToBool("1"));
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}