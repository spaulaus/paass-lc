///@file dataGenerator.cpp
///@brief A program that will generate data when provided with a firmware and frequency.
///@author S. V. Paulauskas
///@date August 9, 2017
///@copyright Copyright (c) 2017 S. V. Paulauskas.
///@copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include <chrono>
#include <functional>
#include <iostream>
#include <random>

#include "XiaListModeDataEncoder.hpp"
#include "hribf_buffers.h"

using namespace std;

int main(int argc, char *argv[]) {
    unsigned int frequency;
    string firmware;
    switch (argc) {
        case 3:
            frequency = stoul(argv[1]);
            firmware = argv[2];
            break;
        default:
            cout << argv[0] << " must have two command line arguments : frequency and firmware!!" << endl
                 << "Usage: " << argv[0] << " <frequency> <firmware>" << endl;
            return 1;
    }

    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    std::uniform_int_distribution<int> channelDistribution(0,1);
    auto randomChannel = std::bind (channelDistribution, rng);

//    std::uniform_real_distribution<double> efficiencyDistribution(0., 1.);
//    auto randomEfficiency = std::bind (efficiencyDistribution, rng);

//    std::uniform_real_distribution<double> backgroundDistribution(0., 1000.);
//    auto randomBackground = std::bind (backgroundDistribution, rng);

    std::normal_distribution<double> gammaDistribution(661.657,33.08285);
    auto randomEnergy = std::bind (gammaDistribution, rng);

    XiaListModeDataMask dataMask(firmware, frequency);
    XiaListModeDataEncoder encoder(dataMask);
    XiaData data;

    PollOutputFile output_file;
    output_file.SetFileFormat(1);
    unsigned int runNumber = 0;
    output_file.OpenNewFile("Here is a title", runNumber, "dataGenTest", "/tmp/");

    const unsigned int totalNumberOfWords = 4;
    const unsigned int numberOfDataWords = 4;
    const unsigned int moduleNumber = 0;
    const unsigned int endOfDataWord = 0xFFFFFFFF;
//    unsigned int dataWord = 0x41544144;
//    unsigned int numberHeaderWords = 2;
//
//    array<char*, 2> dataHeader{reinterpret_cast<char*>(&dataWord), reinterpret_cast<char*>(&numberHeaderWords)};
//    output_file.Write(dataHeader[0], 2);

    for(unsigned int i = 0; i < 2; i++) {
        data.SetEnergy(int(randomEnergy()));
        data.SetSlotNumber(moduleNumber + 2);
        data.SetEventTimeLow(i);
        data.SetChannelNumber(randomChannel());

        vector<unsigned int> encodedData = encoder.EncodeXiaData(data);

        array<char*, totalNumberOfWords> dataArray{reinterpret_cast<char*>(&encodedData[0]),
                                                   reinterpret_cast<char*>(&encodedData[1]),
                                                   reinterpret_cast<char*>(&encodedData[2]),
                                                   reinterpret_cast<char*>(&encodedData[3])};
        output_file.Write(dataArray[0], totalNumberOfWords);
    }
    output_file.CloseFile();
}
