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

TFile *RootHandler::GetRootFile() { return file_; }

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
        file_->Write();
        file_->Close();
        delete file_;
    }

    instance_ = nullptr;
}

///@TODO Update this so that we're being a little more flexible with our histogramming. At the moment, I'm wanting to
/// mimic the function calls to DAMM as closely as possible. This will reduce the amount of rewrites for now.
TH1 *RootHandler::RegisterHistogram(const std::string &name, const std::string &title, const unsigned int &xBins,
                                    const unsigned int &yBins/* = 0*/, const unsigned int &zBins/* = 0*/) {
    auto histogram = histogramList_.find(name);
    if (histogram != histogramList_.end())
        return histogram->second;

    if (!yBins && !zBins)
        return histogramList_.emplace(make_pair(name, new TH1I(name.c_str(), title.c_str(), xBins, 0, xBins))).first->second;
    if(yBins && !zBins)
        return histogramList_.emplace(make_pair(name, new TH2I(name.c_str(), title.c_str(), xBins, 0, xBins, yBins, 0, yBins))).first->second;
    if(!yBins)
        return histogramList_.emplace(make_pair(name, new TH2I(name.c_str(), title.c_str(), xBins, 0, xBins, zBins, 0, zBins))).first->second;

    return histogramList_.emplace(make_pair(name, new TH3I(name.c_str(), title.c_str(), xBins, 0, xBins, yBins, 0, yBins, zBins, 0, zBins))).first->second;
}

void RootHandler::AddBranch(TTree *tree, const std::string &name, const std::string &definition) {
    if (!file_)
        throw invalid_argument("The File wasn't opened!");
}

void RootHandler::Update() {
    for (const auto &tree : treeList_) {
        tree->Fill();
        if (tree->GetEntries() % 1000 == 0)
            tree->AutoSave();
    }
}