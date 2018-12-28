///@file XiaData.cpp
///@brief A class that holds information from the XIA LLC. Pixie-16 List
/// Mode Data
///@authors C. R. Thornsberry and S. V. Paulauskas
#include "XiaData.hpp"

///Clears all of the variables. The vectors are all cleared using the clear() method. This method is called when the class is
/// first initalizied so that it has some default values for the software to use in the event that they are needed.
void XiaData::Initialize() {
    cfdForceTrig_ = cfdTrigSource_ = isPileup_ = isSaturated_ = isVirtualChannel_ = false;

    filterBaseline_ = energy_ = time_ = filterTime_ = 0.0;

    chanNum_ = crateNum_ = cfdTime_ = 0;
    eventTimeHigh_ = eventTimeLow_ = externalTimestamp_ = externalTimeLow_ = externalTimeHigh_ = 0;
    slotNum_ = 2;

    eSums_.clear();
    qdc_.clear();
    trace_.clear();
}