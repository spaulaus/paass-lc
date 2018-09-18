///@brief C++ wrapper to pixie functions including error handling and configuration parameters
///@author David Miller, Jan 2010
#ifndef __PIXIEINTERFACE_H_
#define __PIXIEINTERFACE_H_
#include "AcquisitionInterface.hpp"
#include "HelperEnumerations.hpp"

#include "pixie16app_defs.h"

#include <fstream>
#include <queue>
#include <string>

#include <stdint.h>

#define MIN_FIFO_READ 9

// define PIXIE16_REVD_GENERAL if it isn't defined in the header or else it will evalute to 0 (i.e. the same as PIXIE16_REVA)
#ifndef PIXIE16_REVD_GENERAL
#define PIXIE16_REVD_GENERAL 999 // arbitrary large number
#endif

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

#ifndef LIST_MODE_RUN
#ifdef LIST_MODE_RUN0
#define LIST_MODE_RUN LIST_MODE_RUN0
#endif
#endif

//A variable defined by the pxi library containing the path to the crate configuration.
extern const char *PCISysIniFile;

#ifdef PIF_CATCHER
const int CCSRA_PILEUP  = 15;
const int CCSRA_CATCHER = 16;
#endif

/// Only for SHE custom firmware w/ virtual digital-summed channels
const int CCSRA_SUMMED_SINGLE_EVENT = 18;

class PixieInterface : public AcquisitionInterface {
public:
    static const size_t STAT_SIZE = N_DSP_PAR - DSP_IO_BORDER;
    static const size_t HISTO_SIZE = MAX_HISTOGRAM_LENGTH;
#ifdef PIF_CATCHER
    enum CatcherModes {PC_STANDARD, PC_REJECT, PC_HYBRID, PC_ACCEPT};
#endif

    typedef Pixie16::word_t stats_t[STAT_SIZE];

    PixieInterface(const char *fn);

    ~PixieInterface();

    // wrappers to the pixie-16 app functions
    virtual bool Init();

    bool Boot(int mode = 0x7f, bool useWorkingSetFile = false);

    bool WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval = nullptr);

    bool ReadSglModPar(const char *name, Pixie16::word_t &val, int mod);

    void PrintSglModPar(const char *name, int mod, Pixie16::word_t *prev = nullptr);

    bool WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval = nullptr);

    bool ReadSglChanPar(const char *name, double &val, int mod, int chan);

    void PrintSglChanPar(const char *name, int mod, int chan, double *prev = nullptr);

    bool SaveDSPParameters(const char *fn = NULL);

    bool AcquireTraces(int mod);

    // # AcquireTraces must be called before calling this #
    bool ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan);

    // # #
    bool GetStatistics(unsigned short mod);

    // # GetStatistics must be called before calling these #
    stats_t &GetStatisticsData(void) { return statistics_; }

    double GetInputCountRate(int mod, int chan);

    double GetOutputCountRate(int mod, int chan);

    double GetLiveTime(int mod, int chan);

    double GetRealTime(int mod);

    double GetProcessedEvents(int mod);

    bool GetModuleInfo(const unsigned short &mod, unsigned short *rev, unsigned int *serNum, unsigned short *adcBits,
                       unsigned short *adcMsps);

    // # #
    bool StartHistogramRun(short mod = -1, unsigned short mode = NEW_RUN);

    bool StartListModeRun(short mod = -1, unsigned short listMode = LIST_MODE_RUN, unsigned short runMode = NEW_RUN);

    bool CheckRunStatus(short mod = -1);

#ifdef PIF_FIFO

    unsigned long CheckFIFOWords(unsigned short mod);

    bool ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose = false);

#endif

    bool EndRun(short mod = -1);

    bool RemovePresetRunLength(int mod);

    bool ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch);

    bool AdjustOffsets(unsigned short mod);

    // accessors
    static size_t GetTraceLength(void) { return TRACE_LENGTH; };

    short GetSlotNumber(int mod) const { return config_.GetSlotMaps()[0][mod]; }

    /** Handy functions for manipulating CSRA/B */
    bool ToggleGain(int mod, int chan);

    bool ToggleGood(int mod, int chan);

    bool TogglePolarity(int mod, int chan);

    bool ToggleTraceCapture(int mod, int chan);

#ifdef PIF_CATCHER
    bool ToggleCatcherBit(int mod, int chan);
    bool TogglePileupBit(int mod, int chan);

    bool SetProtonCatcherMode(int mod, int chan, CatcherModes mode);
#endif

protected:
    bool ToggleChannelBit(int mod, int chan, const char *parameter, int bit);

    static const size_t TRACE_LENGTH = RANDOMINDICES_LENGTH;

#ifdef PIF_CATCHER
    void CatcherMessage(void);
#endif

    bool doneInit_;

    // checks retval and outputs default OK/ERROR message
    virtual bool CheckError(bool exitOnError = false) const;

    stats_t statistics_;

    std::queue<Pixie16::word_t> extraWords_[Pixie16::maximumNumberOfModulesPerCrate];

    // temporary variables which hold the parameter which is being modified
    //   to deal with the const-incorrectness of the Pixie-16 API
    static const size_t nameSize_ = 120;
    char tmpName_[nameSize_];
};

#endif // __PIXIEINTERFACE_H_