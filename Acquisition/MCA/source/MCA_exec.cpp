/// @file MCA_exec.cpp
/// @brief Main execution class for taking an MCA run from the hardware.
/// @author S. V. Paulauskas, K. Smith, K. Miernik, C. R. Thornsberry
/// @date August 09, 2018

#include "MCA_ROOT.h"
#include "EmulatedInterface.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    int runLengthInSeconds = 0;
    const char *basename = "mca";

    switch(argc) {
        case 1:
            runLengthInSeconds = 10;
            break;
        case 2:
            runLengthInSeconds = std::stoi(argv[1]);
            break;
        case 3:
            runLengthInSeconds = std::stoi(argv[1]);
            basename = argv[2];
            break;
        default:
            std::cerr << "Usage : mca <run time in seconds> <(optional) basename of output file>" << std::endl;
            return EXIT_FAILURE;
    }

    AcquisitionInterface *interface;
    try {
        interface = new EmulatedInterface("./pixie-config.xml");
    } catch (std::invalid_argument &invalidArgument) {
        return EXIT_FAILURE;
    }

//#ifdef PAASS_BUILD_XIA_INTERFACE
//    interface = new PixieInterface("pixie.cfg");
//#else
//    interface = new EmulatedInterface("pixie.cfg");
//#endif

    ///@TODO : Why do we return anything from these methods if they're not used to validate ANYTHING?!?! I really
    /// think that these should be wrapped in a try/catch. If any of these fails, it's a much bigger problem than just
    /// returning false...
    interface->Init();
    interface->EndRun();
    interface->Boot(Interface::BootType::MCA, true);
    interface->RemovePresetRunLength(0);

    MCA *mca;
    try {
        mca = new MCA_ROOT(interface, basename);
    } catch (std::exception &exception) {
        std::cout << "MCA_exec.cpp : Exception caught while trying to instance MCA_ROOT! what() follows." << std::endl
                  << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (mca->IsOpen())
        mca->Run(runLengthInSeconds);
    delete mca;
    delete interface;

    return EXIT_SUCCESS;
}