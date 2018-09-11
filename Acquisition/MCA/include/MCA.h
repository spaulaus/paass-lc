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
    virtual ~MCA();

    ///Return the length of time the MCA has been running.
    double GetRunTime();

    ///Abstract method describing how the MCA data is stored.
    virtual bool StoreData(int mod, int ch);

    ///Abstract method to open a storage file.
    virtual bool OpenFile(const char *basename);

    ///Flush the current memory to disk.
    virtual void Flush();

    ///Check if the histogram construction was successful.
    virtual bool IsOpen();

    /// The MCA is initialized and run for the specified duration or until a
    /// stop command is received. At specific intervals the MCA output is
    /// updated via MCA::StoreData(). Will continue until external bool (stop)
    /// is set to false. If this pointer is set to NULL, will continue uninterrupted. 
    ///
    /// @param[in] duration Amount of time to run the MCA.
    /// @param[in] stop External boolean flag for stop run command. 
    virtual void Run(const float &duration, const bool *stop = nullptr);

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
    bool isOpen_;

    AcquisitionInterface *pif_;
};
#endif //PAASSLC_MCA_H
