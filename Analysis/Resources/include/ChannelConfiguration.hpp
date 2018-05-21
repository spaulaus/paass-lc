///@file ChannelConfiguration.hpp
///@brief Class that holds information about channel specific information. Including things like the trace delay,
///@author S. V. Paulauskas
///@date May 31, 2017
#ifndef PAASS_CHANNELCONFIGURATION_HPP
#define PAASS_CHANNELCONFIGURATION_HPP
#include "TimingConfiguration.hpp"
#include "TrapFilterParameters.hpp"

#include <set>
#include <string>

/// This class holds all of the information that can be set on a channel by channel basis. Fitting parameters,
/// trace delays, detector type, subtypes, tag lists, etc. This class replaces the Identifier class.
class ChannelConfiguration {
public:
    ChannelConfiguration();

    ~ChannelConfiguration();

    ///Constructor taking arguments and setting values, this constructor is used to maintain functionality with the
    /// previous Identifier class.
    ///@param [in] atype : the type to set
    ///@param [in] subType : the subType to set
    ///@param [in] loc : the location to set
    ChannelConfiguration(const std::string &atype, const std::string &subType, const unsigned int &loc);

    ///Insert a tag to the ChannelConfiguration
    ///@param [in] s : the name of the tag to insert
    ///@param [in] n : the value of the tag to insert 
    void AddTag(const std::string &s);

    ///@returns baselineThreshold_
    double GetBaselineThreshold() const;

    ///@return discriminationStartInSamples_
    unsigned int GetDiscriminationStartInSamples() const;

    ///@return energyFilterParameters_
    TrapFilterParameters GetEnergyFilterParameters() const;

    ///@return location_
    unsigned int GetLocation() const;

    ///@return The channel place name : type_subtype_location
    std::string GetPlaceName() const;

    ///@return subtype_
    std::string GetSubtype() const;

    ///@returns A set containing all the tags
    std::set<std::string> GetTags() const;

    ///@returns timingConfiguration_
    TimingConfiguration GetTimingConfiguration() const;

    ///@return triggerFilterParameters_
    TrapFilterParameters GetTriggerFilterParameters() const;

    ///@return type_
    std::string GetType() const;

    ///@return traceDelayInSamples_
    unsigned int GetTraceDelayInSamples() const;

    ///@return waveformBoundInSeconds_
    std::pair<unsigned int, unsigned int> GetWaveformBoundsInSamples() const;

    ///Check if the channel was tagged with the provided tag
    ///@param [in] s : the tag to search for
    ///@return true if the tag is found in the tag list
    bool HasTag(const std::string &s) const;

    ///Prints the type, subtype, location, and tags for the channel in a pretty format
    void Print(void) const;

    ///Sets the baseline threshold that we use to reject noisey signals. This is measured as the standard deviation
    /// of the baseline.
    ///@param[in] a : the value that we're setting for baselineThreshold_
    void SetBaselineThreshold(const double &a);

    ///Sets the start position for the tail sums for doing particle discrimination
    ///@param[in] a : The value that we're setting for discriminationStartInSamples_
    void SetDiscriminationStartInSamples(const unsigned int &a);

    ///Sets the energy filter parameters for use with the TraceFilter class
    ///@param [in] a : The TrapFilterParameters object containing energyFilterParameters_
    void SetEnergyFilterParameters(const TrapFilterParameters &a);

    ///Sets the location, which is unique to the combination of type and subtype. Can be used to identify specific
    /// detectors out of the map.
    ///@param [in] a : sets the location_ for the channel
    void SetLocation(const unsigned int &a);

    ///Sets the subtype of the channel, which allows for finer gradients amongst detectors e.g. small, medium, big
    /// for VANDLE
    ///@param [in] a : the subtype to set 
    void SetSubtype(const std::string &a);

    ///Sets the timing configuration for CFDs and Fits
    ///@param [in] a : the configuration to set
    void SetTimingConfiguration(const TimingConfiguration &a);

    /// Sets the trace delay in units of trace samples. This should match with the value from the set file loaded into
    /// pixie.
    ///@param[in] a : The value for the trace delay in units of samples.
    void SetTraceDelayInSamples(const unsigned int &a);

    ///Sets the trigger filter parameters for use with the TraceFilter class
    ///@param [in] a : the parameters that we want to set
    void SetTriggerFilterParameters(const TrapFilterParameters &a);

    ///Sets the type. Detector types provide the software with information about how to process data from this channel.
    /// We associate this value with specific processors to ensure that data is routed correctly. We place no restriction
    /// on the type of detectors, but be warned that not all types have an associated processor.
    ///@param [in] a : the type to set
    void SetType(const std::string &a);

    ///Sets the bounds for the waveform measured from the maximum value of the trace in trace samples.
    ///@param[in] a : A pair containing the low bound (first) and high bound (second) for the waveform.
    void SetWaveformBoundsInSamples(const std::pair<unsigned int, unsigned int> &a);

    ///Reinitialize members used for identification. Location is set to 9999. type, and subtype are set to "".
    /// Tags are cleared.
    void Zero();

    ///Equality operator for ChannelConfiguration, we will not check every parameter as the location should be
    /// unique for a given type/subtype.
    ///@param [in] rhs : the ChannelConfiguration to compare to
    ///@return true if the type, subtype, and location are identical 
    bool operator==(const ChannelConfiguration &rhs) const;

    ///Not - Equality operator for ChannelConfiguration
    ///@param [in] x : the ChannelConfiguration to compare to
    ///@return true if the type, subtype, or location do not match 
    bool operator!=(const ChannelConfiguration &rhs) const;

    ///Less-then operator needed for STL containers
    ///@param [in] rhs : the ChannelConfiguration to compare
    ///@return true if the type, subtype, or location are less than those in rhs 
    bool operator<(const ChannelConfiguration &rhs) const;

    ///Greater-than operator needed for STL containers
    ///@param[in] rhs : The right hand side that we are comparing with.
    ///@return The negative of the less than operator.
    bool operator>(const ChannelConfiguration &rhs) const;

private:
    double baselineThreshold_; ///< The threshold for the baseline to handle noisy traces.
    unsigned int discriminationStartInSamples_; ///< The position from the max that we'll do particle discrimination
    TrapFilterParameters energyFilterParameters_; ///< Parameters to use for energy filter calculations
    unsigned int location_; ///< Specifies the real world location of the channel.
    std::string subtype_; ///< Specifies the detector sub type
    std::set<std::string> tags_; ///< A list of associated tags
    TimingConfiguration timingConfiguration_; //!< The timing configuration for the CFD and Fit
    unsigned int traceDelayInSamples_; ///< The trace delay to help find the location of waveforms in traces
    TrapFilterParameters triggerFilterParameters_; ///< Parameters to use for trigger filter calculations
    std::string type_; ///< Specifies the detector type
    std::pair<unsigned int, unsigned int> waveformBoundsInSeconds_; ///< The waveform range for the channel
};


#endif //PAASS_CHANNELCONFIGURATION_HPP
