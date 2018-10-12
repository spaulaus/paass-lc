# Find the XIA Library
#
# Sets the usual variables expected for find_package scripts:
#
# XIA_LIBRARY_DIR
# XIA_INCLUDE_DIR
# XIA_LIBRARIES
# XIA_FOUND - true if XIA was found.
# XIA_FIRMWARE_DIR - Directory to be searched for firmwares for pixie-cfg.xml.
#
# @authors K. Smith, C. R. Thornsberry, S. V. Paulauskas

#Find the library path by looking for the library.
find_path(XIA_LIBRARY_DIR
        NAMES libPixie16App.a libPixie16Sys.a
        HINTS ${XIA_ROOT_DIR}
        PATHS /opt/xia
        PATH_SUFFIXES api current/software
        DOC "Path to pixie library.")

get_filename_component(XIA_LIBRARY_DIR "${XIA_LIBRARY_DIR}" REALPATH)

#Unset any cached value to ensure a fresh search is performed.
#This permits the user to change the XIA_FIRMWARE_DIR and have subsequent paths updated.
unset(XIA_FIRMWARE_DIR CACHE)

if (NOT XIA_FIRMWARE_DIR)
    get_filename_component(XIA_FIRMWARE_DIR "${XIA_LIBRARY_DIR}/../firmwares" REALPATH)
    if (NOT EXISTS ${XIA_FIRMWARE_DIR})
        get_filename_component(XIA_FIRMWARE_DIR "${XIA_LIBRARY_DIR}/../../" REALPATH)
    endif (NOT EXISTS ${XIA_FIRMWARE_DIR})
endif (NOT XIA_FIRMWARE_DIR)
set(XIA_FIRMWARE_DIR ${XIA_FIRMWARE_DIR} CACHE PATH "Path to folder containing XIA firmware.")

find_path(XIA_APP_INCLUDES
        NAMES pixie16app_defs.h
        PATHS ${XIA_LIBRARY_DIR} "${XIA_LIBRARY_DIR}/../"
        PATH_SUFFIXES app include
        DOC "Path to XIA app includes.")
set(XIA_APP_INCLUDES ${XIA_APP_INCLUDES} CACHE INTERNAL "App includes")

find_path(XIA_SYS_INCLUDES
        NAMES pixie16sys_defs.h
        PATHS ${XIA_LIBRARY_DIR} "${XIA_LIBRARY_DIR}/../"
        PATH_SUFFIXES sys include
        DOC "Path to XIA sys includes.")
set(XIA_SYS_INCLUDES ${XIA_SYS_INCLUDES} CACHE INTERNAL "Sys Includes")

# Support the REQUIRED and QUIET arguments, and set XIA_FOUND if found.
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(XIA DEFAULT_MSG XIA_LIBRARY_DIR)

if (XIA_FOUND)
    set(XIA_INCLUDE_DIR ${XIA_APP_INCLUDES} ${XIA_SYS_INCLUDES})
    list(REMOVE_DUPLICATES XIA_INCLUDE_DIR)
    set(XIA_LIBRARIES -lPixie16App -lPixie16Sys)
endif ()

function(XIA_CONFIG)
    message(STATUS "Creating Pixie configuration.")

    set(CONFIGURATION_FILE ${CMAKE_CURRENT_BINARY_DIR}/pixie-cfg.xml)

    get_filename_component(XIA_ROOT_DIR "${XIA_LIBRARY_DIR}/.." REALPATH)

    #Write some useful info.
    file(WRITE ${CONFIGURATION_FILE}
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<!--\n"
            "    The following lines provide the configuration for a XIA Pixie16 data acquisition  \n"
            "    system. The configuration file is broken into two sections a global section for  \n"
            "    tags that affect the entire system and a module specific section. The global tags\n"
            "    include: PixieBaseDir, DspSetFileFile, DspWorkingSetFile, Firmware, and SlotDefinition.\n"
            "    The Firmware tags include: ModuleType, ModuleBaseDir, SpFpgaFile, \n"
            "    ComFpgaFile, DspConfFile, and DspVarFile. The module tags are \n"
            "    associated with the type specified prior to the tag. If no type is specified the \n"
            "    type 'default' is used.                                                          \n"
            "\n"
            "    The tag values are prepended with a base directory unless the first character in \n"
            "    the value is forward slash, '/',  or a period '.', permitting the use of absolute \n"
            "    and relative paths. The global tags are prepended with the PixieBaseDir. Module  \n"
            "    tags are prepended with the ModuleBaseDir if specified otherwise the PixieBaseDir\n"
            "    is used. If no base directory is determined the path is assumed to be local to   \n"
            "    the running directory.\n"
            "-->\n"
            "<Configuration>\n"
            )

    #Write the base directory
    file(APPEND ${CONFIGURATION_FILE} "\t<PixieBaseDir>${XIA_ROOT_DIR}</PixieBaseDir>\n")

    #Following are lists of keys and the glob expr to find the files
    set(CONFIG_NAME DspSetFile CrateConfig)
    set(CONFIG_EXPR
            share/default.set #DspSetFile
            share/pxisys/pxisys*.ini #CrateConfig
            )
    set(ALT_CONFIG_EXPR
            configuration/default.set #DspSetFile
            test/pxisys*.ini #CrateConfig
            )

    foreach (CONFIG_STEP RANGE 0 1)
        #Get key name and expression form the list
        list(GET CONFIG_NAME ${CONFIG_STEP} KEY)
        list(GET CONFIG_EXPR ${CONFIG_STEP} GLOB_EXPR)
        list(GET ALT_CONFIG_EXPR ${CONFIG_STEP} ALT_GLOB_EXPR)

        # Find all files matching the expression
        # Returns the path of the file relative to the base directory.
        file(GLOB FILE_MATCHES RELATIVE ${XIA_ROOT_DIR} ${XIA_ROOT_DIR}/${GLOB_EXPR} ${XIA_ROOT_DIR}/${ALT_GLOB_EXPR})

        if (${KEY} MATCHES "DspSetFile")
            #Check that a unique match was found
            list(LENGTH FILE_MATCHES NUM_MATCHES)
            if (NOT NUM_MATCHES EQUAL 1)
                message(STATUS "WARNING: Unable to autocomplete global configuration!\n\tUnique ${KEY} file (${GLOB_EXPR}) not found!")
                if (NUM_MATCHES EQUAL 0)
                    file(APPEND ${CONFIGURATION_FILE}
                            "\t<!-- #ERROR: No ${KEY} found! Please specify and remove this comment. -->\n")
                else (NUM_MATCHES EQUAL 0)
                    file(APPEND ${CONFIGURATION_FILE}
                            "\t<!-- #ERROR: Multiple ${KEY}s found! Please choose one and remove this comment. -->\n")
                endif (NUM_MATCHES EQUAL 0)
            endif ()

            if (NUM_MATCHES EQUAL 1)
                install(FILES ${XIA_ROOT_DIR}/${FILE_MATCHES} PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
                        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/config)
                #Rename set file to current.set to maintain default.set for backup
                install(FILES ${XIA_ROOT_DIR}/${FILE_MATCHES} RENAME current.set DESTINATION
                        ${CMAKE_INSTALL_PREFIX}/share/config)
                set(FILE_MATCHES ./current.set)
            endif ()
            file(APPEND ${CONFIGURATION_FILE} "\t<${KEY}>${FILE_MATCHES}</${KEY}>\n")
        endif ()
        if (${KEY} MATCHES "CrateConfig")
            set(PXISYS_FILE_LIST ${FILE_MATCHES})
        endif()
    endforeach (CONFIG_STEP RANGE 0 2)

    #Added the working set file name
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/pixie-cfg.xml "\t<DspWorkingSetFile>./current.set</DspWorkingSetFile>\n\n")

    if (NOT EXISTS ${XIA_FIRMWARE_DIR})
        message(WARNING "Configuration Error - Invalid Pixie firmware directory: ${XIA_FIRMWARE_DIR}")
        return()
    endif (NOT EXISTS ${XIA_FIRMWARE_DIR})
    subdirlist(XIA_FIRMWARE_DIRS ${XIA_FIRMWARE_DIR})

    if (NOT XIA_FIRMWARE_DIRS)
        message(STATUS "We did not find any firmware directories to configure!")
        file(APPEND ${CONFIGURATION_FILE}
                "#ERROR: No firmware found to configure! Add your firmware to ${XIA_FIRMWARE_DIR}\n")
    endif (NOT XIA_FIRMWARE_DIRS)

    #remove directories without subdirectories fpga and dsp.
    foreach (FIRMWARE_DIR ${XIA_FIRMWARE_DIRS})
        if (NOT ((EXISTS ${FIRMWARE_DIR}/fpga OR EXISTS ${FIRMWARE_DIR}/firmware) AND EXISTS ${FIRMWARE_DIR}/dsp))
            list(REMOVE_ITEM XIA_FIRMWARE_DIRS ${FIRMWARE_DIR})
        endif (NOT ((EXISTS ${FIRMWARE_DIR}/fpga OR EXISTS ${FIRMWARE_DIR}/firmware) AND EXISTS ${FIRMWARE_DIR}/dsp))
    endforeach (FIRMWARE_DIR ${XIA_ROOT_DIRS})

    #Following are lists of keys and the glob expr to find the files
    set(CONFIG_NAME SpFpgaFile ComFpgaFile DspConfFile DspVarFile)
    set(CONFIG_EXPR
            fpga/fippixie16*.bin #SpFpgaFile
            fpga/syspixie16*.bin #ComFpgaFile
            dsp/Pixie16DSP*.ldr #DspConfFile
            dsp/Pixie16DSP*.var #DspVarFile
            )
    set(ALT_CONFIG_EXPR
            firmware/fippixie16*.bin #SpFpgaFile
            firmware/syspixie16*.bin #ComFpgaFile
            dsp/Pixie16DSP*.ldr #DspConfFile
            dsp/Pixie16DSP*.var #DspVarFile
            )

    foreach (FIRMWARE_DIR ${XIA_FIRMWARE_DIRS})
        #determine the module type from the fippi SpFpga File
        unset(MODULE_TYPE)
        file(GLOB FILE_MATCHES RELATIVE ${FIRMWARE_DIR}
                ${FIRMWARE_DIR}/fpga/fippixie16*.bin ${FIRMWARE_DIR}/firmware/fippixie16*.bin)

        foreach (FILENAME ${FILE_MATCHES})
            string(REGEX MATCH "[0-9]+b" BIT_STR ${FILENAME})
            if ("${BIT_STR}" STREQUAL "")
                set(BIT "unknown")
                set(BIT_STR "unknown")
            else()
                string(REGEX MATCH "[0-9]*" BIT ${BIT_STR})
            endif ()

            string(REGEX MATCH "[0-9]+m" FREQ_STR ${FILENAME})
            if ("${FREQ_STR}" STREQUAL "")
                set(FREQ "unknown")
                set(FREQ_STR "unknown")
            else()
                string(REGEX MATCH "[0-9]*" FREQ ${FREQ_STR})
            endif ()

            string(REGEX MATCH "rev[abcdf]" REVISION_STR ${FILENAME})
            if ("${REVISION_STR}" STREQUAL "")
                set(REVISION "unknown")
            else()
                string(SUBSTRING ${REVISION_STR} 3 1 REVISION)
            endif ()

            set(MODULE_STRING "<BitResolution>${BIT}</BitResolution>\n\t\t\t<SamplingFrequency>${FREQ}</SamplingFrequency>\n\t\t\t<Revision>${REVISION}</Revision>")

            #Only add the module type if it is not in the list.
            if (NOT MODULE_TYPE MATCHES ${MODULE_STRING})
                list(APPEND MODULE_TYPE ${MODULE_STRING})
                message(STATUS "Found Configuration for module type: ${BIT_STR}-${FREQ_STR}-${REVISION_STR}.")
            endif (NOT MODULE_TYPE MATCHES ${MODULE_STRING})
        endforeach (FILENAME ${FILE_MATCHES})

        list(LENGTH MODULE_TYPE NUM_MATCHES)
        if (NUM_MATCHES EQUAL 1)
            if (${MODULE_TYPE} MATCHES "unknown")
                message(STATUS "WARNING: Incomplete module type (${MODULE_TYPE}) found in:")
                message(STATUS "    ${FIRMWARE_DIR}")
                file(APPEND ${CONFIGURATION_FILE}
                        "\t<!-- #ERROR: Incomplete ModuleType found! Please correct and remove this comment. --> \n")
            endif (${MODULE_TYPE} MATCHES "unknown")
        else (NUM_MATCHES EQUAL 1)
            message(STATUS "WARNING: Could not find a unique module type in:")
            message(STATUS "    ${FIRMWARE_DIR}")
            if (NUM_MATCHES EQUAL 0)
                file(APPEND ${CONFIGURATION_FILE}
                        "\t\t<!-- #ERROR: No ModuleType found! Please specify and remove this comment. -->\n")
            else (NUM_MATCHES EQUAL 0)
                file(APPEND ${CONFIGURATION_FILE}
                        "\t\t<!-- #ERROR: Multiple ModuleTypes found! Please choose one and remove this comment. -->\n")
            endif (NUM_MATCHES EQUAL 0)
        endif (NUM_MATCHES EQUAL 1)

        file(APPEND ${CONFIGURATION_FILE} "\t<Module>\n")
        file(APPEND ${CONFIGURATION_FILE} "\t\t<ModuleType>\n\t\t\t${MODULE_TYPE}\n\t\t</ModuleType>\n")
        file(APPEND ${CONFIGURATION_FILE} "\t\t<ModuleBaseDir>${FIRMWARE_DIR}</ModuleBaseDir>\n")

        #We loop over each item in the list and search for a matching file
        foreach (CONFIG_STEP RANGE 0 3)
            #Get key name and expression form the list
            list(GET CONFIG_NAME ${CONFIG_STEP} KEY)
            list(GET CONFIG_EXPR ${CONFIG_STEP} GLOB_EXPR)
            list(GET ALT_CONFIG_EXPR ${CONFIG_STEP} ALT_GLOB_EXPR)

            #Find all files matching the expression
            # Returns the path of the file relative to the base directory.
            file(GLOB FILE_MATCHES RELATIVE ${FIRMWARE_DIR} ${FIRMWARE_DIR}/${GLOB_EXPR} ${FIRMWARE_DIR}/${ALT_GLOB_EXPR})
            list(REMOVE_DUPLICATES FILE_MATCHES)

            #Check that a unique match was found
            list(LENGTH FILE_MATCHES NUM_MATCHES)
            if (NOT NUM_MATCHES EQUAL 1)
                if (NOT ERROR)
                    set(ERROR TRUE)
                    message(STATUS "WARNING: Unable to autocomplete ${MODULE_TYPE} configuration!")
                endif (NOT ERROR)
                message(STATUS "    Unique ${KEY} file (${GLOB_EXPR}) not found!")
                if (NUM_MATCHES EQUAL 0)
                    file(APPEND ${CONFIGURATION_FILE}
                            "\t\t<!-- #ERROR: No ${KEY} found! Please specify and remove this comment. -->\n")
                else (NUM_MATCHES EQUAL 0)
                    file(APPEND ${CONFIGURATION_FILE}
                            "\t\t<!-- #ERROR: Multiple ${KEY}s found! Please choose one and remove this comment. -->\n")
                endif (NUM_MATCHES EQUAL 0)
            endif ()

            #Append the config file
            file(APPEND ${CONFIGURATION_FILE} "\t\t<${KEY}>${FILE_MATCHES}</${KEY}>\n")
        endforeach (CONFIG_STEP RANGE 0 3)
        file(APPEND ${CONFIGURATION_FILE} "\t</Module>\n\n")
    endforeach (FIRMWARE_DIR ${XIA_ROOT_DIRS})

    file(APPEND ${CONFIGURATION_FILE} "\t<SlotDefinition>\n")
    file(APPEND ${CONFIGURATION_FILE}
            "\t\t<!-- The SlotDefinition node replaces slot_def.set. We only boot a single crate. We ignore any "
            "additional Crate \n\t\t\t nodes. Make sure you have only one Pxisys file defined per crate. \n\t\t\t "
            "Add or remove modules from the configuration by deleting its line. DO NOT remove them out of sequence."
            "\n\t\t\t Ensure that slots are in ascending order. You may end up with major issues if they are not."
            "\n\t\t-->\n")

    file(APPEND ${CONFIGURATION_FILE} "\t\t<Crate number=\"0\">\n\t\t\t<Pxisys>${PXISYS_FILE_LIST}</Pxisys>\n")
    foreach(MODULE RANGE 0 6)
        math(EXPR SLOT '${MODULE}+2')
        file(APPEND ${CONFIGURATION_FILE} "\t\t\t<Module number=\"${MODULE}\" slot=\"${SLOT}\"> </Module>\n")
    endforeach(MODULE RANGE 0 6)
    file(APPEND ${CONFIGURATION_FILE} "\t\t</Crate>\n\t</SlotDefinition>\n</Configuration>\n")
    install(FILES ${CONFIGURATION_FILE} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/config)
endfunction()

macro(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} "${curdir}/*")
    FOREACH (child ${children})
        IF (IS_DIRECTORY ${curdir}/${child})
            GET_FILENAME_COMPONENT(child ${curdir}/${child} REALPATH)
            LIST(APPEND dirlist ${child})
        ENDIF ()
    ENDFOREACH ()
    IF (dirlist)
        LIST(REMOVE_DUPLICATES dirlist)
    ENDIF (dirlist)
    SET(${result} ${dirlist})
endmacro(SUBDIRLIST)

