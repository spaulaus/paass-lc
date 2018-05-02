#@authors K. Smith, S. V. Paulauskas, C. R. Thornsberry
option(PAASS_USE_HRIBF "Use HRIBF library for scan base." OFF)

#Check if GSL is installed
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})
link_directories(${GSL_LIBRARY_DIRS})
if (${GSL_VERSION} LESS 2.0)
    add_definitions("-D GSL_VERSION_ONE")
endif (${GSL_VERSION} LESS 2.0)

#Everything below is dependent on these two sets of libaries so we include the
#headers.
include_directories(Resources/include)
include_directories(ScanLibraries/include)

if (PAASS_USE_HRIBF)
    #Find HRIBF Libraries
    find_package(HRIBF REQUIRED)
    add_definitions("-D USE_HRIBF")

    enable_language(Fortran)

    #If we are using HRIBF interface we need to include the ScanorInterface header
    #	for the following code.
    include_directories(Scanor/include)
    add_subdirectory(Scanor)
endif (PAASS_USE_HRIBF)

#We will always build these two since they include static lib for the rest
add_subdirectory(ScanLibraries)
add_subdirectory(Resources)

#Build utilities.
add_subdirectory(Utilities)

if(ROOT_FOUND)
    option(PAASS_BUILD_UTKSCAN "Build utkscan" OFF)

    if (PAASS_BUILD_UTKSCAN)
        add_subdirectory(Utkscan)
    endif (PAASS_BUILD_UTKSCAN)
else()
    message(WARNING "utkscan is unavilable unless you install ROOT!!")
endif(ROOT_FOUND)