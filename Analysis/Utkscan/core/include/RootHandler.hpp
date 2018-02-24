/** \file RootHandler.hpp
 * \brief Processor to dump data from events into a root tree
 *
 * This loops over other event processor to fill appropriate branches
 * @authors D. Miller and S. V. Paulauskas
 */
#ifndef __ROOTHANDLER_HPP__
#define __ROOTHANDLER_HPP__

#include <vector>

#include <TFile.h>

//! A Class to handle outputting things into ROOT
class RootHandler {
public:
    /// The default constructor
    RootHandler();

    /** Constructor taking arguments for file and tree creation
    * \param [in] fileName : the file name for the root file
    * \param [in] treeName : the name of the tree to save to the root file*/
    RootHandler(const char *fileName, const char *treeName);

    /** Default Destructor */
    ~RootHandler();

    void RegisterHistogram(const std::string &name, const std::string &title, const unsigned int &xbins,
                           const unsigned int &yBins = 0, const unsigned int &zBins = 0);
private:
    TFile *file; //!< File where tree is stored
    TTree *tree; //!< ROOT tree where event branches are filled

    std::set<TH1D*> th1dList_;
    std::set<TH2D*> th2dList_;
};

#endif // __ROOTHANDLER_HPP_
