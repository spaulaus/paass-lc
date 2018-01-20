/// @file MCA_exec.cpp
/// @brief Main execution class for taking an MCA run from the hardware.
/// @author S. V. Paulauskas, K. Smith, K. Miernik, C. R. Thornsberry
/// @date August 09, 2018

#include "MCA_ROOT.h"

#include <iostream>

int main(int argc, char *argv[]) {
    int totalTime = 0;
    const char *basename = "mca";

    switch(argc) {
        case 1:
            totalTime = 10;
            break;
        case 2:
            totalTime = std::stoi(argv[1]);
            break;
        case 3:
            totalTime = std::stoi(argv[1]);
            basename = argv[3];
            break;
        default:
            std::cerr << "Usage : mca <run time in seconds> <(optional) basename of output file>" << std::endl;
            return EXIT_FAILURE;
    }

    AcqInterface *pif;

#ifdef PAASS_BUILD_XIA_INTERFACE
    pif = new PixieInterface("pixie.cfg");
#else
    pif = new EmulatedInterface("pixie.cfg");
#endif

    pif->ReadSlotConfig();

    pif->Init();

    //cxx, end any ongoing runs
    pif->EndRun();
    pif->Boot(AcqInterface::BootType::MCA, true);

    pif->RemovePresetRunLength(0);

    MCA *mca = new MCA_ROOT(&pif, basename);

    if (mca->IsOpen())
        mca->Run(totalTime);
    delete mca;
    delete pif;

    return EXIT_SUCCESS;
}

// vim: expandtab tabstop=4 shiftwidth=4 softtabstop=4 autoindent
