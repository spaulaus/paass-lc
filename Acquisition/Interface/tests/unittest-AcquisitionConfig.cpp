/// @file unittest-AcquisitionConfig.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date August 11, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include "AcquisitionConfig.hpp"

#include <UnitTest++.h>

#include <iostream>
#include <stdexcept>
#include <string>

TEST_FIXTURE(AcquisitionConfig, TestAcquisitionConfig) {
    try {
        ReadConfiguration("/tmp/config.xml");
    } catch(std::invalid_argument &invalidArgument) {
        std::cout << invalidArgument.what() << std::endl;
    }
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}