/** \file RootHandler.cpp
 * \brief Implemenation of class to dump event info to a root tree
 * \author David Miller and S. V. Paulauskas
 * \date Jan 2010
 */
#include "RootHandler.hpp"

#include <iostream>
#include <thread>

using namespace std;

RootHandler *RootHandler::instance_ = nullptr;
TFile *RootHandler::histogramFile_ = nullptr; //!< The ROOT file that all the information will be stored in.
TFile *RootHandler::treeFile_ = nullptr; //!< The ROOT file that all the information will be stored in.
map<std::string, TTree *> RootHandler::treeList_; //!< The list of trees known to the system
map<unsigned int, TH1 *> RootHandler::histogramList_; //!< The list of 1D histograms known to the system
mutex RootHandler::flushMutex_;

/// Instance is created upon first call
RootHandler *RootHandler::get() {
    if (!instance_)
        instance_ = new RootHandler("roothandler-default");
    return (instance_);
}

RootHandler *RootHandler::get(const std::string &fileName) {
    if (!instance_)
        instance_ = new RootHandler(fileName);
    return (instance_);
}

RootHandler::RootHandler(const std::string &fileName) {
    histogramFile_ = new TFile((fileName+"-hist.root").c_str(), "recreate");
    treeFile_ = new TFile((fileName+"-tree.root").c_str(), "recreate");
}

RootHandler::~RootHandler() {
    if(histogramFile_) {
        while(!flushMutex_.try_lock())
            usleep(1000000);

        histogramFile_->cd();
        for(const auto &hist : histogramList_)
            if(hist.second->GetEntries() > 0)
                hist.second->Write(nullptr, TObject::kWriteDelete);

        histogramFile_->Write(nullptr, TObject::kWriteDelete);
        histogramFile_->Close();
        delete histogramFile_;
    }

    if(treeFile_) {
        treeFile_->cd();
        treeFile_->Write(nullptr, TObject::kWriteDelete);
        treeFile_->Close();
        delete treeFile_;
    }

    instance_ = nullptr;
}

TH1D *RootHandler::Get1DHistogram(const unsigned int &id) {
    return dynamic_cast<TH1D*>(GetHistogramFromList(id, "Get1DHistogram"));
}

TH2D *RootHandler::Get2DHistogram(const unsigned int &id) {
    return dynamic_cast<TH2D*>(GetHistogramFromList(id, "Get2DHistogram"));
}

TH3D *RootHandler::Get3DHistogram(const unsigned int &id) {
    return dynamic_cast<TH3D*>(GetHistogramFromList(id, "Get2DHistogram"));
}

void RootHandler::RegisterBranch(const std::string &treeName, const std::string &name, void *address,
                                 const std::string &leaflist) {
    auto tree = treeList_.find(treeName);
    if (tree == treeList_.end())
        throw invalid_argument("Roothandler::RegisterBranch - Attempt to graft branch named " + name
                               + " onto tree named " + treeName + ", which is unknown to us!!");
    tree->second->Branch(name.c_str(), address, leaflist.c_str());
}

TTree *RootHandler::RegisterTree(const std::string &name, const std::string &description/*=""*/) {
    auto tree = treeList_.find(name);
    if (tree != treeList_.end())
        return tree->second;

    auto *pTempTree = treeList_.emplace(make_pair(name, new TTree(name.c_str(), description.c_str()))).first->second;
    pTempTree->SetDirectory(treeFile_);
    return pTempTree;
}

bool RootHandler::Plot(const unsigned int &id, const double &xval, const double &yval/*=-1*/, const double &zval/*=-1*/) {
    TH1 *histogram = nullptr;
    try {
        histogram = GetHistogramFromList(id, "Plot");
    } catch(invalid_argument &invalidArgument) {
        ///@TODO Really we want to rethrow here, but for now we're just going to emulate what happened with DAMM. We
        /// just silently ignored any Plot request to an unknown histogram id.
        return false;
    }

    bool hasYval = yval != -1;
    bool hasZval = zval != -1;
    if(!hasYval && !hasZval)
        histogram->Fill(xval);
    if(hasYval && !hasZval)
        dynamic_cast<TH2D*>(histogram)->Fill(xval, yval);
    if(!hasYval && hasZval)
        dynamic_cast<TH2D*>(histogram)->Fill(xval, zval);
    if(hasYval && hasZval)
        dynamic_cast<TH3D*>(histogram)->Fill(xval, yval, zval);
    return true;
}

///@TODO Update this so that we're being a little more flexible with our histogramming. At the moment, I'm wanting to
/// mimic the function calls to DAMM as closely as possible. This will reduce the amount of rewrites for now.
TH1 *RootHandler::RegisterHistogram(const unsigned int &id, const std::string &title, const unsigned int &xBins,
                                    const unsigned int &yBins/* = 0*/, const unsigned int &zBins/* = 0*/) {
    auto histogram = histogramList_.find(id);
    if (histogram != histogramList_.end())
        return histogram->second;

    TH1 *pTempHistogram = nullptr;

    if (!yBins && !zBins)
        pTempHistogram = histogramList_.emplace(make_pair(id, new TH1D(("h"+to_string(id)).c_str(), title.c_str(), xBins, 0, xBins))).first->second;
    else if(yBins && !zBins)
        pTempHistogram = histogramList_.emplace(make_pair(id, new TH2D(("h"+to_string(id)).c_str(), title.c_str(), xBins, 0, xBins, yBins, 0, yBins))).first->second;
    else if(!yBins)
        pTempHistogram = histogramList_.emplace(make_pair(id, new TH2D(("h"+to_string(id)).c_str(), title.c_str(), xBins, 0, xBins, zBins, 0, zBins))).first->second;
    else
        pTempHistogram = histogramList_.emplace(make_pair(id, new TH3D(("h"+to_string(id)).c_str(), title.c_str(), xBins, 0, xBins, yBins, 0, yBins, zBins, 0, zBins))).first->second;

    pTempHistogram->SetDirectory(histogramFile_);
    return pTempHistogram;
}

void RootHandler::AsyncFlush() {
    for(const auto &hist : histogramList_) {
        histogramFile_->cd();
        if(hist.second->GetEntries() > 0)
            hist.second->Write(nullptr, TObject::kWriteDelete);
    }
    flushMutex_.unlock();
}

void RootHandler::Flush() {
    for(const auto &tree : treeList_)
        tree.second->AutoSave("overwrite");

    if(flushMutex_.try_lock()) {
        thread worker0(AsyncFlush);
        worker0.detach();
    }
}

TH1 *RootHandler::GetHistogramFromList(const unsigned int &id, const std::string &callingFunctionName) {
    auto histogramPair = histogramList_.find(id);
    if(histogramPair == histogramList_.end())
        throw invalid_argument("RootHandler::" + callingFunctionName + " - Somebody requested histogram "
                               + to_string(id) + ", which I know nothing about!!");
    return histogramPair->second;
}