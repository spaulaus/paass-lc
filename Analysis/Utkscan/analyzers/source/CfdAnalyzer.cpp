///@file CfdAnalyzer.cpp
///@brief Uses a Digital CFD to obtain waveform phases
/// This will not provide good timing on fast signals (e.g. scintillators) because of its linear treatment of
/// the leading edge.
///@author S. V. Paulauskas
///@date July 22, 2011
#include "CfdAnalyzer.hpp"

#include "PolynomialCfd.hpp"
#include "TraditionalCfd.hpp"
#include "XiaCfd.hpp"

#include <iostream>
#include <vector>
#include <utility>

using namespace std;

CfdAnalyzer::CfdAnalyzer(const std::string &s) : TraceAnalyzer() {
    name = "CfdAnalyzer";
    if (s == "polynomial" || s == "poly")
        driver_ = new PolynomialCfd();
    else if (s == "traditional" || s == "trad")
        driver_ = new TraditionalCfd();
    else if (s == "xia" || s == "XIA")
        driver_ = new XiaCfd();
    else
        driver_ = NULL;
}

void CfdAnalyzer::Analyze(Trace &trace, const ChannelConfiguration &cfg) {
    TraceAnalyzer::Analyze(trace, cfg);

    if (!driver_) {
        EndAnalyze();
        return;
    }

    if (trace.IsSaturated() || trace.empty() || trace.GetWaveform().empty()) {
        EndAnalyze();
        return;
    }

    trace.SetPhase(driver_->CalculatePhase(trace.GetWaveform(), cfg.GetTimingConfiguration(),
                                           trace.GetExtrapolatedMaxInfo(), trace.GetBaselineInfo()) + trace.GetMaxInfo().first);
    EndAnalyze();
}
