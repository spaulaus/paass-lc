/// @file poll2.cpp
/// @brief This program is designed as a replacement of the POLL program for reading
///   data from PIXIE-16 crates. The old POLL program relied on a connection
///   to PACMAN in order to recieve commands whereas this is a stand-alone program
///   with a built-in command line interface.
///   Data is also not transmitted onto a socket (except for shm).
/// @author S. V. Paulauskas and C. R. Thornsberry
/// @date January 21, 2015
/// @date September 15, 2018

#include <iostream>
#include <thread>
#include <utility>
#include <map>
#include <getopt.h>
#include <string.h>

#include <sys/stat.h> //For directory manipulation

#include <poll2_core.h>
#include <Display.h>
#include <CTerminal.h>
#include <StringManipulationFunctions.hpp>

/* Print help dialogue for command line options. */
void help(const char *progName_) {
    std::cout << "\n SYNTAX: " << progName_ << " [options]\n";
    std::cout << "  --alarm (-a) [e-mail] | Call the alarm script with a given e-mail (or no argument)\n";
    std::cout << "  -c <config file>      | Path to the configuration file\n";
    std::cout << "  -e                    | Starts Poll2 with the EmulatedInterface.\n";
    std::cout << "  --fast (-f)           | Fast boot (false by default)\n";
    std::cout << "  --verbose (-v)        | Run quietly (false by default)\n";
    std::cout << "  --no-wall-clock       | Do not insert the wall clock in the data stream\n";
    std::cout << "  --rates               | Display module rates in quiet mode (false by defualt)\n";
    std::cout << "  --thresh (-t) <num>   | Sets FIFO read threshold to num% full (50% by default)\n";
    std::cout << "  --zero                | Zero clocks on each START_ACQ (false by default)\n";
    std::cout << "  --debug (-d)          | Set debug mode to true (false by default)\n";
    std::cout << "  --help (-h)           | Display this help dialogue.\n\n";
}

void start_run_control(Poll *poll_) {
    poll_->RunControl();
}

void start_cmd_control(Poll *poll_) {
    poll_->CommandControl();
}

int main(int argc, char *argv[]) {
    struct option longOpts[] = {
            {"alarm",         optional_argument, nullptr, 'a'},
            {"config",        required_argument, nullptr, 'c'},
            {"fast",          no_argument,       nullptr, 'f'},
            {"verbose",       no_argument,       nullptr, 'v'},
            {"no-wall-clock", no_argument,       nullptr, 0},
            {"rates",         no_argument,       nullptr, 0},
            {"thresh",        required_argument, nullptr, 't'},
            {"zero",          no_argument,       nullptr, 0},
            {"debug",         no_argument,       nullptr, 'd'},
            {"help",          no_argument,       nullptr, 'h'},
            {"prefix",        no_argument,       nullptr, 0},
            {"?",             no_argument,       nullptr, 0},
            {nullptr,         no_argument,       nullptr, 0}
    };

    // Main object
    Poll poll;
    poll.SetQuietMode();

    int idx = 0;
    int retval = 0;

    // Read the FIFO when it is this full
    int fifoThresholdReadPercentage = 50;
    std::string alarmArgument = "";
    bool usePixieInterface = true;
    const char* configurationFile = "./pixie-cfg.xml";

    /// @TODO getopt_long is not POSIX compliant. If we get complaints we can change to getopt or implement a new class.
    while ((retval = getopt_long(argc, argv, "ac:efvt:dph", longOpts, &idx)) != -1) {
        switch (retval) {
            case 'a':
                alarmArgument = optarg;
                poll.SetSendAlarm();
                break;
            case 'c':
                configurationFile = optarg;
                break;
            case 'e':
                usePixieInterface = false;
                break;
            case 'f':
                poll.SetBootFast();
                break;
            case 'v':
                poll.SetQuietMode(false);
                break;
            case 't' :
                fifoThresholdReadPercentage = std::stoi(optarg);
                if (fifoThresholdReadPercentage <= 0) {
                    std::cerr << Display::ErrorStr() << " Failed to set threshold level to ("
                              << fifoThresholdReadPercentage << ")!\n";
                    return EXIT_FAILURE;
                }
                break;
            case 'd':
                poll.SetDebugMode();
                break;
            case 'h' :
                help(argv[0]);
                return EXIT_SUCCESS;
            case 0 :
                if (strcmp("prefix", longOpts[idx].name) == 0) { // --prefix
                    std::cout << INSTALL_PREFIX << "\n";
                    return 0;
                } else if (strcmp("no-wall-clock", longOpts[idx].name) ==
                           0) { // --no-wall-clock
                    poll.SetWallClock();
                } else if (strcmp("rates", longOpts[idx].name) ==
                           0) { // --rates
                    poll.SetShowRates();
                } else if (strcmp("zero", longOpts[idx].name) == 0) { // --zero
                    poll.SetZeroClocks();
                }
                break;
            case '?' :
                help(argv[0]);
                return EXIT_FAILURE;
            default :
                break;
        }//switch(retval)
    }//while

    try {
        poll.Initialize(configurationFile, usePixieInterface);
    } catch (std::exception &exception) {
        std::cout << "poll2 : Caught exception while initializing Poll. \n" << exception.what() << "\n";
        return EXIT_FAILURE;
    }

    // Main interactive terminal.
    Terminal poll_term;

    std::string poll2Dir = getenv("HOME");
    if (!mkdir(poll2Dir.append("/.poll2/").c_str(), S_IRWXU))
        if (errno == EEXIST)
            std::cout << Display::ErrorStr() << "Unable to create poll2 settings directory '" << poll2Dir << "'!\n";

    // Initialize the terminal before doing anything else;
    poll_term.Initialize();

    std::cout << "\n#########      #####    ####      ####       ########\n";
    std::cout << " ##     ##    ##   ##    ##        ##       ##      ##\n";
    std::cout << " ##      ##  ##     ##   ##        ##                ##\n";
    std::cout << " ##     ##  ##       ##  ##        ##               ##\n";
    std::cout << " #######    ##       ##  ##        ##              ##\n";
    std::cout << " ##         ##       ##  ##        ##            ##\n";
    std::cout << " ##         ##       ##  ##        ##           ##\n";
    std::cout << " ##          ##     ##   ##        ##         ##\n";
    std::cout << " ##           ##   ##    ##    ##  ##    ##  ##\n";
    std::cout << "####           #####    ######### ######### ###########\n";

    poll_term.SetCommandHistory(poll2Dir + "poll2.cmd");
    poll_term.SetPrompt(Display::InfoStr("POLL2 $ ").c_str());
    poll_term.AddStatusWindow();
    poll_term.EnableTabComplete();
    poll_term.SetLogFile(poll2Dir + "poll2.log");
    poll.PrintModuleInfo();
    poll.SetThreshWords(fifoThresholdReadPercentage);

#ifdef PIF_REVA
    std::cout << "Using Pixie16 revision A\n";
#elif (defined PIF_REVD)
    std::cout << "Using Pixie16 revision D\n";
#elif (defined PIF_REVF)
    std::cout << "Using Pixie16 revision F\n";
#else
    std::cout << "Using unknown Pixie16 revision!!!\n";
#endif

    std::cout << std::endl;

    poll.SetTerminal(&poll_term);

    if (poll.GetSendAlarm()) {
        Display::LeaderPrint("Sending alarms to");
        if (alarmArgument.empty())
            std::cout << Display::InfoStr("DEFAULT") << std::endl;
        else
            std::cout << Display::WarningStr(alarmArgument) << std::endl;
    }

    // Start the run control thread
    std::cout << StringManipulation::PadString("Starting run control thread", ".", 49);
    std::thread runctrl(start_run_control, &poll);
    std::cout << Display::OkayStr() << std::endl;

    // Start the command control thread. This needs to be the last thing we do to
    // initialize. The user cannot enter commands before setup is complete
    std::cout << StringManipulation::PadString("Starting command thread", ".", 49);
    std::thread comctrl(start_cmd_control, &poll);
    std::cout << Display::OkayStr() << std::endl << std::endl;

    // Synchronize the threads and wait for completion
    comctrl.join();
    runctrl.join();

    // Close the output file, if one is open
    poll.Close();

    // Close the terminal.
    poll_term.Close();

    //Reprint the leader as the carriage was returned
    Display::LeaderPrint(std::string("Running poll2"));
    std::cout << Display::OkayStr("[Done]") << std::endl;

    return EXIT_SUCCESS;
}
