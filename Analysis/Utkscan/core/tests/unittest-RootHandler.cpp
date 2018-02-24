/// @file unittest-RootHandler.cpp
/// @brief Unittests for the RootHandler class.
/// @author S. V. Paulauskas
/// @date February 24, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include "RootHandler.hpp"

#include <UnitTest++.h>

TEST(TestRootHandler) {
    RootHandler::get("/tmp/unittest-RootHandler.root");

}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}
