///@authors D. Miller
// Simple program to boot pixie cards loading the working set file

#include <cstdlib>

#include <unistd.h>

#include "PixieInterface.h"

int main(int argc, char *argv[]) {
    PixieInterface pif("pixie-cfg.xml");
    pif.Init();
    if (!pif.Boot(Interface::BootFlags::BootAll, true)) {
        sleep(1);
        pif.Boot(Interface::BootFlags::BootAll, true);
    }

    return EXIT_SUCCESS;
}
