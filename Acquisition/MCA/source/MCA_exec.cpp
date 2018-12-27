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

    PixieInterface pif("pixie.cfg");
    pif.GetSlots();
    pif.Init();
    pif.EndRun();
    pif.Boot(PixieInterface::DownloadParameters | PixieInterface::ProgramFPGA | PixieInterface::SetDAC, true);
    pif.RemovePresetRunLength(0);

    MCA *mca = new MCA_ROOT(&pif, basename);

    if (mca->IsOpen())
        mca->Run(totalTime);
    delete mca;

    return EXIT_SUCCESS;
}
