#@author S. V. Paulauskas
set(ResourceSources GslFitter.cpp PolynomialCfd.cpp TraditionalCfd.cpp TraceFilter.cpp TimingConfiguration.cpp
        ChannelConfiguration.cpp CrystalBallFunction.cpp CsiFunction.cpp EmCalTimingFunction.cpp
        SiPmtFastTimingFunction.cpp RootFitter.cpp VandleTimingFunction.cpp)

#Add the sources to the library
add_library(ResourceObjects OBJECT ${ResourceSources})

if (BUILD_SHARED_LIBS)
    message(STATUS "Building Utility Shared Objects")
    add_library(UtilityLibrary SHARED $<TARGET_OBJECTS:ResourceObjects>)
    target_link_libraries(UtilityLibrary PaassCoreStatic ${ROOT_LIBRARIES})
    install(TARGETS UtilityLibrary DESTINATION lib)
endif (BUILD_SHARED_LIBS)

#Create Utility static library and add ncurses if we have it
add_library(ResourceStatic STATIC $<TARGET_OBJECTS:ResourceObjects>)
target_link_libraries(ResourceStatic ${ROOT_LIBRARIES})
