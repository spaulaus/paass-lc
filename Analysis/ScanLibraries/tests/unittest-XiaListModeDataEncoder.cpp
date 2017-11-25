///@file unittest-XiaListModeDataEncoder.cpp
///@brief Unit tests for the XiaListModeDataDecoder class
///@author S. V. Paulauskas
///@date December 25, 2016
#include "XiaListModeDataEncoder.hpp"
#include "UnitTestSampleData.hpp"

#include <UnitTest++.h>

using namespace std;
using namespace DataProcessing;
using namespace unittest_encoded_data;
using namespace unittest_decoded_data;
using namespace unittest_encoded_data::R30474_250;

void SetupXiaDataClass(XiaData &data) {
    data.Initialize();
    data.SetEnergy(energy);
    data.SetSlotNumber(slotId);
    data.SetChannelNumber(channelNumber);
    data.SetCrateNumber(crateId);
    data.SetEventTimeLow(ts_low);
    data.SetEventTimeHigh(ts_high);
}

void SetupEsums(XiaData &data) {
    data.SetFilterBaseline(unittest_decoded_data::filterBaseline);
    data.SetEnergySums(unittest_decoded_data::energy_sums);
}

void SetupExternalTimestamp(XiaData &data) {
    data.SetExternalTimeLow(unittest_decoded_data::ex_ts_low);
    data.SetExternalTimeHigh(unittest_decoded_data::ex_ts_high);
}

///For now we use the same encoder for everybody since we're only testing one firmware/frequency combo.
XiaListModeDataEncoder encoder(test_firmware, test_frequency);

///Everybody is going to use the same XiaData class we'll just initialize it each time.
XiaData data;

TEST(TestConstructors){
    XiaListModeDataEncoder encoder1(XiaListModeDataMask(test_firmware, test_frequency));
    XiaListModeDataEncoder encoder2(test_firmware, test_frequency);
}

TEST(TestEmptyDataThrow) {
    CHECK_THROW(encoder.EncodeXiaData(XiaData()), invalid_argument);
}

TEST(TestEncodingBasicHeader) {
    SetupXiaDataClass(data);
    CHECK_ARRAY_EQUAL(header_vec, encoder.EncodeXiaData(data), header_vec.size());
}

TEST(TestEncodingExternalTimestamps) {
    SetupXiaDataClass(data);
    SetupExternalTimestamp(data);
    CHECK_ARRAY_EQUAL(header_vec_Ets, encoder.EncodeXiaData(data), header_vec_Ets.size());
}

TEST(TestEncodingEnergySums) {
    SetupXiaDataClass(data);
    SetupEsums(data);
    CHECK_ARRAY_EQUAL(header_vec_Esums, encoder.EncodeXiaData(data), header_vec_Esums.size());
}

TEST(TestEncodingEnergySumsAndExternalTimeStamps) {
    SetupXiaDataClass(data);
    SetupEsums(data);
    SetupExternalTimestamp(data);
    CHECK_ARRAY_EQUAL(header_vec_EsumsExTs, encoder.EncodeXiaData(data), header_vec_EsumsExTs.size());
}

TEST(TestEncodingQdc) {
    SetupXiaDataClass(data);
    data.SetQdc(qdc);
    CHECK_ARRAY_EQUAL(header_vec_Qdc, encoder.EncodeXiaData(data), header_vec_Qdc.size());
}

TEST(TestEncodingQdcAndExternalTimestamp) {
    SetupXiaDataClass(data);
    data.SetQdc(qdc);
    SetupExternalTimestamp(data);
    CHECK_ARRAY_EQUAL(header_vec_QdcExTs, encoder.EncodeXiaData(data), header_vec_QdcExTs.size());
}

TEST(TestEncodingEnergySumsAndQdc) {
    SetupXiaDataClass(data);
    SetupEsums(data);
    data.SetQdc(qdc);
    CHECK_ARRAY_EQUAL(header_vec_EsumQdc, encoder.EncodeXiaData(data), header_vec_EsumQdc.size());
}

TEST(TestEncodingEnergySumsAndQdcAndTimestamp) {
    SetupXiaDataClass(data);
    SetupEsums(data);
    data.SetQdc(qdc);
    SetupExternalTimestamp(data);
    CHECK_ARRAY_EQUAL(header_vec_EsumQdcEts, encoder.EncodeXiaData(data), header_vec_EsumQdcEts.size());
}

TEST(TestEncodingTrace) {
    SetupXiaDataClass(data);
    data.SetTrace(unittest_trace_variables::trace);
    CHECK_ARRAY_EQUAL(header_vec_w_trc, encoder.EncodeXiaData(data), header_vec_w_trc.size());
}

TEST(TestEncodingTraceAndQdc) {
    SetupXiaDataClass(data);
    data.SetQdc(qdc);
    data.SetTrace(unittest_trace_variables::trace);
    CHECK_ARRAY_EQUAL(header_vec_w_qdc_n_trc, encoder.EncodeXiaData(data), header_vec_w_qdc_n_trc.size());
}

int main(int argv, char *argc[]) {
    return (UnitTest::RunAllTests());
}