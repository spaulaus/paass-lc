///@file unittest-XiaListModeDataEncoder.cpp
///@brief Unit tests for the XiaListModeDataDecoder class
///@author S. V. Paulauskas
///@author December 25, 2016
#include "XiaListModeDataEncoder.hpp"

#include <UnitTest++.h>

#include "UnitTestSampleData.hpp"

using namespace std;
using namespace DataProcessing;
using namespace unittest_encoded_data;
using namespace unittest_decoded_data;
using namespace unittest_encoded_data::R30474_250;

TEST(TestConstructors){
    XiaListModeDataEncoder encoder1(XiaListModeDataMask(test_firmware, test_frequency));
    XiaListModeDataEncoder encoder2(test_firmware, test_frequency);
}

///@TODO Add headers for Esums and external TS.
TEST(TestEncoding) {
    XiaListModeDataEncoder encoder(test_firmware, test_frequency);
    XiaData data;

    CHECK_THROW(encoder.EncodeXiaData(data), invalid_argument);

    data.SetEnergy(energy);
    data.SetSlotNumber(slotId);
    data.SetChannelNumber(channelNumber);
    data.SetCrateNumber(crateId);
    data.SetEventTimeLow(ts_low);
    data.SetEventTimeHigh(ts_high);

    //Check that we can handle just a simple 4 word header
    CHECK_ARRAY_EQUAL(header_vec, encoder.EncodeXiaData(data), header_vec.size());

    //Check that we can handle a header with a trace
    data.SetTrace(unittest_trace_variables::trace);
    CHECK_ARRAY_EQUAL(header_vec_w_trc, encoder.EncodeXiaData(data), header_vec_w_trc.size());

    //Check that we can handle a QDC
    data.SetQdc(qdc);
    data.SetTrace (vector<unsigned int>());

    CHECK_ARRAY_EQUAL(header_vec_w_qdc, encoder.EncodeXiaData(data), header_vec_w_qdc.size());

    //Check that we can handle a QDC and a Trace
    data.SetQdc(qdc);
    data.SetTrace(unittest_trace_variables::trace);
    CHECK_ARRAY_EQUAL(header_vec_w_qdc_n_trc, encoder.EncodeXiaData(data), header_vec_w_qdc_n_trc.size());
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}