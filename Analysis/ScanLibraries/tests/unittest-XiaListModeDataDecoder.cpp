///@file unittest-XiaListModeDataDecoder.cpp
///@brief Unit tests for the XiaListModeDataDecoder class
///@author S. V. Paulauskas
///@date December 25, 2016
#include "XiaListModeDataDecoder.hpp"

#include "HelperEnumerations.hpp"
#include "UnitTestSampleData.hpp"

#include <UnitTest++.h>

#include <stdexcept>

using namespace std;
using namespace DataProcessing;
using namespace unittest_encoded_data;
using namespace unittest_decoded_data;

///@TODO These need to be expanded so that we cover all of the nine different firmware and frequency combinations.
static const XiaListModeDataMask mask(R30474, 250);
using namespace unittest_encoded_data::R30474_250;

TEST_FIXTURE(XiaListModeDataDecoder, TestBufferLengthChecks) {
    CHECK_THROW(DecodeBuffer(&empty_buffer[0], mask), length_error);
    CHECK_EQUAL(empty_buffer[1], DecodeBuffer(&empty_module_buffer[0], mask).size());
}

TEST_FIXTURE(XiaListModeDataDecoder, TestHeaderDecoding) {
    CHECK_EQUAL((unsigned int)0, DecodeBuffer(&header_w_bad_headerlen[0], mask).size());

    XiaData result_data = *(DecodeBuffer(&header[0], mask).front());
    CHECK_EQUAL(slotId, result_data.GetSlotNumber());
    CHECK_EQUAL(channelNumber, result_data.GetChannelNumber());
    CHECK_EQUAL(energy, result_data.GetEnergy());
    CHECK_EQUAL(ts_high, result_data.GetEventTimeHigh());
    CHECK_EQUAL(ts_low, result_data.GetEventTimeLow());
    CHECK_EQUAL(unittest_decoded_data::R30474_250::ts, result_data.GetTime());
}

TEST_FIXTURE(XiaListModeDataDecoder, TestExternalTimestampDecoding) {
    XiaData result = *(DecodeBuffer(&headerWithExternalTimestamp[0], mask).front());
    CHECK_EQUAL(unittest_decoded_data::ex_ts_low, result.GetExternalTimeLow());
    CHECK_EQUAL(unittest_decoded_data::ex_ts_high, result.GetExternalTimeHigh());
}

TEST_FIXTURE(XiaListModeDataDecoder, TestEsumsDecoding) {
    XiaData result = *(DecodeBuffer(&headerWithEnergySums[0], mask).front());
    CHECK_ARRAY_EQUAL(unittest_decoded_data::energy_sums, result.GetEnergySums(), unittest_decoded_data::energy_sums.size());
    CHECK_CLOSE(unittest_decoded_data::filterBaseline, result.GetFilterBaseline(), 1e-4);
}

TEST_FIXTURE(XiaListModeDataDecoder, TestEsumsAndExternalTsDecoding) {
    XiaData result = *(DecodeBuffer(&headerWithEnergySumsExternalTimestamp[0], mask).front());
    CHECK_ARRAY_EQUAL(unittest_decoded_data::energy_sums, result.GetEnergySums(), unittest_decoded_data::energy_sums.size());
    CHECK_CLOSE(unittest_decoded_data::filterBaseline, result.GetFilterBaseline(), 1e-4);

    CHECK_EQUAL(unittest_decoded_data::ex_ts_low, result.GetExternalTimeLow());
    CHECK_EQUAL(unittest_decoded_data::ex_ts_high, result.GetExternalTimeHigh());
}

TEST_FIXTURE(XiaListModeDataDecoder, TestQdcDecoding) {
    XiaData result = *(DecodeBuffer(&headerWithQdc[0], mask).front());
    CHECK_ARRAY_EQUAL(qdc, result.GetQdc(), qdc.size());
}

TEST_FIXTURE(XiaListModeDataDecoder, TestQdcAndExternalTsDecoding) {
    XiaData result = *(DecodeBuffer(&headerWithQdcExternalTimestamp[0], mask).front());
    CHECK_ARRAY_EQUAL(qdc, result.GetQdc(), qdc.size());
    CHECK_EQUAL(unittest_decoded_data::ex_ts_low, result.GetExternalTimeLow());
    CHECK_EQUAL(unittest_decoded_data::ex_ts_high, result.GetExternalTimeHigh());
}

TEST_FIXTURE(XiaListModeDataDecoder, TestEsumsAndQdcDecoding) {
    XiaData result = *(DecodeBuffer(&headerWithEnergySumsQdc[0], mask).front());
    CHECK_ARRAY_EQUAL(unittest_decoded_data::energy_sums, result.GetEnergySums(), unittest_decoded_data::energy_sums.size());
    CHECK_CLOSE(unittest_decoded_data::filterBaseline, result.GetFilterBaseline(), 1e-4);
    CHECK_ARRAY_EQUAL(qdc, result.GetQdc(), qdc.size());
}

TEST_FIXTURE(XiaListModeDataDecoder, TestEsumsAndQdcAndExTsDecoding) {
    XiaData result = *(DecodeBuffer(&headerWithEnergySumsQdcExternalTimestamp[0], mask).front());
    CHECK_ARRAY_EQUAL(unittest_decoded_data::energy_sums, result.GetEnergySums(), unittest_decoded_data::energy_sums.size());
    CHECK_CLOSE(unittest_decoded_data::filterBaseline, result.GetFilterBaseline(), 1e-4);
    CHECK_ARRAY_EQUAL(qdc, result.GetQdc(), qdc.size());
    CHECK_EQUAL(unittest_decoded_data::ex_ts_low, result.GetExternalTimeLow());
    CHECK_EQUAL(unittest_decoded_data::ex_ts_high, result.GetExternalTimeHigh());
}

TEST_FIXTURE(XiaListModeDataDecoder, TestTraceDecoding) {
    CHECK_EQUAL((unsigned int)0, DecodeBuffer(&header_w_bad_eventlen[0], mask).size());

    XiaData result = *(DecodeBuffer(&headerWithTrace[0], mask).front());
    CHECK_ARRAY_EQUAL(unittest_trace_variables::trace, result.GetTrace(), unittest_trace_variables::trace.size());
}

TEST_FIXTURE(XiaListModeDataDecoder, TestCfdTimeCalculation) {
    XiaData result = *(DecodeBuffer(&headerWithCfd[0], mask).front());
    CHECK_EQUAL(cfd_fractional_time, result.GetCfdFractionalTime());
    CHECK_CLOSE(unittest_decoded_data::R30474_250::ts_w_cfd, result.GetTime(), 1e-5);
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}