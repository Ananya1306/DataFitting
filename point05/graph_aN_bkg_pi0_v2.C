#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <string>

// ROOT Headers for plotting
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TLine.h"

// A struct to hold the combined results for each data point
struct CombinedResult {
    double average_value;
    double combined_statistical_error;
    double systematic_error;
    double total_error;
};

// Function to calculate the average value, combined statistical error,
// and systematic error for a set of three measurements per bin.
// This function takes a set of three measurement vectors and their
// corresponding statistical uncertainty vectors.
void calculate_total_uncertainty(
    const std::vector<double>& raw_values,
    const std::vector<double>& raw_errors,
    const std::vector<double>& levy_values,
    const std::vector<double>& levy_errors,
    const std::vector<double>& weibull_values,
    const std::vector<double>& weibull_errors,
    std::vector<CombinedResult>& results) {

    // Ensure all input vectors have the same size.
    size_t nPoints = raw_values.size();
    if (nPoints != levy_values.size() || nPoints != weibull_values.size()) {
        std::cerr << "Error: Input vectors must have the same size." << std::endl;
        return;
    }

    // Resize the results vector to match the number of points
    results.resize(nPoints);

    // Loop through each data point (or bin)
    for (size_t i = 0; i < nPoints; ++i) {
        // Step 1: Calculate the average value of the three measurements
        double average_value = (raw_values[i] + levy_values[i] + weibull_values[i]) / 3.0;

        // Step 2: Calculate the combined statistical uncertainty in quadrature
        double combined_stat_err_sq = std::pow(raw_errors[i], 2) +
                                      std::pow(levy_errors[i], 2) +
                                      std::pow(weibull_errors[i], 2);
        double combined_stat_err = std::sqrt(combined_stat_err_sq);

        // Step 3: Calculate the systematic uncertainty (standard deviation of the three values)
        double sq_diff_tot = std::pow(raw_values[i] - average_value, 2) +
                             std::pow(levy_values[i] - average_value, 2) +
                             std::pow(weibull_values[i] - average_value, 2);
        // We use (3 - 1) for the degrees of freedom since we have 3 data points
        double systematic_err = std::sqrt(sq_diff_tot / 2.0);

        // Step 4: Calculate the total uncertainty (quadrature sum of statistical and systematic)
        double total_err_sq = std::pow(combined_stat_err, 2) + std::pow(systematic_err, 2);
        double total_err = std::sqrt(total_err_sq);

        // Store the results in the struct
        results[i] = {average_value, combined_stat_err, systematic_err, total_err};
    }
}

// Function to create and draw a plot using TGraphAsymmErrors
void plotter(const std::vector<double>& x_vals, const std::vector<CombinedResult>& results) {
    // Set a consistent plot style
    gStyle->SetOptStat(0);
    gStyle->SetTitleOffset(1.4, "y");
    gStyle->SetPalette(55);

    size_t nPoints = x_vals.size();
    if (nPoints == 0) {
        std::cerr << "Error: No data points to plot." << std::endl;
        return;
    }

    // Create vectors for the TGraph objects
    std::vector<double> y_stat_points(nPoints);
    std::vector<double> y_errors_stat(nPoints);
    std::vector<double> y_errors_sys(nPoints);

    // Populate the vectors from the CombinedResult struct
    for (size_t i = 0; i < nPoints; ++i) {
        y_stat_points[i] = results[i].average_value;
        y_errors_stat[i] = results[i].combined_statistical_error;
        y_errors_sys[i] = results[i].systematic_error;
    }

    // Create the systematic uncertainty TGraph, which will be a shaded band
    TGraphAsymmErrors* sys_graph = new TGraphAsymmErrors(
        nPoints,
        x_vals.data(),
        y_stat_points.data(),
        std::vector<double>(nPoints, 0.002).data(), // Small fake x-error to create a box
        std::vector<double>(nPoints, 0.002).data(), // Small fake x-error
        y_errors_sys.data(),
        y_errors_sys.data()
    );
    sys_graph->SetFillStyle(3001);
    sys_graph->SetFillColor(kGray);
    sys_graph->SetTitle("Combined Asymmetry with Statistical and Systematic Errors; xF; A_{N}");

    // Create the statistical uncertainty TGraph
    TGraphAsymmErrors* stat_graph = new TGraphAsymmErrors(
        nPoints,
        x_vals.data(),
        y_stat_points.data(),
        0, 0,
        y_errors_stat.data(),
        y_errors_stat.data()
    );
    stat_graph->SetMarkerStyle(20);
    stat_graph->SetMarkerSize(1.2);
    stat_graph->SetMarkerColor(kBlack);
    stat_graph->SetLineColor(kBlack);
    
    // Create a canvas and draw the graphs
    TCanvas* canvas = new TCanvas("c1", "Combined A_{N} Plot", 800, 600);
    
    // Set up the plot aesthetics
    sys_graph->GetYaxis()->SetRangeUser(-0.2, 0.2); // Adjust y-axis range
    sys_graph->Draw("a2"); // Draw systematic uncertainty band first
    stat_graph->Draw("p same"); // Overlay statistical points with errors

    // Draw the horizontal line at y=0
    TLine* line = new TLine(sys_graph->GetXaxis()->GetXmin(), 0, sys_graph->GetXaxis()->GetXmax(), 0);
    line->SetLineStyle(2); // Dotted line
    line->SetLineColor(kRed);
    line->Draw("same");

    // Create a legend
    TLegend* legend = new TLegend(0.7, 0.8, 0.9, 0.9);
    legend->AddEntry(stat_graph, "Statistical Error", "lep");
    legend->AddEntry(sys_graph, "Systematic Error", "f");
    legend->Draw();

    canvas->Update();
    // In a real application, you might want to save the canvas here.
    // canvas->SaveAs("combined_aN_plot.png");
}

void graph_aN_bkg_pi0_v2() {
    // Sample data to demonstrate the function
    std::vector<double> xF_bins = {0.1, 0.2, 0.3, 0.4};

    // aN_raw values and their statistical errors
    std::vector<double> aN_raw = {0.02, 0.05, 0.08, 0.11};
    std::vector<double> aN_raw_err = {0.005, 0.006, 0.007, 0.008};

    // aN_pi0_levy values and their statistical errors
    std::vector<double> aN_pi0_levy = {0.025, 0.052, 0.078, 0.105};
    std::vector<double> aN_pi0_levy_err = {0.004, 0.005, 0.006, 0.007};

    // aN_pi0_weibull values and their statistical errors
    std::vector<double> aN_pi0_weibull = {0.021, 0.048, 0.082, 0.115};
    std::vector<double> aN_pi0_weibull_err = {0.003, 0.005, 0.007, 0.009};

    std::vector<CombinedResult> final_results;

    // Call the function to perform the calculations
    calculate_total_uncertainty(
        aN_raw, aN_raw_err,
        aN_pi0_levy, aN_pi0_levy_err,
        aN_pi0_weibull, aN_pi0_weibull_err,
        final_results);

    // Print the results for each data point
    for (size_t i = 0; i < final_results.size(); ++i) {
        std::cout << "xF Bin " << xF_bins[i] << ":" << std::endl;
        std::cout << "  Average Asymmetry: " << final_results[i].average_value << std::endl;
        std::cout << "  Combined Statistical Error: " << final_results[i].combined_statistical_error << std::endl;
        std::cout << "  Systematic Error (Model Dependence): " << final_results[i].systematic_error << std::endl;
        std::cout << "  Total Uncertainty: " << final_results[i].total_error << std::endl;
        std::cout << std::endl;
    }
    
    // Call the new plotting function
    plotter(xF_bins, final_results);

//    return 0;
}

