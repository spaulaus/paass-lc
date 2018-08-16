# @file CMakeLists.txt
# @author S. V. Paulauskas
# @date Updated August 11, 2018
add_subdirectory(source)

if(PAASS_BUILD_TESTS)
    add_subdirectory(tests)
endif(PAASS_BUILD_TESTS)