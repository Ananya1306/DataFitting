#include <TGraphMultiErrors.h>
#include <TCanvas.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <TMultiGraph.h>
#include <TLine.h>

void graph_raw_aN_relLum2(int range_input, string beam_input, string method_input) {
    // These are placeholders for your variables.
    const double pol_blue = 0.5785; 
    const double pol_yellow = 0.5872;
    const double R_sys_blue = 0.035848;
    const double R_sys_yellow = 0.0276967;
    double pol; double R_sys;
    int marker_color;
    int marker_style_sig, marker_style_sb;
    double marker_size;

/*    std::string beam_input = "Blue";
    std::string method_input = "relLum";
    int range_input = 1;
*/
    if(beam_input == "Blue"){
        pol = pol_blue;
        marker_color = 600;
	R_sys = R_sys_blue;
    } else if(beam_input == "Yellow"){
        pol = pol_yellow;
        marker_color = 807;
	R_sys = R_sys_yellow;
    }
    if(method_input == "crossRatio"){
        marker_style_sig = 20;
        marker_style_sb = 24;
        marker_size = 1.5;
    } else if(method_input == "relLum"){
        marker_style_sig = 21;
        marker_style_sb = 25;
        marker_size = 1.5;
    }

    std::cout << "BlueBeam signal region" << std::endl;
    std::fstream file0;
    file0.open(Form("sig_sb_range%d/%sBeam/aN_mod_%s_sig_goodChi2_test2.txt", range_input, beam_input.c_str(), method_input.c_str()), std::ios::in);
    
    double xF_val_in, aN_sig_phi_in, aN_sig_phi_err_in, aN_sig_cosphi_in, aN_sig_cosphi_err_in;
    
    // 1. Read all data into vectors first
    std::vector<double> xF_val_vec;
    std::vector<double> aN_sig_phi_vec;
    std::vector<double> aN_sig_phi_err_vec;
    
    while(file0 >> xF_val_in >> aN_sig_phi_in >> aN_sig_phi_err_in >> aN_sig_cosphi_in >> aN_sig_cosphi_err_in) {
        xF_val_vec.push_back(xF_val_in);
        aN_sig_phi_vec.push_back(aN_sig_phi_in);
        aN_sig_phi_err_vec.push_back(aN_sig_phi_err_in);
    }
    file0.close();

    int n_points = xF_val_vec.size();
    
    // 2. Prepare C-style arrays for the TGraphMultiErrors constructor
    // The TGraphMultiErrors constructor and AddYError method use C-style arrays
    double* ax = new double[n_points];
    double* ay = new double[n_points];
    double* aexl = new double[n_points];
    double* aexh = new double[n_points];
    double* aeyl_stat = new double[n_points];
    double* aeyh_stat = new double[n_points];
    double* aeyl_sys = new double[n_points];
    double* aeyh_sys = new double[n_points];
    
    for (int i = 0; i < n_points; ++i) {
        // Normalize the central value
        ay[i] = aN_sig_phi_vec[i] / pol;
        // Normalize the statistical error
        aeyl_stat[i] = aN_sig_phi_err_vec[i] / pol;
        aeyh_stat[i] = aN_sig_phi_err_vec[i] / pol;
        // Calculate and normalize the systematic error
        aeyl_sys[i] = (R_sys * aN_sig_phi_vec[i]) / pol;
        aeyh_sys[i] = (R_sys * aN_sig_phi_vec[i]) / pol;
        // Set x-errors to your specified value (0.003)
        aexl[i] = 0.002;
        aexh[i] = 0.002;
        ax[i] = xF_val_vec[i];

	// Print the values for debugging
        std::cout << "Point " << i << ": x=" << ax[i] << ", y=" << ay[i]
                  << ", exl=" << aexl[i] << ", exh=" << aexh[i]
                  << ", eyl_stat=" << aeyl_stat[i] << ", eyh_stat=" << aeyh_stat[i]
                  << ", eyl_sys=" << aeyl_sys[i] << ", eyh_sys=" << aeyh_sys[i]
                  << std::endl;
    }
    
    // 3. Create TGraphMultiErrors with the statistical errors
    TGraphMultiErrors* gme = new TGraphMultiErrors("gme", "Raw A_{N} for signal region; xF; A^{raw}_{N}", n_points, ax, ay, aexl, aexh, aeyl_stat, aeyh_stat);
    
    // 4. Add the systematic errors as the second error source
    gme->AddYError(n_points, aeyl_sys, aeyh_sys);
    
    // 5. Set drawing styles
    gme->SetMarkerStyle(marker_style_sig);
    gme->SetMarkerColor(marker_color);
    gme->SetLineColor(marker_color);
    gme->SetMarkerSize(marker_size);
    // Set the fill style for the second error set (index 1) which is systematic
    gme->GetAttLine(0)->SetLineColor(marker_color);
    gme->GetAttFill(1)->SetFillStyle(3001); 
    gme->GetAttFill(1)->SetFillColor(kGray);
    gme->GetAttLine(1)->SetLineColor(kGray);
    //gme->GetAttLine(1)->SetLineStyle(1);
    
    // 6. Create and draw on a canvas
    TCanvas *c47 = new TCanvas("c47","c47",800,600);
    gme->SetTitle(Form("Raw A_{N} for %s Beam; xF; A^{raw}_{N}", beam_input.c_str()));
    gme->Draw("aps; 5 s=0.5"); // "a" for axes, "p" for points, "s" for stat+sys bars
    c47->BuildLegend();

    // Clean up allocated memory
    delete[] ay;
    delete[] aexl;
    delete[] aexh;
    delete[] aeyl_stat;
    delete[] aeyh_stat;
    delete[] aeyl_sys;
    delete[] aeyh_sys;
}

