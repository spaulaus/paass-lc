/** \file RootHandler.cpp
 * \brief Implemenation of class to dump event info to a root tree
 * \author David Miller and S. V. Paulauskas
 * \date Jan 2010
 */
#include <algorithm>
#include <iostream>

#include <TTree.h>

#include "DetectorDriver.hpp"
#include "RootHandler.hpp"

using namespace std;

RootHandler *RootHandler::instance_ = NULL;

/** Instance is created upon first call */
RootHandler *RootHandler::get() {
    if (!instance_)
        instance_ = new RootHandler("histograms.root");
    return (instance_);
}

RootHandler *RootHandler::get(const std::string &fileName) {
    if (!instance_)
        instance_ = new RootHandler(fileName);
    return (instance_);
}

RootHandler::RootHandler(const std::string &fileName) {
    file_ = new TFile(fileName.c_str(), "recreate");
}

RootHandler::~RootHandler() {
    for (const auto &tree : treeList_) {
        cout << "RootHandler::~RootHandler - Saving " << tree->GetEntries() << " tree entries to "
             << tree->GetTitle() << endl;
        tree->AutoSave();
    }

    if(file_) {
        file_->Write(0, TObject::kOverwrite);
        file_->Close();
        delete file_;
    }

    instance_ = nullptr;
}

void RootHandler::AddBranch(TTree *tree, const std::string &name, const std::string &definition) {
    if (!file_)
        throw invalid_argument("The File wasn't opened!");
}

TFile *RootHandler::GetRootFile() {
    return file_;
}

bool RootHandler::Plot(const unsigned int &id, const double &xval, const double &yval/*=-1*/, const double &zval/*=-1*/) {
    auto histogramPair = histogramList_.find(id);
    if(histogramPair == histogramList_.end())
        return false;
    ///@TODO : We need to enable this again at some point. It was causing unspecified errors since it wasn't caught
    /// properly.
//        throw invalid_argument("RootHandler::Plot - Received a request for histogram ID " + to_string(id)
//                               + ", which is unknown to us. Please check that you have defined this histogram.");
    bool hasYval = yval != -1;
    bool hasZval = zval != -1;

    if(!hasYval && !hasZval)
        histogramPair->second->Fill(xval);
    if(hasYval && !hasZval)
        dynamic_cast<TH2I*>(histogramPair->second)->Fill(xval, yval);
    if(!hasYval && hasZval)
        dynamic_cast<TH2I*>(histogramPair->second)->Fill(xval, zval);
    if(hasYval && hasZval)
        dynamic_cast<TH3I*>(histogramPair->second)->Fill(xval, yval, zval);
    return true;
}

///@TODO Update this so that we're being a little more flexible with our histogramming. At the moment, I'm wanting to
/// mimic the function calls to DAMM as closely as possible. This will reduce the amount of rewrites for now.
TH1 *RootHandler::RegisterHistogram(const unsigned int &id, const std::string &title, const unsigned int &xBins,
                                    const unsigned int &yBins/* = 0*/, const unsigned int &zBins/* = 0*/) {
    auto histogram = histogramList_.find(id);
    if (histogram != histogramList_.end())
        return histogram->second;

    if (!yBins && !zBins)
        return histogramList_.emplace(make_pair(id, new TH1I(("h"+to_string(id)).c_str(), title.c_str(), xBins, 0, xBins))).first->second;
    if(yBins && !zBins)
        return histogramList_.emplace(make_pair(id, new TH2I(("h"+to_string(id)).c_str(), title.c_str(), xBins, 0, xBins, yBins, 0, yBins))).first->second;
    if(!yBins)
        return histogramList_.emplace(make_pair(id, new TH2I(("h"+to_string(id)).c_str(), title.c_str(), xBins, 0, xBins, zBins, 0, zBins))).first->second;
    return histogramList_.emplace(make_pair(id, new TH3I(("h"+to_string(id)).c_str(), title.c_str(), xBins, 0, xBins, yBins, 0, yBins, zBins, 0, zBins))).first->second;
}

void RootHandler::Flush() {
    for (const auto &tree : treeList_) {
        tree->Fill();
        if (tree->GetEntries() % 10000 == 0)
            tree->AutoSave();
    }

    for(const auto &hist: histogramList_)
        hist.second->Write(0, TObject::kOverwrite);
}