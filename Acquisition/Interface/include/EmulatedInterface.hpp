///@brief Emulated XIA Interface.
///@author Karl Smith
#ifndef __EMULATEDINTERFACE_HPP_
#define __EMULATEDINTERFACE_HPP_

#include <PixieInterface.h>

class EmulatedInterface : public PixieInterface {
public:
    EmulatedInterface();
    EmulatedInterface(const char *cfgFile = "");

    ~EmulatedInterface();

    bool AcquireTraces(int mod);
    bool AdjustOffsets(unsigned short mod);
    unsigned long CheckFIFOWords(unsigned short mod);
    bool CheckRunStatus(short mod = -1);
    double GetInputCountRate(int mod, int chan);

    double GetOutputCountRate(int mod, int chan);
    bool GetStatistics(unsigned short mod);
    bool EndRun(short mod = -1);

    /// Overrides Pixie16::Init so that we always initialize the XIA API in Offline mode.
    bool Init() override;

    void PrintSglChanPar(const char *name, int mod, int chan, double *prev = nullptr) {};
    void PrintSglModPar(const char *name, int mod, Pixie16::word_t *prev = nullptr) {};

    bool ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose = false);
    bool ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch);
    bool ReadSglChanPar(const char *name, double &val, int mod, int chan);
    bool ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan);
    bool ReadSglModPar(const char *name, Pixie16::word_t &val, int mod);
    bool RemovePresetRunLength(int mod);

    bool StartHistogramRun(short mod, unsigned short mode);
    bool StartListModeRun(short mod = -1, unsigned short listMode = 0, unsigned short runMode = 0);
    bool SaveDSPParameters(const char *fn = nullptr);

    bool WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval = nullptr);
    bool WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval = nullptr);
};

#endif //__EMULATEDINTERFACE_HPP_
