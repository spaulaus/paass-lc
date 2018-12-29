///@authors D. Miller, C. R. Thornsberry
/********************************************************************/
/*	pmread.cpp                                                       */
/*		last updated: April 19th, 2015 CRT                          */
/********************************************************************/

#include <iostream>

#include <PixieInterface.h>
#include "PixieSupport.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << " Invalid number of arguments to " << argv[0] << std::endl;
        std::cout << "  SYNTAX: " << argv[0] << " [module] [parameter]\n\n";
        return 1;
    }

    int mod = atoi(argv[1]);

    PixieInterface pif("pixie-cfg.xml");

    pif.Init();
    pif.Boot(Interface::BootFlags::DownloadParameters | Interface::BootFlags::ProgramFPGA |
             Interface::BootFlags::SetDAC, true);

    std::string temp_str(argv[2]);
    ParameterModuleReader reader;
    forModule(&pif, mod, reader, temp_str);

    return 0;
}
