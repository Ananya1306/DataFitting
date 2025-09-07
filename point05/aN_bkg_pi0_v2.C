//This code also calculates the sytematics

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

// A struct to hold the results for each data point
struct DerivedResult {
    double pi0_asymmetry;
    double bkg_asymmetry;
    double pi0_stat_error;
    double bkg_stat_error;
    double pi0_sys_error;
    double bkg_sys_error;
    double pi0_total_error;
    double bkg_total_error;
};


// Function to calculate derived asymmetries and their uncertainties
// for a set of measurements.
std::vector<DerivedResult> calculate_derived_asymmetries(
    const std::vector<double>& aN_raw_sig,
    const std::vector<double>& aN_raw_sb,
    const std::vector<double>& aN_raw_sig_err,
    const std::vector<double>& aN_raw_sb_err,
    const std::vector<double>& aN_raw_sig_sys,
    const std::vector<double>& aN_raw_sb_sys,
    const std::vector<double>& f_sig_sig,
    const std::vector<double>& f_sig_sb) {
    
    // Check for consistent vector sizes
    size_t nPoints = aN_raw_sig.size();
    if (nPoints != aN_raw_sb.size() || nPoints != aN_raw_sig_err.size() ||
        nPoints != aN_raw_sb_err.size() || nPoints != aN_raw_sig_sys.size() ||
        nPoints != aN_raw_sb_sys.size() || nPoints != f_sig_sig.size() ||
        nPoints != f_sig_sb.size()) {
        std::cerr << "Error: All input vectors must have the same size." << std::endl;
        return {};
    }

    std::vector<DerivedResult> results(nPoints);

    // Loop through each data point (i)
    for (size_t i = 0; i < nPoints; ++i) {
        // Calculate the denominator, which is common to all formulas
        double denominator = f_sig_sig[i] - f_sig_sb[i];
        
        // Handle division by zero
        if (std::abs(denominator) < 1e-9) {
            std::cerr << "Warning: Denominator is zero or near-zero for point " << i << ". Skipping calculation." << std::endl;
            results[i] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
            continue;
        }

        // Calculate the derived pi0 asymmetry
        double pi0_asymmetry_val = (aN_raw_sig[i] * (1.0 - f_sig_sb[i]) - aN_raw_sb[i] * (1.0 - f_sig_sig[i])) / denominator;

        // Calculate the derived background asymmetry
        double bkg_asymmetry_val = (aN_raw_sb[i] * f_sig_sig[i] - aN_raw_sig[i] * f_sig_sb[i]) / denominator;

        // Calculate the statistical error for pi0 asymmetry using error propagation
        double pi0_stat_err_sq = std::pow(1.0 - f_sig_sb[i], 2) * std::pow(aN_raw_sig_err[i], 2) +
                                 std::pow(1.0 - f_sig_sig[i], 2) * std::pow(aN_raw_sb_err[i], 2);
        double pi0_stat_err_val = std::sqrt(pi0_stat_err_sq) / std::abs(denominator);

        // Calculate the statistical error for background asymmetry using error propagation
        double bkg_stat_err_sq = std::pow(f_sig_sb[i], 2) * std::pow(aN_raw_sig_err[i], 2) +
                                 std::pow(f_sig_sig[i], 2) * std::pow(aN_raw_sb_err[i], 2);
        double bkg_stat_err_val = std::sqrt(bkg_stat_err_sq) / std::abs(denominator);

        // Calculate the systematic error for pi0 asymmetry
        double pi0_sys_err_sq = std::pow(1.0 - f_sig_sb[i], 2) * std::pow(aN_raw_sig_sys[i], 2) +
                                std::pow(1.0 - f_sig_sig[i], 2) * std::pow(aN_raw_sb_sys[i], 2);
        double pi0_sys_err_val = std::sqrt(pi0_sys_err_sq) / std::abs(denominator);

        // Calculate the systematic error for background asymmetry
        double bkg_sys_err_sq = std::pow(f_sig_sb[i], 2) * std::pow(aN_raw_sig_sys[i], 2) +
                                std::pow(f_sig_sig[i], 2) * std::pow(aN_raw_sb_sys[i], 2);
        double bkg_sys_err_val = std::sqrt(bkg_sys_err_sq) / std::abs(denominator);

        // Calculate the total error by combining statistical and systematic in quadrature
        double pi0_total_err = std::sqrt(std::pow(pi0_stat_err_val, 2) + std::pow(pi0_sys_err_val, 2));
        double bkg_total_err = std::sqrt(std::pow(bkg_stat_err_val, 2) + std::pow(bkg_sys_err_val, 2));

        // Store the results
        results[i] = {
            pi0_asymmetry_val,
            bkg_asymmetry_val,
            pi0_stat_err_val,
            bkg_stat_err_val,
            pi0_sys_err_val,
            bkg_sys_err_val,
            pi0_total_err,
            bkg_total_err
        };
    }

    return results;
}

// Function to create and draw plots on two separate canvases
void plotter(const std::vector<double>& x_vals, const std::vector<DerivedResult>& results, int range_input, string beam_input, string func_input) {
    gStyle->SetOptStat(0);
    gStyle->SetTitleOffset(1.4, "y");
    
    int marker_color;
    if(beam_input == "Blue"){marker_color = 600;}
        else if(beam_input == "Yellow"){marker_color = 807;}

    size_t nPoints = x_vals.size();
    if (nPoints == 0) {
        std::cerr << "Error: No data points to plot." << std::endl;
        return;
    }

    // Prepare data for pi0 plot
    std::vector<double> pi0_y_points(nPoints);
    std::vector<double> pi0_y_errors_stat(nPoints);
    std::vector<double> pi0_y_errors_sys(nPoints);

    for (size_t i = 0; i < nPoints; ++i) {
        pi0_y_points[i] = results[i].pi0_asymmetry;
        pi0_y_errors_stat[i] = results[i].pi0_stat_error;
        pi0_y_errors_sys[i] = results[i].pi0_sys_error;
    }
    
    // Prepare data for bkg plot
    std::vector<double> bkg_y_points(nPoints);
    std::vector<double> bkg_y_errors_stat(nPoints);
    std::vector<double> bkg_y_errors_sys(nPoints);

    for (size_t i = 0; i < nPoints; ++i) {
        bkg_y_points[i] = results[i].bkg_asymmetry;
        bkg_y_errors_stat[i] = results[i].bkg_stat_error;
        bkg_y_errors_sys[i] = results[i].bkg_sys_error;
    }

    double fake_x_error = 0.003;
    
    // --- Create and draw the pi0 plot ---
    
    TCanvas* c_pi0 = new TCanvas("c_pi0_asymmetry", "Pi0 Asymmetry Plot", 800, 600);
    
    // Systematic uncertainty graph for pi0
    TGraphAsymmErrors* pi0_sys_graph = new TGraphAsymmErrors(
        nPoints, x_vals.data(), pi0_y_points.data(),
        std::vector<double>(nPoints, fake_x_error).data(), std::vector<double>(nPoints, fake_x_error).data(),
        pi0_y_errors_sys.data(), pi0_y_errors_sys.data()
    );
    pi0_sys_graph->SetFillStyle(3001);
    pi0_sys_graph->SetFillColor(kGray);
    pi0_sys_graph->SetTitle("#pi^{0} Asymmetry; xF; A_{N}");

    // Statistical uncertainty graph for pi0
    TGraphAsymmErrors* pi0_stat_graph = new TGraphAsymmErrors(
        nPoints, x_vals.data(), pi0_y_points.data(),
        std::vector<double>(nPoints, 0.0).data(), std::vector<double>(nPoints, 0.0).data(),
        pi0_y_errors_stat.data(), pi0_y_errors_stat.data()
    );
    pi0_stat_graph->SetMarkerStyle(20);
    pi0_stat_graph->SetMarkerSize(1.2);
    pi0_stat_graph->SetMarkerColor(marker_color);
    pi0_stat_graph->SetLineColor(marker_color);

        // Dynamically adjust the Y-axis range for the pi0 plot
    double pi0_y_min = std::numeric_limits<double>::max();
    double pi0_y_max = std::numeric_limits<double>::lowest();
    for (size_t i = 0; i < nPoints; ++i) {
        pi0_y_min = std::min(pi0_y_min, pi0_y_points[i] - pi0_y_errors_stat[i]);
        pi0_y_max = std::max(pi0_y_max, pi0_y_points[i] + pi0_y_errors_stat[i]);
    }
    double pi0_y_buffer = (pi0_y_max - pi0_y_min) * 0.1;
    if (pi0_y_buffer == 0) pi0_y_buffer = 0.05;
    pi0_sys_graph->GetYaxis()->SetRangeUser(pi0_y_min - pi0_y_buffer, pi0_y_max + pi0_y_buffer);


    pi0_sys_graph->Draw("a2");
    pi0_stat_graph->Draw("p same");
    c_pi0->Update();

    TMultiGraph* mg1 = new TMultiGraph();
    mg1->SetTitle("#pi^{0} asymmetry; xF ; #pi^{0} #A_{N}");
    mg1->Add(pi0_sys_graph, "A2"); pi0_sys_graph->SetTitle("Sys Err");
    mg1->Add(pi0_stat_graph,"P"); pi0_stat_graph->SetTitle("#pi^{0} A_{N}");
    //mg1->Add(pi0_sys_graph, "A2");
    TCanvas *c1 = new TCanvas();
    mg1->Draw("a"); c1->BuildLegend();


    // --- Create and draw the bkg plot ---
    
    TCanvas* c_bkg = new TCanvas("c_bkg_asymmetry", "Bkg Asymmetry Plot", 800, 600);

    // Systematic uncertainty graph for bkg
    TGraphAsymmErrors* bkg_sys_graph = new TGraphAsymmErrors(
        nPoints, x_vals.data(), bkg_y_points.data(),
        std::vector<double>(nPoints, fake_x_error).data(), std::vector<double>(nPoints, fake_x_error).data(),
        bkg_y_errors_sys.data(), bkg_y_errors_sys.data()
    );
    bkg_sys_graph->SetFillStyle(3001);
    bkg_sys_graph->SetFillColor(kGray);
    bkg_sys_graph->SetTitle("Background Asymmetry; xF; A_{N}");
    
    // Statistical uncertainty graph for bkg
    TGraphAsymmErrors* bkg_stat_graph = new TGraphAsymmErrors(
        nPoints, x_vals.data(), bkg_y_points.data(),
        std::vector<double>(nPoints, 0.0).data(), std::vector<double>(nPoints, 0.0).data(),
        bkg_y_errors_stat.data(), bkg_y_errors_stat.data()
    );
    bkg_stat_graph->SetMarkerStyle(24);
    bkg_stat_graph->SetMarkerSize(1.2);
    bkg_stat_graph->SetMarkerColor(marker_color);
    bkg_stat_graph->SetLineColor(marker_color);

    // Dynamically adjust the Y-axis range for the bkg plot
    double bkg_y_min = std::numeric_limits<double>::max();
    double bkg_y_max = std::numeric_limits<double>::lowest();
    for (size_t i = 0; i < nPoints; ++i) {
        bkg_y_min = std::min(bkg_y_min, bkg_y_points[i] - bkg_y_errors_stat[i]);
        bkg_y_max = std::max(bkg_y_max, bkg_y_points[i] + bkg_y_errors_stat[i]);
    }
    double bkg_y_buffer = (bkg_y_max - bkg_y_min) * 0.1;
    if (bkg_y_buffer == 0) bkg_y_buffer = 0.05;
    bkg_sys_graph->GetYaxis()->SetRangeUser(bkg_y_min - bkg_y_buffer, bkg_y_max + bkg_y_buffer);

    bkg_sys_graph->Draw("a2");
    bkg_stat_graph->Draw("p same");
    
    c_bkg->Update();
  
    TMultiGraph* mg2 = new TMultiGraph();
    mg2->SetTitle("Bg asymmetry; xF; Bg #A_{N}");
    mg2->Add(bkg_sys_graph, "A2"); bkg_sys_graph->SetTitle("Sys Err");
    mg2->Add(bkg_stat_graph,"P"); bkg_stat_graph->SetTitle("Bg A_{N}");
    //mg2->Add(bkg_sys_graph, "A2");
    TCanvas *c2 = new TCanvas();
    mg2->Draw("a"); c2->BuildLegend();

    TFile *f = TFile::Open(Form("aN_all_range%d.root",range_input),"UPDATE");
    f->cd();
    mg1->Write(Form("graph_pi0_aN_%sBeam_%s_sys",beam_input.c_str(), func_input.c_str()));
    mg2->Write(Form("graph_bkg_aN_%sBeam_%s_sys",beam_input.c_str(), func_input.c_str()));

}

void aN_bkg_pi0_v2(int range_input, string beam_input, string func_input) {

    std::vector<double> xF_bins;
    std::vector<double> aN_raw_sig;
    std::vector<double> aN_raw_sb;
    std::vector<double> aN_raw_sig_err;
    std::vector<double> aN_raw_sb_err;
    std::vector<double> aN_raw_sig_sys;
    std::vector<double> aN_raw_sb_sys;
    std::vector<double> f_sig_sig;
    std::vector<double> f_sig_sb;

    cout<<"Combined value of crossRatio and relLum"<<endl;
    int n0 = 0; fstream file0;
    file0.open(Form("sig_sb_range%d/%sBeam/aN_raw_sys_bothMethods_sig.txt",range_input,beam_input.c_str()),std::ios::in);
    double xF_val0, aN_raw_sig0, aN_raw_sig_err0, aN_raw_sig_sys0;
    while(file0>>xF_val0>>aN_raw_sig0>>aN_raw_sig_err0>>aN_raw_sig_sys0){
		xF_bins.push_back(xF_val0);
		aN_raw_sig.push_back(aN_raw_sig0);
		aN_raw_sig_err.push_back(aN_raw_sig_err0);
		aN_raw_sig_sys.push_back(aN_raw_sig_sys0);
		cout<<xF_val0<<" "<<aN_raw_sig0<<" "<<aN_raw_sig_err0<<" "<<aN_raw_sig_sys0<<endl;
	}
	file0.close();

    cout<<"Combined value of crossRatio and relLum"<<endl;
    int n1 = 0; fstream file1;
    file1.open(Form("sig_sb_range%d/%sBeam/aN_raw_sys_bothMethods_sb.txt",range_input,beam_input.c_str()),std::ios::in);
    double xF_val1, aN_raw_sb0, aN_raw_sb_err0, aN_raw_sb_sys0;
    while(file1>>xF_val1>>aN_raw_sb0>>aN_raw_sb_err0>>aN_raw_sb_sys0){
                aN_raw_sb.push_back(aN_raw_sb0);
                aN_raw_sb_err.push_back(aN_raw_sb_err0);
                aN_raw_sb_sys.push_back(aN_raw_sb_sys0);
		cout<<xF_val1<<" "<<aN_raw_sb0<<" "<<aN_raw_sb_err0<<" "<<aN_raw_sb_sys0<<endl;
        }
	file1.close();

     cout<<"fractions from function"<<endl;
     int n2 = 0; fstream file2;
     file2.open(Form("sig_sb_range%d/%sBeam/fSig_%s.txt",range_input,beam_input.c_str(),func_input.c_str()),std::ios::in);
     double f_sig_sig0, f_sig_sb0;
     while(file2>>f_sig_sig0>>f_sig_sb0){
		f_sig_sig.push_back(f_sig_sig0);
		f_sig_sb.push_back(f_sig_sb0);
		cout<<f_sig_sig0<<" "<<f_sig_sb0<<endl;
     }
     file2.close(); 

    // Perform the calculations
    std::vector<DerivedResult> results = calculate_derived_asymmetries(
        aN_raw_sig, aN_raw_sb, aN_raw_sig_err, aN_raw_sb_err, aN_raw_sig_sys, aN_raw_sb_sys, f_sig_sig, f_sig_sb
    );

    std::ofstream outfile(Form("sig_sb_range%d/%sBeam/aN_values_%s.txt",range_input,beam_input.c_str(),func_input.c_str()),std::ios::out);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open combined_data.txt for writing." << std::endl;
        return;
    }

    // Print the results for each point
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "Data Point " << i << ":" << std::endl;
	std::cout << "xF val :                 " << xF_bins[i] << std::endl;
        std::cout << "  Derived pi0 Asymmetry: " << results[i].pi0_asymmetry << std::endl;
        std::cout << "    - Statistical Error: " << results[i].pi0_stat_error << std::endl;
        std::cout << "    - Systematic Error:  " << results[i].pi0_sys_error << std::endl;
        std::cout << "    - Total Error:       " << results[i].pi0_total_error << std::endl;
        std::cout << "  Derived Bkg Asymmetry: " << results[i].bkg_asymmetry << std::endl;
        std::cout << "    - Statistical Error: " << results[i].bkg_stat_error << std::endl;
        std::cout << "    - Systematic Error:  " << results[i].bkg_sys_error << std::endl;
        std::cout << "    - Total Error:       " << results[i].bkg_total_error << std::endl;
        std::cout << std::endl;
	
	outfile << xF_bins[i] <<" "<< results[i].pi0_asymmetry << " "<< results[i].pi0_stat_error << " " << results[i].pi0_sys_error<< " " << results[i].pi0_total_error << " " << results[i].bkg_asymmetry << " " << results[i].bkg_stat_error << " "<< results[i].bkg_sys_error<< " "<< results[i].bkg_total_error <<endl;
    }

// Plot the derived pi0 asymmetry for Blue beam
    plotter(xF_bins, results, range_input, beam_input, func_input);
    
    // Plot the derived background asymmetry for Yellow beam
  //  plotter(xF_bins, results, "Yellow", "bkg");

//    return 0;
}

