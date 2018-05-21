///@file UnitTestSampleData.hpp
///@brief This header provides sample data that can be used by Unit Tests to
/// ensure proper functionality.
///@author S. V. Paulauskas
///@date December 18, 2016

#ifndef PIXIESUITE_UNITTESTSAMPLEDATA_HPP
#define PIXIESUITE_UNITTESTSAMPLEDATA_HPP

#include <string>
#include <tuple>
#include <utility>
#include <vector>

///@TODO This whole file needs to be reorganized. This information is repeated a bunch. Maybe consider a function that will
/// construct the headers for us?

///This namespace contains the raw channel information that was used to construct the headers in the unittest_encoded_data
/// namespace. These values are also used when testing the data encoding.
namespace unittest_decoded_data {
    static const unsigned int channelNumber = 13;
    static const unsigned int crateId = 0;
    static const unsigned int expected_size = 1;
    static const unsigned int ts_high = 26001;
    static const unsigned int ts_low = 123456789;
    static const unsigned int cfd_fractional_time = 1234;
    static const unsigned int slotId = 2;
    static const unsigned int energy = 2345;
    static const unsigned int ex_ts_high = 26003;
    static const unsigned int ex_ts_low = 987654321;
    static const bool cfd_forced_trigger = true;
    static const bool cfd_source_trigger_bit = true;
    static const bool pileup_bit = true;
    static const bool trace_out_of_range = true;
    static const bool virtual_channel = true;
    static const double filterBaseline = 3780.7283;

    static const std::vector<unsigned int> energy_sums = {12, 13, 14};

    static const std::vector<unsigned int> qdc = {123, 456, 789, 987, 654, 321, 135, 791};

    //Need to figure out where to put these as they are Firmware / Frequency
    // specific values. They are for R30747, 250 MS/s.
    namespace R30474_250 {
        static const double ts = 111673568120085;
        static const double ts_w_cfd = 223347136240170.075317;
    }
}

///This namespace contains Firmware / Frequency specific headers that are used to test the decoding of data. The headers that
/// we have here include the 2 words that are inserted by poll2 so that the XiaListModeDataDecoder::DecodeBuffer method will
/// function properly.
namespace unittest_encoded_data {
    //A buffer with zero length
    std::vector<unsigned int> empty_buffer{0, 0};

    //A buffer that for an empty module
    std::vector<unsigned int> empty_module_buffer{2, 0};

    ///A header with a header length 20 instead of the true header length 4
    std::vector<unsigned int> header_w_bad_headerlen{4, 0, 3887149, unittest_decoded_data::ts_low, 26001, 2345};

    ///A header where the event length doesn't match what it should be.
    std::vector<unsigned int> header_w_bad_eventlen{59, 0, 7749677, unittest_decoded_data::ts_low, 26001, 8128809};

    static const unsigned int encodedFilterBaseline = 1164725159;

    namespace R30474_250 {
        const std::string test_firmware = "R30474";
        const unsigned int test_frequency = 250;

        const unsigned int word0_header = 540717;
        const unsigned int word0_headerWithExternalTimestamp = 811053;
        const unsigned int word0_headerWithEsums = 1081389;
        const unsigned int word0_headerWithEsumsExternalTimestamp = 1351725;
        const unsigned int word0_headerWithQdc = 1622061;
        const unsigned int word0_headerWithQdcExternalTimestamp = 1892397;
        const unsigned int word0_headerWithEsumsQdc = 2162733;
        const unsigned int word0_headerWithEsumsQdcExternalTimestamp = 2433069;
        const unsigned int word0_headerWithTrace = 8667181;
        const unsigned int word1 = unittest_decoded_data::ts_low;
        const unsigned int word2_energyOnly = 26001;
        const unsigned int word2_energyWithCfd = 80897425;
        const unsigned int word3_headerOnly = 2345;
        const unsigned int word3_headerWithTrace = 8128809;

        /// Just the header, including the first two words inserted by poll2
        std::vector<unsigned int> header{4, 0, word0_header, word1, word2_energyOnly, word3_headerOnly};

        /// This header has the CFD fractional time.
        std::vector<unsigned int> headerWithCfd{4, 0, word0_header, word1, word2_energyWithCfd, word3_headerOnly};

        /// Header that includes an external time stamp.
        std::vector<unsigned int> headerWithExternalTimestamp{
                6, 0, word0_headerWithExternalTimestamp, word1, word2_energyOnly, word3_headerOnly,
                unittest_decoded_data::ex_ts_low, unittest_decoded_data::ex_ts_high};

        ///Header that has Esums
        std::vector<unsigned int> headerWithEnergySums{8, 0, word0_headerWithEsums, word1, word2_energyOnly, word3_headerOnly,
                                                       12, 13, 14, encodedFilterBaseline};

        ///Header that has Esums and an External Timestamp
        std::vector<unsigned int> headerWithEnergySumsExternalTimestamp{
                10, 0, word0_headerWithEsumsExternalTimestamp, word1, word2_energyOnly, word3_headerOnly,
                12, 13, 14, encodedFilterBaseline, unittest_decoded_data::ex_ts_low, unittest_decoded_data::ex_ts_high};

        ///A header that also contains a QDC
        std::vector<unsigned int> headerWithQdc{12, 0, word0_headerWithQdc, word1, word2_energyOnly, word3_headerOnly,
                                                123, 456, 789, 987, 654, 321, 135, 791};

        ///Header that has QDCs and External Timestamps
        std::vector<unsigned int> headerWithQdcExternalTimestamp{
                14, 0, word0_headerWithQdcExternalTimestamp, word1, word2_energyOnly, word3_headerOnly,
                123, 456, 789, 987, 654, 321, 135, 791, unittest_decoded_data::ex_ts_low, unittest_decoded_data::ex_ts_high};

        ///Header that has Esums and a QDC
        std::vector<unsigned int> headerWithEnergySumsQdc{
                16, 0, word0_headerWithEsumsQdc, word1, word2_energyOnly, word3_headerOnly,
                12, 13, 14, encodedFilterBaseline, 123, 456, 789, 987, 654, 321, 135, 791};

        ///Header that has Esums, QDC, and External Timestamp
        std::vector<unsigned int> headerWithEnergySumsQdcExternalTimestamp{
                18, 0, word0_headerWithEsumsQdcExternalTimestamp, word1, word2_energyOnly, word3_headerOnly,
                12, 13, 14, encodedFilterBaseline, 123, 456, 789, 987, 654, 321, 135, 791,
                unittest_decoded_data::ex_ts_low, unittest_decoded_data::ex_ts_high};

        //The header is the standard 4 words. The trace is 62 words, which gives a trace length of 124. This gives us an event
        // length of 66. We have 2 words for the Pixie Module Data Header.
        std::vector<unsigned int> headerWithTrace = {
                66, 0, word0_headerWithTrace, word1, word2_energyOnly, word3_headerWithTrace,
                28574133, 28443058, 28639669, 28508598, 28705202, 28639671,
                28443062, 28770739, 28443062, 28508594, 28836277, 28508599,
                28770741, 28508598, 28574132, 28770741, 28377523, 28574130,
                28901815, 28639668, 28705207, 28508598, 28443058, 28705206,
                28443058, 28836277, 28705207, 28574130, 28770743, 28574133,
                28574130, 28639670, 28639668, 28836280, 28574135, 28639667,
                73531893, 229968182, 227217128, 155716457, 100796282, 68355300,
                49152877, 40567451, 36897359, 30016014, 26411403, 31326660,
                32637420, 31261166, 30081484, 30212558, 29884876, 29622724,
                29688263, 28901822, 29098424, 30081480, 29491651, 29163967,
                29884865, 29819336
        };

        std::vector<unsigned int> headerWithQdcTrace = {
                74, 0, 9748525, word1, word2_energyOnly, word3_headerWithTrace,
                123, 456, 789, 987, 654, 321, 135, 791,
                28574133, 28443058, 28639669, 28508598, 28705202, 28639671,
                28443062, 28770739, 28443062, 28508594, 28836277, 28508599,
                28770741, 28508598, 28574132, 28770741, 28377523, 28574130,
                28901815, 28639668, 28705207, 28508598, 28443058, 28705206,
                28443058, 28836277, 28705207, 28574130, 28770743, 28574133,
                28574130, 28639670, 28639668, 28836280, 28574135, 28639667,
                73531893, 229968182, 227217128, 155716457, 100796282, 68355300,
                49152877, 40567451, 36897359, 30016014, 26411403, 31326660,
                32637420, 31261166, 30081484, 30212558, 29884876, 29622724,
                29688263, 28901822, 29098424, 30081480, 29491651, 29163967,
                29884865, 29819336
        };
    }
}

namespace unittest_cfd_variables {
    namespace traditional {
        static constexpr double fraction = 0.5;
        static constexpr unsigned int delay = 2;
        static constexpr double phase = 74.972;
    }

    namespace xia {
        static constexpr double fraction = 0.25;
        static constexpr unsigned int delay = 12;
        static constexpr double phase = 0.7380742;
    }

    namespace polynomial {
        static constexpr double fraction = 0.5;
        static constexpr unsigned int delay = 2;
        static constexpr double phase = 73.9898;
    }
}

namespace unittest_fit_variables {
    //Set the <beta, gamma> for the fitting from the results of a gnuplot script
    namespace pmt{
        static const double beta = 0.2659404170;
        static const double gamma = 0.2080547991796;
        static const double phase = -0.0826487;
    }

    namespace gaussian {
        static const double beta = 1.0;
        static const double gamma = 0.0;
        static const double phase = 23.5;
    }
}

namespace unittest_trace_variables {
    //This is a trace taken using a 12-bit 250 MS/s module from a medium VANDLE module.
    static const std::vector<unsigned int> trace = {
            437, 436, 434, 434, 437, 437, 438, 435, 434, 438, 439, 437, 438,
            434, 435, 439, 438, 434, 434, 435, 437, 440, 439, 435, 437, 439,
            438, 435, 436, 436, 437, 439, 435, 433, 434, 436, 439, 441, 436,
            437, 439, 438, 438, 435, 434, 434, 438, 438, 434, 434, 437, 440,
            439, 438, 434, 436, 439, 439, 437, 436, 434, 436, 438, 437, 436,
            437, 440, 440, 439, 436, 435, 437, 501, 1122, 2358, 3509, 3816,
            3467, 2921, 2376, 1914, 1538, 1252, 1043, 877, 750, 667, 619,
            591, 563, 526, 458, 395, 403, 452, 478, 492, 498, 494, 477, 460,
            459, 462, 461, 460, 456, 452, 452, 455, 453, 446, 441, 440, 444,
            456, 459, 451, 450, 447, 445, 449, 456, 456, 455
    };

    static const std::vector<double> trace_sans_baseline = {
            0.257143, -0.742857, -2.74286, -2.74286, 0.257143, 0.257143,
            1.25714, -1.74286, -2.74286, 1.25714, 2.25714, 0.257143, 1.25714,
            -2.74286, -1.74286, 2.25714, 1.25714, -2.74286, -2.74286, -1.74286,
            0.257143, 3.25714, 2.25714, -1.74286, 0.257143, 2.25714, 1.25714,
            -1.74286, -0.742857, -0.742857, 0.257143, 2.25714, -1.74286,
            -3.74286, -2.74286, -0.742857, 2.25714, 4.25714, -0.742857,
            0.257143, 2.25714, 1.25714, 1.25714, -1.74286, -2.74286, -2.74286,
            1.25714, 1.25714, -2.74286, -2.74286, 0.257143, 3.25714, 2.25714,
            1.25714, -2.74286, -0.742857, 2.25714, 2.25714, 0.257143, -0.742857,
            -2.74286, -0.742857, 1.25714, 0.257143, -0.742857, 0.257143,
            3.25714, 3.25714, 2.25714, -0.742857, -1.74286, 0.257143, 64.2571,
            685.257, 1921.26, 3072.26, 3379.26, 3030.26, 2484.26, 1939.26,
            1477.26, 1101.26, 815.257, 606.257, 440.257, 313.257, 230.257,
            182.257, 154.257, 126.257, 89.2571, 21.2571, -41.7429, -33.7429,
            15.2571, 41.2571, 55.2571, 61.2571, 57.2571, 40.2571, 23.2571,
            22.2571, 25.2571, 24.2571, 23.2571, 19.2571, 15.2571, 15.2571,
            18.2571, 16.2571, 9.25714, 4.25714, 3.25714, 7.25714, 19.2571,
            22.2571, 14.2571, 13.2571, 10.2571, 8.25714, 12.2571, 19.2571,
            19.2571, 18.2571
    };
    
    static constexpr unsigned int length = 2;
    static constexpr unsigned int gap = 4;
    static const std::vector<double> filteredTrace = {
            0 ,0 ,0 ,0 ,0 ,0 ,0 ,1.0 ,-1.0 ,0.0 ,4.0 ,2.0 ,0.0 ,0.0 ,-1.0 ,1.0 ,1.0 ,-1.0 ,-3.0 ,-4.0 ,1.0 ,2.0 ,1.0 ,
            1.0 ,1.0 ,3.0 ,4.0 ,1.0 ,-5.0 ,-3.0 ,1.0 ,0.0 ,0.0 ,-3.0 ,-2.0 ,-2.0 ,0.0 ,2.0 ,2.0 ,1.0 ,4.0 ,5.0 ,2.0 ,
            -1.0 ,-6.0 ,-2.0 ,-3.0 ,-1.0 ,0.0 ,-4.0 ,-1.0 ,3.0 ,6.0 ,1.0 ,0.0 ,0.0 ,2.0 ,2.0 ,-1.0 ,-2.0 ,-2.0 ,0.0 ,
            0.0 ,-1.0 ,-2.0 ,-1.0 ,1.0 ,6.0 ,4.0 ,1.0 ,-1.0 ,-1.0 ,0.0 ,61.0 ,682.0 ,1919.0 ,3073.0 ,3381.0 ,3030.0 ,
            2420.0 ,1254.0 ,-444.0 ,-1971.0 ,-2564.0 ,-2424.0 ,-2044.0 ,-1626.0 ,-1247.0 ,-919.0 ,-661.0 ,-480.0 ,
            -351.0 ,-292.0 ,-272.0 ,-216.0 ,-139.0 ,-85.0 ,-34.0 ,40.0 ,99.0 ,74.0 ,8.0 ,-19.0 ,-30.0 ,-37.0 ,-34.0 ,
            -21.0 ,-8.0 ,-7.0 ,-7.0 ,-8.0 ,-14.0 ,-15.0 ,-12.0 ,-8.0 ,1.0 ,6.0 ,5.0 ,9.0 ,7.0 ,1.0 ,-7.0 ,-3.0 ,5.0
    };

    static const std::pair<unsigned int, unsigned int> waveform_range(71, 86);

    static const std::vector<double> waveform(trace_sans_baseline.begin() + waveform_range.first,
                                              trace_sans_baseline.begin() + waveform_range.second);

    static const double waveform_qdc = 21329.9;

    //An empty data vector to test error checking.
    static const std::vector<unsigned int> empty_vector_uint;
    static const std::vector<double> empty_vector_double;

    static const std::vector<unsigned int> const_vector_uint = {1000, 4};
    static const double maximum_value = 3816;
    static const unsigned int max_position = 76;
    static const std::pair<unsigned int, double> max_pair(max_position, maximum_value);

    static const double baseline = 436.7428571;
    static const double standard_deviation = 1.976184739;
    static const std::pair<double, double> baseline_pair(baseline, standard_deviation);

    static const unsigned int trace_delay = 80;
    static const double tail_ratio = 0.2960894762;

    static const std::vector<double> extrapolated_max_coeffs =
            {-15641316.0007084, 592747.666694852, -7472.00000037373, 31.3333333349849};
    static const double extrapolated_maximum = 3818.0718412264;
    static const std::pair<unsigned int, double> extrapolated_maximum_pair(max_position, extrapolated_maximum);
}

namespace unittest_gaussian_trace {
    static const std::vector<double> waveform = {
            1, 2, 9, 17, 46, 92, 175, 377, 643, 1190, 2075, 3463, 5700, 8865, 13070, 19054, 25979, 34204, 43464, 52979,
            62388, 70753, 76153, 79198, 79222, 75935, 70457, 62153, 53156, 43894, 34154, 26270, 19029, 13162, 8852,
            5716, 3551, 2091, 1182, 616, 324, 186, 85, 36, 20,  8, 4
    };

    static const unsigned int maxPosition = 24;
    static const double maxValue = 79222;
    static const std::pair<unsigned int, double> maxPair(maxPosition, maxValue);

    static const double baseline = 0.0;
    static const double standardDeviation = 0.0;
    static const std::pair<double, double> baselinePair(baseline, standardDeviation);
    static const double qdc = 1e6;
    static const double sigma = 1.0;
}

namespace unittest_helper_functions {
    static const std::pair<double, double> xy1(74, -729.0);
    static const std::pair<double, double> xy2(75, 21.0);
    static const double slope = 750.0;
    static const double intercept = -56229.0;

    static const std::vector<unsigned int> integration_data = {0, 1, 2, 3, 4, 5};
    static const double integral = 12.5;

    static const std::pair<unsigned int, unsigned int> qdc_pair(2, 5);
    static const double integration_qdc = 6;

    static const std::vector<unsigned int> poly3_data(unittest_trace_variables::trace.begin() + 74,
                                                      unittest_trace_variables::trace.begin() + 78);
    static const double poly3_maximum = unittest_trace_variables::extrapolated_maximum;

    //A vector containing the coefficients obtained from gnuplot using the data
    // from pol3_data with an x value starting at 0
    static const std::vector<double> poly3_coeffs = {2358.0, 1635.66666666667, -516.0, 31.3333333333333};

    static const std::vector<unsigned int> poly2_data(unittest_trace_variables::trace.begin() + 73,
                                                      unittest_trace_variables::trace.begin() + 76);
    static const double poly2_val = 10737.0720588236;
    static const std::vector<double> poly2_coeffs = {1122.0, 1278.5, -42.4999999999999};

    static const unsigned int leading_edge_position = 73;
    static const double leading_edge_fraction = 0.5;
    static const double bad_fraction = -0.5;
}

#endif //PIXIESUITE_UNITTESTEXAMPLETRACE_HPP_HPP
