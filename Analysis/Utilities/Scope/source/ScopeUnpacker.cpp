///@file ScopeUnpacker.cpp
///@brief Unpacker class for scope program
///@author C. R. Thornsberry, S. V. Paulauskas
///@date May 19, 2017
#include "ScopeUnpacker.hpp"

#include "HelperFunctions.hpp"
#include "RootInterface.hpp"
#include "TrapFilterParameters.hpp"
#include "TraceFilter.hpp"

#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TH2F.h>
#include <TFile.h>
#include <TF1.h>
#include <TLine.h>
#include <TProfile.h>
#include <TPaveStats.h>

#include <deque>
#include <fstream>
#include <vector>

#include <cmath>
#include <ctime>

using namespace std;
using namespace TraceFunctions;

/// Default constructor.
ScopeUnpacker::ScopeUnpacker(const unsigned int &mod/*=0*/, const unsigned int &chan/*=0*/) : Unpacker() {
    saveFile_ = "";
    mod_ = mod;
    chan_ = chan;
    threshLow_ = 0;
    threshHigh_ = numeric_limits<unsigned int>::max();
    resetGraph_ = false;

    time(&last_trace);

    performFit_ = false;
    performCfd_ = false;
    performFiltering_ = false;
    numEvents_ = 20;
    numAvgWaveforms_ = 1;
    cfdF_ = 0.5;
    cfdD_ = 1;
    cfdL_ = 1;
    fitLow_ = 10;
    fitHigh_ = 15;
    delayInSeconds_ = 2;
    numTracesDisplayed_ = 0;

    filterer_ = new TraceFilter(int((1. / mask_.GetFrequency())*1000));
    filterGraph_ = new TGraph();
    filterGraph_->SetLineColor(kRed);
    filtererText_ = new TPaveText(0.15, 0.75, 0.45, 0.85, "NDC");
    filtererText_->SetFillColorAlpha(kWhite, 1.0);

    graph = new TGraph();
    hist = new TH2F("hist", "", 256, 0, 1, 256, 0, 1);

    cfdLine = new TLine();
    cfdLine->SetLineColor(kRed);
    cfdPol3 = new TF1("cfdPol3", "pol3");
    cfdPol3->SetLineColor(kGreen + 1);
    cfdPol2 = new TF1("cfdPol2", "pol2");
    cfdPol2->SetLineColor(kMagenta + 1);

    SelectFittingFunction("vandle");

    gStyle->SetPalette(51);

    //Display the stats: Integral
    gStyle->SetOptStat(1000000);

    //Display Fit Stats: Fit Values, Errors, and ChiSq.
    gStyle->SetOptFit(111);
}

ScopeUnpacker::~ScopeUnpacker() {
    delete graph;
    delete cfdLine;
    delete cfdPol3;
    delete cfdPol2;
    delete hist;
    delete fittingFunction_;
    delete crystalBallFunction_;
    delete csiFunction_;
    delete emCalTimingFunction_;
    delete siPmtFastTimingFunction_;
    delete vandleTimingFunction_;
    delete triggerFilterParameters_;
    delete energyFilterParameters_;
    delete filterer_;
    delete filtererText_;
    delete filterGraph_;
}

void ScopeUnpacker::SetFilterParameters(const std::string &type, const double &l, const double &g, const double &t) {
    if(type == "trigger" || type == "fast") {
        triggerFilterParameters_ = new TrapFilterParameters(l, g, t);
        filterer_->SetTriggerParams(*triggerFilterParameters_);
    }
    if(type == "energy" || type == "slow") {
        energyFilterParameters_ = new TrapFilterParameters(l, g, t);
        filterer_->SetEnergyParams(*energyFilterParameters_);
    }
}

void ScopeUnpacker::ResetGraph(const unsigned int &size) {
    delete graph;

    graph = new TGraph(size);
    graph->SetMarkerStyle(kFullDotSmall);

    if (size != x_vals.size()) {
        cout << "ScopeUnpacker::ResetGraph : " << "Changing trace length from " << x_vals.size() << " to " << size << " ns.\n";
        x_vals.resize(size);
        for (size_t index = 0; index < x_vals.size(); index++)
            x_vals[index] = index;
    }

    hist->SetBins(x_vals.size(), x_vals.front(), x_vals.back(), 1, 0, 1);

    stringstream stream;
    stream << "M" << mod_ << "C" << chan_;
    graph->SetTitle(stream.str().c_str());
    hist->SetTitle(stream.str().c_str());

    resetGraph_ = false;
}

bool ScopeUnpacker::SelectFittingFunction(const std::string &func) {
    if (func == "crystalball" || func == "cb") {
        crystalBallFunction_ = new CrystalBallFunction();
        fittingFunction_ = new TF1("func", crystalBallFunction_, 0., 1.e6, 6);
        fittingFunction_->SetParNames("phase", "amplitude", "alpha", "n", "sigma", "baseline");
    } else if (func == "csi") {
        csiFunction_ = new CsiFunction();
        fittingFunction_ = new TF1("func", csiFunction_, 0., 1.e6, 5);
        fittingFunction_->SetParNames("phase", "amplitude", "n", "tau0", "baseline");
    } else if (func == "emcal") {
        emCalTimingFunction_ = new EmCalTimingFunction();
        fittingFunction_ = new TF1("func", emCalTimingFunction_, 0., 1.e6, 5);
        fittingFunction_->SetParNames("phase", "amplitude", "n", "tau", "baseline");
    } else if (func == "sipm-fast") {
        siPmtFastTimingFunction_ = new SiPmtFastTimingFunction();
        fittingFunction_ = new TF1("func", siPmtFastTimingFunction_, 0., 1.e6, 4);
        fittingFunction_->SetParNames("phase", "amplitude", "sigma", "baseline");
    } else if (func == "vandle") {
        vandleTimingFunction_ = new VandleTimingFunction();
        fittingFunction_ = new TF1("func", vandleTimingFunction_, 0., 1.e6, 5);
        fittingFunction_->SetParNames("phase", "amplitude", "beta", "gamma", "baseline");
    } else
        return false;
    return true;
}

/** Process all events in the event list.
  * \param[in]  addr_ Pointer to a location in memory.
  * \return Nothing.
  */
void ScopeUnpacker::ProcessRawEvent() {
    XiaData *current_event = NULL;

    // Fill the processor event deques with events
    while (!rawEvent.empty()) {
        if (!running)
            break;

        //Get the first event in the FIFO.
        current_event = rawEvent.front();
        rawEvent.pop_front();

        // Safety catches for null event or empty ->GetTrace().
        if (!current_event || current_event->GetTrace().empty())
            continue;

        if (current_event->GetModuleNumber() != mod_ &&
            current_event->GetChannelNumber() != chan_)
            continue;

        pair<double, double> baseline = CalculateBaseline(current_event->GetTrace(), make_pair(0, 10));
        pair<double, double> maximum = FindMaximum(current_event->GetTrace(), current_event->GetTrace().size());
        double qdc = CalculateQdc(current_event->GetTrace(), make_pair(5, 15));

        if (maximum.second < threshLow_ || (threshHigh_ > threshLow_ && maximum.second > threshHigh_)) {
            delete current_event;
            continue;
        }

        //Convert the XiaData object into a ProcessedXiaData object
        ProcessedXiaData *channel_event = new ProcessedXiaData(*current_event);

        channel_event->GetTrace().SetBaseline(baseline);
        channel_event->GetTrace().SetMax(maximum);
        channel_event->GetTrace().SetQdc(qdc);

        //Push the channel event into the deque.
        chanEvents_.push_back(channel_event);

        // Handle the individual XiaData.
        if (chanEvents_.size() >= numAvgWaveforms_)
            ProcessEvents();
    }
}

void ScopeUnpacker::Plot() {
    static float histAxis[2][2];

    if (chanEvents_.size() < numAvgWaveforms_)
        return;

    unsigned long traceSize = chanEvents_.front()->GetTrace().size();

    ResetGraph(traceSize);

    if (traceSize != x_vals.size())
        resetGraph_ = true;

    if (resetGraph_) {
        ResetGraph(traceSize);
        RootInterface::get()->ResetZoom();
        for (int i = 0; i < 2; i++) {
            histAxis[i][0] = numeric_limits<float>::max();
            histAxis[i][1] = -numeric_limits<float>::max();
        }
    }

    if (numAvgWaveforms_ == 1) {
        Trace trc = chanEvents_.front()->GetTrace();
        int index = 0;
        for (size_t i = 0; i < traceSize; ++i, index++)
            graph->SetPoint(index, x_vals[i], trc.at(i));

        RootInterface::get()->UpdateZoom();

        graph->Draw("AP0");

        float lowVal = (trc.GetMaxInfo().first - fitLow_);
        float highVal = (trc.GetMaxInfo().first + fitHigh_);

//        if (performCfd_ && trc.GetTraceSansBaseline().size() != 0) {
//            PolynomialCfd cfd;
//            double phase = cfd.CalculatePhase(trc.GetTraceSansBaseline(),
//                                              make_pair(cfdF_, cfdD_),
//                                              trc.GetMaxInfo(),
//                                              trc.GetBaselineInfo());
//            cout << "Unpacker::Plot - CFD Phase = " << phase << endl;
//        }

        if (performFit_) {
            fittingFunction_->SetParameters(trc.GetMaxInfo().first, 0.5 * trc.GetQdc(), 0.4, 0.1, 4);
            fittingFunction_->FixParameter(fittingFunction_->GetParNumber("baseline"), trc.GetBaselineInfo().first);
            graph->Fit(fittingFunction_, "WRQ", "", lowVal, highVal);
        }

        if(performFiltering_) {
            filtererText_->Clear();
            if(!filterer_->CalcFilters(&chanEvents_.front()->GetTrace())) {
                auto triggerFilter = filterer_->GetTriggerFilter();
                for (size_t i = 0; i < triggerFilter.size(); ++i)
                    filterGraph_->SetPoint(i, x_vals[i], triggerFilter.at(i) + trc.GetBaselineInfo().first);
                filterGraph_->Draw("SAME");
                filtererText_->AddText(("Filtered Energy = " + to_string(filterer_->GetEnergy())).c_str());
                filtererText_->AddText(("Trigger Pos = " + to_string(filterer_->GetTrigger())).c_str());
                ((TText*)filtererText_->GetListOfLines()->Last())->SetTextColor(kRed);
                filtererText_->Draw("NDC");
            }
        }
    } else {
        //For multiple events with make a 2D histogram and plot the profile on top.
        //Determine the maximum and minimum values of the events.
        for (unsigned int i = 0; i < numAvgWaveforms_; i++) {
            ProcessedXiaData *evt = chanEvents_.at(i);
            float evtMin = *min_element(evt->GetTrace().begin(), evt->GetTrace().end());
            float evtMax = *max_element(evt->GetTrace().begin(), evt->GetTrace().end());
            evtMin -= fabs(0.1 * evtMax);
            evtMax += fabs(0.1 * evtMax);
            if (evtMin < histAxis[1][0]) histAxis[1][0] = evtMin;
            if (evtMax > histAxis[1][1]) histAxis[1][1] = evtMax;
        }

        //Reset the histogram
        hist->Reset();

        //Rebin the histogram
        hist->SetBins(x_vals.size(), x_vals.front(), x_vals.back(), histAxis[1][1] - histAxis[1][0],
                      histAxis[1][0], histAxis[1][1]);

        //Fill the histogram
        for (unsigned int i = 0; i < numAvgWaveforms_; i++) {
            ProcessedXiaData *evt = chanEvents_.at(i);
            for (size_t i = 0; i < evt->GetTrace().size(); ++i) {
                hist->Fill(x_vals[i], evt->GetTrace()[i]);
            }
        }

        prof = hist->ProfileX("AvgPulse");
        prof->SetLineColor(kRed);
        prof->SetMarkerColor(kRed);

        double lowVal = prof->GetBinCenter(prof->GetMaximumBin() - fitLow_);
        double highVal = prof->GetBinCenter(prof->GetMaximumBin() + fitHigh_);

        if (performFit_) {
            fittingFunction_->SetParameters(lowVal, 0.5 * chanEvents_.front()->GetTrace().GetQdc(), 0.3, 0.1);
            fittingFunction_->FixParameter(4, chanEvents_.front()->GetTrace().GetBaselineInfo().first);
            hist->Fit(fittingFunction_, "WRQ", "", lowVal, highVal);
        }

        hist->SetStats(false);
        hist->Draw("COLZ");
        prof->Draw("SAMES");

        RootInterface::get()->UpdateZoom();
        RootInterface::get()->GetCanvas()->Update();

        TPaveStats *stats = (TPaveStats *) prof->GetListOfFunctions()->FindObject("stats");
        if (stats) {
            stats->SetX1NDC(0.55);
            stats->SetX2NDC(0.9);
        }
    }

    // Update the canvas.
    RootInterface::get()->GetCanvas()->Update();

    // Save the TGraph to a file.
    if (saveFile_ != "") {
        TFile f((saveFile_ + ".root").c_str(), "RECREATE");
        graph->Clone("trace")->Write();
        f.Close();

        ofstream ascii((saveFile_ + ".dat").c_str());
        vector<unsigned int> trc = chanEvents_.front()->GetTrace();
        for (vector<unsigned int>::iterator it = trc.begin(); it != trc.end(); it++)
            ascii << int(it - trc.begin()) << " " << *it << endl;
        saveFile_ = "";
    }

    // Remove the events from the deque.
    for (unsigned int i = 0; i < numAvgWaveforms_; i++) {
        delete chanEvents_.front();
        chanEvents_.pop_front();
    }

    numTracesDisplayed_++;
}

/** Process all channel events read in from the rawEvent.
  * This method should only be called from skeletonUnpacker::ProcessRawEvent().
  * \return True if events were processed and false otherwise.
  */
bool ScopeUnpacker::ProcessEvents() {
    //Check if we have delayed the plotting enough
    time_t cur_time;
    time(&cur_time);
    while (difftime(cur_time, last_trace) < delayInSeconds_) {
        //If in shm mode and the plotting time has not allotted the events are
        // cleared and this function is aborted.
//        if (ShmMode()) {
//            ClearEvents();
//            return false;
//        } else {
//            IdleTask();
        time(&cur_time);
//        }
    }

    //When we have the correct number of waveforms we plot them.
    Plot();

    //If this is a single capture we stop the plotting.
    if (singleCapture_)
        running = false;

    //Update the time.
    time(&last_trace);

    return true;
}

void ScopeUnpacker::ClearEvents() {
    while (!chanEvents_.empty()) {
        delete chanEvents_.front();
        chanEvents_.pop_front();
    }
}