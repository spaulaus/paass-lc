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
        virtual bool StartHistogramRun(unsigned short mode = 0) {return false;};

};

#endif //__EMULATEDINTERFACE_HPP_

// vim: expandtab tabstop=4 shiftwidth=4 softtabstop=4 autoindent

