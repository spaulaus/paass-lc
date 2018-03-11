///@file TwoChanTimingProcessor.cpp
///@brief Calculates high resolution timing between two channels in a setup.
///@author S. V. Paulauskas
///@date July 10, 2009
#include <fstream>

#include "DammPlotIds.hpp"
#include "Globals.hpp"
#include "HighResTimingData.hpp"
#include "RawEvent.hpp"
#include "TwoChanTimingProcessor.hpp"

#include <TFile.h>
#include <TTree.h>
#include <TH1I.h>
#include <TH2I.h>

static HighResTimingData::HrtRoot rstart;
static HighResTimingData::HrtRoot rstop;

TTree *tree;

enum CODES {
    PROCESS_CALLED,
    MAP_EMPTY,
    MAP_UNEVEN
};

namespace dammIds {
    namespace experiment {
        const int D_CODES = 0;
        const int DD_AMPLITUDE_DISTRIBUTION_START = 1;
        const int DD_AMPLITUDE_DISTRIBUTION_STOP = 2;
        const int DD_TRACES_START = 3;
        const int DD_TRACES_STOP = 4;
    }
}

using namespace std;
using namespace dammIds::experiment;

TwoChanTimingProcessor::TwoChanTimingProcessor() : EventProcessor(OFFSET, RANGE, "TwoChanTimingProcessor") {
    associatedTypes.insert("pulser");

    tree = new TTree("timing", "");
    tree->Branch("start", &rstart, "qdc/D:time:snr:wtime:phase:abase:sbase:id/b");
    tree->Branch("stop", &rstop, "qdc/D:time:snr:wtime:phase:abase:sbase:id/b");
}

TwoChanTimingProcessor::~TwoChanTimingProcessor() {}

void TwoChanTimingProcessor::DeclarePlots() {
    histo.DeclareHistogram1D(D_CODES, S2, "Error codes");
    histo.DeclareHistogram2D(DD_AMPLITUDE_DISTRIBUTION_START, S8, SC, "Amplitude Distribution - Start");
    histo.DeclareHistogram2D(DD_AMPLITUDE_DISTRIBUTION_STOP, S8, SC, "Amplitude Distribution - Stop");
    histo.DeclareHistogram2D(DD_TRACES_START, S8, SB, "Random Sample of Start Traces");
    histo.DeclareHistogram2D(DD_TRACES_STOP, S8, SB, "Random Sample of Stop Traces");
}

bool TwoChanTimingProcessor::PreProcess(RawEvent &event) {
    if (!EventProcessor::PreProcess(event))
        return false;

    //Define a map to hold the information
    TimingMap pulserMap;

    //plot the number of times we called the function
    histo.Plot(D_CODES, PROCESS_CALLED);

    static const auto &pulserEvents = event.GetSummary("pulser")->GetList();

    for (const auto &it : pulserEvents) {
        int location = it->GetChanID().GetLocation();
        string subType = it->GetChanID().GetSubtype();

        TimingDefs::TimingIdentifier key(location, subType);
        pulserMap.insert(make_pair(key, HighResTimingData(*it)));
    }

    if (pulserMap.empty() || pulserMap.size() % 2 != 0) {
        if(pulserMap.empty())
            histo.Plot(D_CODES, MAP_EMPTY);
        if(pulserMap.size() % 2 != 0)
            histo.Plot(D_CODES, MAP_UNEVEN);
        EndProcess();
        return false;
    }

    HighResTimingData start = (*pulserMap.find(make_pair(0, "start"))).second;
    HighResTimingData stop = (*pulserMap.find(make_pair(0, "stop"))).second;

    static int trcCounter = 0;
    int bin = 0;
    for (auto it = start.GetTrace().begin(); it != start.GetTrace().end(); it++) {
        bin = (int) (it - start.GetTrace().begin());
        //startTraces->Fill(bin, trcCounter, *it);
        histo.Plot(DD_AMPLITUDE_DISTRIBUTION_START, bin, *it);
    }

    for (auto it = stop.GetTrace().begin(); it != stop.GetTrace().end(); it++) {
        bin = (int) (it - stop.GetTrace().begin());
        //stopTraces->Fill(bin, trcCounter, *it);
        histo.Plot(DD_AMPLITUDE_DISTRIBUTION_STOP, bin, *it);
    }
    trcCounter++;

    //We only plot and analyze the data if the data is validated
    if (start.GetIsValid() && stop.GetIsValid()) {
        start.FillRootStructure(rstart);
        stop.FillRootStructure(rstop);
        tree->Fill();
        start.ZeroRootStructure(rstart);
        stop.ZeroRootStructure(rstop);
    }
    EndProcess();
    return true;
}