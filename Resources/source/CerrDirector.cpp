/// @file CerrDirector.cpp
/// @brief Directs cerr to a file or other location chosen by the user.
/// @author S. V. Paulauskas
/// @date September 15, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License

#include "CerrDirector.hpp"

#include <iostream>

#include <unistd.h>

CerrDirector::CerrDirector(const char *logFile) {
    for (int i = 0; i < BUFSIZ; i++)
        buf[i] = '\0';
    if (!freopen(logFile, "a", stderr))
        std::cerr << "cerr_redirect::cerr_redirect(logFile) - Couldn't reopen the stream!" << std::endl;
    setbuf(stderr, buf);
}

CerrDirector::~CerrDirector() {
    dup2(fileno(stdout), fileno(stderr));
    setvbuf(stderr, NULL, _IONBF, BUFSIZ);
}

void CerrDirector::Print() {
    fprintf(stdout, "%s", buf);
    fflush(stdout);
}
