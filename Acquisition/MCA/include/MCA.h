/// @file MCA.h
/// @brief Base class for the MCA writer classes
/// @author K. Smith, C. R. Thornsberry, S. V. Paulauskas
/// @date Updated July 25, 2018
#ifndef PAASSLC_MCA_H
#define PAASSLC_MCA_H

#include <chrono>

class AcquisitionInterface;
class PixieInterface;
class EmulatedInterface;

///Abstract MCA class
class MCA {
public:
    ///Default constructor.
    MCA(AcquisitionInterface *pif);

    ///Default constructor.
    MCA(PixieInterface *pif);

    ///Default constructor.
    MCA(EmulatedInterface *pif);

    ///Default destructor.
    virtual ~MCA() {};

    ///Return the length of time the MCA has been running.
    double GetRunTime();

    ///Abstract method describing how the MCA data is stored.
    virtual bool StoreData(int mod, int ch) = 0;

    ///Abstract method to open a storage file.
    virtual bool OpenFile(const char *basename) = 0;

    ///Flush the current memory to disk.
    virtual void Flush() = 0;

    ///Check if the histogram construction was successful.
    virtual bool IsOpen() { return _isOpen; };

    ///Start the MCA running.
    virtual void Run(float duration, bool *stop = nullptr);

    ///Update the MCA histograms.
    virtual bool Step();

protected:
    /// Timers for the MCA object
    std::chrono::steady_clock::time_point startTime_;
    std::chrono::steady_clock::time_point stopTime_;

    ///Default number of bins in histogram.
    static const size_t HIS_SIZE = 16384;
    ///Default number of channels in ADC.
    static const size_t ADC_SIZE = 32768;

    ///Flag indicating if histogram construction was successful.
    bool _isOpen;

    AcquisitionInterface *_pif;
};
#endif //PAASSLC_MCA_H
