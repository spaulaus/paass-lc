# @authors S. V. Paulauskas
add_executable(dataGenerator dataGenerator.cpp)
target_link_libraries(dataGenerator PaassScanStatic PaassResourceStatic)
install(TARGETS dataGenerator DESTINATION bin)
