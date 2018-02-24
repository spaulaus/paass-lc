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
    file_ = new TFile(fileName.c_str(), "recreate"); //! overwrite tree for now
}

void RootHandler::RegisterHistogram(const std::string &name, const std::string &title, const unsigned int &xbins,
                                    const unsigned int &yBins/* = 0*/, const unsigned int &zBins/* = 0*/) {

}

RootHandler::~RootHandler() {
    if (!treeList_.empty()) {
        for (const auto &tree : treeList_) {
            cout << "RootHandler::~RootHandler - Saving " << tree->GetEntries() << " tree entries to "
                 << tree->GetTitle() << endl;
            tree->AutoSave();
        }
    }

    if (file_ != NULL) {
        file_->Close();
        delete file_; // this also frees the tree
    }
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