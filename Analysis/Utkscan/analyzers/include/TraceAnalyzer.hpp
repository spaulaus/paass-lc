/** \file TraceAnalyzer.hpp
 * \brief Header file for the TraceAnalyzer class
 * \author S. Liddick, D. Miller, S. V. Paulauskas
 * \date 02 July 2007
 */
#ifndef __TRACEANALYZER_HPP_
#define __TRACEANALYZER_HPP_

#include <string>
#include <sys/times.h>

#include "ChannelConfiguration.hpp"
#include "Plots.hpp"
#include "Trace.hpp"

///Abstract class that all trace analyzers are derived from
class TraceAnalyzer {
public:
    /** Default Constructor */
    TraceAnalyzer();

    /** Default Destructor */
    virtual ~TraceAnalyzer();

    /** Constructor taking histogram information
    * \param [in] offset : the offset for the histograms
    * \param [in] range : the range of the histograms
    * \param [in] name : the name of the processor */
    TraceAnalyzer(const unsigned int &offset, const unsigned int &range, const std::string &name);

    /** Initializes the Analyzer
    * \return True if the init was successful */
    virtual bool Init(void) { return (true); };

    /** Declare Plots (empty for now) */
    virtual void DeclarePlots(void) {};

    ///Function to analyze a trace online.
    ///@param [in] trace: the trace
    ///@param [in] cfg : Configuration for the channel to analyze.
    virtual void Analyze(Trace &trace, const ChannelConfiguration &cfg);

    /** End the analysis and record the analyzer level in the trace
     * \param [in] trace : the trace */
    void EndAnalyze(Trace &trace);

    /** Finish analysis updating the analyzer timing information */
    void EndAnalyze(void);

    /** Set the level of the trace analysis
     * \param [in] i : the level of the analysis to be done */
    void SetLevel(int i) { level = i; }

    /** \return the level of the trace analysis */
    int GetLevel() { return level; }

protected:
    int level;                ///< the level of analysis to proceed with
    static int numTracesAnalyzed;    ///< rownumber for DAMM spectrum 850
    std::string name;         ///< name of the analyzer

    /** Plots class for given Processor, takes care of declaration
    * and plotting within boundaries allowed by PlotsRegistry */
    Plots histo;

    /** plot trace into a 1D histogram
    * \param [in] trc : The trace that we want to plot
    * \param [in] id : histogram ID to plot into */
    void Plot(const std::vector<unsigned int> &trc, const int &id);

    /** plot trace into row of a 2D histogram
    * \param [in] trc : The trace that we want to plot
    * \param [in] id : histogram ID to plot into
    * \param [in] row : the row to plot into */
    void Plot(const std::vector<unsigned int> &trc, int id, int row);

    /** plot trace absolute value and scaled into a 1D histogram
    * \param [in] trc : The trace that we want to plot
    * \param [in] id : histogram ID to plot into
    * \param [in] scale : the scaling for the trace */
    void ScalePlot(const std::vector<unsigned int> &trc, int id, double
    scale);

    /** plot trace absolute value and scaled into a 2D histogram
     * \param [in] trc : The trace that we want to plot
    * \param [in] id : histogram ID to plot into
    * \param [in] row : the row to plot the histogram into
    * \param [in] scale : the scaling for the trace */
    void ScalePlot(const std::vector<unsigned int> &trc, int id, int row, double scale);

    /** plot trace with a vertical offset in a 1D histogram
     * \param [in] trc : The trace that we want to plot
    * \param [in] id : histogram ID to plot into
    * \param [in] offset : the offset for the trace */
    void OffsetPlot(const std::vector<unsigned int> &trc, int id, double offset);

    /** plot trace with a vertical offset in a 2D histogram
     * \param [in] trc : The trace that we want to plot
    * \param [in] id : histogram ID to plot into
    * \param [in] row : the row to plot the trace into
    * \param [in] offset : the offset for the trace*/
    void OffsetPlot(const std::vector<unsigned int> &trc, int id, int row, double offset);
private:
    tms tmsBegin;             ///< time at which the analyzer began
    double userTime;          ///< user time used by this class
    double systemTime;        ///< system time used by this class
    double clocksPerSecond;   ///< frequency of system clock
};

#endif // __TRACEANALYZER_HPP_
