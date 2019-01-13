/// @file BaGeLProcessor.hpp
/// @brief Experiment processor for the BaGeL campaign at iThemba LABS.
/// @suthor S. V. Paulauskas
/// @date December 1, 2018
/// @copyright Copyright (c) 2017 S. V. Paulauskas.
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
/// Used for feature-pr270 branch (off upstream/master)
#ifndef __BAGELPROCESSOR_HPP__
#define __BAGELPROCESSOR_HPP__
#include <EventProcessor.hpp>

class BaGeLProcessor : public EventProcessor {
public:
    /// Default Constructor
    BaGeLProcessor();

    /// Default Destructor
    ~BaGeLProcessor();

    /// Declare the plots used in the analysis
    void DeclarePlots();

    /// Process the event
    /// @param [in] event : the event to process
    /// @return Returns true if the processing was successful 
    bool Process(RawEvent &event);

private:
    ///Sets the detector types that are associated with this processor
    void SetAssociatedTypes();
};
#endif