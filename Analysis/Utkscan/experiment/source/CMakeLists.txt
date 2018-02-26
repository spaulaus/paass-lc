#@authors S. V. Paulauskas
set(EXPERIMENT_SOURCES
        Anl1471Processor.cpp
        #Beta4Hen3Processor.cpp
        #CrosstalkProcessor.cpp
        #Dssd4SHEProcessor.cpp
        E11027Processor.cpp
        #Ge4Hen3Processor.cpp
        IS600Processor.cpp
        #LaBr3TestProcessor.cpp
        SheCorrelator.cpp
        TemplateExpProcessor.cpp
        TwoChanTimingProcessor.cpp
        VandleOrnl2012Processor.cpp
        #WalkVandleBetaProcessor.cpp
        )

add_library(UtkscanExperimentObjects OBJECT ${EXPERIMENT_SOURCES})
