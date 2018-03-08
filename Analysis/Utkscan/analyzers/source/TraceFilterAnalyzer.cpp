/** \file  TraceFilterAnalyzer.cpp
 *  \brief Implements the analysis of traces using trapezoidal filters
 * \author D. Miller, S. V. Paulauskas
 * \date January 2011
 */
#include <sstream>

#include "DammPlotIds.hpp"
#include "Globals.hpp"
#include "RandomInterface.hpp"
#include "TraceFilter.hpp"
#include "TraceFilterAnalyzer.hpp"

using namespace std;
using namespace dammIds::analyzers::tracefilter;

namespace dammIds {
    namespace analyzers {
        namespace tracefilter {
            const unsigned int D_RETVALS = 0; //!< Return values for TraceFilter class
            const unsigned int DD_TRIGGER_FILTER = 1;//!< Trigger Filter
            const unsigned int DD_REJECTED_TRACE = 2;//!< Traces with some kind of error
            const unsigned int DD_PILEUP = 3; //!< Piled up traces
        }
    }
}

TraceFilterAnalyzer::TraceFilterAnalyzer(const bool &analyzePileup) :
        TraceAnalyzer(OFFSET, RANGE, "TraceFilterAnalyzer") {
    analyzePileup_ = analyzePileup;
    name = "TraceFilterAnalyzer";
}

void TraceFilterAnalyzer::DeclarePlots() {
    const int traceBins = dammIds::analyzers::traceBins;
    histo.DeclareHistogram1D(D_RETVALS, S3, "Retvals for Filtering");
    ///@TODO : Enable this one we have completed the note in TraceFilterAnalyzer::Analyze
    //histo.DeclareHistogram2D(DD_TRIGGER_FILTER, traceBins, S7, "Trigger Filter");
    histo.DeclareHistogram2D(DD_REJECTED_TRACE, traceBins, S7, "Rejected Traces");
    histo.DeclareHistogram2D(DD_PILEUP, 2000, S7, "Piled-up Traces");
}

void TraceFilterAnalyzer::Analyze(Trace &trace, const ChannelConfiguration &cfg) {
    TraceAnalyzer::Analyze(trace, cfg);
    static Globals *globs = Globals::get();
    static int numRejected = 0;
    static int numPileup = 0;
    static unsigned short numTraces = S7;

    //Want to put filter clock units of ns/Sample
    TraceFilter filter(globs->GetFilterClockInSeconds() * 1e9, cfg.GetTriggerFilterParameters(),
                       cfg.GetEnergyFilterParameters(), analyzePileup_);
    unsigned int retval = filter.CalcFilters(&trace);
    histo.Plot(D_RETVALS, retval);

    if (retval != 0) {
        if (numRejected < numTraces)
            histo.Plot(DD_REJECTED_TRACE, numRejected++);
        EndAnalyze();
        return;
    }

    trace.SetTriggerFilter(filter.GetTriggerFilter());
    trace.SetTriggerPositions(filter.GetTriggers());
    trace.SetFilteredEnergies(filter.GetEnergies());
    trace.SetEnergySums(filter.GetEnergySums());
    trace.SetFilteredBaseline(filter.GetBaseline());

    if (filter.GetHasPileup() && numPileup < numTraces)
        histo.Plot(DD_PILEUP, numPileup++);

    ///@TODO : We have not enabled users to set histograms with a weight in ROOT. In this routine, we're trying to
    /// plot the actual trace value as the "z-value" or number of counts in a 2D-bin.
//    static int numTrigFilters = 0;
//    unsigned int xval = 0;
//    if(numTrigFilters == 0) {
//        for (const auto &val : filter.GetTriggerFilter())
//            histo.Plot(DD_TRIGGER_FILTER, xval++, numTrigFilters, val);
//    }
//    numTrigFilters++;

    EndAnalyze(trace);
}
