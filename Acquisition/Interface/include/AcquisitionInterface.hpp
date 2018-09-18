///@brief Abstracted interface to acquistion API.
///@author Karl Smith
#ifndef __ACQUISTIONINTERFACE_HPP_
#define __ACQUISTIONINTERFACE_HPP_
#include "AcquisitionConfig.hpp"
#include "Constants.hpp"
#include "HelperEnumerations.hpp"
#include "Lock.h"

#include <cstdint>

class AcquisitionInterface {
public:
    AcquisitionInterface(const char *cfgFile);

    virtual ~AcquisitionInterface() {};

    /// Initializes the API.
    virtual bool Init() = 0;

    virtual bool Boot(int mode, bool useWorkingSetFile);

    virtual bool WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval = nullptr) = 0;

    virtual bool ReadSglModPar(const char *name, Pixie16::word_t &val, int mod) = 0;

    virtual void PrintSglModPar(const char *name, int mod, Pixie16::word_t *prev = nullptr) = 0;

    virtual bool WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval = nullptr) = 0;

    virtual bool ReadSglChanPar(const char *name, double &val, int mod, int chan) = 0;

    virtual void PrintSglChanPar(const char *name, int mod, int chan, double *prev = nullptr) = 0;

    virtual bool SaveDSPParameters(const char *fn = nullptr) = 0;

    virtual bool AcquireTraces(int mod) = 0;

    virtual double GetInputCountRate(int mod, int chan) = 0;

    virtual double GetOutputCountRate(int mod, int chan) = 0;

    // # AcquireTraces must be called before calling this #
    virtual bool ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan) = 0;

    virtual bool AdjustOffsets(unsigned short mod) = 0;

    virtual bool StartHistogramRun(short mod = -1, unsigned short mode = 0) = 0;

    virtual bool StartListModeRun(short mod = -1, unsigned short listMode = 0,
                                  unsigned short runMode = 0) = 0;

    virtual bool CheckRunStatus(short mod = -1) = 0;

    virtual bool EndRun(short mod = -1) = 0;

    virtual bool ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch) = 0;

    virtual bool RemovePresetRunLength(int mod) = 0;

    AcquisitionConfig GetConfiguration() const { return config_; }

    virtual bool GetModuleInfo(const unsigned short &mod, unsigned short *rev, unsigned int *serNum,
            unsigned short *adcBits, unsigned short *adcMsps) = 0;

    virtual bool GetStatistics(unsigned short mod) = 0;
    virtual unsigned long CheckFIFOWords(unsigned short mod) = 0;
    virtual bool ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose = false) = 0;

protected:
    int retval_;

    AcquisitionConfig config_;
private:
    Lock lock_;  // class to prevent simultaneous access to pixies
};

#endif // __ACQUISTIONINTERFACE_HPP_