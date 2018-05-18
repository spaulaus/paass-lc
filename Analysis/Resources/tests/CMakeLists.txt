#author S. V. Paulauskas
add_executable(unittest-ChannelConfiguration unittest-ChannelConfiguration.cpp ../source/ChannelConfiguration.cpp
        ../source/TimingConfiguration.cpp)
target_link_libraries(unittest-ChannelConfiguration UnitTest++ ${LIBS})
install(TARGETS unittest-ChannelConfiguration DESTINATION bin/unittests)
add_test(ChannelConfiguration unittest-ChannelConfiguration)

set(GSL_FITTER_SOURCES ${GSL_FITTER_SOURCES} unittest-GslFitter.cpp ../source/GslFitter.cpp ../source/TimingConfiguration.cpp)
add_executable(unittest-GslFitter ${GSL_FITTER_SOURCES})
target_link_libraries(unittest-GslFitter ${GSL_LIBRARIES} UnitTest++)
install(TARGETS unittest-GslFitter DESTINATION bin/unittests)
add_test(GslFitter unittest-GslFitter)

add_executable(unittest-PolynomialCfd unittest-PolynomialCfd.cpp ../source/PolynomialCfd.cpp)
target_link_libraries(unittest-PolynomialCfd UnitTest++)
install(TARGETS unittest-PolynomialCfd DESTINATION bin/unittests)
add_test(PolynomialCfd unittest-PolynomialCfd)

add_executable(unittest-TraditionalCfd unittest-TraditionalCfd.cpp ../source/TraditionalCfd.cpp
        ../source/TimingConfiguration.cpp)
target_link_libraries(unittest-TraditionalCfd UnitTest++)
install(TARGETS unittest-TraditionalCfd DESTINATION bin/unittests)
add_test(TraditionalCfd unittest-TraditionalCfd)

add_executable(unittest-XiaCfd unittest-XiaCfd.cpp ../source/XiaCfd.cpp ../source/TimingConfiguration.cpp)
target_link_libraries(unittest-XiaCfd UnitTest++)
install(TARGETS unittest-XiaCfd DESTINATION bin/unittests)
add_test(XiaCfd unittest-XiaCfd)

if (ROOT_FOUND)
    add_executable(unittest-RootFitter unittest-RootFitter.cpp ../source/RootFitter.cpp
            ../source/VandleTimingFunction.cpp)
    target_link_libraries(unittest-RootFitter ${ROOT_LIBRARIES} UnitTest++)
    install(TARGETS unittest-RootFitter DESTINATION bin/unittests)
    add_test(RootFitter unittest-RootFitter)
endif (ROOT_FOUND)
