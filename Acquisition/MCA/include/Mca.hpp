/// @file MCA.h
/// @brief Base class for the MCA writer classes
/// @author K. Smith, C. R. Thornsberry, S. V. Paulauskas
/// @date Updated July 25, 2018
#ifndef PAASSLC_MCA_H
#define PAASSLC_MCA_H

#include <chrono>

class AcquisitionInterface;

///Abstract MCA class, provides functionality for all the derived MCA classes.
class Mca {
public:
    ///@TODO We still need to figure out the right way that we want to call all this nonsense. Right now it's a bit
    /// of a mess. We'll probably want to remove the AcquisitionInterface constructor and just leave the child
    /// constructors, that probably makes the most sense.

    ///Default constructor.
    Mca(AcquisitionInterface *pif);

    ///Default destructor.
    virtual ~Mca();

    unsigned int CalculateHistogramId(const unsigned int &crate, const unsigned int &module, const unsigned int &channel);

    ///Flush the current memory to disk. will be overloaded by the children.
    virtual void Flush();

    ///@returns the length of time the MCA has been running.
    double GetRunTimeInSeconds();

    /// Check if the output file is open and ready for writing.
    /// @returns true if the file is open and available for writing.
    virtual bool IsOpen();

    /// Opens a file for writing
    /// @param[in] basename : A character string containing just the file name w/o extension.
    /// @returns true if the file was successfully opened. Returns false if the derived class does not override.
    virtual bool OpenFile(const char *basename);

    /// The MCA is initialized and run for the specified duration or until a
    /// stop command is received. At specific intervals the MCA output is
    /// updated via MCA::StoreData(). Will continue until external bool (stop)
    /// is set to false. If this pointer is set to NULL, will continue uninterrupted. 
    ///
    /// @param[in] duration Amount of time to run the MCA.
    /// @param[in] stop External boolean flag for stop run command. 
    virtual void Run(const float &duration);

    /// Checks the Pixie run status, Updates the MCA histograms, sets the stopTime_
    virtual bool Step();

    /// Abstract method describing how the MCA data is stored.
    /// @param[in] mod : the module that we want to store data for
    /// @param[in] ch : the channel that we're going to store data for.
    /// @returns true if we were able to successfully store the data for the given module / channel combo.
    virtual bool StoreData(const unsigned short &mod, const unsigned short &ch);

protected:
    std::chrono::steady_clock::time_point startTime_;//!< The time that run was called.
    std::chrono::steady_clock::time_point stopTime_;//!< The time that we are going to be checking duration against.

    static const size_t ADC_SIZE = 65536; //!< Default number of channels in ADC, assume the max Pixie-16 of 16 bit.
    bool isOpen_; //!< Flag indicating if histogram construction was successful.

    AcquisitionInterface *pif_; //!< A pointer to the Interface that we'll be using to run things.
};
#endif //PAASSLC_MCA_H
