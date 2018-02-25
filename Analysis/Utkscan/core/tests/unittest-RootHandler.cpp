/// @file unittest-RootHandler.cpp
/// @brief Unittests for the RootHandler class.
/// @author S. V. Paulauskas
/// @date February 24, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include "RootHandler.hpp"

#include <UnitTest++.h>

TEST(TestRootHandler) {
    RootHandler *handler = RootHandler::get("/tmp/unittest-RootHandler.root");
    CHECK(handler);
    CHECK(handler->GetRootFile());

    handler->RegisterHistogram("test1d", "test1d", 10)->Fill(5);
    dynamic_cast<TH2I*>(handler->RegisterHistogram("test2d_xy", "test2d-xy", 10, 10))->Fill(5,5);
    dynamic_cast<TH2I*>(handler->RegisterHistogram("test1d_xz", "test2d-xz", 10, 0, 10))->Fill(5,5);
    dynamic_cast<TH3I*>(handler->RegisterHistogram("test3d", "test3d", 10, 10, 10))->Fill(5,5,5);

    delete RootHandler::get();
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}
