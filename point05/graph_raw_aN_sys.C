#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

// ROOT Headers
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TAxis.h"

// Function to create a TGraphAsymmErrors plot
void plotter(
    const std::vector<double>& aN_crossRatio,
    const std::vector<double>& aN_crossRatio_err,
    const std::vector<double>& aN_relLum,
    const std::vector<double>& aN_relLum_err,
    const std::vector<double>& xF_val_crossRatio,
    double R_sys, int range_input, string beam_input, string region_input) {

    // Set a consistent plot style
    gStyle->SetOptStat(0);
    gStyle->SetTitleOffset(1.4, "y");
    gStyle->SetPalette(55);

	cout<<"Plotter function "<<beam_input<<" Beam with R_sys = "<<R_sys<<endl;

    // Number of data points
    size_t nPoints = aN_crossRatio.size();

    // Vectors for the combined values and uncertainties
    std::vector<double> x(nPoints);
    std::vector<double> y_stat_points(nPoints);
    std::vector<double> y_sys_points(nPoints);
    std::vector<double> y_errors_stat(nPoints);
    std::vector<double> y_errors_sys(nPoints); // New vector for point-by-point systematic uncertainty

   // Open an output file to store the data
    std::ofstream outfile(Form("sig_sb_range%d/%sBeam/aN_raw_sys_bothMethods_%s.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::out);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open combined_data.txt for writing." << std::endl;
        return;
    }

    // Fill the vectors for the TGraph
    for (size_t i = 0; i < nPoints; ++i) {
        // Calculate combined value
        double w_cross = 1.0 / std::pow(aN_crossRatio_err[i], 2);
        double w_rel = 1.0 / std::pow(aN_relLum_err[i], 2);
        double combined_value = (w_cross * aN_crossRatio[i] + w_rel * aN_relLum[i]) / (w_cross + w_rel);


        // Calculate combined statistical uncertainty
        double combined_stat_err = 1.0 / std::sqrt(w_cross + w_rel);

        x[i] = xF_val_crossRatio[i];  //or xF_val_relLum[i], doesn't matter;
        y_stat_points[i] = combined_value;
        y_errors_stat[i] = combined_stat_err;
        // Systematic points are the same as statistical points
        y_sys_points[i] = combined_value;
        // --- NEW LOGIC: Calculate point-by-point systematic uncertainty ---
        y_errors_sys[i] = std::abs(aN_crossRatio[i] - aN_relLum[i]);
        // We are ignoring R_sys for this calculation as requested.
        // -----------------------------------------------------------------

        // Print values per bin
        std::cout << "Data Point " << i << " (xF bin " << x[i] << "):"
                  << " Combined Value = " << combined_value
                  << ", Combined Statistical Error = " << combined_stat_err
                  << ", Point-by-point Systematic Uncertainty = " << y_errors_sys[i] << std::endl;
	outfile << x[i] << " " << combined_value << " " << combined_stat_err << " " << y_errors_sys[i] << std::endl;

    }
	int marker_color; int marker_style;
	if(beam_input == "Blue"){marker_color = 600;}
	else if(beam_input == "Yellow"){marker_color = 807;}
	if(region_input == "sig"){marker_style = 20;}
	else if(region_input == "sb"){marker_style = 24;}

    // Create the statistical uncertainty TGraph
    TGraphAsymmErrors* stat_graph = new TGraphAsymmErrors(nPoints, x.data(), y_stat_points.data(), 0, 0, y_errors_stat.data(), y_errors_stat.data());
    stat_graph->SetMarkerStyle(marker_style);
    stat_graph->SetMarkerSize(1.2);
    stat_graph->SetMarkerColor(marker_color);
    stat_graph->SetLineColor(marker_color);
    stat_graph->SetTitle("Combined aN Values with Statistical and Systematic Errors");
    stat_graph->GetXaxis()->SetTitle("Data Bin");
    stat_graph->GetYaxis()->SetTitle("Raw A_{N}");

    // Define a small fake x-error to create a systematic error box
    double fake_x_error = 0.003;
    
    // Create the systematic uncertainty TGraph, now using the new point-by-point errors
    TGraphAsymmErrors* sys_graph = new TGraphAsymmErrors(nPoints, x.data(), y_sys_points.data(),
                                                          std::vector<double>(nPoints, fake_x_error).data(),
                                                          std::vector<double>(nPoints, fake_x_error).data(),
                                                          y_errors_sys.data(),
                                                          y_errors_sys.data());
    sys_graph->SetFillStyle(3001);
    sys_graph->SetFillColor(kGray);
    sys_graph->SetTitle("");

    // Create a canvas and draw the graphs
    TCanvas* canvas = new TCanvas();
    
    // Dynamically adjust the Y-axis range
    double y_min = std::numeric_limits<double>::max();
    double y_max = std::numeric_limits<double>::lowest();
    for (size_t i = 0; i < nPoints; ++i) {
        y_min = std::min(y_min, y_stat_points[i] - y_errors_stat[i]);
        y_max = std::max(y_max, y_stat_points[i] + y_errors_stat[i]);
    }
    // Add a buffer to the range
    double y_buffer = (y_max - y_min) * 0.1;
    if (y_buffer == 0) y_buffer = 0.05; // Default buffer for single-point or zero-range data
    sys_graph->GetYaxis()->SetRangeUser(y_min - y_buffer, y_max + y_buffer);

    sys_graph->Draw("a2"); // Draw systematic uncertainty band first
    stat_graph->Draw("p same"); // Overlay statistical points with errors

    //Adding these to a MultiGraph
    TMultiGraph* mg = new TMultiGraph();
    mg->SetTitle(Form("Raw AN for %s Beam %s region; A_{N}^{raw} combined; xF",beam_input.c_str(),region_input.c_str()));
    mg->Add(sys_graph, "A2"); //sys_graph->SetTitle("systematic error");
    mg->Add(stat_graph,"P"); stat_graph->SetTitle("Combined Raw AN");
    //mg->Add(sys_graph, "A2"); //sys_graph->SetTitle("systematic error");
    TCanvas *c2 = new TCanvas();
    mg->Draw("a"); c2->BuildLegend();

    // Draw the horizontal line at y=0
    TLine* line = new TLine(mg->GetHistogram()->GetXaxis()->GetXmin(), 0, mg->GetHistogram()->GetXaxis()->GetXmax(), 0);
    line->SetLineColor(kBlack);
    line->Draw("same");


    sys_graph->Draw("a2"); // Draw systematic uncertainty band first
    stat_graph->Draw("p same"); // Overlay statistical points with errors

    // Update the canvas
    canvas->Update();
    //canvas->SaveAs("aN_plot.png");

    TFile *f = TFile::Open(Form("aN_all_range%d.root",range_input),"UPDATE");
    f->cd();
    mg->Write(Form("graph_raw_aN_%sBeam_bothMethods_%s_sys",beam_input.c_str(),region_input.c_str()));

    // Clean up
    //delete stat_graph;
    //delete sys_graph;
    //delete canvas;
}

void graph_raw_aN_sys(int range_input, string beam_input, string region_input) {
    // Example data
    /*std::vector<double> xF_val_crossRatio = {1,2,3,4}; std::vector<double> xF_val_relLum = {1,2,3,4};
    std::vector<double> aN_crossRatio = {0.10, 0.12, 0.11, 0.13, 0.14};
    std::vector<double> aN_crossRatio_err = {0.01, 0.015, 0.009, 0.012, 0.011};
    std::vector<double> aN_relLum = {0.09, 0.11, 0.10, 0.11, 0.12};
    std::vector<double> aN_relLum_err = {0.008, 0.013, 0.010, 0.009, 0.010};
    */

    std::vector<double> xF_val_crossRatio; std::vector<double> xF_val_relLum;
    std::vector<double> aN_crossRatio; std::vector<double> aN_relLum;
    std::vector<double> aN_crossRatio_err; std::vector<double> aN_relLum_err;

    //Relative Luminosty and polarisation values
	const double pol_blue = 0.5785;
        const double pol_yellow = 0.5872;
        double pol; double R_sys;
        const double R_sys_blue = 0.035848;
        const double R_sys_yellow = 0.0276967;

	if(beam_input == "Blue"){pol = pol_blue; R_sys = R_sys_blue;}
        else if(beam_input == "Yellow"){pol = pol_yellow; R_sys = R_sys_yellow;}

	cout<<"CrossRatio input"<<endl;
        int n0 = 0;
        fstream file0;
        file0.open(Form("sig_sb_range%d/%sBeam/aN_mod_crossRatio_%s_goodChi2.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::in); //test2 is for experimentation
        double xF_val, aN_phi, aN_phi_err, aN_cosphi, aN_cosphi_err;
	while(file0>>xF_val>>aN_phi>>aN_phi_err>>aN_cosphi>>aN_cosphi_err){	
		xF_val_crossRatio.push_back(xF_val);
		aN_crossRatio.push_back(aN_phi);
		aN_crossRatio_err.push_back(aN_phi_err); //Includes polarisation now
		//cout<<"aN_phi = "<<aN_phi<<" aN_phi/pol = "<<aN_phi/pol<<endl;
	}
	file0.close(); 

	cout<<"RelLum input"<<endl;
        int n1 = 0;
        fstream file1;
        file1.open(Form("sig_sb_range%d/%sBeam/aN_mod_relLum_%s_goodChi2.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::in); //test2 is for experimentation
        double xF_val1, aN_phi1, aN_phi_err1, aN_cosphi1, aN_cosphi_err1;
        while(file1>>xF_val1>>aN_phi1>>aN_phi_err1>>aN_cosphi1>>aN_cosphi_err1){
                xF_val_relLum.push_back(xF_val1);
                aN_relLum.push_back(aN_phi1);
                aN_relLum_err.push_back(aN_phi_err1);
        }
        file1.close();

    // Call the plotting function
    plotter(aN_crossRatio, aN_crossRatio_err, aN_relLum, aN_relLum_err, xF_val_crossRatio, R_sys, range_input, beam_input, region_input);

    //return 0;
}

