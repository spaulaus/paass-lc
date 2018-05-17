/// @file TimingConfiguration.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date May 15, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License

#include "TimingConfiguration.hpp"

TimingConfiguration::TimingConfiguration() = default;

TimingConfiguration::~TimingConfiguration() = default;

double TimingConfiguration::GetBeta() const {
    return beta_;
}

unsigned int TimingConfiguration::GetDelay() const {
    return delay_;
}

double TimingConfiguration::GetFraction() const {
    return fraction_;
}

double TimingConfiguration::GetGamma() const {
    return gamma_;
}

unsigned int TimingConfiguration::GetGap() const {
    return gap_;
}

unsigned int TimingConfiguration::GetLength() const {
    return length_;
}

double TimingConfiguration::GetQdc() const {
    return qdc_;
}

bool TimingConfiguration::IsFastSiPm() const {
    return isFastSiPm_;
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

void TimingConfiguration::SetIsFastSiPm(const bool &a) {
    isFastSiPm_ = a;
}

void TimingConfiguration::SetLength(const unsigned int &a) {
    length_ = a;
}

void TimingConfiguration::SetQdc(const double &a) {
    qdc_ = a;
}
