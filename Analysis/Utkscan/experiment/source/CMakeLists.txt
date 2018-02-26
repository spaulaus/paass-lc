#@authors S. V. Paulauskas
set(EXPERIMENT_SOURCES
        Anl1471Processor.cpp
        E11027Processor.cpp
        IS600Processor.cpp
        TemplateExpProcessor.cpp
        TwoChanTimingProcessor.cpp
        VandleOrnl2012Processor.cpp
        )

add_library(UtkscanExperimentObjects OBJECT ${EXPERIMENT_SOURCES})
