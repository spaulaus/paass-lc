/// @file EmulatedInterface.hpp
/// @brief Implementation for the emulated pixie interface for Poll2, we'll use a combination of the XIA offline API
/// and custom work. The XIA API does not allow us to actually generate / display data.
/// @authors S. V. Paulauskas and K. Smith
/// @date March 24, 2018
#ifndef __EMULATEDINTERFACE_HPP_
#define __EMULATEDINTERFACE_HPP_

#include <PixieInterface.h>

/// Interface class that emulates communication with Pixie16 modules.
class EmulatedInterface : public PixieInterface {
public:
    /// Default constructor that does absolutely nothing.
    EmulatedInterface();

    /// Constructor taking the configuration file name as an put. Passes that along to the base class.
    /// @param[in] cfgFile : Name of the ocnfiguration file.
    EmulatedInterface(const char *cfgFile = "");

    /// Default Destructor. Relies on the parent for cleanup.
    ~EmulatedInterface();

    /// Does nothing other than print a message to the screen.
    /// @param[in] mod : Used as part of the output message.
    /// @returns true, always.
    bool AcquireTraces(int mod);

    /// Sets VOFFSET parameters to 8008135. Crate number is fixed at 0.
    /// @param[in] mod : The module number used to generate the parameter key.
    /// @returns true, always.
    /// @TODO : We could update this to generate a legitimate value.
    bool AdjustOffsets(unsigned short mod) override;

    /// This FIFO is always full. Uses unittest_encoded_data::R30474_250::rawHeader to provide a size, and multiplies
    /// that by the numberOfTriggersInFifo_ variable.
    /// @param[in] mod : Ignored
    /// @returns 12. Raw Header is 4 words, and we fixed triggers to 3.
    /// @TODO : This shouldn't be fixed and should zero out. Could be expanded to generate random data sets.
    unsigned long CheckFIFOWords(unsigned short mod) override;

    /// Module is always in a running state.
    /// @param[in] mod : Ignored
    /// @returns true, always.
    bool CheckRunStatus(short mod = -1) override;

    /// Module is always ending its run.
    /// @param[in] mod : Ignored.
    /// @returns true, always.
    bool EndRun(short mod = -1) override;

    /// Gets the input count rate for the specified module and channel.
    /// @param[in] mod : Ignored
    /// @param[in] chan : Ignored
    /// @returns 100. Constant count rate for now.
    /// @TODO : Should probably have this matching up with the FIFO data.
    double GetInputCountRate(int mod, int chan) override;

    /// Gets the output count rate for the specified module and channel.
    /// @param[in] mod : Ignored.
    /// @param[in] chan : Ignored.
    /// @returns 100. Constant rate of 100.
    /// @TODO : Should probably have this matching the FIFO data.
    double GetOutputCountRate(int mod, int chan) override;

    /// Doesn't do anything useful at the moment.
    /// @param[in] mod : Ignored.
    /// @returns true, always.
    /// @TODO : This needs to actually compute / put something into the stats array.
    bool GetStatistics(unsigned short mod) override;

    /// Overrides Pixie16::Init so that we always initialize the XIA API in Offline mode. We call Pixie16InitSystem
    /// with the offline mode parameter set to true. We set the numberOfTriggersInFifo_ parameter to 3.
    /// @returns true, always.
    /// @TODO : We should probably do something with the retval here.
    bool Init() override;

    /// Calls FillBuffer with unittest_encoded_data::R30474_250::rawHeader
    /// @param[in] buf : pointer to the data buffer we'll stuff data into
    /// @param[in] nWords : The number of words in the buffer we want to fill.
    /// @param[in] mod : Ignored.
    /// @param[in] verbose : Ignored
    /// @returns The result of the FillBuffer call.
    bool ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose = false) override;

    /// Generates a gaussian distribution and stuffs it into the data block provided. Mean for the distribution is
    /// half the size of the data block, and the standard deviation is 0.1% of the size.
    /// @param[in] hist : pointer to a data buffer big enough to hold the histogram data.
    /// @param[in] sz : size of the data block that hist points to
    /// @param[in] mod : ignored.
    /// @param[in] ch : ignored.
    /// @returns true, always.
    bool ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch) override;

    /// Sets val to the result of GetParameterValue. We generate the key with crate number of 0.
    /// @param[in] name : The parameter's name
    /// @param[in] val : Variable that will hold the result of the GetParameterValue call.
    /// @param[in] mod : The parameter's module
    /// @param[in] chan : The parameter's channel
    /// @returns true, always.
    /// @TODO Make sure we handle crate values later!
    bool ReadSglChanPar(const char *name, double &val, int mod, int chan) override;

    /// Fills the data buffer with unittest_trace_variables::trace
    /// @param[in] buf : Pointer to the data block that will store traces
    /// @param[in] sz : the size of buf.
    /// @param[in] mod : Ignored
    /// @param[in] chan : Ignored.
    /// @returns The result of the FillBuffer call.
    bool ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan) override;

    /// Gets the value of the parameter from parameterValue_. Key generated with crate number 0.
    /// @param[in] name : The parameter's name
    /// @param[in] val : variable to contain the result of the GetParameterValue call.
    /// @param[in] mod : The parameter's module
    /// @returns true, always.
    bool ReadSglModPar(const char *name, Pixie16::word_t &val, int mod) override;

    /// Just prints out a message to the terminal.
    /// @param[in] mod : Ignored.
    /// @returns true, always.
    bool RemovePresetRunLength(int mod) override;

    /// Just prints out a message to disk that we saved the parameters.
    /// @param[in] fn : Ignored.
    /// @returns true, always.
    bool SaveDSPParameters(const char *fn = nullptr) override;

    /// Just prints a message that we're staring a hisotgram run.
    /// @param[in] mod : ignored.
    /// @param[in] mode : ignored.
    /// @returns true, always.
    bool StartHistogramRun(short mod, unsigned short mode) override;

    /// Just prints a message saying we're staring a list mode run.
    /// @param[in] mod : ignored.
    /// @param[in] listMode : ignored.
    /// @param[in] runMode : ignored.
    /// @returns true, always.
    bool StartListModeRun(short mod = -1, unsigned short listMode = 0, unsigned short runMode = 0) override;

    /// Sets a parameter value, if the pval pointer isn't null we set that to be the previous value of the parameter.
    /// Key generated with crate number 0.
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @param[in] chan : The parameter's channel
    /// @param[in] pval : Variable to hold the previous value of the parameter
    /// @returns true, always.
    bool WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval = nullptr) override;

    /// Sets a parameter value, if the pval pointer isn't null we set that to be the previous value of the parameter.
    /// Key generated with crate number 0, channel 0.
    /// @param[in] name : The parameter's name
    /// @param[in] val : The parameter's value
    /// @param[in] mod : The parameter's module
    /// @param[in] pval : Variable to hold the previous value of the parameter
    /// @returns true, always.
    bool WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval = nullptr) override;

private:
    /// Method to fill a buffer with the contents of a vector. If the vector is too short for the data buffer, then
    /// we just keep looping through the vector until the contents are filled.
    /// @TODO : We may need to rethink this policy. This might not always be true, but it is for now.
    /// @param[in] buf : Pointer to the beginning of the data buffer that we'll be filling
    /// @param[in] bufSize : The size of the data buffer buf points to.
    /// @param[in] filler : The vector that we'll fill the data buffer with.
    /// @returns true if we could fill the buffer, false otherwise.
    template<typename T>
    bool FillBuffer(T *buf, const unsigned long &bufSize, const std::vector<unsigned int> &filler);

    /// Generates the parameter key used in the parameterValues_ map. The parameter key concatenates the name, crate
    /// number, module number, and channel number into a single string. For example, CHAN_CSRA-0-0-0.
    /// @param[in] name : The parameter's name
    /// @param[in] crate : The parameter's crate number
    /// @param[in] mod : The parameter's module number
    /// @param[in] chan : The parameter's channel number. This should be 0 if the parameter is a module only parameter.
    /// @returns The key generated by concatenating all the inputs.
    std::string GenerateParameterKey(const char *name, const int &crate, const int &mod, const int &chan);

    /// Searches parameterValues_ for the specified key. If no key is found, we call SetParameterValue with the key
    /// and the default value of 8008135.
    /// @param[in] key : The key that we want to find the value of.
    /// @returns : The value of the provided key if found, otherwise returns 8008135.
    double GetParameterValue(const std::string &key);

    /// Sets the value of a key. If that key cannot be found in parameterValues_, then it's inserted with the default
    /// value of 8008135. If the key is found, we set parameter to the provided value.
    /// @param[in] key : The key that we want to set
    /// @param[in] value : The value of the key
    void SetParameterValue(const std::string &key, const double &value);

    unsigned int numberOfTriggersInFifo_; //!< Defines the number of triggers present in our fake fifo.
    std::map<std::string, double> parameterValues_; //!< Key is ParameterName+CrateNum+ModNum+ChanNum, value assigned upon use.
};

#endif //__EMULATEDINTERFACE_HPP_
