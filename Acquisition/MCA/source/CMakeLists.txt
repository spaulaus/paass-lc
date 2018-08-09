# @authors K. Smith
add_executable(MCA MCA_exec.cpp MCA.cpp MCA_ROOT.cpp)
target_link_libraries(MCA PixieInterface Utility ${ROOT_LIBRARIES})
install(TARGETS MCA DESTINATION bin)

