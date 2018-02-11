///@brief Emulated XIA Interface.
///@author Karl Smith

#ifndef __EMULATEDINTERFACE_HPP_
#define __EMULATEDINTERFACE_HPP_

#include "AcqInterface.hpp"

class EmulatedInterface : public AcqInterface {
    public:
	    EmulatedInterface(const char* cfgFile);
		
        virtual ~EmulatedInterface() {};

        virtual bool Init(bool offlineMode=false) {return true;};

        virtual bool Boot(BootType mode, bool useWorkingSetFile) {return true;};

        virtual unsigned long CheckFIFOWords(unsigned short mod) {return 0;};

        virtual bool ReadFIFOWords(word_t *buf, unsigned long nWords,
                                   unsigned short mod, bool verbose = false) {return false;};

        virtual bool StartHistogramRun(short mod, unsigned short mode) {return false;};

        virtual bool WriteSglModPar(const char *name, word_t val, int mod, word_t *pval=nullptr) {return false;};

        virtual bool ReadSglModPar(const char *name, word_t &val, int mod) {return false;};

        virtual void PrintSglModPar(const char *name, int mod, word_t *prev=nullptr) {};

        virtual bool WriteSglChanPar(const char *name, double val, int mod, int chan, double *pval=nullptr) {return false;};

        virtual bool ReadSglChanPar(const char *name, double &val, int mod, int chan) {return false;};

        virtual void PrintSglChanPar(const char *name, int mod, int chan, double *prev=nullptr) {};

        virtual bool SaveDSPParameters(const char *fn = nullptr) {return false;};

        virtual bool AcquireTraces(int mod) {return false;};

        // # AcquireTraces must be called before calling this #
        virtual bool ReadSglChanTrace(unsigned short *buf, unsigned long sz,
                                      unsigned short mod, unsigned short chan) {return false;};

        virtual bool AdjustOffsets(unsigned short mod) {return false;};

        virtual bool StartListModeRun(short mod=-1, unsigned short listMode=0,
                          unsigned short runMode=0) {return false;};

        bool CheckRunStatus(short mod=-1) {return false;};

        virtual bool EndRun(short mod = -1) {return false;};

        virtual bool ReadHistogram(word_t *hist, unsigned long sz,
                                   unsigned short mod, unsigned short ch) {return false;};

        virtual bool RemovePresetRunLength(int mod) {return false;};
};

#endif //__EMULATEDINTERFACE_HPP_

// vim: expandtab tabstop=4 shiftwidth=4 softtabstop=4 autoindent

