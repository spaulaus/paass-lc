///@brief Abstracted interface to acquistion API.
///@author Karl Smith

#ifndef __ACQINTERFACE_HPP_
#define __ACQINTERFACE_HPP_

#define NUMBER_OF_CHANNELS 16

#include <cstdint>

class AcqInterface {
    public:
        typedef uint32_t word_t;

        AcqInterface(const char *cfgFile);

        virtual bool ReadConfigurationFile(const char *cfgFile);

        /// @TODO This should be called ReadSlotConfig.
        /// This reads the slot configuration file.
        virtual bool GetSlots(const char *slotCfgFile = nullptr);

        /// @TODO Remove the offlineMode here, this should be separated into a 
        ///   Setter / Getter.
        /// Initializes the API.
        virtual bool Init(bool offlineMode=false);

        virtual bool Boot(int mode, bool useWorkingSetFile);

        virtual bool WriteSglModPar(const char *name, word_t val, int mod);

        virtual bool WriteSglModPar(const char *name, word_t val, int mod, word_t &pval);

        virtual bool ReadSglModPar(const char *name, word_t &val, int mod);

        virtual void PrintSglModPar(const char *name, int mod);

        virtual void PrintSglModPar(const char *name, int mod, word_t prev);

        virtual bool WriteSglChanPar(const char *name, double val, int mod, int chan);

        virtual bool WriteSglChanPar(const char *name, double val, int mod, int chan,
                             double &pval);

        virtual bool ReadSglChanPar(const char *name, double &val, int mod, int chan);

        virtual void PrintSglChanPar(const char *name, int mod, int chan);

        virtual void PrintSglChanPar(const char *name, int mod, int chan, double prev);

        virtual bool SaveDSPParameters(const char *fn = nullptr);

        virtual bool AcquireTraces(int mod);

        // # AcquireTraces must be called before calling this #
        virtual bool ReadSglChanTrace(unsigned short *buf, unsigned long sz,
                                      unsigned short mod, unsigned short chan);

        // # #
        virtual bool GetStatistics(unsigned short mod);

        // # #
        virtual bool StartHistogramRun(unsigned short mode = 0);

        virtual bool StartHistogramRun(unsigned short mod, unsigned short mode);

        virtual bool StartListModeRun(unsigned short listMode,
                                      unsigned short runMode);

        virtual bool StartListModeRun(unsigned short mod, unsigned short listMode,
                                      unsigned short runMode);

        virtual bool CheckRunStatus(void); // check status in all modules
        virtual bool CheckRunStatus(int mod);

        virtual unsigned long CheckFIFOWords(unsigned short mod) = 0;

        virtual bool ReadFIFOWords(word_t *buf, unsigned long nWords,
                                   unsigned short mod, bool verbose = false) = 0;

        virtual bool EndRun(void); // end run in all modules
        virtual bool EndRun(int mod);

        virtual bool ReadHistogram(word_t *hist, unsigned long sz,
                                   unsigned short mod, unsigned short ch);

        virtual bool AdjustOffsets(unsigned short mod);

        unsigned short GetNumberCards(void) const { return numberCards; };

        static unsigned short GetNumberChannels(void) { return numberChannels; };

        private:
            short numberCards;
            static const short numberChannels = NUMBER_OF_CHANNELS;
};

#endif // __ACQINTERFACE_HPP_

// vim: expandtab tabstop=4 shiftwidth=4 softtabstop=4 autoindent
