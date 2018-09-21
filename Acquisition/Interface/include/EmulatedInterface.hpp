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
    bool AdjustOffsets(unsigned short mod) override;
    unsigned long CheckFIFOWords(unsigned short mod) override;
    bool CheckRunStatus(short mod = -1) override;
    double GetInputCountRate(int mod, int chan) override;
    double GetOutputCountRate(int mod, int chan) override;
    bool GetStatistics(unsigned short mod) override;
    bool EndRun(short mod = -1) override;

    /// Overrides Pixie16::Init so that we always initialize the XIA API in Offline mode.
    bool Init() override;

    bool ReadFIFOWords(Pixie16::word_t *buf, unsigned long nWords, unsigned short mod, bool verbose = false) override;
    bool ReadHistogram(Pixie16::word_t *hist, unsigned long sz, unsigned short mod, unsigned short ch) override;
    bool ReadSglChanPar(const char *name, double &val, int mod, int chan) override;
    bool ReadSglChanTrace(unsigned short *buf, unsigned long sz, unsigned short mod, unsigned short chan) override;
    bool ReadSglModPar(const char *name, Pixie16::word_t &val, int mod) override;
    bool RemovePresetRunLength(int mod) override;

    bool StartHistogramRun(short mod, unsigned short mode) override;
    bool StartListModeRun(short mod = -1, unsigned short listMode = 0, unsigned short runMode = 0) override;
    bool SaveDSPParameters(const char *fn = nullptr) override;

    bool WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval = nullptr) override;
    bool WriteSglModPar(const char *name, Pixie16::word_t val, int mod, Pixie16::word_t *pval = nullptr) override;

private:
    unsigned int numberOfTriggersInFifo_;
    std::map<std::string, double> parameterValues_;

    template <typename T>
    bool FillBuffer(T *buf, const unsigned long &bufSize, const std::vector<unsigned int> &filler);

    std::string GenerateParameterKey(const char* &name, const int &crate, const int &mod, const int&chan);
    double GetParameterValue(const std::string &key);
    void SetParameterValue(const std::string &key, const double &value);
};

#endif //__EMULATEDINTERFACE_HPP_
