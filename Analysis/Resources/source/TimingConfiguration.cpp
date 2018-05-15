/// @file TimingConfiguration.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date May 15, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License

#include "TimingConfiguration.hpp"

TimingConfiguration::TimingConfiguration() = default;

TimingConfiguration::~TimingConfiguration() = default;

double TimingConfiguration::GetBeta() {
    return beta_;
}

unsigned int TimingConfiguration::GetDelay() {
    return delay_;
}

double TimingConfiguration::GetFraction() {
    return fraction_;
}

double TimingConfiguration::GetGamma() {
    return gamma_;
}

unsigned int TimingConfiguration::GetGap() {
    return gap_;
}

unsigned int TimingConfiguration::GetLength() {
    return length_;
}

void TimingConfiguration::SetBeta(const double &a) {
    beta_ = a;
}

void TimingConfiguration::SetDelay(const unsigned int &a) {
    delay_ = a;
}

void TimingConfiguration::SetFraction(const double &a) {
    fraction_ = a;
}

void TimingConfiguration::SetGamma(const double &a) {
    gamma_ = a;
}

void TimingConfiguration::SetGap(const unsigned int &a) {
    gap_ = a;
}

void TimingConfiguration::SetLength(const unsigned int &a) {
    length_ = a;
}
