///@file CfdAnalyzer.cpp
///@brief Uses a Digital CFD to obtain waveform phases
/// This code will obtain the phase of a waveform using a digital CFD. Currently the only method is a polynomial fit
/// to the crossing point. For 100-250 MHz systems, this is not going to produce good timing. This code was originally
/// written by S. Padgett.
///@author S. V. Paulauskas
///@date July 22, 2011
#include "CfdAnalyzer.hpp"

#include "PolynomialCfd.hpp"
#include "TraditionalCfd.hpp"

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


    const tuple<double, double, double> pars = cfg.GetCfdParameters();

    ///@TODO We do not currently have any CFDs that require L, so we are not going to pass that variable. In
    /// addition, we do not have an overloaded version of CalculatePhase that takes a tuple<double, double, double>
    trace.SetPhase(driver_->CalculatePhase(trace.GetWaveform(), make_pair(get<0>(pars), get<1>(pars)),
                                           trace.GetExtrapolatedMaxInfo(), trace.GetBaselineInfo()));
    EndAnalyze();
}
