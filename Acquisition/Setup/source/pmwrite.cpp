///@authors D. Miller, R. Grzywacz, C. R. Thornsberry
/********************************************************************/
/*	pmwrite.cpp                                                       */
/*		last updated: April 19th, 2015 CRT                          */
/********************************************************************/

#include <iostream>
#include <stdlib.h>

#include <PixieInterface.h>
#include "PixieSupport.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << " Invalid number of arguments to " << argv[0] << std::endl;
        std::cout << "  SYNTAX: " << argv[0]
                  << " [module] [parameter] [value]\n\n";
        return 1;
    }

    int mod = atoi(argv[1]);
    unsigned int value = (unsigned int) std::strtoul(argv[3], NULL, 0);

    PixieInterface pif("pixie-cfg.xml");
    pif.Init();
    pif.Boot(Interface::BootFlags::DownloadParameters | Interface::BootFlags::ProgramFPGA |
             Interface::BootFlags::SetDAC, true);

    std::string temp_str(argv[2]);
    ParameterModuleWriter writer;
    if (forModule(&pif, mod, writer,
                  make_pair(temp_str, value))) { pif.SaveDSPParameters(); }

    return 0;
}
