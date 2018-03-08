///@file UtkUnpacker.cpp
///@brief A child of the Unpacker class that is used to replace some of the
/// functionality of the PixieStd.cpp from pixie_scan
///@author S. V. Paulauskas
///@date June 17, 2016
#ifndef __UTKUNPACKER_HPP__
#define __UTKUNPACKER_HPP__

#include "DetectorDriver.hpp"
#include "DetectorLibrary.hpp"
#include "RawEvent.hpp"
#include "Unpacker.hpp"

#include <chrono>


///A class that is derived from Unpacker that defines what we are going to do
/// with all of the events that are built by the Unpacker class. We only
/// define a single class (ProcessRawEvent) and overload the RawStats class
/// to take a pointer to a DetectorDriver instance. The rest of the virtual
/// methods in the parent are used as default.
class UtkUnpacker : public Unpacker {
public:
    /// Default constructor that does nothing in particular
    UtkUnpacker();

    /// Default destructor that deconstructs the DetectorDriver singleton
    ~UtkUnpacker();

private:
    DetectorDriver *driver_;
    DetectorLibrary *detectorLibrary_;

    ///@brief Process all events in the event list.
    ///@param[in]  addr_ Pointer to a ScanInterface object.
    void ProcessRawEvent();

    ///@brief Initializes the DetectorLibrary and DetectorDriver
    ///@param[in] driver A pointer to the DetectorDriver that we're using.
    ///@param[in] detlib A pointer to the DetectorLibrary that we're using.
    ///@param[in] start The time that we called this method.
    void InitializeDriver(DetectorDriver *driver, DetectorLibrary *detlib, RawEvent &rawev,
                          const std::chrono::steady_clock::time_point &startTime);

    ///@brief Prints information about time spent processing to the screen
    ///@param[in] eventTime : The start time of the current event
    ///@param[in] eventCounter : The event number we are processing
    ///@param[in] processingTime : The time that we have spent processing data.
    void PrintProcessingTimeInformation(const double &eventTime, const unsigned int &eventCounter,
                                        std::chrono::duration<double> &processingTime);

    ///@brief Add an event to generic statistics output.
    ///@param[in] event_ Pointer to the current XIA event.
    ///@param[in] driver Pointer to the DetectorDriver class that we're using.
    ///@param[in] addr_  Pointer to a ScanInterface object.
    void RawStats(XiaData *event_, DetectorDriver *driver);
};

#endif //__UTKUNPACKER_HPP__
