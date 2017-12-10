///@brief Emulated XIA Interface.
///@author Karl Smith

#ifndef __EMULATEDINTERFACE_HPP_
#define __EMULATEDINTERFACE_HPP_

#include "AcqInterface.hpp"

class EmulatedInterface : public AcqInterface {
	public:
        virtual unsigned long CheckFIFOWords(unsigned short mod) = 0;

        virtual bool ReadFIFOWords(word_t *buf, unsigned long nWords,
                                   unsigned short mod, bool verbose = false) = 0;

};

#endif //__EMULATEDINTERFACE_HPP_

