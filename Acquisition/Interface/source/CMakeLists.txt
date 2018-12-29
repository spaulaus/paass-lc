# @authors C. R. Thornsberry, K. Smith
set(Interface_SOURCES AcquisitionConfig.cpp AcquisitionInterface.cpp EmulatedInterface.cpp Lock.cpp PixieSupport.cpp)

if (PAASS_BUILD_XIA_INTERFACE)
	list(APPEND Interface_SOURCES PixieInterface.cpp)
endif (PAASS_BUILD_XIA_INTERFACE)

add_library(PixieInterface STATIC ${Interface_SOURCES})

#Order is important, XIA before PLX
target_link_libraries(PixieInterface PaassCoreStatic Pugixml ${XIA_LIBRARIES} ${PLX_LIBRARIES})

add_library(Utility STATIC Utility.cpp)
