/// @file BaGeLProcessor.cpp
/// @brief Experiment processor for the BaGeL campaign at iThemba LABS.
/// @author S. V. Paulauskas
/// @date December 1, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas.
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include <BaGeLProcessor.hpp>

#include <DammPlotIds.hpp>
#include <DetectorDriver.hpp>
#include <RawEvent.hpp>

#include <queue>

using namespace std;

namespace dammIds {
    namespace experiment {
        const int DD_CLOVER_EN = 0; //!< Clover vs Gamma Energy
        const int D_TRIGGER_TDIFF = 2; //!< CI rates (out of full Range of 1000 Hz)
        const int DD_EVENTCOUNTER_VS_TDIFF = 3;
        const int DD_K600COUNT_GECOUNT = 4;
        const int DD_K600_GATED_GAMMAS = 5;
        const int DD_K600_ANTIGATED_GAMMAS = 6;
    }
}

using namespace dammIds::experiment;

BaGeLProcessor::~BaGeLProcessor() {}

void BaGeLProcessor::DeclarePlots() {
    histo.DeclareHistogram2D(DD_CLOVER_EN, SD, 48, "2D Clover Energy per Segment");
    histo.DeclareHistogram1D(D_TRIGGER_TDIFF, SA, "Trigger rates");
    histo.DeclareHistogram2D(DD_EVENTCOUNTER_VS_TDIFF, 6000, 4000, "Event Number vs K600Time - Ge Time in ns");
    histo.DeclareHistogram2D(DD_K600COUNT_GECOUNT, 48, 3, "K600 Counts vs. Ge Counts");
    histo.DeclareHistogram2D(DD_K600_GATED_GAMMAS, 16000, 48, "K600 Gated Gammas");
    histo.DeclareHistogram2D(DD_K600_ANTIGATED_GAMMAS, 16000, 48, "K600 Anti-Gated Gammas");

}

BaGeLProcessor::BaGeLProcessor() : EventProcessor(OFFSET, RANGE, "BaGeLProcessor") {
    SetAssociatedTypes();
}


void BaGeLProcessor::SetAssociatedTypes() {
    associatedTypes.insert("tac");
    associatedTypes.insert("ge");
}

bool BaGeLProcessor::Process(RawEvent &event) {
    if (!EventProcessor::Process(event))
        return false;

    static const auto &triggers = event.GetSummary("tac")->GetList();
    static const auto &clovers = event.GetSummary("ge")->GetList();

    static queue<ChanEvent> triggerQueue;
    static queue<ChanEvent> cloverQueue;

    histo.Plot(DD_K600COUNT_GECOUNT, clovers.size(), triggers.size());

    static int event_counter = 0;

    for (const auto &leaf : clovers) {
        histo.Plot(DD_CLOVER_EN, leaf->GetCalibratedEnergy(), leaf->GetChanID().GetLocation());
        cloverQueue.emplace(*leaf);
    }

    static double previousTriggerTime = 0;
    for (const auto &trig : triggers) {
        triggerQueue.emplace(*trig);
        double triggerTdiff = (trig->GetTimeSansCfd() - previousTriggerTime)
                              * Globals::get()->GetFilterClockInSeconds() * 1e3;
        if (previousTriggerTime == 0)
            triggerTdiff = 0;

        histo.Plot(D_TRIGGER_TDIFF, triggerTdiff);
        previousTriggerTime = trig->GetTimeSansCfd();

        for (const auto &leaf : clovers) {
            auto tdiff_in_ns = (trig->GetTimeSansCfd() - leaf->GetTimeSansCfd())
                               * Globals::get()->GetFilterClockInSeconds() * 1e9;
            histo.Plot(DD_EVENTCOUNTER_VS_TDIFF, tdiff_in_ns, event_counter);
        }
    }

    while (!triggerQueue.empty()) {
        if(cloverQueue.empty())
            break;

        double diff_in_ns = (triggerQueue.front().GetTimeSansCfd() - cloverQueue.front().GetTimeSansCfd())
                                   * Globals::get()->GetFilterClockInSeconds() * 1e9;
        if (diff_in_ns > 500) {
            histo.Plot(DD_K600_ANTIGATED_GAMMAS, cloverQueue.front().GetCalibratedEnergy(),
                       cloverQueue.front().GetChanID().GetLocation());
            cloverQueue.pop();
            continue;
        } else if (diff_in_ns <= 0) {
            triggerQueue.pop();
        } else {
            histo.Plot(DD_K600_GATED_GAMMAS, cloverQueue.front().GetCalibratedEnergy(),
                       cloverQueue.front().GetChanID().GetLocation());
            cloverQueue.pop();
        }
    }

    event_counter++;
    EndProcess();
    return true;
}
