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
    RootHandler *handler = RootHandler::get("/tmp/unittest-RootHandler.root");
    CHECK(handler);

    unsigned int id0 = 0;
    unsigned int id1 = 1;
    unsigned int id2 = 2;
    unsigned int id3 = 3;

    handler->RegisterHistogram(id0, "test1d", 10);
    handler->RegisterHistogram(id1, "test2d-xy", 10, 10);
    handler->RegisterHistogram(id2, "test2d-xz", 10, 0, 10);
    handler->RegisterHistogram(id3, "test3d", 10, 10, 10);

    CHECK(!handler->Plot(123,123));
    CHECK_THROW(handler->RegisterBranch("notatree", "branchname", &id0, "leaf list"), std::invalid_argument);
    CHECK_THROW(handler->Get1DHistogram(123), std::invalid_argument);
    CHECK_THROW(handler->Get2DHistogram(123), std::invalid_argument);
    CHECK_THROW(handler->Get3DHistogram(123), std::invalid_argument);

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::normal_distribution<double> distribution (5.0,1.0);

    double val;
    for (int i=0; i<100; ++i) {
        val = distribution(generator);
        handler->Plot(id0, val);
        handler->Plot(id1, val, val);
        handler->Plot(id2, val, -1, val);
        handler->Plot(id3, val, val, val);
    }

    delete RootHandler::get();
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}
