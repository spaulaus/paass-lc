/// @file MCA_ROOT.h
/// @brief Class to handle outputting MCA data into ROOT files
/// @author K. Smith, C. R. Thornsberry, S. V. Paulauskas
/// @date Updated July 25, 2018
#ifndef PAASSLC_MCA_ROOT_H
#define PAASSLC_MCA_ROOT_H
#include "MCA.h"

#include <iostream>
#include <map>

#include <cstdio>

#include <unistd.h>

class TFile;
class TH1D;

/// Class derived from MCA that allows us to write MCA to a root file.
class MCA_ROOT : public MCA {
public:
    ///Default constructor
    MCA_ROOT(AcquisitionInterface *pif, const char *basename);

    ///Default destructor
    ~MCA_ROOT();

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

    /// A class to handle redirecting stderr to a text file. Saves output in case the user
    /// would like to print it. Upon destruction stderr is restored to stdout.
    class cerr_redirect {
    private:
        char buf[BUFSIZ];

    public:
        cerr_redirect(const char *logFile) {
            for (int i = 0; i < BUFSIZ; i++)
                buf[i] = '\0';
            if(!freopen(logFile, "a", stderr))
                std::cerr << "cerr_redirect::cerr_redirect(logFile) - Couldn't reopen the stream!" << std::endl;
            setbuf(stderr, buf);
        };

        /**stderr is pointed to a duplicate of stdout, buffering is then set to no buffering. */
        ~cerr_redirect() {
            dup2(fileno(stdout), fileno(stderr));
            setvbuf(stderr, NULL, _IONBF, BUFSIZ);
        };

        void Print() {
            fprintf(stdout, "%s", buf);
            fflush(stdout);
        }
    };
};

#endif //#ifndef PAASSLC_MCA_ROOT_H
