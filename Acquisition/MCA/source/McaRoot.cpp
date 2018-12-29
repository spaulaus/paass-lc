/// @file McaRoot.cpp
/// @brief Class derived from MCA that writes the MCA spectra to a root file.
/// @author K. Smith, C. R. Thornsberry, S. V. Paulauskas
/// @date September 11, 2018
#include "McaRoot.hpp"

#include "AcquisitionInterface.hpp"
#include "CerrDirector.hpp"
#include "Display.h"

#include <TFile.h>
#include <TH1D.h>

McaRoot::McaRoot(AcquisitionInterface *pif, const char *basename) : Mca(pif) {
    OpenFile(basename);
}

McaRoot::~McaRoot() {
    if (IsOpen()) {
        file_->Write(nullptr, TObject::kWriteDelete);
        file_->Close();
    }
    delete file_;
}

bool McaRoot::OpenFile(const char *basename) {
    Display::LeaderPrint(std::string("Creating new empty ROOT histogram ") + std::string(basename) + std::string(".root"));

    auto *redirect = new CerrDirector("Pixie16msg.txt");

    file_ = new TFile(Form("%s.root", basename), "RECREATE");
    isOpen_ = file_->IsOpen() && file_->IsWritable();

    if (Display::StatusPrint(!isOpen_)) {
        redirect->Print();
        return isOpen_;
    }

    redirect->Print();
    delete redirect;

    for (unsigned int card = 0; card < pif_->GetConfiguration().GetNumberOfModules(); card++) {
        for (unsigned int ch = 0; ch < pif_->GetConfiguration().GetNumberOfChannels(); ch++) {
            auto id = CalculateHistogramId(0, card, ch);
            histograms_[id] =
                    new TH1D(Form("h%d", id), Form("Mod %d Ch %d", card, ch), ADC_SIZE, 0, ADC_SIZE);
        }
    }

    file_->Write(nullptr, TObject::kWriteDelete);

    return true;
}

TH1D *McaRoot::GetHistogram(const unsigned short &mod, const unsigned short &ch) {
    auto loc = histograms_.find(CalculateHistogramId(0, mod, ch));

    if (loc == histograms_.end())
        return nullptr;

    return loc->second;
}

bool McaRoot::StoreData(const unsigned short &mod, const unsigned short &ch) {
    auto histogram = GetHistogram(mod, ch);

    if (!histogram)
        return false;

    Pixie16::word_t histo[ADC_SIZE] = {0};
    pif_->ReadHistogram(histo, ADC_SIZE, mod, ch);

    for (unsigned int i = 0; i < ADC_SIZE; i++)
        histogram->SetBinContent(i + 1, histo[i]);

    return true;
}

void McaRoot::Reset() {
    for (const auto &pair : histograms_)
        pair.second->Reset();
}

void McaRoot::Flush() {
    file_->Write(nullptr, TObject::kWriteDelete);
    file_->Flush();
}
