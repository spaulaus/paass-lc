/// @file ChannelConfiguration.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date May 17, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include "ChannelConfiguration.hpp"

#include <iomanip>
#include <iostream>

ChannelConfiguration::ChannelConfiguration() : location_(9999), subtype_(""), type_("") {}

ChannelConfiguration::~ChannelConfiguration() = default;

ChannelConfiguration::ChannelConfiguration(const std::string &atype, const std::string &subType,
                                           const unsigned int &loc) : location_(loc), subtype_(subType), type_(atype) {}

void ChannelConfiguration::AddTag(const std::string &s) { tags_.insert(s); }

double ChannelConfiguration::GetBaselineThreshold() const { return baselineThreshold_; }

unsigned int ChannelConfiguration::GetDiscriminationStartInSamples() const { return discriminationStartInSamples_; }

TrapFilterParameters ChannelConfiguration::GetEnergyFilterParameters() const { return energyFilterParameters_; }

unsigned int ChannelConfiguration::GetLocation() const { return location_; }

std::string ChannelConfiguration::GetPlaceName() const {
    return type_ + "_" + subtype_ + "_" + std::to_string(location_);
}

std::string ChannelConfiguration::GetSubtype() const { return subtype_; }

std::set<std::string> ChannelConfiguration::GetTags() const { return tags_; }

TimingConfiguration ChannelConfiguration::GetTimingConfiguration() const { return timingConfiguration_; }

TrapFilterParameters ChannelConfiguration::GetTriggerFilterParameters() const { return triggerFilterParameters_; }

std::string ChannelConfiguration::GetType() const { return type_; }

unsigned int ChannelConfiguration::GetTraceDelayInSamples() const { return traceDelayInSamples_; }

std::pair<unsigned int, unsigned int> ChannelConfiguration::GetWaveformBoundsInSamples() const {
    return waveformBoundsInSeconds_;
}

bool ChannelConfiguration::HasTag(const std::string &s) const { return tags_.find(s) != tags_.end(); }

void ChannelConfiguration::Print(void) const {
    std::cout << std::setw(10) << type_ << std::setw(10) << subtype_ << std::setw(4) << location_ << "    ";
    for (std::set<std::string>::const_iterator it = tags_.begin(); it != tags_.end(); it++) {
        if (it != tags_.begin())
            std::cout << ", ";
        std::cout << *it;
    }
    std::cout << std::endl;
}

void ChannelConfiguration::SetBaselineThreshold(const double &a) { baselineThreshold_ = a; }

void ChannelConfiguration::SetDiscriminationStartInSamples(const unsigned int &a) { discriminationStartInSamples_ = a; }

void ChannelConfiguration::SetEnergyFilterParameters(const TrapFilterParameters &a) { energyFilterParameters_ = a; }

void ChannelConfiguration::SetLocation(const unsigned int &a) { location_ = a; }

void ChannelConfiguration::SetSubtype(const std::string &a) { subtype_ = a; }

void ChannelConfiguration::SetTimingConfiguration(const TimingConfiguration &a) { timingConfiguration_ = a; }

void ChannelConfiguration::SetTraceDelayInSamples(const unsigned int &a) { traceDelayInSamples_ = a; }

void ChannelConfiguration::SetTriggerFilterParameters(const TrapFilterParameters &a) { triggerFilterParameters_ = a; }

void ChannelConfiguration::SetType(const std::string &a) { type_ = a; }

void ChannelConfiguration::SetWaveformBoundsInSamples(const std::pair<unsigned int, unsigned int> &a) {
    waveformBoundsInSeconds_ = a;
}

void ChannelConfiguration::Zero() {
    location_ = 9999;
    type_ = "";
    subtype_ = "";
    tags_.clear();
}

bool ChannelConfiguration::operator==(const ChannelConfiguration &rhs) const {
    return type_ == rhs.type_ && subtype_ == rhs.subtype_ && location_ == rhs.location_;
}

bool ChannelConfiguration::operator!=(const ChannelConfiguration &rhs) const { return !operator==(rhs); }

bool ChannelConfiguration::operator<(const ChannelConfiguration &rhs) const {
    if (type_.compare(rhs.type_) > 0)
        return false;
    else if (type_.compare(rhs.type_) < 0)
        return true;
    else {
        if (subtype_.compare(rhs.subtype_) > 0)
            return false;
        else if (subtype_.compare(rhs.subtype_))
            return true;
        else
            return (location_ < rhs.location_);
    }
}

bool ChannelConfiguration::operator>(const ChannelConfiguration &rhs) const { return !operator<(rhs); }