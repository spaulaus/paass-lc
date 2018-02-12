#@authors S. V. Paulauskas
set(EXPERIMENT_SOURCES
        E11027Processor.cpp
        TemplateExpProcessor.cpp
        VandleOrnl2012Processor.cpp
        )

if (ROOT_FOUND)
    list(APPEND EXPERIMENT_SOURCES
            Anl1471Processor.cpp
            IS600Processor.cpp
            TwoChanTimingProcessor.cpp
            )
endif(ROOT_FOUND)

add_library(UtkscanExperimentObjects OBJECT ${EXPERIMENT_SOURCES})
