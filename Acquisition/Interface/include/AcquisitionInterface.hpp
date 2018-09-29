/// @file AcquisitionInterface.hpp
/// @brief Abstracted interface to the Hardware API. Focuses on Pixie related commands at the moment.
/// @authors S. V. Paulauskas and K. Smith
/// @date December 9, 2017
#ifndef __ACQUISTIONINTERFACE_HPP_
#define __ACQUISTIONINTERFACE_HPP_

#include <AcquisitionConfig.hpp>
#include <Constants.hpp>
#include <Lock.h>

/// Provides an abstract base class that all interfaces can derive from. Useful for different hardware interfaces.
/// Almost none of the classes do anything useful. Just the constructor really.
class AcquisitionInterface {
public:
    /// Constructor that takes a configuration file. This constructor instances the AcquisitionConfig and creates the
    /// lock file.
    /// @param[in] cfgFile : The configuration file that we're going to load with the AcquisitionConfig. 
    AcquisitionInterface(const char *cfgFile);

    /// The virtualized default destructor. 
    virtual ~AcquisitionInterface();

    /// Acquires traces from the hardware
    /// @param[in] mod : The module we'll be reading traces from.
    /// @returns false. Will be overloaded in the children.
    virtual bool AcquireTraces(int mod);

    /// Adjusts the baseline offsets for the specified module.
    /// @param[in] mod : The module that we want to adjust offsets for
    /// @returns false. Will be overloaded in the children.
    virtual bool AdjustOffsets(unsigned short mod);

    /// Boots the hardware according to the children's implementation.
    /// @param[in] mode : The mode use for booting. Refer to the hardware's documentation for values.
    /// @param[in] useWorkingSetFile : Specifies if we should use an intermediate settings file defined in the config
    /// @returns false. Will be overloaded in the children.
    virtual bool Boot(int mode, bool useWorkingSetFile);

    /// Checks the number of words in the fifo.
    /// @param[in] mod : The module that will tell us about its fifo words
    /// @returns 0. Will be overloaded in the children.
    virtual unsigned long CheckFIFOWords(unsigned short mod);

    /// Tells us if the module is performing a run
    /// @param[in] mod : The module that will tell us its run status
    /// @returns false. Will be overloaded in the children.
    virtual bool CheckRunStatus(short mod = -1);

    /// Ends a run in the module.
    /// @param[in] mod : The module that will end its run.
    /// @returns false. Will be overloaded in the children.
    virtual bool EndRun(short mod = -1);

    /// @returns The configuration that we built reading from the configuration file provided to the constructor.
    AcquisitionConfig GetConfiguration() const;

    /// Gets the input count rate for the specified module and channel.
    /// @param[in] mod : The module we want the ICRs from.
    /// @param[in] chan : The channel we want the ICRs from.
    /// @returns 0. Will be overloaded in the children.
    virtual double GetInputCountRate(int mod, int chan);

    /// Gets information about the module via the API
    /// @param[in] mod : The module that we want information about
    /// @param[in] rev : pointer that we'll point to the module's revision.
    /// @param[in] serNum : pointer that we'll point to the module's serial number
    /// @param[in] adcBits : pointer that we'll point to the module's bit resolution
    /// @param[in] adcMsps : pointer that we'll point to the module's sampling frequency.
    /// @returns false. Will be overloaded in the children.
    virtual bool GetModuleInfo(const unsigned short &mod, unsigned short *rev, unsigned int *serNum,
                               unsigned short *adcBits, unsigned short *adcMsps);

    /// Gets the output count rate for the specified module and channel.
    /// @param[in] mod : The module we want the OCRs from.
    /// @param[in] chan : The channel we want the OCRs from
    /// @returns 0. Will be overloaded in the children.
    virtual double GetOutputCountRate(int mod, int chan);

    /// Gets statistics from the hardware
    /// @param[in] mod : The module we want statistics from
    /// @returns false. Will be overloaded in the children.
    virtual bool GetStatistics(unsigned short mod);

    /// Initializes the API. Implementation specified by the children
    /// @returns false. Will be overloaded in the children.
    virtual bool Init();

    /// Prints a channel parameter to the screen
    /// @param[in] name : The parameter's name
    /// @param[in] mod : The parameter's module
    /// @param[in] chan : The parameter's channel
    /// @param[in] pval : The parameter's previous value (if applicable)
    virtual void PrintSglChanPar(const char *name, int mod, int chan, double *prev = nullptr);

    /// Prints a module paramter to the screen
    /// @param[in] name : The parameter's name
    /// @param[in] mod : The parameter's module
    /// @param[in] pval : The parameter's previous value (if applicable)
    virtual void PrintSglModPar(const char *name, int mod, Pixie16::word_t *prev = nullptr);

    /// Reads data out of the module.
    /// @param[in] buf : pointer to the data buffer we'll stuff data into
    /// @param[in] nWords : The number of words in the buffer we want to fill.
    /// @param[in] mod : The module that we'll get data from
    /// @param[in] verbose : set to true if we want additional messages during execution
    /// @returns false. Will be overloaded in the children.
    virtual bool ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose = false);

    /// Reads a histogram from the module and channel.
    /// @param[in] hist : pointer to a data buffer big enough to hold the histogram data.
    /// @param[in] sz : size of the data block that hist points to
    /// @param[in] mod : The module that we'll read the histogram from
    /// @param[in] ch : The channel that we'll read the histogram from
    /// @returns false. Will be overloaded in the children.
    virtual bool ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch);

    /// Reads a channel parameter from the hardware
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @param[in] chan : The parameter's channel
    /// @returns false. Will be overloaded in the children.
    virtual bool ReadSglChanPar(const char *name, double &val, int mod, int chan);

    /// Reads a trace from a channel. AcquireTraces must be called before calling this!!
    /// @param[in] buf : Pointer to the data block that will store traces
    /// @param[in] sz : the size of buf.
    /// @param[in] mod : The module we're going to read traces from.
    /// @param[in] chan : The channel we're going to read traces from.
    /// @returns false. Will be overloaded in the children.
    virtual bool ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan);

    /// Reads a module parameter from the hardware.
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @returns false. Will be overloaded in the children.
    virtual bool ReadSglModPar(const char *name, Pixie16::word_t &val, int mod);

    /// Removes the preset run length from the hardware.
    /// @param[in] mod : The module that will have its preset run length removed
    /// @returns false. Will be overloaded in the children.
    virtual bool RemovePresetRunLength(int mod);

    /// Saves the DSP parameters to disk
    /// @param[in] fn : The function that will define how to save the parameters.
    /// @returns false. Will be overloaded in the children.
    virtual bool SaveDSPParameters(const char *fn = nullptr);

    /// Starts a histogram run in the specified module.
    /// @param[in] mod : The module that will perform the histogram run
    /// @param[in] mode : The type of run we'll start. Refer to the API documentation.
    /// @returns false. Will be overloaded in the children.
    virtual bool StartHistogramRun(short mod = -1, unsigned short mode = 0);

    /// Starts a list mode run in the specified module.
    /// @param[in] mod : The module that we want to start a list mode run
    /// @param[in] listMode : the type of list mode run that we'll start. Refer to the API documentation.
    /// @param[in] runMode : The type of run we'll start. Refer to the API documentation
    /// @returns false. Will be overloaded in the children.
    virtual bool StartListModeRun(short mod = -1, unsigned short listMode = 0, unsigned short runMode = 0);

    /// Writes a channel parameter to the hardware
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @param[in] chan : The parameter's channel
    /// @param[in] pval : The parameter's previous value (if applicable)
    /// @returns false. Will be overloaded in the children.
    virtual bool WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval = nullptr);

    /// Writes a module parameter to the hardware
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @param[in] pval : The parameter's previous value (if applicable)
    /// @returns false. Will be overloaded in the children.
    virtual bool WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval = nullptr);

protected:
    AcquisitionConfig config_; //!< The configuration file that we load during construction.
    int retval_; //!< The return value passed back from the API that we use for error handling.
private:
    Lock lock_;  //!< Provides persistent lock to prevent simultaneous access to hardware.
};

#endif // __ACQUISTIONINTERFACE_HPP_