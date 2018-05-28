///@file RootHandler.hpp
///@brief Processor to dump data from events into a root tree. Mostly rewritten in February 2018.
///@authors D. Miller and S. V. Paulauskas
///@date January 2010
#ifndef __ROOTHANDLER_HPP__
#define __ROOTHANDLER_HPP__

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

#include <map>
#include <mutex>

//! A Class to handle outputting things into ROOT, registering histograms, filling trees, all that jazzy stuff.
class RootHandler {
public:
    ///Get method that initializes the RootHandler with a default name for the ROOT File: histograms.root.
    ///@return a pointer to the instance of RootHandler
    static RootHandler *get();

    ///Get method that initializes the RootHandler with the name of the root file. This method will NOT create a new
    /// file if the class has already been initialzed once.
    ///@returns a pointer to the instance of RootHandler
    static RootHandler *get(const std::string &fileName);

    ///Default Destructor that does the final write to the ROOT file and clears out all our pointers. Because this is
    /// a singleton class, we MUST delete instance_ so that the ROOT file finalizes properly. This will ensure that
    /// the destructor is called.
    /// Ex. delete RootHandler::get();
    ~RootHandler();

    /// Method to access a specific histogram
    /// @param [in] id : The id of the histogram that we're after, this should include the OFFSET that the
    /// Analyzer/Processor defines in its namespace.
    /// @returns a TH1D pointer to the correct histogram
    TH1D *Get1DHistogram(const unsigned int &id);

    /// Method to access a specific histogram
    /// @param [in] id : The id of the histogram that we're after, this should include the OFFSET that the
    /// Analyzer/Processor defines in its namespace.
    /// @returns a TH2D pointer to the histogram.
    TH2D *Get2DHistogram(const unsigned int &id);

    /// Method to access a specific histogram
    /// @param [in] id : The id of the histogram that we're after, this should include the OFFSET that the
    /// Analyzer/Processor defines in its namespace.
    /// @returns a TH3D pointer to the histogram.
    TH3D *Get3DHistogram(const unsigned int &id);

    ///Registers a branch with the provided tree.
    ///@param[in] treeName : The name of the tree that they want to add a branch to.
    ///@param[in] name : The name of the branch that they're adding
    ///@param[in] address : A pointer to the memory address for the object we're adding to the tree
    ///@param[in] leaflist : The leaf definition for the branch.
    void RegisterBranch(const std::string &treeName, const std::string &name, void *address, const std::string &leaflist);

    ///Plots into histogram defined by an integer ID
    /// @param [in] dammId : The histogram number to define
    /// @param [in] val1 : the x value
    /// @param [in] val2 : the y value or weight for a 1D histogram
    /// @param [in] val3 : the z value or weight in a 2D histogram
    /// @return true if successful
    bool Plot(const unsigned int &id, const double &xval, const double &yval = -1, const double &zval = -1);

    /// Wrapper function for the ROOT TH* constructors. We've simplified things to make it look more like DAMM for now.
    ///@param[in] id : The numerical ID of the histogram to register. The method prepends it with an "h", ex. h1
    ///@param[in] title : The Title of the histogram
    ///@param[in] xbins : The numbers of bins in the X Direction
    ///@param[in] yBins : The Number of bins in the Y Direction
    ///@param[in] zBins : The Number of bins in teh Z direction.
    ///@return a pointer to the newly registered histogram, or a pointer to the histogram of the same name
    TH1 *RegisterHistogram(const unsigned int &id, const std::string &title, const unsigned int &xbins,
                           const unsigned int &yBins = 0, const unsigned int &zBins = 0);

    ///Registers a TTree with the provided name and description.
    ///@param[in] name : The name of the tree to register
    ///@param[in] description : The description of the tree that we're registering
    ///@return A pointer to the existing TTree if one is found in the treeList_ or a pointer to the newly created
    /// TTree if one was inserted.
    TTree *RegisterTree(const std::string &name, const std::string &description = "");

    ///Method that will update all the trees and histograms in the system.
    void Flush();

    void FlushTree(TTree *tree);
private:
    ///The static instance of the RootHandler that everybody can access.
    static RootHandler *instance_;

    ///The Copy constructor telling us how to copy this rascal
    ///@param[in] : The RootHandler object that we want to copy
    void operator=(RootHandler const &);

    ///Constructor taking arguments for file and tree creation
    ///@param [in] fileName : The name of the ROOT File
    RootHandler(const std::string &fileName);

    ///Checks that a histogram is defined in the histogramList_
    ///@param[in] id : The ID of the histogram that we're looking for
    ///@param[in] callingFunctionName : The name of the function that called this one, so that we can generate the
    /// throw message
    ///@throws invalid_argument if we couldn't find the histogram in the list
    ///@returns a pointer to the histogram in the list if we found it.
    TH1 *GetHistogramFromList(const unsigned int &id, const std::string &callingFunctionName);

    ///Method that will asynchronously flush trees and histograms.
    static void AsyncFlush();

    static TFile *histogramFile_; //!< The ROOT file storing histograms
    static std::map<unsigned int, TH1 *> histogramList_; //!< The list of 1D histograms known to the system
    static TFile *treeFile_; //!< The ROOT file storing TTrees
    static std::map<std::string, TTree *> treeList_; //!< The list of trees known to the system
    static std::mutex flushMutex_; //!< Mutex to ensure that we don't spawn more than one AsyncWrite thread.
};

#endif // __ROOTHANDLER_HPP_
