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

#include <set>
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

    ///Wrapper for users to add a branch to a tree
    ///@param[in] tree : The name of the tree that they want to add a branch to.
    ///@param[in] name : The name of the branch that they're adding
    ///@param[in] definition : The definition for the branch
    void AddBranch(TTree *tree, const std::string &name, const std::string &definition);

    ///@return A pointer to the root file so that the users can manipulate it if they need.
    TFile *GetRootFile();

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

    ///Wrapper for users to define a Tree to write their data into.
    ///@param[in] treeName : The name of the tree that they want to create
    void RegisterTree(const std::string &treeName);

    ///Method that will update all the trees and histograms in the system.
    void Flush();
private:
    ///The static instance of the RootHandler that everybody can access.
    static RootHandler *instance_;

    ///The Copy constructor telling us how to copy this rascal
    ///@param[in] : The RootHandler object that we want to copy
    void operator=(RootHandler const &); //!< copy constructor

    ///Constructor taking arguments for file and tree creation
    ///@param [in] fileName : The name of the ROOT File
    RootHandler(const std::string &fileName);

    ///Method that will asynchronously flush trees and histograms.
    static void AsyncFlush();

    static TFile *file_; //!< The ROOT file that all the information will be stored in.
    static std::set<TTree *> treeList_; //!< The list of trees known to the system
    static std::map<unsigned int, TH1 *> histogramList_; //!< The list of 1D histograms known to the system
    static std::mutex flushMutex_;
};

#endif // __ROOTHANDLER_HPP_
