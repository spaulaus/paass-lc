# @authors K. Smith

add_executable(MCA MCA_exec.cpp MCA.cpp MCA_ROOT.cpp)
target_link_libraries(MCA PixieInterface Utility ${ROOT_LIBRARIES})
install(TARGETS MCA DESTINATION bin)

set(MCA_LIB_SOURCES MCA.cpp MCA_ROOT.cpp)
add_library(MCA_LIBRARY STATIC ${MCA_LIB_SOURCES})
target_link_libraries(MCA_LIBRARY PixieInterface Utility ${ROOT_LIBRARIES})