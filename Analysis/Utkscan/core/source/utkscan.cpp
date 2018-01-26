///@author S. V. Paulauskas
#include <exception>
#include <iostream>
#include <stdexcept>

// Local files
#include "Display.h"
#include "UtkScanInterface.hpp"
#include "UtkUnpacker.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    // Define the unpacker and scan objects.
    cout << "utkscan.cpp : Instancing the UtkScanInterface" << endl;
    UtkScanInterface scanner;
    cout << "utkscan.cpp : Instancing the UtkUnpacker" << endl;
    UtkUnpacker unpacker;

    // Set the output message prefix.
    cout << "utkscan.cpp : Setting the Program Name" << endl;
    scanner.SetProgramName("utkscan");

    try {
        // Initialize the scanner.
        cout << "utkscan.cpp : Performing the setup routine" << endl;
        if(!scanner.Setup(argc, argv, &unpacker))
            throw PaassException("utkscan.cpp : Unspecified error while inside UtkScanInterface::Setup.");

        // Run the main loop.
        cout << "utkscan.cpp : Performing Execute method" << endl;
        scanner.Execute();
    } catch (std::exception &ex) {
        cerr << Display::ErrorStr(ex.what()) << endl;
    }

    cout << "utkscan.cpp : Closing things out" << endl;
    scanner.Close();

    return 0;
}
