#@author S. V. Paulauskas
#Set the utility sources that we will make a lib out of
set(ResourceSources PolynomialCfd.cpp TraditionalCfd.cpp TraceFilter.cpp)

if (${GSL_VERSION} GREATER 1.9)
    list(APPEND ResourceSources Gsl2Fitter.cpp)
else (${GSL_VERSION} LESS 2.0)
    list(APPEND ResourceSources Gsl1Fitter.cpp)
endif (${GSL_VERSION} GREATER 1.9)

if (ROOT_FOUND)
    list(APPEND ResourceSources CrystalBallFunction.cpp CsiFunction.cpp EmCalTimingFunction.cpp
            SiPmtFastTimingFunction.cpp RootFitter.cpp VandleTimingFunction.cpp)
endif (ROOT_FOUND)

#Add the sources to the library
add_library(ResourceObjects OBJECT ${ResourceSources})

if (BUILD_SHARED_LIBS)
    message(STATUS "Building Utility Shared Objects")
    add_library(UtilityLibrary SHARED $<TARGET_OBJECTS:ResourceObjects>)
    target_link_libraries(UtilityLibrary PaassCoreStatic)
    if (ROOT_FOUND)
        target_link_libraries(UtilityLibrary ${ROOT_LIBRARIES})
    endif (ROOT_FOUND)
    install(TARGETS UtilityLibrary DESTINATION lib)
endif (BUILD_SHARED_LIBS)

#Create Utility static library and add ncurses if we have it
add_library(ResourceStatic STATIC $<TARGET_OBJECTS:ResourceObjects>)
if (ROOT_FOUND)
    target_link_libraries(ResourceStatic ${ROOT_LIBRARIES})
endif (ROOT_FOUND)