/// @file unittest-RootHandler.cpp
/// @brief Unittests for the RootHandler class.
/// @author S. V. Paulauskas
/// @date February 24, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include "RootHandler.hpp"

#include <UnitTest++.h>

#include <chrono>
#include <random>

TEST(TestRootHandler) {
    RootHandler *handler = RootHandler::get("/tmp/unittest-RootHandler");
    CHECK(handler);

    CHECK_EQUAL("TH1D", handler->RegisterHistogram(0, "test1d", 10)->ClassName());
    CHECK_EQUAL("TH2D", handler->RegisterHistogram(1, "test2d-xy", 10, 10)->ClassName());
    CHECK_EQUAL("TH2D", handler->RegisterHistogram(2, "test2d-xz", 10, 0, 10)->ClassName());
    CHECK_EQUAL("TH3D", handler->RegisterHistogram(3, "test3d", 10, 10, 10)->ClassName());

    CHECK(!handler->Plot(123,123));
    CHECK_THROW(handler->Get1DHistogram(123), std::invalid_argument);
    CHECK_THROW(handler->Get2DHistogram(123), std::invalid_argument);
    CHECK_THROW(handler->Get3DHistogram(123), std::invalid_argument);

    unsigned int dummy = 0;
    CHECK_THROW(handler->RegisterBranch("notatree", "branchname", &dummy, "leaf list"), std::invalid_argument);

    delete RootHandler::get();
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}
