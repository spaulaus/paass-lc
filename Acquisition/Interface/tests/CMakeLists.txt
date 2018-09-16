# @file CMakeLists.txt
# @brief Lists file for Acquisition > Interface tests
# @author S. V. Paulauskas
# @date August 11, 2018
# @copyright Copyright (c) 2018 S. V. Paulauskas.
# @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License

add_executable(unittest-AcquisitionConfig unittest-AcquisitionConfig.cpp ../source/AcquisitionConfig.cpp)
target_include_directories(unittest-AcquisitionConfig PUBLIC BEFORE ../include/ PUBLIC BEFORE
        ${CMAKE_SOURCE_DIR}/Core/include ${CMAKE_SOURCE_DIR}/Resources/include)
target_link_libraries(unittest-AcquisitionConfig UnitTest++ PaassCoreStatic PaassResourceStatic)
add_test(NAME AcquisitionConfig COMMAND unittest-AcquisitionConfig WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})