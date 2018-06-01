///@file TwoChanTimingProcessor.hpp
///@brief Class to perform simple two channel timing experiments
///@author S. V. Paulauskas
///@date July 10, 2009
#ifndef __TWOCHANTIMINGPROCESSOR_HPP_
#define __TWOCHANTIMINGPROCESSOR_HPP_

#include "EventProcessor.hpp"

/// A class that handles a simple timing experiment. The class recognizes the "pulser" type. It matches up a pair of
/// channels with subtypes of "start" and "stop". The logic assumes that you will have two (and ONLY TWO) such
/// channels defined in the map.
class TwoChanTimingProcessor : public EventProcessor {
public:
    /// Default Constructor
    TwoChanTimingProcessor();

    ///Default Destructor
    ~TwoChanTimingProcessor();

    /// Declares the histograms that we're going to be using in the analysis.
    /// We define the following histograms:
    ///     # 6050 - 1D - Processing Codes filled while the processor is chugging away.
    ///         # 0 = PreProcess method was called
    ///         # 1 = We didn't have ANY starts or stops to match.
    ///         # 2 = We didn't have an even number of Starts/Stops to match.
    ///     # 6051 - 2D - Amplitude distribution for start traces
    ///     # 6052 - 2D - Amplitude distribution for stop traces
    ///     # 6053 - 2D - Random sampling of start traces
    ///     # 6054 - 2D - Random sampling of stop traces
    void DeclarePlots();

    ///Processes the data, plots histograms, and fills trees.
    /// @param [in] event : the event to process
    /// @return true if processing was successful */
    bool Process(RawEvent &event);
private:
    TTree *tree_; //!< Pointer to the tree that we register in the constructor
};

#endif // __TWOCHANTIMINGPROCESSOR_HPP_
