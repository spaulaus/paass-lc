///@file dataGenerator.cpp
///@brief A program that will generate data when provided with a firmware and frequency.
///@author S. V. Paulauskas
///@date August 9, 2017
///@copyright Copyright (c) 2017 S. V. Paulauskas.
///@copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include <array>
#include <iostream>

#include "XiaListModeDataEncoder.hpp"
#include "hribf_buffers.h"

using namespace std;

int main(int argc, char *argv[]) {
    const unsigned int channelNumber = 0;
    vector<unsigned int> encodedData;
    const unsigned int energy = 55;
    const string firmware = "30474";
    const unsigned int frequency = 250;
    const unsigned int moduleNumber = 0;
    const unsigned int numberOfDataWords = 4;
    const unsigned int numberOfDataBuffers = 2;
    const string outputName = "dataGeneratorTest";
    const string outputPath = "/tmp/";
    const unsigned int slotNumber = moduleNumber + 2;
    unsigned int runNumber = 0;
    const string runTitle = "Constant Data with Energy = 55, Mod=Chan=Crate=0";
    const unsigned int totalNumberOfWords = 6;

    XiaListModeDataEncoder encoder(firmware, frequency);
    XiaData data;

    PollOutputFile pld;
    pld.OpenNewFile(runTitle, runNumber, outputName, outputPath);

    for(unsigned int i = 0; i < numberOfDataBuffers; i++) {
        data.SetEnergy(energy);
        data.SetEventTimeLow(i);
        data.SetSlotNumber(slotNumber);
        data.SetChannelNumber(channelNumber);

        try {
            encodedData = encoder.EncodeXiaData(data);
        } catch (invalid_argument &invalidArgument) {
            cout << invalidArgument.what() << endl;
            return 1;
        }

        encodedData.emplace(encodedData.begin(), numberOfDataWords);
        encodedData.emplace(encodedData.begin() + 1, moduleNumber);

        array<char*, totalNumberOfWords> dataArray{reinterpret_cast<char*>(&encodedData[0]),
                                                   reinterpret_cast<char*>(&encodedData[1]),
                                                   reinterpret_cast<char*>(&encodedData[2]),
                                                   reinterpret_cast<char*>(&encodedData[3]),
                                                   reinterpret_cast<char*>(&encodedData[4]),
                                                   reinterpret_cast<char*>(&encodedData[5])};

        pld.Write(dataArray[0], totalNumberOfWords);
    }
    pld.CloseFile();
}