///@file HelperEnumerations.hpp
///@brief Header containing namespaced enumerations that are useful to
/// different parts of the software.
///@author S. V. Paulauskas
///@date December 6, 2016
#ifndef PIXIESUITE_HELPERENUMERATIONS_HPP
#define PIXIESUITE_HELPERENUMERATIONS_HPP

namespace Timing {
    /// An enum listing the known CFD analysis types
    enum CFD_TYPE {
        POLY, XIA, BASIC
    };
    /// An enum listing the known Fitter types
    enum FITTER_TYPE {
        GSL, UNKNOWN
    };
}

namespace DataProcessing {
    ///An enum for the different firmware revisions for the Pixie-16 modules. These revisions only mark changes in
    /// the header, and do not represent the full set of known firmwares.
    ///See https://github.com/spaulaus/paass-laughing-conqueror/wiki/Pixie-Firmware-Information
    enum FIRMWARE {
        R17562=17562, R20466=20466, R27361=27361, R29432=29432, R30474=30474, R30980=30980, R30981=30981, R34688=34688,
        UNKNOWN
    };

    ///An enumeration that tells how long headers from the XIA List mode data
    /// are depending on the different options that are enabled. The order
    /// that the components are listed is the order that they appear in the
    /// header. For example, HEADER_W_ESUM_ETS is the length of the header
    /// when we have the default 4 words, plus the Energy Sums (4 words) and
    /// the External Time Stamps (2 words).
    enum HEADER_CODES {
        STATS_BLOCK = 1, HEADER = 4, HEADER_W_ETS = 6, HEADER_W_ESUM = 8,
        HEADER_W_ESUM_ETS = 10, HEADER_W_QDC = 12, HEADER_W_QDC_ETS = 14,
        HEADER_W_ESUM_QDC = 16, HEADER_W_ESUM_QDC_ETS = 18
    };
}

namespace Interface {
    enum class BootType {
        MCA
    };

    enum BootFlags {
        BootComm = 0x01, BootTrigger = 0x02, BootSignal = 0x04, BootDSP = 0x08, DownloadParameters = 0x10,
        ProgramFPGA = 0x20, SetDAC = 0x40, BootAll = 0x7f
    };
}


#endif //PIXIESUITE_HELPERENUMERATIONS_HPP
