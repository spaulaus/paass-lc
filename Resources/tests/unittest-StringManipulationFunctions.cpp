///@file unittest-StringManipulationFunctions.cpp
///@brief A unit tests for StringManipulationFunctions header functions
///@author S. V. Paulauskas
///@date February 09, 2017
#include "StringManipulationFunctions.hpp"

#include <UnitTest++.h>

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
    CHECK(!StringToBool("FALSE"));
    CHECK(!StringToBool("0"));
    CHECK(!StringToBool("No"));
    CHECK(!StringToBool("no"));

    CHECK(StringToBool("true"));
    CHECK(StringToBool("TRUE"));
    CHECK(StringToBool("YES"));
    CHECK(StringToBool("yes"));
    CHECK(StringToBool("Yes"));
    CHECK(StringToBool("1"));
}

TEST(TestBoolToString) {
    CHECK_EQUAL("Yes", BoolToString(true));
    CHECK_EQUAL("No", BoolToString(false));
}

TEST(TestPadString) {
    string delimiter = ".";
    string message = "success";
    unsigned int desiredLength = 20;
    string expectedFront = ".............success";
    string expectedBack = "success.............";

    CHECK_THROW(PadString(expectedFront, delimiter, 3), length_error);
    CHECK_THROW(PadString(expectedBack, expectedFront, 40), invalid_argument);
    CHECK_EQUAL(expectedFront, PadString(message, delimiter, desiredLength));
    CHECK_EQUAL(expectedBack, PadString(message, delimiter, desiredLength, false));
}

TEST(TestFormatHumanReadableSizes) {
    double bytes = 10;
    CHECK_EQUAL("10 B", FormatHumanReadableSizes(bytes));

    double kilobytes = 3000;
    CHECK_EQUAL("2.93 kB", FormatHumanReadableSizes(kilobytes));

    double megabytes = 1289000;
    CHECK_EQUAL("1.23 MB", FormatHumanReadableSizes(megabytes));

    double gigabytes = 8339000000;
    CHECK_EQUAL("7.77 GB", FormatHumanReadableSizes(gigabytes));
}

TEST(TestIsNumeric) {
    CHECK(IsNumeric("123456789"));
    CHECK(IsNumeric("-123456789"));
    CHECK(IsNumeric("12345.6789"));
    CHECK(IsNumeric("-12345.6789"));
    CHECK(!IsNumeric("123ABC456DEF"));
    CHECK(!IsNumeric("123abc"));
    CHECK(!IsNumeric("qwerty"));
    CHECK(!IsNumeric("!@#$%"));
    CHECK(!IsNumeric("1.34E-34"));
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}