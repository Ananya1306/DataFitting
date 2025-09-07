#include "TFile.h"
#include "TH1.h"
#include "TKey.h"
#include "TList.h"
#include "TString.h"
#include <iostream>
#include <string>

void processHistograms(const std::string& inputFile, const std::string& outputFile) {
    // Open the input ROOT file
    TFile* inFile = TFile::Open(inputFile.c_str(), "READ");
    if (!inFile || inFile->IsZombie()) {
        std::cerr << "Error: Could not open input file " << inputFile << std::endl;
        return;
    }

    // Create a new output ROOT file
    TFile* outFile = TFile::Open(outputFile.c_str(), "UPDATE");
    if (!outFile || outFile->IsZombie()) {
        std::cerr << "Error: Could not create output file " << outputFile << std::endl;
        inFile->Close();
        return;
    }

    // Get the list of keys (objects) in the input file
    TList* list = inFile->GetListOfKeys();
    TIter next(list);
    TKey* key;

    // Loop over all objects in the file
    while ((key = (TKey*)next())) {
        std::string objName = key->GetName();
        std::string className = key->GetClassName();

        // Check if the object is a 1D histogram
        if (className.rfind("TH1", 0) == 0) {
            
            // Check if the histogram name matches the desired pattern
            bool matchFound = false;
            for (int i = 0; i < 24; ++i) {
                if (objName == TString::Format("pi0M_BDown_xF0_phi%d", i).Data()) {
                    matchFound = true;
                    break;
                }
            }

            // Only process and write the histogram if a match is found
            if (matchFound) {
                TH1* hist = dynamic_cast<TH1*>(key->ReadObj());
                if (hist) {
                    std::cout << "Applying SetRangeUser and writing " << objName << std::endl;
                    // Set the desired range
                    hist->GetXaxis()->SetRangeUser(0, 0.4);

                    // Write the modified histogram to the output file
                    outFile->cd();
                    hist->Write();

                    // Clean up memory
                    delete hist;
                }
            }
        }
    }

    // Close the files
    outFile->Close();
    inFile->Close();

    std::cout << "Processing complete. Selected histograms saved to " << outputFile << std::endl;
}

void prepData() {
    processHistograms("EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et06pt0_xFTest_1.root", "BlueBeamDownxF0.root");
}
