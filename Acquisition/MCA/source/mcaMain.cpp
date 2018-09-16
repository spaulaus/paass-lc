/// @file mcaMain.cpp
/// @brief Main execution class for taking an MCA run from the hardware.
/// @author S. V. Paulauskas, K. Smith, K. Miernik, C. R. Thornsberry
/// @date August 09, 2018

#include <McaRoot.hpp>

#include <EmulatedInterface.hpp>
#include <HelperEnumerations.hpp>
#include <PixieInterface.h>

#include <iostream>

#include <getopt.h>

int main(int argc, char *argv[]) {
    int runLengthInSeconds = 0;
    const char *basename = "mca";
    const char *configurationFile = "./pixie-config.xml";
    bool usePixieInterface = true;

    int argument;
    while ((argument = getopt(argc, argv, "c:eo:t:")) != -1) {
        switch (argument) {
            case 'c':
                configurationFile = optarg;
                break;
            case 'e':
                usePixieInterface = false;
                break;
            case 'o':
                basename = optarg;
                break;
            case 't':
                runLengthInSeconds = std::stoi(optarg);
                break;
            case '?':
            default:
                std::cerr << "mca : Couldn't parse the provided argument : " << optopt << std::endl
                          << "Usage : mca [options]" << std::endl
                          << "    -c <path to config> -> The configuration file" << std::endl
                          << "    -e -> Uses the emulated interface." << std::endl
                          << "    -o <output path> -> The output file name (no extension!)" << std::endl
                          << "    -t <run time in seconds>" << std::endl;
                return EXIT_FAILURE;
        }
    }

    AcquisitionInterface *interface;
    try {
        if(usePixieInterface)
            interface = new PixieInterface(configurationFile);
        else
            interface = new EmulatedInterface(configurationFile);
    } catch (std::invalid_argument &invalidArgument) {
        std::cout << "mca : Exception caught while trying to instance the interface!" << std::endl
                  << invalidArgument.what() << std::endl;
        return EXIT_FAILURE;
    }

    ///@TODO : Why do we return anything from these methods if they're not used to validate ANYTHING?!?! I really
    /// think that these should be wrapped in a try/catch. If any of these fails, it's a much bigger problem than just
    /// returning false...
    interface->Init();
    interface->EndRun();
    interface->Boot(Interface::BootFlags::BootAll, true);
    interface->RemovePresetRunLength(0);

    Mca *mca;
    try {
        mca = new McaRoot(interface, basename);
    } catch (std::exception &exception) {
        std::cout << "mcaMain.cpp : Exception caught while trying to instance MCA_ROOT! what() follows." << std::endl
                  << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (mca->IsOpen())
        mca->Run(runLengthInSeconds);
    delete mca;
    delete interface;

    return EXIT_SUCCESS;
}