#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TBox.h"
#include "TApplication.h"
#include "TAxis.h"

void graph_Diff_relLum_crossRatio() {

    // Set a style for the plot to make it look nicer
    gStyle->SetOptStat(0);
    //gStyle->SetGrid();

    // Define the file paths for the two input files
    const std::string relLum_file = "aN_pi0_bkg_Weibull_relLum.txt";
    const std::string crossRatio_file = "aN_pi0_bkg_Weibull_crossRatio.txt";
  
/*    const std::string relLum_file = "aN_mod_indv_relLum_sig_goodChi2.txt";
    const std::string crossRatio_file = "aN_mod_indv_crossRatio_sig_goodChi2.txt";
*/
    // Open the input files
    std::ifstream relLum_data(relLum_file);
    std::ifstream crossRatio_data(crossRatio_file);

    // Check if both files were opened successfully
    if (!relLum_data.is_open()) {
        std::cerr << "Error: Could not open file " << relLum_file << std::endl;
    //    return 1;
    }
    if (!crossRatio_data.is_open()) {
        std::cerr << "Error: Could not open file " << crossRatio_file << std::endl;
    //    return 1;
    }

    // Define vectors to store the data
    std::vector<double> xF_vals, aN_pi0_relLum_vec, aN_pi0_err_relLum_vec, aN_pi0_crossRatio_vec, aN_pi0_err_crossRatio_vec;
    std::vector<double> aN_bkg_relLum_vec, aN_bkg_err_relLum_vec, aN_bkg_crossRatio_vec, aN_bkg_err_crossRatio_vec;

    // Define temporary variables to store the data from each line
    double xF_val, aN_pi0_relLum, aN_pi0_err_relLum, aN_bkg_relLum, aN_bkg_err_relLum;
    double xF_val_crossRatio, aN_pi0_crossRatio, aN_pi0_err_crossRatio, aN_bkg_crossRatio, aN_bkg_err_crossRatio;

    // Set output formatting for the console
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "xF_val\t\tStd Dev(pi0)\tStd Dev(bkg)\tDiff(pi0)\tDiff(bkg)\n";
    std::cout << "------------------------------------------------------------------------------------\n";

    // Read the files line by line (row by row) and store data in vectors
    while (relLum_data >> xF_val >> aN_pi0_relLum >> aN_pi0_err_relLum >> aN_bkg_relLum >> aN_bkg_err_relLum &&
           crossRatio_data >> xF_val_crossRatio >> aN_pi0_crossRatio >> aN_pi0_err_crossRatio >> aN_bkg_crossRatio >> aN_bkg_err_crossRatio) {

        // Store the values for plotting
        xF_vals.push_back(xF_val);
        
	aN_pi0_relLum_vec.push_back(aN_pi0_relLum);
        aN_pi0_err_relLum_vec.push_back(aN_pi0_err_relLum);
        aN_pi0_crossRatio_vec.push_back(aN_pi0_crossRatio);
        aN_pi0_err_crossRatio_vec.push_back(aN_pi0_err_crossRatio);
	
	aN_bkg_relLum_vec.push_back(aN_bkg_relLum);
        aN_bkg_err_relLum_vec.push_back(aN_bkg_err_relLum);
        aN_bkg_crossRatio_vec.push_back(aN_bkg_crossRatio);
        aN_bkg_err_crossRatio_vec.push_back(aN_bkg_err_crossRatio);

        // --- Calculation for aN_pi0 ---
        // Calculate the standard deviation of the difference and the actual difference
        double aN_pi0_diff_std_dev = std::sqrt(std::pow(aN_pi0_err_relLum, 2) + std::pow(aN_pi0_err_crossRatio, 2));
        double aN_pi0_diff_val = aN_pi0_relLum - aN_pi0_crossRatio;

        // --- Calculation for aN_bkg ---
        // Calculate the standard deviation of the difference and the actual difference
        double aN_bkg_diff_std_dev = std::sqrt(std::pow(aN_bkg_err_relLum, 2) + std::pow(aN_bkg_err_crossRatio, 2));
        double aN_bkg_diff_val = aN_bkg_relLum - aN_bkg_crossRatio;

        // Output the results to the console
        std::cout << xF_val << "\t\t" << aN_pi0_diff_std_dev << "\t" << aN_bkg_diff_std_dev << "\t" << aN_pi0_diff_val << "\t" << aN_bkg_diff_val << std::endl;
    }

    // Close the files
    relLum_data.close();
    crossRatio_data.close();

    // Create vectors for the plotting errors
    std::vector<double> xF_errs(xF_vals.size(), 0.0); // No error in the x direction for the points
    std::vector<double> xF_small_errs(xF_vals.size(), 0.0025); // Small x error to create visible boxes
    std::vector<double> aN_pi0_diff_abs_val;
    std::vector<double> aN_bkg_diff_abs_val;
    for (size_t i = 0; i < aN_pi0_crossRatio_vec.size(); ++i) {
        aN_pi0_diff_abs_val.push_back(std::abs(aN_pi0_relLum_vec[i] - aN_pi0_crossRatio_vec[i]));
    	//aN_bkg_diff_abs_val.push_back(std::abs(aN_bkg_relLum_vec[i] - aN_bkg_crossRatio_vec[i]));
	}

    for (size_t i = 0; i < aN_bkg_crossRatio_vec.size(); ++i) {
        aN_bkg_diff_abs_val.push_back(std::abs(aN_bkg_relLum_vec[i] - aN_bkg_crossRatio_vec[i]));
    }
    // Create the TGraphAsymmErrors object for the crossRatio data points with their errors
    TGraphAsymmErrors *grcrossRatio = new TGraphAsymmErrors(
        xF_vals.size(),
        xF_vals.data(),
        aN_pi0_crossRatio_vec.data(),
        xF_errs.data(),
        xF_errs.data(),
        aN_pi0_err_crossRatio_vec.data(),
        aN_pi0_err_crossRatio_vec.data()
    );

    // Set style for the crossRatio graph
    grcrossRatio->SetTitle("aN_pi0 crossRatio vs relLum using Weibull; xF; aN_pi0");
    grcrossRatio->SetMarkerStyle(20);
    grcrossRatio->SetMarkerSize(1.5);
    grcrossRatio->SetMarkerColor(kBlue);
    grcrossRatio->SetLineColor(kBlue);

    // Create the TGraphAsymmErrors object for the difference as shaded boxes
    TGraphAsymmErrors *grDiff = new TGraphAsymmErrors(
        xF_vals.size(),
        xF_vals.data(),
        aN_pi0_crossRatio_vec.data(), // Use the crossRatio values as the center of the boxes
        xF_small_errs.data(), // Use small x error to create visible boxes
        xF_small_errs.data(), // Use small x error to create visible boxes
        aN_pi0_diff_abs_val.data(), // Lower y-error (to create a shaded box)
        aN_pi0_diff_abs_val.data()  // Upper y-error
    );

    // Set style for the difference graph (shaded gray boxes)
    grDiff->SetFillColor(kGray);
    grDiff->SetFillStyle(3001); // Solid fill
    grDiff->SetLineColor(kGray);

    // Create a new canvas
    TCanvas *c1 = new TCanvas();

    // Draw the difference graph with the error boxes first
    grDiff->Draw("A2"); // "A" for axis, "2" for shaded error band

    // Draw the crossRatio data points on top
    grcrossRatio->Draw("P SAME"); // "P" for points, "SAME" to draw on the same canvas

    // The TGraphAsymmErrors object for the difference has a y-error
    // equal to the difference, centered on the crossRatio value.
    // This creates the shaded box as requested.
    grcrossRatio->GetYaxis()->SetTitle("a_{N}^{#pi^{0}}");
    grcrossRatio->GetXaxis()->SetTitle("x_{F}");

    // Update the canvas to show the plot
    c1->Update();
     
    auto *mg = new TMultiGraph();
    mg->Add(grDiff,"A2");
    mg->Add(grcrossRatio,"P");
    mg->SetTitle("Blue Beam sideband Asymmetry");
    TCanvas *c3 = new TCanvas();
    mg->Draw("A2P");


    // Create the TGraphAsymmErrors object for the Weibull background data
    TGraphAsymmErrors *grBkgcrossRatio = new TGraphAsymmErrors(
        xF_vals.size(),
        xF_vals.data(),
        aN_bkg_crossRatio_vec.data(),
        xF_errs.data(),
        xF_errs.data(),
        aN_bkg_err_crossRatio_vec.data(),
        aN_bkg_err_crossRatio_vec.data()
    );

    // Set style for the crossRatio background graph
    grBkgcrossRatio->SetTitle("aN_bkg Weibull vs Levy; xF; aN_bkg");
    grBkgcrossRatio->SetMarkerStyle(20);
    grBkgcrossRatio->SetMarkerSize(1.5);
    grBkgcrossRatio->SetMarkerColor(kBlue);
    grBkgcrossRatio->SetLineColor(kBlue);

    // Create the TGraphAsymmErrors object for the background difference as shaded boxes
    TGraphAsymmErrors *grBkgDiff = new TGraphAsymmErrors(
        xF_vals.size(),
        xF_vals.data(),
        aN_bkg_crossRatio_vec.data(), // Use the crossRatio values as the center of the boxes
        xF_small_errs.data(), // Use small x error to create visible boxes
        xF_small_errs.data(), // Use small x error to create visible boxes
        aN_bkg_diff_abs_val.data(), // Lower y-error (to create a shaded box)
        aN_bkg_diff_abs_val.data()  // Upper y-error
    );

    // Set style for the difference graph (shaded gray boxes)
    grBkgDiff->SetFillColor(kGray);
    grBkgDiff->SetFillStyle(3001); // Solid fill
    grBkgDiff->SetLineColor(kGray);

    // Create a new canvas for the background plot
    TCanvas *c2 = new TCanvas("c2", "aN_bkg Comparison", 800, 600);

    // Draw the difference graph with the error boxes first
    grBkgDiff->Draw("A2"); // "A" for axis, "2" for shaded error band

    // Draw the crossRatio data points on top
    grBkgcrossRatio->Draw("P SAME"); // "P" for points, "SAME" to draw on the same canvas

    // The TGraphAsymmErrors object for the difference has a y-error
    // equal to the difference, centered on the Weibull value.
    // This creates the shaded box as requested.
    grBkgcrossRatio->GetYaxis()->SetTitle("a_{N}^{bkg}");
    grBkgcrossRatio->GetXaxis()->SetTitle("x_{F}");
    
    // Update the canvas to show the plot
    c2->Update();

    auto *mgBkg = new TMultiGraph();
    mgBkg->Add(grBkgDiff,"A2");
    mgBkg->Add(grBkgcrossRatio,"P");
    mgBkg->SetTitle("Blue Beam signal asymmetry");
    TCanvas *c4 = new TCanvas();
    mgBkg->Draw("A2P");

    TFile *f = TFile::Open("PreliminaryPlots_BlueBeam.root", "UPDATE");
    f->cd();
    mg->Write("BB_pi0");
    mgBkg->Write("BB_bkg");

    // Clean up memory
//    delete grcrossRatio;
//    delete grDiff;
//    delete c1;

//    return 0;
}

