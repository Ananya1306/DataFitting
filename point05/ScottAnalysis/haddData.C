#include "TFile.h"
#include "TH1.h"
#include "TKey.h"
#include "TList.h"
#include "TString.h"
#include <iostream>
#include <string>

// This is the function that performs the histogram addition.
// It takes the filenames as separate string arguments.
void haddHistograms(const std::string& inputBUpFile, const std::string& inputBDownFile, const std::string& outputFile, int xF_val) {
    // Open the input ROOT files
    TFile* inFileBUp = TFile::Open(inputBUpFile.c_str(), "READ");
    if (!inFileBUp || inFileBUp->IsZombie()) {
        std::cerr << "Error: Could not open input BUp file " << inputBUpFile << std::endl;
        return;
    }

    TFile* inFileBDown = TFile::Open(inputBDownFile.c_str(), "READ");
    if (!inFileBDown || inFileBDown->IsZombie()) {
        std::cerr << "Error: Could not open input BDown file " << inputBDownFile << std::endl;
        inFileBUp->Close();
        return;
    }

    // Create a new output ROOT file
    TFile* outFile = TFile::Open(outputFile.c_str(), "RECREATE");
    if (!outFile || outFile->IsZombie()) {
        std::cerr << "Error: Could not create output file " << outputFile << std::endl;
        inFileBUp->Close();
        inFileBDown->Close();
        return;
    }
    
    // Loop through the phi sectors (0 to 23)
    for (int i = 0; i < 24; ++i) {
	if(i == 11 || i == 12){continue;}
        // Construct the names for the histograms using the provided xF_val
        TString histNameBUp = TString::Format("pi0M_YUp_xF%d_phi%d", xF_val, i);
        TString histNameBDown = TString::Format("pi0M_YDown_xF%d_phi%d", xF_val, i);
        TString histNameBAll = TString::Format("pi0M_YAll_xF%d_phi%d", xF_val, i);

        // Retrieve the histograms from both input files
        TH1* histBUp = dynamic_cast<TH1*>(inFileBUp->Get(histNameBUp.Data()));
        TH1* histBDown = dynamic_cast<TH1*>(inFileBDown->Get(histNameBDown.Data()));

        if (histBUp && histBDown) {
            std::cout << "Found and adding " << histNameBUp << " and " << histNameBDown << std::endl;

            // Clone the BUp histogram to create the BAll histogram
            TH1* histBAll = (TH1*)histBUp->Clone(histNameBAll.Data());

            // Add the BDown histogram to the new BAll histogram
            histBAll->Add(histBDown);

            // Optional: apply SetRangeUser to the combined histogram
            histBAll->GetXaxis()->SetRangeUser(0, 0.4);

            // Write the new BAll histogram to the output file
            outFile->cd();
            histBAll->Write();

            // Clean up memory
            delete histBUp;
            delete histBDown;
            delete histBAll;
        } else {
            std::cerr << "Warning: Could not find both histograms for phi" << i << std::endl;
        }
    }

    // Close the files
    outFile->Close();
    inFileBUp->Close();
    inFileBDown->Close();

    std::cout << "Hadd operation complete. Combined histograms saved to " << outputFile << std::endl;
}

// This function now correctly constructs the filenames and calls haddHistograms.
void haddData(int xF_val) {
    // Construct the filenames as separate TString objects
    TString inputBUpFile = TString::Format("YellowBeamUpxF%d.root", xF_val);
    TString inputBDownFile = TString::Format("YellowBeamDownxF%d.root", xF_val);
    TString outputFile = TString::Format("YellowBeamAllxF%d.root", xF_val);

    // Call the main function with the string data from the TString objects
    haddHistograms(inputBUpFile.Data(), inputBDownFile.Data(), outputFile.Data(), xF_val);
}

