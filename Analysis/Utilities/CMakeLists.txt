# @author S. V. Paulauskas
option(PAASS_BUILD_EVENT_READER "Program that outputs event information to the terminal" ON)
option(PAASS_BUILD_HEAD_READER "Program that outputs the header information from the file" ON)
option(PAASS_BUILD_HEX_READER "Program that outputs data as hex values" ON)
option(PAASS_BUILD_ROOT_SCANNER "Program used for live scanning of files into ROOT hists" ON)
option(PAASS_BUILD_SCOPE "Program used to view traces in data stream" ON)
option(PAASS_BUILD_SKELETON "Program that can be used to build custom Analysis" ON)
option(PAASS_BUILD_TRACE_FILTERER "Program that performs filtering on traces" ON)

if(PAASS_BUILD_EVENT_READER)
    add_subdirectory(EventReader)
endif(PAASS_BUILD_EVENT_READER)

if(PAASS_BUILD_HEAD_READER)
    add_subdirectory(HeadReader)
endif(PAASS_BUILD_HEAD_READER)

if(PAASS_BUILD_HEX_READER)
    add_subdirectory(HexReader)
endif(PAASS_BUILD_HEX_READER)

if(PAASS_BUILD_SKELETON)
    add_subdirectory(Skeleton)
endif(PAASS_BUILD_SKELETON)

if(PAASS_BUILD_TRACE_FILTERER)
    add_subdirectory(TraceFilterer)
endif(PAASS_BUILD_TRACE_FILTERER)

if (ROOT_FOUND)
    if(PAASS_BUILD_ROOT_SCANNER)
        add_subdirectory(RootScanner)
    endif(PAASS_BUILD_ROOT_SCANNER)

    if(PAASS_BUILD_SCOPE)
        add_subdirectory(Scope)
    endif(PAASS_BUILD_SCOPE)
endif (ROOT_FOUND)

