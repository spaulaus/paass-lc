/// @file poll2_core.h
/// @brief Controls the poll2 command interpreter and data acquisition system
///     The Poll class is used to control the command interpreter and data acqusition systems. Command input and the
///     command line interface of poll2 are handled by the external library CTerminal. Pixie16 data acquisition is
///     handled by interfacing with the PixieInterface library.
/// @author S. V. Paulauskas, Cory R. Thornsberry, Karl Smith, David Miller, Robert Grzywacz
/// @date October 6, 2015

#ifndef POLL2_CORE_H
#define POLL2_CORE_H

#include <hribf_buffers.h>
#include <Constants.hpp>

#include <vector>

#define maxEventSize 4095 // (0x1FFE0000 >> 17)

// Maximum length of UDP data packet (in bytes)
#define MAX_ORPH_DATA 1464

typedef Pixie16::word_t eventdata_t[maxEventSize];

struct UDP_Packet {
    int Sequence; /// Sequence number for reliable transport
    int DataSize; /// Number of useable bytes in Data
    unsigned char Data[MAX_ORPH_DATA]; /// The data to be transmitted
};

struct data_pack{
    int Sequence; /// Sequence number for reliable transport
    int DataSize; /// Number of useable bytes in Data
    int TotalEvents; /// Total number of events
    unsigned short Events; /// Number of events in this packet
    unsigned short Cont; /// Continuation flag for large events
    unsigned char Data[4*(4050 + 4)]; /// The data to be transmitted
    int BufLen; /// Length of original Pixie buffer
};

// Forward class declarations
class Mca;
class AcquisitionInterface;
class StatsHandler;
class Client;
class Server;
class Terminal;

class Poll{
public:
    /// Default constructor.
    Poll();

    /// Destructor.
    ~Poll();

    /// Initialize the poll object.
    void Initialize(const char* configurationFile, const bool &useAcquisitionInterface = true);

    void SetBootFast(bool input_=true){ boot_fast = input_; }

    void SetWallClock(bool input_=true){ insert_wall_clock = input_; }

    void SetQuietMode(bool input_=true){ is_quiet = input_; }

    void SetSendAlarm(bool input_=true){ send_alarm = input_; }

    void SetShowRates(bool input_=true){ show_module_rates = input_; }

    void SetZeroClocks(bool input_=true){ zero_clocks = input_; }

    void SetDebugMode(bool input_=true){ debug_mode = input_; }

    void SetShmMode(bool input_=true){ shm_mode = input_; }

    void SetNcards(const size_t &n_cards_){ n_cards = n_cards_; }

    void SetThreshWords(const int &thresholdPercentage);

    void SetTerminal(Terminal *term){ poll_term_ = term; };

    bool GetBootFast(){ return boot_fast; }

    bool GetWallClock(){ return insert_wall_clock; }

    bool GetQuietMode(){ return is_quiet; }

    bool GetSendAlarm(){ return send_alarm; }

    bool GetShowRates(){ return show_module_rates; }

    bool GetZeroClocks(){ return zero_clocks; }

    bool GetDebugMode(){ return debug_mode; }

    bool GetShmMode(){ return shm_mode; }

    size_t GetNcards(){ return n_cards; }

    size_t GetThreshWords(){ return threshWords; }

    ///\brief Prints the information about each module.
    void PrintModuleInfo();

    /// Main control loop for handling user input.
    void CommandControl();

    /// Main acquisition control loop for handling data acq.
    void RunControl();

    /// Close the sockets, any open files, and clean up.
    /// Clean up things that are created during Poll::Initialize().
    /// @return Returns true if successful.
    bool Close();
private:
    Terminal *poll_term_;

    std::vector<Pixie16::word_t> *partialEvents; //!< A vector to store the partial events

    double startTime; //!<Time when the acquisition was started.
    double lastSpillTime; //!<Time when the last spill finished.

    struct tm *time_info; //!< Contains time information used for timing.

    Client *client; //!< UDP client for network access
    Server *server; //!< UDP server to listen for pacman commands

    AcquisitionInterface *pif_; //!< The main pixie interface pointer

    std::string sys_message_head; //!< Command line message header
    bool kill_all; //!< Set to true when the program is exiting
    bool do_start_acq; //!< Set to true when the command is given to start a run
    bool do_stop_acq; //!< Set to true when the command is given to stop a run
    bool record_data; //!< Set to true if data is to be recorded to disk
    bool do_reboot; //!< Set to true when the user tells POLL to reboot PIXIE
    bool force_spill; //!< Force poll2 to dump the current data spill
    bool acq_running; //!< Set to true when run_command is recieving data from PIXIE
    bool run_ctrl_exit; //!< Set to true when run_command exits
    bool had_error; //!< True if the command in question had an error.
    bool file_open; //!< True if the output file is open
    time_t raw_time; //!< The raw time used for calculations in the software.

    bool doMcaRun_; /// Set to true when the "mca" command is received
    bool isMcaRunning_; //!< True when the MCA is running.
    double mcaRunLengthInSeconds_; //!< The user set run length of the MCA in seconds.
    std::string mcaBasename_; //!< The name provided for the MCA. If empty we'll use a default name of mca.
    Mca *mca_; //!< A pointer that we'll use to run the MCA.

    bool boot_fast; //!< True if we can fast boot the pixie modules
    bool insert_wall_clock; //!< True if we should insert the wall clock into the data stream
    bool is_quiet; //!< True if we don't want debugging statements
    bool send_alarm; //!< True if we should send alarms somewhere
    bool show_module_rates; //!< True if we should show the module rates
    bool zero_clocks; //!< True if we should zero the module clocks
    bool debug_mode; //!< True if we're running in debug mode. 
    bool shm_mode; //!<  New style shared-memory mode.
    bool init_; //!< True if we have successfully made it through the Initialization method.
    double runTime; //!<  Time to run the acquisition, in seconds.

    // Options relating to output data file
    std::string output_directory; //!<  Sets the output directory. Set with 'fdir' command
    std::string filename_prefix; //!<  Sets the filename prefix. Set with 'ouf' command
    std::string output_title; //!<  Sets the output run title. Set with 'htit' command
    unsigned int next_run_num; //!<  Sets the next run number. Set with 'hnum' command
    unsigned int output_format; //!<  Sets the output format. Set with 'oform' command

    // The main output data file and related variables
    int current_file_num; //!< Run number of the current file.
    PollOutputFile output_file; //!< Class that handles outputting files.

    size_t n_cards; //!< The number of modules reported by the interface
    size_t threshWords; //!< The number of FIFO words that will trigger a read.

    StatsHandler *statsHandler; //!< Pointer to instance the statistics handler.
    static const int statsInterval_ = 3; //!<The amount time between scaler reads in seconds.

    const static std::vector<std::string> runControlCommands_; //!< List of commands known to run control.
    const static std::vector<std::string> paramControlCommands_; //!< List of commands for manipulating parameters.
    const static std::vector<std::string> pollStatusCommands_; //!< list of commands that affects poll's status.
    std::vector<std::string> commands_; //!< full list of all commands known to the sytem.

    data_pack AcqBuf; //!< Data packet for class shared-memory broadcast

    /// @brief Print help dialogue for POLL options.
    void help();

    /// @brief Print help dialogue for reading/writing pixie channel parameters.
    void pchan_help();

    /// @brief Print help dialogue for reading/writing pixie module parameters.
    void pmod_help();

    /// @brief Print help dialogue for writing pixie DSP parameters.
    void save_help();

    /// Starts a data recording run. Open data file is closed, the run number is iterated and a new file is opened.
    /// If the file was successfully opened the acquisition is started.
    /// If a run is already started a warning is displayed and the process is stopped.
    /// @param[in] record_ Record a data file.
    /// @param[in] time_ The approximate number of seconds to run the acquisition.
    /// @return Returns true if successfully starts a run.
    bool start_run(const bool &record_=true, const double &time_=-1.0);

    /// Stop an active data recording run.
    ///Current run is stopped. This includes disabling data recording.
    /// This command stops the acquisition even if data recording is not active.
    /// @return Returns true if successful.
    bool stop_run();

    /// Display run status information.
    void show_status();

    /// Display polling threshold.
    void show_thresh();

    /// Acquire raw traces from a pixie module.
    void get_traces(int mod_, int chan_, int thresh_=0);

    /// Method responsible for handling tab complete.
    std::vector<std::string> TabComplete(const std::string &value_, const std::vector<std::string> &valid_);

    ///Routine to read Pixie FIFOs
    bool ReadFIFO();

    ///Routine to read Pixie scalers.
    void ReadScalers();

    ///Routine to update the status message.
    void UpdateStatus();

    /// Set IN_SYNCH and SYNCH_WAIT parameters on all modules.
    bool synch_mods();

    /// Return the current output filename.
    std::string get_filename();

    /// @brief Safely close current data file if one is open. The scalers are cleared when this is called.
    /// @param[in] continueRun Flag indicating whether we are continuing the same run, but opening a new continuation file.
    /// @return True if the file was closed successfully.
    bool CloseOutputFile(const bool continueRun = false);

    /// @brief Opens a new file if no file is currently open. The new file is
    /// determined from the output directory, run number and prefix. The run
    /// number may be iterated forward if a file already exists.
    /// If this is a continuation run the run number is not iterated and
    /// instead a suffix number is incremented
    /// The scalers are cleared when this is called if a file is not open already.
    /// @param [in] continueRun Flag indicating that this file should be a
    ///  continuation run and that the run number should not be iterated.
    ///  @return True if successfully opened a new file.
    bool OpenOutputFile(bool continueRun = false);

    /// Write a data spill to disk.
    int write_data(Pixie16::word_t *data, unsigned int nWords);

    /// Broadcast a data spill onto the network.
    void broadcast_data(Pixie16::word_t *data, unsigned int nWords);

    /// Broadcast a data spill onto the network in the classic pacman format.
    void broadcast_pac_data();

    /// @brief This method splits the arguments for pread and pwrite on a colon delimeter.
    /// This allows the user to proivde a range for the function for example,
    /// \code pread 0 0:4 TRIGGER_THRESHOLD \code
    /// will only read the TRIGGER_THRESHOLD for module 0, channels 0 to 4.
    /// If the argument has no colons start and stop will be equal.
    /// If the attempt is unsuccessful the method returns false.
    /// @param[in] arg The argument to be split.
    /// @param[out] start The first value in the string indicating the first mod / ch.
    /// @param[out] start The second value in the string indicating the last mod / ch.
    /// @return Whether the attempt was successful.
    bool SplitParameterArgs(const std::string &arg, int &start, int &stop);
};

#endif