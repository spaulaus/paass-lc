/// @file CerrDirector.hpp
/// @brief Directs Cerr to the location chosen by the user.
/// @author S. V. Paulauskas, C. R. Thornsberry
/// @date September 15, 2018
#ifndef PAASSLC_CERRDIRECTOR_HPP
#define PAASSLC_CERRDIRECTOR_HPP

#include <cstdio>

/// A class to handle redirecting stderr to a text file. Saves output in case the user would like to print it. Upon
/// destruction stderr is restored to stdout.
class CerrDirector {
public:
    /// Default Constructor
    CerrDirector(const char *logFile);

    /// stderr is pointed to a duplicate of stdout, buffering is then set to no buffering.
    ~CerrDirector();

    void Print();
private:
    char buf[BUFSIZ];
};

#endif //PAASSLC_CERRDIRECTOR_HPP
