/// @file unittest-AcquisitionConfig.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date August 11, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include "AcquisitionConfig.hpp"

#include <UnitTest++.h>

#include <stdexcept>

TEST_FIXTURE(AcquisitionConfig, TestAcquisitionConfig) {
    CHECK_THROW(ReadConfiguration("test-xml-files/missing-bit.xml"), std::invalid_argument);
    CHECK_THROW(ReadConfiguration("test-xml-files/missing-frequency.xml"), std::invalid_argument);
    CHECK_THROW(ReadConfiguration("test-xml-files/missing-module-type.xml"), std::invalid_argument);
    CHECK_THROW(ReadConfiguration("test-xml-files/missing-pixie-base-dir.xml"), std::invalid_argument);
    CHECK_THROW(ReadConfiguration("test-xml-files/missing-revision.xml"), std::invalid_argument);
    CHECK_THROW(ReadConfiguration("test-xml-files/missing-firmware-child.xml"), std::invalid_argument);
    CHECK_THROW(ReadConfiguration("test-xml-files/too-many-modules.xml"), std::invalid_argument);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}