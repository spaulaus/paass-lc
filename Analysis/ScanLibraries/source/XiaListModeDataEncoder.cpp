/// @file XiaListModeDataEncoder.hpp
/// @brief Class that handles encoding Pixie-16 list mode data from a XiaData class
/// @author S. V. Paulauskas
/// @date December 30, 2016
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <cmath>

#include "HelperFunctions.hpp"
#include "XiaListModeDataEncoder.hpp"

using namespace std;
using namespace DataProcessing;

std::vector<unsigned int> XiaListModeDataEncoder::EncodeXiaData(const XiaData &data) {
    if (data == XiaData())
        throw invalid_argument("XiaListModeDataEncoder::EncodeXiaData - We received an empty XiaData structure.");

    vector<unsigned int> header;

    header.push_back(EncodeWordZero(data, mask_));
    header.push_back(EncodeWordOne(data, mask_));
    header.push_back(EncodeWordTwo(data, mask_));
    header.push_back(EncodeWordThree(data, mask_));

    //The following calls are required in this order due to the structure of the XIA list mode data format.

    if (data.GetEnergySums().size() != 0) {
        for(const auto &val : data.GetEnergySums())
            header.push_back(val);
        header.push_back(IeeeStandards::DecimalToIeeeFloating(data.GetFilterBaseline()));
    }

    if (data.GetQdc().size() != 0) {
        for (unsigned int i = 0; i < data.GetQdc().size(); i++)
            header.push_back((unsigned int &&) data.GetQdc().at(i));
    }

    if (data.GetExternalTimeLow() != 0) {
        header.push_back(data.GetExternalTimeLow());
        header.push_back(data.GetExternalTimeHigh());
    }

    if (data.GetTrace().size() != 0) {
        vector<unsigned int> tmp = EncodeTrace(data.GetTrace(), mask_.GetTraceMask());
        header.insert(header.end(), tmp.begin(), tmp.end());
    }

    return header;
}

unsigned int XiaListModeDataEncoder::EncodeWordZero(const XiaData &data, const XiaListModeDataMask &mask) {
    //These magic numbers are dependent upon the XIA List Mode Data Structure.
    // For more information about them please consult the relevant
    // documentation.
    unsigned int headerLength = mask.GetNumberOfBasicHeaderWords();
    if (data.GetExternalTimeLow() != 0)
        headerLength += mask.GetNumberOfExternalTimestampWords();
    if (data.GetEnergySums().size() != 0)
        headerLength += mask.GetNumberOfEnergySumWords();
    if (data.GetQdc().size() != 0)
        headerLength += mask.GetNumberOfQdcWords();
    unsigned int eventLength = (unsigned int) ceil(data.GetTrace().size() * 0.5) + headerLength;

    unsigned int word = 0;
    word |= data.GetChannelNumber() & mask.GetChannelNumberMask().first;
    word |= (data.GetSlotNumber() << mask.GetSlotIdMask().second) &
            mask.GetSlotIdMask().first;
    word |= (data.GetCrateNumber() << mask.GetCrateIdMask().second) &
            mask.GetCrateIdMask().first;
    word |= (headerLength << mask.GetHeaderLengthMask().second) &
            mask.GetHeaderLengthMask().first;
    word |= (eventLength << mask.GetEventLengthMask().second) &
            mask.GetEventLengthMask().first;
    word |= (data.IsPileup() << mask.GetFinishCodeMask().second) &
            mask.GetFinishCodeMask().first;
    return word;
}

unsigned int XiaListModeDataEncoder::EncodeWordOne(
        const XiaData &data, const XiaListModeDataMask &mask) {
    return data.GetEventTimeLow();
}

unsigned int XiaListModeDataEncoder::EncodeWordTwo(
        const XiaData &data, const XiaListModeDataMask &mask) {
    unsigned int word = 0;
    word |= data.GetEventTimeHigh() & mask.GetEventTimeHighMask().first;
    word |= (data.GetCfdFractionalTime()
            << mask.GetCfdFractionalTimeMask().second) &
            mask.GetCfdFractionalTimeMask().first;
    word |= (data.GetCfdForcedTriggerBit() << mask.GetCfdForcedTriggerBitMask()
            .second) & mask.GetCfdForcedTriggerBitMask().first;
    word |= (data.GetCfdTriggerSourceBit()
            << mask.GetCfdTriggerSourceMask().second) &
            mask.GetCfdTriggerSourceMask().first;
    return word;
}

unsigned int XiaListModeDataEncoder::EncodeWordThree(
        const XiaData &data, const XiaListModeDataMask &mask) {
    unsigned int word = 0;
    word |= (unsigned int) data.GetEnergy() & mask.GetEventEnergyMask().first;
    word |= (data.IsSaturated() << mask.GetTraceOutOfRangeFlagMask()
            .second) & mask.GetTraceOutOfRangeFlagMask().first;
    word |= (data.GetTrace().size() << mask.GetTraceLengthMask().second) &
            mask.GetTraceLengthMask().first;
    return word;
}

vector<unsigned int> XiaListModeDataEncoder::EncodeTrace(
        const std::vector<unsigned int> &trc, const std::pair<unsigned int,
        unsigned int> &mask) {
    vector<unsigned int> tmp;
    for (vector<unsigned int>::const_iterator it = trc.begin();
         it != trc.end(); it += 2) {
        vector<unsigned int>::const_iterator next = it + 1;
        unsigned int word = 0;
        word |= *it;
        if (next != trc.end())
            word |= *next << mask.second;
        tmp.push_back(word);
    }
    return tmp;
}