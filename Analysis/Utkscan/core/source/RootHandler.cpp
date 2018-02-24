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

RootHandler::RootHandler(const char *fileName, const char *treeName) {
    file = new TFile(fileName, "recreate"); //! overwrite tree for now
    tree = new TTree(treeName, treeName);
}

bool RootProcessor::AddBranch(RawEvent &rawev) {
    DetectorDriver *driver = DetectorDriver::get();
    if (file == NULL || tree == NULL) {
        cout << "Failed to create ROOT objects for "
             << name << " processor" << endl;
        return false;
    }

    const vector<EventProcessor *> &drvProcess = driver->GetProcessors();

    for (vector<EventProcessor *>::const_iterator it = drvProcess.begin();
         it != drvProcess.end(); it++) {
        if ((*it)->AddBranch(tree)) {
            vecProcess.push_back(*it);
            set_union((*it)->GetTypes().begin(), (*it)->GetTypes().end(),
                      associatedTypes.begin(), associatedTypes.end(),
                      inserter(associatedTypes, associatedTypes.begin()));
        }
    }
    return EventProcessor::Init(rawev);
}

bool RootProcessor::Update(RawEvent &event) {
    if (!EventProcessor::Process(event))
        return false;

    for (vector<EventProcessor *>::iterator it = vecProcess.begin();
         it != vecProcess.end(); it++) {
        (*it)->FillBranch();
    }

    tree->Fill();
    if (tree->GetEntries() % 1000 == 0) {
        tree->AutoSave();
    }

    EndProcess();
    return true;
}

RootProcessor::~RootProcessor() {
    if (tree != NULL) {
        cout << "  saving " << tree->GetEntries() << " tree entries" << endl;
        tree->AutoSave();
    }

    if (file != NULL) {
        file->Close();
        delete file; // this also frees the tree
    }
}

#endif // useroot
