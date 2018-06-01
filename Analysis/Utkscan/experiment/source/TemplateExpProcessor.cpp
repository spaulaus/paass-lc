///@file TemplateExpProcessor.cpp
///@brief Example class for experiment specific setups
///@author S. V. Paulauskas
///@date May 20, 2016
#include "TemplateExpProcessor.hpp"

#include "DammPlotIds.hpp"
#include "DetectorDriver.hpp"
#include "CloverProcessor.hpp"
#include "TemplateProcessor.hpp"

static double tof_;
static double tEnergy;

namespace dammIds {
    ///This namespace contains histogram IDs that we want to use. The actual histogram ID will be the number defined
    /// here plus experiment::OFFSET (6050). D_TSIZE has an ID of 6050 and D_GEENERGY has an ID of 6051.
    namespace experiment {
        const int D_TSIZE = 0; //!< Size of the event
        const int D_GEENERGY = 1; //!< Gamma energy
        const int DD_TENVSGEN = 2; //!< Energy vs Gamma Energy
    }
}

using namespace std;
using namespace dammIds::experiment;

///DeclarehistogramXD registers the histograms with ROOT. If you want to define a new histogram add its
/// ID in the experiment namespace above, and then declare it here. The ROOT file contains IDs prefixed with an
/// "h". This is due to a C++ restriction of variable names starting with a number.
void TemplateExpProcessor::DeclarePlots() {
    histo.DeclareHistogram1D(D_TSIZE, S3, "Num Template Evts");
    histo.DeclareHistogram1D(D_GEENERGY, SA, "Gamma Energy with Cut");
    histo.DeclareHistogram2D(DD_TENVSGEN, SA, SA, "Template En vs. Ge En");
}

TemplateExpProcessor::TemplateExpProcessor() : EventProcessor(OFFSET, RANGE, "TemplateExpProcessor") {
    gCutoff_ = 0.; ///Set the gamma cutoff energy to a default of 0.
    SetAssociatedTypes();
    SetupRootOutput();
}

TemplateExpProcessor::TemplateExpProcessor(const double &gcut) : EventProcessor(OFFSET, RANGE, "TemplateExpProcessor") {
    gCutoff_ = gcut;
    SetAssociatedTypes();
    SetupRootOutput();
}

TemplateExpProcessor::~TemplateExpProcessor() = default;

///Associates this Experiment Processor with template and ge detector types
void TemplateExpProcessor::SetAssociatedTypes() {
    associatedTypes.insert("template");
    associatedTypes.insert("clover");
}

///Registers the ROOT tree and branches with RootHandler.
void TemplateExpProcessor::SetupRootOutput() {
    tree_ = RootHandler::get()->RegisterTree("data", "Tree that stores some of our data");
    RootHandler::get()->RegisterBranch("data", "tof", &tof_, "tof/D");
    RootHandler::get()->RegisterBranch("ten", "ten", &tEnergy, "ten/D");
}

///Main processing of data of interest
bool TemplateExpProcessor::Process(RawEvent &event) {
    if (!EventProcessor::Process(event))
        return (false);

    ///Vectors to hold the information we will get from the various processors
    vector < ChanEvent * > geEvts, tEvts;
    vector <vector<AddBackEvent>> geAddback;

    ///Obtain the list of pre-processed template events that were created in TemplateProcessor::PreProecess
    if (event.GetSummary("template")->GetList().empty())
        tEvts = dynamic_cast<TemplateProcessor*>(DetectorDriver::get()->GetProcessor("TemplateProcessor"))->GetTemplateEvents();

    ///Obtain the list of Ge events and addback events that were created in CloverProcessor::PreProcess
    if (event.GetSummary("clover")->GetList().empty()) {
        CloverProcessor *cloverProcessor =
                dynamic_cast<CloverProcessor*>(DetectorDriver::get()->GetProcessor("CloverProcessor"));
        geEvts = cloverProcessor->GetGeEvents();
        geAddback = cloverProcessor->GetAddbackEvents();
    }

    ///Plot the size of the template events vector in two ways
    histo.Plot(D_TSIZE, tEvts.size());

    ///Begin loop over template events
    for (const auto &templateEvent : tEvts) {
        ///Begin loop over ge events
        for (const auto &geEvent : geEvts) {
            double gEnergy = geEvent->GetCalibratedEnergy();

            ///Plot the Template Energy vs. Ge Energy
            histo.Plot(DD_TENVSGEN, gEnergy, templateEvent->GetEnergy());

            tEnergy = templateEvent->GetEnergy();
            tof_ = templateEvent->GetTime() - geEvent->GetWalkCorrectedTime();
            tree_->Fill();
            tEnergy = tof_ = -9999;

            ///Plot the Ge energy with a cut
            if (gEnergy > gCutoff_)
                histo.Plot(D_GEENERGY, gEnergy);
        }
    }
    EndProcess();
    return true;
}
