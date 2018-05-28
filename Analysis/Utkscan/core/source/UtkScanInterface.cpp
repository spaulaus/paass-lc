///@file UtkScanInterface.cpp
///@brief Derived class handling the interface with utkscan.
///@author S. V. Paulauskas
///@date September 23, 2016
#include <iostream>
#include <stdexcept>

#include "DetectorDriver.hpp"
#include "Display.h"
#include "RootHandler.hpp"
#include "TreeCorrelator.hpp"
#include "UtkScanInterface.hpp"
#include "UtkUnpacker.hpp"
#include "XmlInterface.hpp"

using namespace std;

/// Default constructor.
UtkScanInterface::UtkScanInterface() : ScanInterface() {}

/// Destructor.
UtkScanInterface::~UtkScanInterface() {
}

/** Initialize the map file, the config file, the processor handler, 
 * and add all of the required processors.
 * \param[in]  prefix_ String to append to the beginning of system output.
 * \return True upon successfully initializing and false otherwise. */
bool UtkScanInterface::Initialize(string prefix_) {
    if (scan_init)
        return false;

    if (GetOutputFilename() == "")
        throw invalid_argument("UtkScaninterface::Initialize : The output file name was not provided.");

    try {
        cout << "UtkScanInterface::Initialize : Now attempting to load and "
                "parse " << GetSetupFilename() << endl;
        XmlInterface::get(GetSetupFilename());
        Globals::get(GetSetupFilename());
        DetectorLibrary::get();
        TreeCorrelator::get()->buildTree();
    } catch (invalid_argument &ex) {
        throw;
    }

    unpacker_->SetEventWidth(Globals::get()->GetEventLengthInTicks());
    Globals::get()->SetOutputFilename(GetOutputFilename());
    Globals::get()->SetOutputPath(GetOutputPath());
    RootHandler::get(GetOutputPath() + GetOutputFilename());

#ifndef USE_HRIBF
    try {
        /** The DetectorDriver constructor will load processors
         *  from the xml configuration file upon first call.
         *  The DeclarePlots function will instantiate the DetectorLibrary
         *  class which will read in the "map" of channels.
         *  Subsequently the raw histograms, the diagnostic histograms
         *  and the processors and analyzers plots are declared.
         *
         *  Note that in the PixieStd the Init function of DetectorDriver
         *  is called upon first buffer. This include reading in the
         *  calibration and walk correction factors.
         */
        DetectorDriver::get()->DeclarePlots();
    } catch (exception &e) {
        cout << Display::ErrorStr(
                prefix_ + "Exception caught at UtkScanInterface::Initialize")
             << endl;
        throw;
    }
#endif
    return (scan_init = true);
}