/// @file CfdConfiguration.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date May 15, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License

#include "CfdConfiguration.hpp"

CfdConfiguration::CfdConfiguration() {
    //Does nothing
}

CfdConfiguration::~CfdConfiguration() {
    //Does nothing
}

unsigned int CfdConfiguration::GetDelay() {
    return delay_;
}

double CfdConfiguration::GetFraction() {
    return fraction_;
}

unsigned int CfdConfiguration::GetGap() {
    return gap_;
}

unsigned int CfdConfiguration::GetLength() {
    return length_;
}

void CfdConfiguration::SetDelay(const unsigned int &a) {
    delay_ = a;
}

void CfdConfiguration::SetFraction(const double &a) {
    fraction_ = a;
}

void CfdConfiguration::SetGap(const unsigned int &a) {
    gap_ = a;
}

void CfdConfiguration::SetLength(const unsigned int &a) {
    length_ = a;
}
