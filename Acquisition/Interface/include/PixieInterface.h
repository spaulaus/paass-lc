/// @file PixieInterface.h
/// @brief C++ wrapper to pixie functions including error handling and configuration parameters
/// @author S. V. Paulauskas, David Miller, C. R. Thornsberry, K. Smith
/// @date January 2010
#ifndef __PIXIEINTERFACE_H_
#define __PIXIEINTERFACE_H_

#include <AcquisitionInterface.hpp>
#include <HelperEnumerations.hpp>

#include <pixie16app_defs.h>

#include <fstream>
#include <queue>
#include <string>

#include <stdint.h>

/// The minimum number of words that the fifo needs to contain before we'll read from it.
#define MIN_FIFO_READ 9

/// define PIXIE16_REVD_GENERAL if it isn't defined in the header or else it will evaluate to 0 (i.e. the same as PIXIE16_REVA)
#ifndef PIXIE16_REVD_GENERAL
#define PIXIE16_REVD_GENERAL 999 // arbitrary large number
#endif

/// Makes sure that we define the proper flags based on the firmware.
#if PIXIE16_REVISION == PIXIE16_REVF
#define PIF_REVF
#define PIF_FIFO
#elif PIXIE16_REVISION == PIXIE16_REVD_GENERAL
#define PIF_CATCHER
#define PIF_REVD
#define PIF_FIFO
#else
#define PIF_REVA
#define PIF_BUFFER
#endif

/// Define a list mode run. I believe these are holdouts from the pacman part?
#ifndef LIST_MODE_RUN
#ifdef LIST_MODE_RUN0
#define LIST_MODE_RUN LIST_MODE_RUN0
#endif
#endif

/// A variable defined by the pxi library containing the path to the crate configuration.
extern const char *PCISysIniFile;

/// Defines the bits for the pileup catcher from the SHE experiments.
#ifdef PIF_CATCHER
const int CCSRA_PILEUP  = 15;
const int CCSRA_CATCHER = 16;
#endif

/// Only for SHE custom firmware w/ virtual digital-summed channels
const int CCSRA_SUMMED_SINGLE_EVENT = 18;

/// @TODO : We need to clean these methods so that we actually use the retvals from the XIA API. Right now they're
/// all either ignored, or misused.
/// @TODO: Every one of these methods returning a bool need to throw if there's an error, and we need to update poll2
/// to use a try/catch. The current implementation is prone to a lot of errors and confusion.
/// Interface class that glues together the XIA API with our software.
class PixieInterface : public AcquisitionInterface {
public:
    static const size_t STAT_SIZE = N_DSP_PAR - DSP_IO_BORDER; //!< Size of a statistics block so we can define the array
    static const size_t HISTO_SIZE = MAX_HISTOGRAM_LENGTH; //!< Size of the histogram length so we can define the array

    /// Defines an enumeration needed when we use the SHE catcher firmware.
#ifdef PIF_CATCHER
    enum CatcherModes {PC_STANDARD, PC_REJECT, PC_HYBRID, PC_ACCEPT};
#endif

    /// @typedef Defines a statistics block that we'll use to store stats information received from modules.
    typedef Pixie16::word_t stats_t[STAT_SIZE];

    /// Constructor accepting the name of the configuration file we'll be using. It overwrites the PCISysIniFile with
    /// the value taken from the configuration file.
    /// @param[in] fn : The name of the configuration file that's passed along to the AcquisitionConfig base.
    PixieInterface(const char *fn);

    /// Destructor that checks the pixie run status, stops any runs, and then exits the pixie system.
    ~PixieInterface();

    /// Calls Pixie16AcquireADCTrace to get a module's traces.
    /// @param[in] mod : The module we'll be reading traces from.
    /// @returns false if the API returned a value less than 0, true otherwise.
    bool AcquireTraces(int mod);

    /// Calls Pixie16AdjustOffsets to adjust the offsets for the provided module.
    /// @param[in] mod : The module that we want to adjust offsets for
    /// @returns True if the API exited without error, false otherwise.
    bool AdjustOffsets(unsigned short mod);

    /// Boots the modules according to their specification in the configuration file. Uses XIA-API::GetModuleInfo to
    /// compile a list of module types in the system and checks that list against the configuration. Confirms that
    // the modules have the correct SlotID set, and sets the correct SlotID if necessary.
    /// @param[in] mode : The mode use for booting. Refer to the hardware's documentation for values.
    /// @param[in] useWorkingSetFile : Specifies if we should use an intermediate settings file defined in the config
    /// @returns True if we had no errors booting or updating slots, false otherwise.
    bool Boot(int mode = 0x7f, bool useWorkingSetFile = false);

    /// Cals XIA-API::Pixie16CheckRunStatus on the module.
    /// @param[in] mod : The module that will tell us its run status. If mod is less than 0 it loops through all
    /// modules in the system to return their run status.
    /// @returns False if it was able to check the run status in all modules, true otherwise.
    /// @TODO strange that we return false here. Seems like that's what the API does though. Needs investigation or
    /// at least some updates to the variable names.
    bool CheckRunStatus(short mod = -1);

    /// Calls XIA-API::EndRun(mod), which ends any active data runs.
    /// @param[in] mod : The module that will end its run. If mod is less than 0 it loops through all modules in the
    /// system as defined by the configuration
    /// @returns true if the runs successfully ended, false otherwise.
    bool EndRun(short mod = -1);

    /// Calls XIA-API::Pixie16ComputeInputCountRate with statistics_ to determine the ICR for the Mod/Chan. The input
    /// count rate is the number of signals above threshold encountered by the system.
    /// @param[in] mod : The module we want the ICRs from.
    /// @param[in] chan : The channel we want the ICRs from.
    /// @returns The calculated input count rate for the mod/chan
    double GetInputCountRate(int mod, int chan);

    /// Calls XIA-API::Pixie16ComputeLiveTime with statistics_ to determine the live time of the Moc/Chan
    /// @param[in] mod : The module we want the live time from.
    /// @param[in] chan : The channel we want the live time from.
    /// @returns The calculated live time for the mod/chan
    double GetLiveTime(int mod, int chan);

    /// Calls XIA-API::Pixie16ReadModuleInfo to get back information about the module.
    /// @param[in] mod : The module that we want information about
    /// @param[in] rev : pointer that we'll point to the module's revision.
    /// @param[in] serNum : pointer that we'll point to the module's serial number
    /// @param[in] adcBits : pointer that we'll point to the module's bit resolution
    /// @param[in] adcMsps : pointer that we'll point to the module's sampling frequency.
    /// @returns true if the API didn't encounter an error, false otherwise.
    bool GetModuleInfo(const unsigned short &mod, unsigned short *rev, unsigned int *serNum, unsigned short *adcBits,
                       unsigned short *adcMsps);

    /// Calls XIA-API::Pixie16ComputeOutputCountRate with statistics_ to determine the OCR for the Mod/Chan. The OCR
    /// is the number of validated triggers in the system. It is affected by coincidence logic.
    /// @param[in] mod : The module we want the OCRs from.
    /// @param[in] chan : The channel we want the OCRs from.
    /// @returns The calculated output count rate for the mod/chan
    double GetOutputCountRate(int mod, int chan);

    /// Calls XIA-API::Pixie16ComputeProcessedEvents with statistics_ to determine the number of events processed by
    /// the module. A processed event is one that was actually recorded into the data stream.
    /// @param[in] mod : The module we want the processed event count from
    /// @returns the number of events processed by the module
    double GetProcessedEvents(int mod);

    /// Calls XIA-API::Pixie16ComputeRealTime with statistics_ to get the amount of time the module's been active. (?)
    /// @param[in] mod : the module we want the real time from
    /// @returns the time the module was active.
    double GetRealTime(int mod);

    /// Queries the configuration to determine the module's slot number
    /// @param[in] mod : The module number to search for.
    /// @returns the slot number for the provided module.
    /// @TODO : Move implementation to cpp file so we adhere to Open/Close
    short GetSlotNumber(int mod) const { return config_.GetSlotMaps()[0][mod]; }

    /// Calls XIA-API:::Pixie16ReadStatisticsFromModule and fills statistics_ with the result.
    /// @param[in] mod : The module we want statistics from
    /// @returns true if the API didn't return an error, false otherwise.
    bool GetStatistics(unsigned short mod);

    /// @returns the address to the statistics_ block read from the modules, will return nullptr if GetStatistics
    /// hasn't been called first.
    /// @TODO : Move implementation to cpp file so we adhere to Open/Close
    stats_t &GetStatisticsData() { return statistics_; }

    /// @returns the static length of traces defined by TRACE_LENGTH.
    /// @TODO : Move implementation to cpp file so we adhere to Open/Close
    static size_t GetTraceLength() { return TRACE_LENGTH; };

    /// Calls XIA-API::Pixie16InitSystem with offline false.
    /// @returns true, always.
    /// @TODO : There's no way that this should always return true. The structure here is insane.
    virtual bool Init();

    /// Prints a channel parameter to cout
    /// @param[in] name : The parameter's name
    /// @param[in] mod : The parameter's module
    /// @param[in] chan : The parameter's channel
    /// @param[in] pval : The parameter's previous value (if applicable)
    void PrintSglChanPar(const char *name, int mod, int chan, double *prev = nullptr);

    /// Prints a module parameter to cout
    /// @param[in] name : The parameter's name
    /// @param[in] mod : The parameter's module
    /// @param[in] pval : The parameter's previous value (if applicable)
    virtual void PrintSglModPar(const char *name, int mod, Pixie16::word_t *prev = nullptr);

    /// Calls XIA-API::Pixie16ReadHistogramFromModule to get the histogram information for the module and channel.
    /// @param[in] hist : pointer to a data buffer big enough to hold the histogram data.
    /// @param[in] sz : size of the data block that hist points to
    /// @param[in] mod : The module that we'll read the histogram from
    /// @param[in] ch : The channel that we'll read the histogram from
    /// @returns true, unless sz was bigger than MAX_HISTOGRAM_LENGTH or the API errored.
    bool ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch);

    /// Calls XIA-API::Pixie16ReadSglChanPar to get the requested channel parameter
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @param[in] chan : The parameter's channel
    /// @returns true if we got the parameter, false otherwise.
    virtual bool ReadSglChanPar(const char *name, double &val, int mod, int chan);

    /// Calls XIA-API::Pixie16ReadSglChanADCTrace to get the trace for the requested module and channel.
    /// @param[in] buf : Pointer to the data block that will store traces
    /// @param[in] sz : the size of buf.
    /// @param[in] mod : The module we're going to read traces from.
    /// @param[in] chan : The channel we're going to read traces from.
    /// @returns true, unless sz was bigger than TRACE_LENGTH or the API errored.
    bool ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan);

    /// Calls XIA-API::Pixie16ReadSglModPar to get the requested parameter value.
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @returns true if we got the paramter value, false if the API errored.
    virtual bool ReadSglModPar(const char *name, Pixie16::word_t &val, int mod);

    /// Writes a obnoxiously large (99999) value into HOST_RT_PRESET. I'll be honest I'm not entirely sure why.
    /// @param[in] mod : The module that will have its preset run length removed
    /// @returns true if we wrote the value, false if the API errored.
    bool RemovePresetRunLength(int mod);

    /// Calls XIA-API::Pixie16SaveDSPParametersToFile with the provided filename.
    /// @param[in] fn : The function that will define how to save the parameters. If null we use the name of the
    /// DspWorkingSetFile defined in the configuration file.
    /// @returns true if we were successful, false otherwise.
    bool SaveDSPParameters(const char *fn = NULL);

    /// Calls XIA-API::Pixie16StartHistogramRun to start the MCA run.
    /// @param[in] mod : The module that will perform the histogram run
    /// @param[in] mode : The type of run we'll start. Refer to the API documentation.
    /// @returns true if we started the run, false otherwise.
    bool StartHistogramRun(short mod = -1, unsigned short mode = NEW_RUN);

    /// Calls XIA-API::Pixie16StartListModeRun to start a list mode run in the modules
    /// @param[in] mod : The module that we want to start a list mode run
    /// @param[in] listMode : the type of list mode run that we'll start. Refer to the API documentation.
    /// @param[in] runMode : The type of run we'll start. Refer to the API documentation
    /// @returns true if we started the run, false otherwise.
    bool StartListModeRun(short mod = -1, unsigned short listMode = LIST_MODE_RUN, unsigned short runMode = NEW_RUN);

    /// Toggles the CHANNEL_CSRA bit CCSRA_ENARELAY, which defines the gain of the channel (high or low).
    /// @returns true if the bit was toggled, false if the API errored.
    bool ToggleGain(int mod, int chan);

    /// Toggles the CHANNEL_CSRA bit CCSRA_GOOD, which defines if the channel will be active.
    /// @returns true if the bit was toggled, false if the API errored.
    bool ToggleGood(int mod, int chan);

    /// Toggles CHANNEL_CSRA bit CCSRA_POLARITY, which defines the polarity of the input signal.
    /// @returns true if the bit was toggled, false if the API errored.
    bool TogglePolarity(int mod, int chan);

    /// Calls XIA-API::Pixie16WriteSglChanPar to write the parameter value to the module.
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @param[in] chan : The parameter's channel
    /// @param[in] pval : If null, we read the previous value from the module with Calls XIA-API::Pixie16ReadSglChanPar
    /// @returns true if we read the paramter, false otherwise.
    virtual bool WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval = nullptr);

    /// Calls XIA-API::Pixie16WriteSglModPar to write the value of the paramter to the module.
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @param[in] pval : If null, we read the previous value from the module with Calls XIA-API::Pixie16ReadSglModPar
    /// @returns true if we read the paramter, false otherwise.
    virtual bool WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval = nullptr);

#ifdef PIF_FIFO
    /// Calls XIA-API::Pixie16CheckExternalFIFOStatus to check the number of words in the fifo.
    /// @param[in] mod : The module that will tell us about its fifo words
    /// @returns the number of words in the FIFO plus the number of extraWords_ stored for the module.
    unsigned long CheckFIFOWords(unsigned short mod);

    /// Fills the extraWords buffer or the data buffer depending on the words available in the module, and the words
    /// in the extraWords_ vector.
    /// @param[in] buf : pointer to the data buffer we'll stuff data into
    /// @param[in] nWords : The number of words in the buffer we want to fill.
    /// @param[in] mod : The module that we'll get data from
    /// @param[in] verbose : set to true if we want additional messages during execution
    /// @returns true, if we could read words from the FIFO, false otherwise.
    bool ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose = false);
#endif

    ///@TODO Not sure why toggle trace capture is defined in here. Could probably be pulled out.
#ifdef PIF_CATCHER
    ///@TODO : Not going to document these right now since nobody really uses them.
    bool ToggleCatcherBit(int mod, int chan);
    bool TogglePileupBit(int mod, int chan);
    bool ToggleTraceCapture(int mod, int chan);
    bool SetProtonCatcherMode(int mod, int chan, CatcherModes mode);
#endif

protected:
    /// Checks the value of retval_ and exitOnError, if they're both true, then it exits the program. This is a
    // problem since we're going to be killing the program before cleanup. It leads to major errors.
    /// @returns false if there was an error (retval_ less than 0), true otherwise.
    virtual bool CheckError(bool exitOnError = false) const;

    /// Toggles the requested bit of the parameter.
    /// @returns true if the write was successful, false otherwise.
    bool ToggleChannelBit(int mod, int chan, const char *parameter, int bit);

#ifdef PIF_CATCHER
    void CatcherMessage(void);
#endif

    bool doneInit_;
    std::queue<Pixie16::word_t> extraWords_[Pixie16::maximumNumberOfModulesPerCrate];
    // temporary variables which hold the parameter which is being modified
    //   to deal with the const-incorrectness of the Pixie-16 API
    static const size_t nameSize_ = 120;
    stats_t statistics_;
    char tmpName_[nameSize_];
    static const size_t TRACE_LENGTH = RANDOMINDICES_LENGTH;
};

#endif // __PIXIEINTERFACE_H_