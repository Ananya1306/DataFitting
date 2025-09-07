#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <fstream>
#include <string>

// ROOT headers for plotting
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TLegend.h"

// Function to calculate the weighted average of a set of values with their errors.
// The weights are based on the inverse of the squared statistical errors.
double calculateWeightedAverage(const std::vector<double>& values, const std::vector<double>& errors) {
    if (values.size() != errors.size() || values.empty()) {
        std::cerr << "Error: Vectors must have the same non-zero size." << std::endl;
        return 0.0;
    }
    double sum_w_x = 0.0;
    double sum_w = 0.0;
    for (size_t i = 0; i < values.size(); ++i) {
        // Ensure error is not zero to avoid division by zero.
        if (errors[i] == 0.0) {
            std::cerr << "Error: Statistical error cannot be zero." << std::endl;
            return 0.0;
        }
        double weight = 1.0 / (errors[i] * errors[i]);
        sum_w_x += weight * values[i];
        sum_w += weight;
    }
    return sum_w_x / sum_w;
}

// Function to calculate the statistical error of the weighted average.
double calculateWeightedError(const std::vector<double>& errors) {
    if (errors.empty()) {
        std::cerr << "Error: Error vector cannot be empty." << std::endl;
        return 0.0;
    }
    double sum_w = 0.0;
    for (const auto& err : errors) {
        if (err == 0.0) {
            std::cerr << "Error: Statistical error cannot be zero." << std::endl;
            return 0.0;
        }
        sum_w += 1.0 / (err * err);
    }
    return std::sqrt(1.0 / sum_w);
}

// Function to calculate the systematic error as the spread (standard deviation) of the values.
double calculateSystematicSpread(const std::vector<double>& values) {
    if (values.size() <= 1) {
        std::cerr << "Error: Need more than one measurement to calculate spread." << std::endl;
        return 0.0;
    }
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    double mean = sum / values.size();
    double sum_sq_diff = 0.0;
    for (const auto& val : values) {
        sum_sq_diff += (val - mean) * (val - mean);
    }
    return std::sqrt(sum_sq_diff / (values.size() - 1));
}

// Function to combine statistical and systematic errors in quadrature.
double combineErrorsInQuadrature(double err1, double err2) {
    return std::sqrt(err1 * err1 + err2 * err2);
}

// Function to read data from a .txt file.
// It also reads the systematic errors for both pi0 and bkg.
bool readDataFromFile(const std::string& filename, std::vector<double>& aN_pi0, std::vector<double>& aN_pi0_stat_err, std::vector<double>& aN_pi0_sys_err, std::vector<double>& aN_bkg, std::vector<double>& aN_bkg_stat_err, std::vector<double>& aN_bkg_sys_err) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    double val1, val2, val3, val4, val5, val6, val7, val8;
    while (file >> val1 >> val2 >> val3 >> val4 >> val5 >> val6 >> val7 >> val8) {
        aN_pi0.push_back(val1);
        aN_pi0_stat_err.push_back(val2);
        aN_pi0_sys_err.push_back(val3); // Read systematic error
        aN_bkg.push_back(val5);
        aN_bkg_stat_err.push_back(val6);
        aN_bkg_sys_err.push_back(val7); // Read systematic error
    }

    file.close();
    return true;
}

// New function to draw the TGraphs with asymmetric errors
void drawGraphs(const std::vector<double>& xF_bins,
                const std::vector<double>& aN_pi0_values,
                const std::vector<double>& aN_pi0_stat_err,
                const std::vector<double>& aN_pi0_sys_err,
                const std::vector<double>& aN_bkg_values,
                const std::vector<double>& aN_bkg_stat_err,
                const std::vector<double>& aN_bkg_sys_err) {
    
    // Set a nice style for the plots
    gStyle->SetOptStat(0);
    gStyle->SetErrorX(0.5); // Use this to control the width of the systematic box

    // Create a TGraphErrors for statistical errors
    TGraphErrors* graph_stat_pi0 = new TGraphErrors(
        xF_bins.size(),
        xF_bins.data(),
        aN_pi0_values.data(),
        nullptr, // x_error_low
        aN_pi0_stat_err.data() // y_error
    );
    graph_stat_pi0->SetTitle("Combined #pi^{0} Asymmetry vs x_{F}");
    graph_stat_pi0->GetXaxis()->SetTitle("x_{F}");
    graph_stat_pi0->GetYaxis()->SetTitle("A_{N}");
    graph_stat_pi0->SetMarkerStyle(20);
    graph_stat_pi0->SetMarkerColor(kRed);
    graph_stat_pi0->SetLineColor(kRed);
    
    // Create a TGraphAsymmErrors for systematic errors (drawn as boxes)
    TGraphAsymmErrors* graph_sys_pi0 = new TGraphAsymmErrors(
        xF_bins.size(),
        xF_bins.data(),
        aN_pi0_values.data(),
        nullptr, // x_error_low (we'll make the box a specific width)
        nullptr, // x_error_high
        aN_pi0_sys_err.data(), // y_error_low (systematic)
        aN_pi0_sys_err.data() // y_error_high (systematic)
    );
    graph_sys_pi0->SetFillColor(kRed);
    graph_sys_pi0->SetFillStyle(3001); // Dotted pattern
    
    // Create a TGraphErrors for background statistical errors
    TGraphErrors* graph_stat_bkg = new TGraphErrors(
        xF_bins.size(),
        xF_bins.data(),
        aN_bkg_values.data(),
        nullptr, // x_error
        aN_bkg_stat_err.data() // y_error
    );
    graph_stat_bkg->SetTitle("Combined Background Asymmetry vs x_{F}");
    graph_stat_bkg->GetXaxis()->SetTitle("x_{F}");
    graph_stat_bkg->GetYaxis()->SetTitle("A_{N}");
    graph_stat_bkg->SetMarkerStyle(21);
    graph_stat_bkg->SetMarkerColor(kBlue);
    graph_stat_bkg->SetLineColor(kBlue);
    
    // Create a TGraphAsymmErrors for background systematic errors (drawn as boxes)
    TGraphAsymmErrors* graph_sys_bkg = new TGraphAsymmErrors(
        xF_bins.size(),
        xF_bins.data(),
        aN_bkg_values.data(),
        nullptr, // x_error_low
        nullptr, // x_error_high
        aN_bkg_sys_err.data(), // y_error_low (systematic)
        aN_bkg_sys_err.data() // y_error_high (systematic)
    );
    graph_sys_bkg->SetFillColor(kBlue);
    graph_sys_bkg->SetFillStyle(3001); // Dotted pattern
    
    // Draw on separate canvases
    TCanvas* c1 = new TCanvas("c1", "pi0 Asymmetry", 800, 600);
    graph_sys_pi0->Draw("E2"); // Draw systematic error boxes first
    graph_stat_pi0->Draw("P SAME"); // Draw statistical error bars on top
    
    // Add a legend
    TLegend* legend1 = new TLegend(0.7, 0.8, 0.9, 0.9);
    legend1->AddEntry(graph_stat_pi0, "#pi^{0} Stat. Err.", "lpe");
    legend1->AddEntry(graph_sys_pi0, "#pi^{0} Syst. Err.", "f");
    legend1->Draw();
    c1->SaveAs("aN_pi0_combined.png");
    
    TCanvas* c2 = new TCanvas("c2", "Background Asymmetry", 800, 600);
    graph_sys_bkg->Draw("E2"); // Draw systematic error boxes first
    graph_stat_bkg->Draw("P SAME"); // Draw statistical error bars on top
    
    // Add a legend
    TLegend* legend2 = new TLegend(0.7, 0.8, 0.9, 0.9);
    legend2->AddEntry(graph_stat_bkg, "Bkg Stat. Err.", "lpe");
    legend2->AddEntry(graph_sys_bkg, "Bkg Syst. Err.", "f");
    legend2->Draw();
    c2->SaveAs("aN_bkg_combined.png");
}

int main(int argc, char** argv) {
    // Initialize ROOT application
    TApplication app("myApp", &argc, argv);
    gROOT->SetBatch(kFALSE); // Set to kTRUE for no GUI

    std::vector<std::string> filenames = {"aN_values_Levy.txt", "aN_values_Weibull.txt", "aN_values_Lognormal.txt"};
    
    // aN_pi0_values[i][j] where i is measurement (Levy, Weibull, Lognormal) and j is bin.
    std::vector<std::vector<double>> aN_pi0_values;
    std::vector<std::vector<double>> aN_pi0_stat_errors;
    std::vector<std::vector<double>> aN_pi0_sys_errors;
    std::vector<std::vector<double>> aN_bkg_values;
    std::vector<std::vector<double>> aN_bkg_stat_errors;
    std::vector<std::vector<double>> aN_bkg_sys_errors;

    // Read data from each file and populate the vectors.
    for (const auto& filename : filenames) {
        std::vector<double> pi0_vals, pi0_stat_errs, pi0_sys_errs, bkg_vals, bkg_stat_errs, bkg_sys_errs;
        if (readDataFromFile(filename, pi0_vals, pi0_stat_errs, pi0_sys_errs, bkg_vals, bkg_stat_errs, bkg_sys_errs)) {
            aN_pi0_values.push_back(pi0_vals);
            aN_pi0_stat_errors.push_back(pi0_stat_errs);
            aN_pi0_sys_errors.push_back(pi0_sys_errs);
            aN_bkg_values.push_back(bkg_vals);
            aN_bkg_stat_errors.push_back(bkg_stat_errs);
            aN_bkg_sys_errors.push_back(bkg_sys_errs);
        }
    }
    
    // Check if data was read successfully.
    if (aN_pi0_values.empty() || aN_pi0_values[0].empty()) {
        std::cerr << "No data was read from files. Exiting." << std::endl;
        return 1;
    }

    const int num_bins = aN_pi0_values[0].size();
    std::vector<double> combined_aN_pi0(num_bins);
    std::vector<double> combined_aN_pi0_stat_err(num_bins);
    std::vector<double> combined_aN_pi0_sys_err(num_bins);
    std::vector<double> combined_aN_pi0_tot_err(num_bins);

    std::vector<double> combined_aN_bkg(num_bins);
    std::vector<double> combined_aN_bkg_stat_err(num_bins);
    std::vector<double> combined_aN_bkg_sys_err(num_bins);
    std::vector<double> combined_aN_bkg_tot_err(num_bins);

    // --- Calculations for each xF bin ---
    for (int i = 0; i < num_bins; ++i) {
        std::cout << "--- Bin " << i << " ---" << std::endl;
        std::cout << std::fixed << std::setprecision(5);

        // Extract values for the current bin
        std::vector<double> pi0_vals_for_bin;
        std::vector<double> pi0_stat_errs_for_bin;
        std::vector<double> pi0_sys_errs_for_bin;
        std::vector<double> bkg_vals_for_bin;
        std::vector<double> bkg_stat_errs_for_bin;
        std::vector<double> bkg_sys_errs_for_bin;

        for (size_t j = 0; j < aN_pi0_values.size(); ++j) {
            pi0_vals_for_bin.push_back(aN_pi0_values[j][i]);
            pi0_stat_errs_for_bin.push_back(aN_pi0_stat_errors[j][i]);
            pi0_sys_errs_for_bin.push_back(aN_pi0_sys_errors[j][i]);
            bkg_vals_for_bin.push_back(aN_bkg_values[j][i]);
            bkg_stat_errs_for_bin.push_back(aN_bkg_stat_errors[j][i]);
            bkg_sys_errs_for_bin.push_back(aN_bkg_sys_errors[j][i]);
        }

        // --- aN_pi0 calculations ---
        combined_aN_pi0[i] = calculateWeightedAverage(pi0_vals_for_bin, pi0_stat_errs_for_bin);
        combined_aN_pi0_stat_err[i] = calculateWeightedError(pi0_stat_errs_for_bin);
        
        // Calculate the systematic error from the spread of values.
        double sys_err_spread_pi0 = calculateSystematicSpread(pi0_vals_for_bin);
        // Calculate the average of the original systematic errors.
        double avg_orig_sys_err_pi0 = std::accumulate(pi0_sys_errs_for_bin.begin(), pi0_sys_errs_for_bin.end(), 0.0) / pi0_sys_errs_for_bin.size();
        // Combine the spread and the average original systematic error in quadrature.
        combined_aN_pi0_sys_err[i] = combineErrorsInQuadrature(sys_err_spread_pi0, avg_orig_sys_err_pi0);
        
        combined_aN_pi0_tot_err[i] = combineErrorsInQuadrature(combined_aN_pi0_stat_err[i], combined_aN_pi0_sys_err[i]);

        std::cout << "Combined aN_pi0 value: " << combined_aN_pi0[i] << std::endl;
        std::cout << "Statistical error: " << combined_aN_pi0_stat_err[i] << std::endl;
        std::cout << "Systematic error: " << combined_aN_pi0_sys_err[i] << std::endl;
        std::cout << "Total error: " << combined_aN_pi0_tot_err[i] << std::endl;
        std::cout << std::endl;

        // --- aN_bkg calculations ---
        combined_aN_bkg[i] = calculateWeightedAverage(bkg_vals_for_bin, bkg_stat_errs_for_bin);
        combined_aN_bkg_stat_err[i] = calculateWeightedError(bkg_stat_errs_for_bin);

        // Calculate the systematic error from the spread of values.
        double sys_err_spread_bkg = calculateSystematicSpread(bkg_vals_for_bin);
        // Calculate the average of the original systematic errors.
        double avg_orig_sys_err_bkg = std::accumulate(bkg_sys_errs_for_bin.begin(), bkg_sys_errs_for_bin.end(), 0.0) / bkg_sys_errs_for_bin.size();
        // Combine the spread and the average original systematic error in quadrature.
        combined_aN_bkg_sys_err[i] = combineErrorsInQuadrature(sys_err_spread_bkg, avg_orig_sys_err_bkg);

        combined_aN_bkg_tot_err[i] = combineErrorsInQuadrature(combined_aN_bkg_stat_err[i], combined_aN_bkg_sys_err[i]);

        std::cout << "Combined aN_bkg value: " << combined_aN_bkg[i] << std::endl;
        std::cout << "Statistical error: " << combined_aN_bkg_stat_err[i] << std::endl;
        std::cout << "Systematic error: " << combined_aN_bkg_sys_err[i] << std::endl;
        std::cout << "Total error: " << combined_aN_bkg_tot_err[i] << std::endl;
        std::cout << std::endl;
    }
    
    // --- Plotting with ROOT ---
    // You'll need to define your xF bin centers here, as they are not in the input files.
    // Example xF bins:
    std::vector<double> xF_bins = {0.15, 0.25, 0.35, 0.45, 0.55};
    
    if (xF_bins.size() != num_bins) {
        std::cerr << "Error: The number of defined xF bins does not match the data bins." << std::endl;
        return 1;
    }

    drawGraphs(xF_bins,
               combined_aN_pi0,
               combined_aN_pi0_stat_err,
               combined_aN_pi0_sys_err,
               combined_aN_bkg,
               combined_aN_bkg_stat_err,
               combined_aN_bkg_sys_err);

    // Keep the ROOT application running to display the canvases.
    app.Run();
    
    return 0;
}

