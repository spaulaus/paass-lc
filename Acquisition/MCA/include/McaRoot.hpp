/// @file McaRoot.h
/// @brief Class to handle outputting MCA data into ROOT files
/// @author K. Smith, C. R. Thornsberry, S. V. Paulauskas
/// @date Updated July 25, 2018
#ifndef PAASSLC_MCAROOT_HPP
#define PAASSLC_MCAROOT_HPP
#include "Mca.hpp"

#include <map>

class TFile;
class TH1D;

/// Class derived from MCA that allows us to write MCA to a root file.
class McaRoot : public Mca {
public:
    ///Default constructor
    McaRoot(AcquisitionInterface *pif, const char *basename);

    ///Default destructor
    ~McaRoot();

    /// Calls TFile::Write and TFile::Flush.
    void Flush();

    /// Returns the histogram registered for the listed module and channel
    /// @param[in] mod - The module of the histogram we want.
    /// @param[in] ch - The channel of the histogram we want.
    /// @returns a pointer to the TH1D histogram associated with the given module and channel. Returns nullptr if no
    /// histogram could be found.
    TH1D *GetHistogram(const unsigned short &mod, const unsigned short &ch);

    /// Opens up the root file for writing and generates the list of TH1D histograms that we'll need.
    /// @param[in] basename : The basename of the file that we want to open for writing.
    /// @returns false if the file could not be opened or couldn't be written to, true otherwise.
    bool OpenFile(const char *basename);

    /// Resets all the histograms registered in the histograms_ map. Calls TH1::Reset().
    void Reset();

    /// Gets the histogram data from the interface, then writes the data into the histogram.
    /// @param[in] mod - The module of the histogram we want.
    /// @param[in] ch - The channel of the histogram we want.
    /// @returns false if the desired histogram couldn't be found in histograms_, true otherwise.
    bool StoreData(const unsigned short &mod, const unsigned short &ch);

private:
    TFile *file_; //!< The ROOT file that we'll write the histograms into
    std::map<int, TH1D *> histograms_; //!< A map associating IDs with histograms.
};

#endif //#ifndef PAASSLC_McaRoot_H
