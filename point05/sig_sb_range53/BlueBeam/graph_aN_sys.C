#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TBox.h"
#include "TApplication.h"
#include "TAxis.h"

void graph_aN_sys(string method_input){

	// Set a style for the plot to make it look nicer
    gStyle->SetOptStat(0);
    //gStyle->SetGrid();

    // Define the file paths for the two input files
    const std::string weibull_file = "aN_pi0_bkg_Weibull_" + method_input + ".txt";
    const std::string levy_file = "aN_pi0_bkg_Levy_" + method_input + ".txt";
    const std::string cheb_file = "aN_pi0_bkg_Cheb_" + method_input + ".txt";

/*    const std::string relLum_file = "aN_mod_indv_relLum_sig_goodChi2.txt";
    const std::string crossRatio_file = "aN_mod_indv_crossRatio_sig_goodChi2.txt";
*/
    // Open the input files
    std::ifstream weibull_data(weibull_file);
    std::ifstream levy_data(levy_file);
    std::ifstream cheb_data(cheb_file);

    // Check if both files were opened successfully
    if (!weibull_data.is_open()) {
        std::cerr << "Error: Could not open file " << weibull_file << std::endl;
    //    return 1;
    }
    if (!levy_data.is_open()) {
        std::cerr << "Error: Could not open file " << levy_file << std::endl;
    //    return 1;
    }
    if (!cheb_data.is_open()) {
        std::cerr << "Error: Could not open file " << cheb_file << std::endl;
    //    return 1;
    }

    const int bins = 5;
    double xF_val_arr[bins];
    double aN_pi0_weibull_arr[bins], aN_pi0_err_weibull_arr[bins];
    double aN_bkg_weibull_arr[bins], aN_bkg_err_weibull_arr[bins];
    double aN_pi0_levy_arr[bins], aN_pi0_err_levy_arr[bins];
    double aN_bkg_levy_arr[bins], aN_bkg_err_levy_arr[bins];
    double aN_pi0_cheb_arr[bins], aN_pi0_err_cheb_arr[bins];
    double aN_bkg_cheb_arr[bins], aN_bkg_err_cheb_arr[bins];

    // Arrays to store the final weighted mean results
    double aN_pi0_weighted_arr[bins], aN_pi0_err_weighted_arr[bins];
    double aN_bkg_weighted_arr[bins], aN_bkg_err_weighted_arr[bins];

    double xF_val_weibull, aN_pi0_weibull, aN_pi0_err_weibull, aN_bkg_weibull, aN_bkg_err_weibull;
    double xF_val_levy, aN_pi0_levy, aN_pi0_err_levy, aN_bkg_levy, aN_bkg_err_levy;
    double xF_val_cheb, aN_pi0_cheb, aN_pi0_err_cheb, aN_bkg_cheb, aN_bkg_err_cheb;

    int i = 0;
    while(weibull_data >> xF_val_weibull >> aN_pi0_weibull >> aN_pi0_err_weibull >> aN_bkg_weibull >> aN_bkg_err_weibull &&
          levy_data >> xF_val_levy >> aN_pi0_levy >> aN_pi0_err_levy >> aN_bkg_levy >> aN_bkg_err_levy &&
          cheb_data >> xF_val_cheb >> aN_pi0_cheb >> aN_pi0_err_cheb >> aN_bkg_cheb >> aN_bkg_err_cheb)
	{
		xF_val_arr[i] = xF_val_weibull;

		aN_pi0_weibull_arr[i] = aN_pi0_weibull; aN_pi0_err_weibull_arr[i] = aN_pi0_err_weibull;
		aN_bkg_weibull_arr[i] = aN_bkg_weibull; aN_bkg_err_weibull_arr[i] = aN_bkg_err_weibull;
		
		aN_pi0_levy_arr[i] = aN_pi0_levy; aN_pi0_err_levy_arr[i] = aN_pi0_err_levy;
                aN_bkg_levy_arr[i] = aN_bkg_levy; aN_bkg_err_levy_arr[i] = aN_bkg_err_levy;

		aN_pi0_cheb_arr[i] = aN_pi0_cheb; aN_pi0_err_cheb_arr[i] = aN_pi0_err_cheb;
                aN_bkg_cheb_arr[i] = aN_bkg_cheb; aN_bkg_err_cheb_arr[i] = aN_bkg_err_cheb;
	
		i++;
	}    

	//Calculate weighted mean for aN_pi0 and aN_bkg that you get from the 3 different functions - weibull, levy and cheb taking their respective errors into account

	   std::cout << "Calculating weighted means..." << std::endl;

    for (int i = 0; i < bins; ++i) {
        // --- Calculation for aN_pi0 ---
        // Weights are the inverse of the error squared
        double w_weibull_pi0 = 1.0 / (aN_pi0_err_weibull_arr[i] * aN_pi0_err_weibull_arr[i]);
        double w_levy_pi0 = 1.0 / (aN_pi0_err_levy_arr[i] * aN_pi0_err_levy_arr[i]);
        double w_cheb_pi0 = 1.0 / (aN_pi0_err_cheb_arr[i] * aN_pi0_err_cheb_arr[i]);

        // Numerator of the weighted mean formula: sum(value * weight)
        double numerator_pi0 = (aN_pi0_weibull_arr[i] * w_weibull_pi0) +
                             (aN_pi0_levy_arr[i] * w_levy_pi0) +
                             (aN_pi0_cheb_arr[i] * w_cheb_pi0);
        
        // Denominator of the weighted mean formula: sum(weights)
        double denominator_pi0 = w_weibull_pi0 + w_levy_pi0 + w_cheb_pi0;

        // Final weighted mean
        if (denominator_pi0 != 0) { // Avoid division by zero
            aN_pi0_weighted_arr[i] = numerator_pi0 / denominator_pi0;
            // Error on the weighted mean is 1/sqrt(sum(weights))
            aN_pi0_err_weighted_arr[i] = 1.0 / sqrt(denominator_pi0);
        } else {
            aN_pi0_weighted_arr[i] = 0;
            aN_pi0_err_weighted_arr[i] = 0;
        }


        // --- Calculation for aN_bkg ---
        // Weights for aN_bkg
        double w_weibull_bkg = 1.0 / (aN_bkg_err_weibull_arr[i] * aN_bkg_err_weibull_arr[i]);
        double w_levy_bkg = 1.0 / (aN_bkg_err_levy_arr[i] * aN_bkg_err_levy_arr[i]);
        double w_cheb_bkg = 1.0 / (aN_bkg_err_cheb_arr[i] * aN_bkg_err_cheb_arr[i]);

        // Numerator for aN_bkg
        double numerator_bkg = (aN_bkg_weibull_arr[i] * w_weibull_bkg) +
                             (aN_bkg_levy_arr[i] * w_levy_bkg) +
                             (aN_bkg_cheb_arr[i] * w_cheb_bkg);
        
        // Denominator for aN_bkg
        double denominator_bkg = w_weibull_bkg + w_levy_bkg + w_cheb_bkg;

        // Final weighted mean for aN_bkg
        if (denominator_bkg != 0) {
            aN_bkg_weighted_arr[i] = numerator_bkg / denominator_bkg;
            // Error for aN_bkg
            aN_bkg_err_weighted_arr[i] = 1.0 / sqrt(denominator_bkg);
        } else {
            aN_bkg_weighted_arr[i] = 0;
            aN_bkg_err_weighted_arr[i] = 0;
        }
    }

    //Define arrays for storing systematic errors. I define systematics here as :
    //weibull sys = weibull central val - weighted mean
    //levy sys = levy central val - weighted mean
    //cheb sys = cheb central val - weighted mean
   
     double aN_pi0_sys_weibull_arr[bins], aN_pi0_sys_levy_arr[bins], aN_pi0_sys_cheb_arr[bins];
     double aN_bkg_sys_weibull_arr[bins], aN_bkg_sys_levy_arr[bins], aN_bkg_sys_cheb_arr[bins];

    // Print the results for verification
    std::cout << "\nResults for aN_pi0 and its error:" << std::endl;
    for (int i = 0; i < bins; ++i) {
        //std::cout << "Bin " << i << ": aN_pi0 = " << aN_pi0_weighted_arr[i] << " +/- " << aN_pi0_err_weighted_arr[i] << std::endl;
	aN_pi0_sys_weibull_arr[i] = fabs( aN_pi0_weibull_arr[i] - aN_pi0_weighted_arr[i]); 
        aN_pi0_sys_levy_arr[i] = fabs( aN_pi0_levy_arr[i] - aN_pi0_weighted_arr[i]);   	
        aN_pi0_sys_cheb_arr[i] = fabs( aN_pi0_cheb_arr[i] - aN_pi0_weighted_arr[i]); 
	}

     for (int i = 0; i < bins; ++i) {
        std::cout << "Bin " << i << ": aN_pi0 = " << aN_pi0_weighted_arr[i] << " +/- " << aN_pi0_err_weighted_arr[i] << std::endl;
        cout<<"Weibull pi0 systematics = "<<aN_pi0_sys_weibull_arr[i]<<endl;
        cout<<"Levy pi0 systematics = "<<aN_pi0_sys_levy_arr[i]<<endl;
        cout<<"Cheb pi0 systematics = "<<aN_pi0_sys_cheb_arr[i]<<endl;
	cout<<endl;
        }

    std::cout << "\nResults for aN_bkg and its error:" << std::endl;
    for (int i = 0; i < bins; ++i) {
        //std::cout << "Bin " << i << ": aN_bkg = " << aN_bkg_weighted_arr[i] << " +/- " << aN_bkg_err_weighted_arr[i] << std::endl;
        aN_bkg_sys_weibull_arr[i] = fabs( aN_bkg_weibull_arr[i] - aN_bkg_weighted_arr[i]); 
        aN_bkg_sys_levy_arr[i] = fabs( aN_bkg_levy_arr[i] - aN_bkg_weighted_arr[i]);          
        aN_bkg_sys_cheb_arr[i] = fabs( aN_bkg_cheb_arr[i] - aN_bkg_weighted_arr[i]); 
     }

     for (int i = 0; i < bins; ++i) {
        std::cout << "Bin " << i << ": aN_bkg = " << aN_bkg_weighted_arr[i] << " +/- " << aN_bkg_err_weighted_arr[i] << std::endl;
        cout<<"Weibull bkg systematics = "<<aN_bkg_sys_weibull_arr[i]<<endl;
        cout<<"Levy bkg systematics = "<<aN_bkg_sys_levy_arr[i]<<endl;
        cout<<"Cheb bkg systematics = "<<aN_bkg_sys_cheb_arr[i]<<endl;
        cout<<endl;
        }
	
     double low_err[bins], high_err[bins], optimum_pi0_err[bins], optimum_bkg_err[bins], diff_err[bins];

     for(int i = 0; i < bins; i++){

		low_err[i] = fabs(aN_pi0_weibull_arr[i] - std::min(aN_pi0_levy_arr[i],aN_pi0_cheb_arr[i])) ;
		high_err[i] = fabs(aN_pi0_weibull_arr[i] - std::max(aN_pi0_levy_arr[i],aN_pi0_cheb_arr[i])) ;
		optimum_pi0_err[i] = std::max(aN_pi0_sys_levy_arr[i],aN_pi0_sys_cheb_arr[i]);
		optimum_bkg_err[i] = std::max(aN_bkg_sys_levy_arr[i],aN_bkg_sys_cheb_arr[i]);
		diff_err[i] = fabs( std::max( aN_pi0_weibull_arr[i] - aN_pi0_levy_arr[i], aN_pi0_weibull_arr[i] - aN_pi0_cheb_arr[i]));
	}
	


	std::ofstream outfile_weibull;
        outfile_weibull.open(Form("aN_pi0_bkg_Weibull_%s_sys.txt",method_input.c_str()),std::ios::out);
        for (int i = 0; i < bins; ++i) {
		outfile_weibull<<xF_val_arr[i]<<" "<<aN_pi0_weibull_arr[i]<<" "<<aN_pi0_err_weibull_arr[i]<<" "<<aN_pi0_sys_weibull_arr[i]<<" "<<aN_bkg_weibull_arr[i]<<" "<<aN_bkg_err_weibull_arr[i]<<" "<<aN_bkg_sys_weibull_arr[i]<<endl;
	}
        outfile_weibull.close();


	std::ofstream outfile_levy;
        outfile_levy.open(Form("aN_pi0_bkg_Levy_%s_sys.txt",method_input.c_str()),std::ios::out);
        for (int i = 0; i < bins; ++i) {
                outfile_levy<<xF_val_arr[i]<<" "<<aN_pi0_levy_arr[i]<<" "<<aN_pi0_err_levy_arr[i]<<" "<<aN_pi0_sys_levy_arr[i]<<" "<<aN_bkg_levy_arr[i]<<" "<<aN_bkg_err_levy_arr[i]<<" "<<aN_bkg_sys_levy_arr[i]<<endl;
        }
        outfile_levy.close();


	std::ofstream outfile_cheb;
        outfile_cheb.open(Form("aN_pi0_bkg_Cheb_%s_sys.txt",method_input.c_str()),std::ios::out);
        for (int i = 0; i < bins; ++i) {
                outfile_cheb<<xF_val_arr[i]<<" "<<aN_pi0_cheb_arr[i]<<" "<<aN_pi0_err_cheb_arr[i]<<" "<<aN_pi0_sys_cheb_arr[i]<<" "<<aN_bkg_cheb_arr[i]<<" "<<aN_bkg_err_cheb_arr[i]<<" "<<aN_bkg_sys_cheb_arr[i]<<endl;
        }
        outfile_cheb.close();

	//Plot
       // Create a TCanvas to draw the plots
    TCanvas *c1 = new TCanvas("c1", "aN vs xF", 1000, 500);

    // Create a TGraph for statistical errors.
    // TGraphAsymmErrors takes 6 arrays: x, y, x_err_low, x_err_high, y_err_low, y_err_high
    // Since we don't have x errors, we pass nullptrs. For symmetric y errors, we use the same array.
    TGraphAsymmErrors *gr_stats = new TGraphAsymmErrors(bins, xF_val_arr, aN_bkg_weibull_arr,
                                                        nullptr, nullptr, aN_bkg_err_weibull_arr, aN_bkg_err_weibull_arr);
    
    // Set style for the statistical points and error bars
    gr_stats->SetMarkerStyle(20); // Circle marker
    gr_stats->SetMarkerSize(1.4);
    gr_stats->SetMarkerColor(kBlue);
    gr_stats->SetLineColor(kBlue);
    gr_stats->SetLineWidth(2);

    // Create arrays for the small, non-zero x-error for the systematic boxes
    double x_err_sys_low[bins], x_err_sys_high[bins];
    for (int i = 0; i < bins; ++i) {
        x_err_sys_low[i] = 0.0025; // Fake small x error low
        x_err_sys_high[i] = 0.0025; // Fake small x error high
    }

    // Create a TGraph for systematic errors, which will be shown as gray boxes.
    TGraphAsymmErrors *gr_sys = new TGraphAsymmErrors(bins, xF_val_arr, aN_bkg_weibull_arr,
                                                      x_err_sys_low, x_err_sys_high, optimum_bkg_err, optimum_bkg_err);
  
    
    // Set style for the systematic error boxes
    gr_sys->SetFillColor(kGray);
    gr_sys->SetFillStyle(1001); // A solid fill style
    gr_sys->SetMarkerStyle(20);
    gr_sys->SetMarkerSize(1.2);
    gr_sys->SetLineColor(kBlue);
    
    // Use a TMultiGraph to draw both graphs on the same canvas
    TMultiGraph *mg = new TMultiGraph();
    mg->Add(gr_sys, "2E");  // "2" for box style, "E" for error bars
    mg->Add(gr_stats, "PE"); // "P" for points, "E" for error bars

    // Draw the multigraph
    mg->Draw("AP"); // "A" to draw the axes, "P" to draw the points
    mg->SetTitle("aN_{#pi^{0}}_bkg from Weibull Fit;x_{F};a_{N}"); // Set title and axis labels

    TFile *f = TFile::Open("PreliminaryPlots_BlueBeam.root","UPDATE");
    f->cd();
    mg->Write(Form("bkg_AN_%s",method_input.c_str()));


}
