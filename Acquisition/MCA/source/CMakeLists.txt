# @authors K. Smith

add_executable(mca mcaMain.cpp Mca.cpp McaRoot.cpp)
target_link_libraries(mca PixieInterface Utility Pugixml PaassResourceStatic ${ROOT_LIBRARIES})
install(TARGETS mca DESTINATION bin)

set(MCA_LIB_SOURCES Mca.cpp McaRoot.cpp)
add_library(McaLibrary STATIC ${MCA_LIB_SOURCES})
target_link_libraries(McaLibrary PixieInterface Utility Pugixml PaassResourceStatic ${ROOT_LIBRARIES})