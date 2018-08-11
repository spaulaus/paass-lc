# @authors C. R. Thornsberry, K. Smith
include_directories(include)
if(ROOT_FOUND)
    add_subdirectory(source)
else ()
    message(STATUS "MCA not built as no compatible histogramming routines selected.")
endif ()