///@file Anl1471Processor.cpp
///@brief A class to process data from ANL1471 experiment using VANDLE.
///@author S. Z. Taylor and S. V. Paulauskas
///@date July 14, 2015
#include "Anl1471Processor.hpp"

#include "BarBuilder.hpp"
#include "CloverProcessor.hpp"
#include "DammPlotIds.hpp"
#include "DetectorDriver.hpp"
#include "DoubleBetaProcessor.hpp"
#include "RootHandler.hpp"
#include "VandleProcessor.hpp"

#include <TTree.h>

#include <iostream>

struct VandleRoot {
    double tof;
    double qdc;
    double snrl;
    double snrr;
    double pos;
    double tdiff;
    double ben;
    double bqdcl;
    double bqdcr;
    double bsnrl;
    double bsnrr;
    double cyc;
    double bcyc;
    double HPGE;
    int vid;
    int vtype;
    int bid;
};

struct TapeInfo {
    bool move;
    bool beam;
};

struct GammaRoot {
    double gen;
    double gtime;
    double gcyc;//might be repetative with gtime?
    double gben;
    double gbtime;
    double gbcyc;
    int gid;
    int gbid;
};

TapeInfo tapeinfo;
VandleRoot vroot;
GammaRoot groot;

namespace dammIds {
    namespace experiment {
        const int DD_DEBUGGING0 = 0;
        const int DD_DEBUGGING1 = 1;
        const int DD_DEBUGGING2 = 2;
        const int DD_DEBUGGING3 = 3;
        const int DD_DEBUGGING4 = 4;
        const int DD_MedCTOFvQDC = 5;
        const int DD_MedVetoed = 6;
        const int DD_SmCTOFvQDC = 7;
        const int DD_SmVetoed = 8;
        const int DD_DEBUGGING9 = 9;
        const int D_tape = 10;
        const int D_beam = 11;
        const int DD_grow_decay = 12;
        const int D_GAMMA_MULT =13;
    }
}//namespace dammIds

using namespace std;
using namespace dammIds::experiment;

void Anl1471Processor::DeclarePlots(void) {
    histo.DeclareHistogram2D(DD_DEBUGGING0, SB, SD, "left-MaxValvsTDIFF");
    histo.DeclareHistogram2D(DD_DEBUGGING1, SB, SD, "right-MaxValvsTDIFF");
    histo.DeclareHistogram2D(DD_DEBUGGING2, SB, S6, "TDIFF-vandle");
    histo.DeclareHistogram1D(DD_DEBUGGING3, S7, "Vandle Multiplicity");
    histo.DeclareHistogram1D(DD_DEBUGGING4, S7, "Beta Multiplicity");
    histo.DeclareHistogram2D(DD_MedCTOFvQDC, SC, SD, "ANL-medium-<E>-vs-CorTof");
    histo.DeclareHistogram2D(DD_MedVetoed, SC, SD, "ANL-medium-vetoed");
    histo.DeclareHistogram2D(DD_SmCTOFvQDC, SC, SD, "ANL-small-<E>-vs-CorTof");
    histo.DeclareHistogram2D(DD_SmVetoed, SC, SD, "ANL-small-vetoed");
    histo.DeclareHistogram2D(DD_DEBUGGING9, SD, S6, "BSNRLvsBQDCL");
    histo.DeclareHistogram1D(D_tape, S1, "tape move");
    histo.DeclareHistogram1D(D_beam, S1, "beam on/off");
    histo.DeclareHistogram2D(DD_grow_decay, SC, SA, "Grow/Decay");
    histo.DeclareHistogram1D(D_GAMMA_MULT, S3, "Gamma Multiplicity");
}//end declare plots

Anl1471Processor::Anl1471Processor() : EventProcessor(OFFSET, RANGE, "Anl1471Processor") {
    associatedTypes.insert("vandle");
    associatedTypes.insert("beta");
    associatedTypes.insert("ge");

    vandleTree_ = RootHandler::get()->RegisterTree("vandle", "VANDLE event data");
    RootHandler::get()->RegisterBranch("vandle", "vandle", &vroot,"tof/D:qdc:snrl:snrr:pos:tdiff:ben:bqdcl:bqdcr:bsnrl:bsnrr:cyc:bcyc:HPGE:vid/I:vtype:bid");

    gammaTree_ = RootHandler::get()->RegisterTree("gammas", "Gamma event data");
    RootHandler::get()->RegisterBranch("gammas", "gamma", &groot, "gen/D:gtime:gcyc:gben:gbtime:gbcyc:gid/I:gbid");
}

Anl1471Processor::~Anl1471Processor() = default;

bool Anl1471Processor::Process(RawEvent &event) {
    if (!EventProcessor::Process(event))
        return (false);
    double plotMult_ = 2;
    double plotOffset_ = 1000;

    BarMap vbars, betas;
    map<unsigned int, pair<double, double> > lrtBetas;

    BarMap betaStarts_;
    vector<ChanEvent *> geEvts;
    vector<vector<AddBackEvent>> geAddback;

    if (!event.GetSummary("vandle")->GetList().empty())
        vbars = ((VandleProcessor *) DetectorDriver::get()->GetProcessor("VandleProcessor"))->GetBars();


    static const vector<ChanEvent *> &doubleBetaStarts = event.GetSummary("beta:double:start")->GetList();
    BarBuilder startBars(doubleBetaStarts);
    startBars.BuildBars();
    betaStarts_ = startBars.GetBarMap();

    if (!event.GetSummary("ge")->GetList().empty()) {
        geEvts = ((CloverProcessor *) DetectorDriver::get()->GetProcessor("CloverProcessor"))->GetGeEvents();
        geAddback = ((CloverProcessor *) DetectorDriver::get()->GetProcessor("CloverProcessor"))->GetAddbackEvents();
    }

    tapeinfo.move = TreeCorrelator::get()->place("TapeMove")->status();
    tapeinfo.beam = TreeCorrelator::get()->place("Beam")->status();

    histo.Plot(D_tape, tapeinfo.move);
    histo.Plot(D_beam, tapeinfo.beam);
    histo.Plot(DD_DEBUGGING3, vbars.size());
    histo.Plot(DD_DEBUGGING4, betaStarts_.size());
    histo.Plot(D_GAMMA_MULT, geEvts.size());

    //Begin processing for VANDLE bars
    unsigned int barType = 0;
    for (BarMap::iterator it = vbars.begin(); it != vbars.end(); it++) {
        TimingDefs::TimingIdentifier barId = (*it).first;
        BarDetector bar = (*it).second;

        if (!bar.GetHasEvent())
            continue;

        if (bar.GetType() == "small")
            barType = 0;
        else if (bar.GetType() == "medium")
            barType = 1;

        unsigned int barLoc = barId.first;
        const TimingCalibration cal = bar.GetCalibration();

        for (BarMap::iterator itStart = betaStarts_.begin(); itStart != betaStarts_.end(); itStart++) {
            BarDetector beta_start = (*itStart).second;
            unsigned int startLoc = (*itStart).first.first;
            if (!beta_start.GetHasEvent())
                continue;
            double tofOffset = cal.GetTofOffset(startLoc);
            double tof = bar.GetCorTimeAve() - beta_start.GetCorTimeAve() + tofOffset;

            histo.Plot(DD_DEBUGGING1, tof * plotMult_ + plotOffset_, bar.GetQdc());

            double corTof = ((VandleProcessor *) DetectorDriver::get()->GetProcessor("VandleProcessor"))->CorrectTOF(tof, bar.GetFlightPath(), cal.GetZ0());

            //tape move veto cut damm
            if (!tapeinfo.move) { //plot only if tape is NOT moving
                if (bar.GetType() == "medium")
                    histo.Plot(DD_MedCTOFvQDC, corTof * 2 + 1000, bar.GetQdc());

                if (bar.GetType() == "small")
                    histo.Plot(DD_SmCTOFvQDC, corTof * 2 + 1000, bar.GetQdc());
            } else {
                if (bar.GetType() == "medium")
                    histo.Plot(DD_MedVetoed, corTof * 2 + 1000, bar.GetQdc());

                if (bar.GetType() == "small")
                    histo.Plot(DD_SmVetoed, corTof * 2 + 1000, bar.GetQdc());
            }

            histo.Plot(DD_DEBUGGING9, beta_start.GetLeftSide().GetTraceQdc(),
                       beta_start.GetLeftSide().GetTrace().GetSignalToNoiseRatio());

            //adding HPGE energy info to vandle tree
            double HPGE_energy = -9999.0;
            if (!geEvts.empty()) {
                for (vector<ChanEvent *>::const_iterator itHPGE = geEvts.begin(); itHPGE != geEvts.end(); itHPGE++)
                    HPGE_energy = (*itHPGE)->GetCalibratedEnergy();
            } else
                HPGE_energy = -8888.0;

            vroot.tof = corTof * 2 + 1000;//to make identical to damm output
            vroot.qdc = bar.GetQdc();
            vroot.snrl = bar.GetLeftSide().GetTrace().GetSignalToNoiseRatio();
            vroot.snrr = bar.GetRightSide().GetTrace().GetSignalToNoiseRatio();
            vroot.pos = bar.GetQdcPosition();
            vroot.tdiff = bar.GetTimeDifference();
            vroot.ben = beta_start.GetQdc();
            vroot.bqdcl = beta_start.GetLeftSide().GetTrace().GetQdc();
            vroot.bqdcr = beta_start.GetRightSide().GetTrace().GetQdc();
            vroot.bsnrl = beta_start.GetLeftSide().GetTrace().GetSignalToNoiseRatio();
            vroot.bsnrr = beta_start.GetRightSide().GetTrace().GetSignalToNoiseRatio();
            vroot.cyc = 0;  /////////it.GetEventTime();
            vroot.bcyc = 0;  /////////itStart.GetEventTime()
            vroot.HPGE = HPGE_energy;
            vroot.vid = barLoc;
            vroot.vtype = barType;
            vroot.bid = startLoc;
            vandleTree_->Fill();
        } // for(TimingMap::iterator itStart
    } //(BarMap::iterator itBar
    //End processing for VANDLE bars

    //Stuff to fill HPGe branch
    if (!geEvts.empty()) {
        for (vector<ChanEvent *>::const_iterator itGe = geEvts.begin(); itGe != geEvts.end(); itGe++) {
            double ge_energy, ge_time, gb_time_L, gb_time_R, gb_time, grow_decay_time, gb_en, gcyc_time;
            ge_energy = ge_time = gb_time_L = gb_time_R = gb_time = grow_decay_time = gb_en = gcyc_time = -9999.0;
            int ge_id = -9999;
            int gb_startLoc = -9999;
            BarDetector gb_start;
            ge_energy = (*itGe)->GetCalibratedEnergy();
            ge_id = (*itGe)->GetChanID().GetLocation();
            ge_time = (*itGe)->GetWalkCorrectedTime();
            ge_time *= (Globals::get()->GetClockInSeconds() * 1.e9);//in ns now

            if (TreeCorrelator::get()->place("Cycle")->status()) {
                gcyc_time = TreeCorrelator::get()->place("Cycle")->last().time;
                gcyc_time *= (Globals::get()->GetClockInSeconds() * 1.e9);//in ns now
                grow_decay_time = (ge_time - gcyc_time) * 1e-9 * 1e2;//in seconds, then ms
                histo.Plot(DD_grow_decay, ge_energy, grow_decay_time);
            }

            if (!doubleBetaStarts.empty()) {
                for (BarMap::iterator itGB = betaStarts_.begin(); itGB != betaStarts_.end(); itGB++) {
                    gb_start = (*itGB).second;
                    gb_startLoc = (*itGB).first.first;
                    gb_en = gb_start.GetQdc();
                    gb_time_L = gb_start.GetLeftSide().GetHighResTimeInNs();
                    //GetCorrectedTime()??
                    gb_time_R = gb_start.GetRightSide().GetHighResTimeInNs();
                    //GetTimeAverage()??
                    gb_time = (gb_time_L + gb_time_R) / 2;
                    gb_time *= (Globals::get()->GetClockInSeconds() * 1.e9);//in ns now
                }
            } else {
                gb_startLoc = -9999;
                gb_en = -9999;
                gb_time = -9999;
            }

            groot.gen = ge_energy;
            groot.gtime = ge_time;
            groot.gcyc = ge_time - gcyc_time;
            groot.gben = gb_en;
            groot.gbtime = gb_time;
            groot.gbcyc = gb_time - gcyc_time;
            groot.gid = ge_id;
            groot.gbid = gb_startLoc;
            gammaTree_->Fill();
        }
    }

    EndProcess();
    return true;
}