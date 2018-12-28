# @author S. V. Paulauskas, K. Smith
set(UTKSCAN_ANALYZER_SOURCES CfdAnalyzer.cpp FittingAnalyzer.cpp TauAnalyzer.cpp TraceExtractor.cpp
        TraceFilterAnalyzer.cpp TraceAnalyzer.cpp WaaAnalyzer.cpp WaveformAnalyzer.cpp)
add_library(UtkscanAnalyzerObjects OBJECT ${UTKSCAN_ANALYZER_SOURCES})



