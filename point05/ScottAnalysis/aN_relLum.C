#include "TH1.h"
#include "TF1.h"
#include "TList.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TFile.h"
#include "TStyle.h"
#include "TROOT.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Math/ProbFuncMathCore.h"
using namespace std;

void aN_relLum(int xF_input, string beam_input, string region_input){

	const int bins = 24;

        cout<<" Rel Luminosity of Blue Beam = 0.990695 Stat Error = 0.00307404 Sys Err = 0.035848"<<endl;
        cout<<" Rel Luminosity of Yellow Beam = 0.992644 Stat Error = 0.00310132 Sys Err = 0.0276967"<<endl;
        cout<<"Sector by Sector using Relative Luminosity"<<endl;
        cout<<endl;

	const double pi = 3.14159;
        double phi_val_blue[25]={-1.0*pi,-0.916667*pi,-0.833333*pi,-0.75*pi,-0.666667*pi,-0.583333*pi,-0.5*pi,-0.416667*pi,-0.33333*pi,-0.25*pi,-0.16667*pi,-0.08333*pi,0.0*pi,0.08333*pi,0.16667*pi,0.25*pi,0.33333*pi,0.416667*pi,0.5*pi,0.583333*pi,0.66667*pi,0.75*pi,0.83333*pi,0.916667*pi,1.0*pi };

	const int phiVal = 25;
        double phi_val_yellow[25], phi_val[25]; double phi_bins[24];

        for(int i=0; i<phiVal; i++){phi_val_yellow[i] = phi_val_blue[ phiVal - 1 - i];}

        if(beam_input == "Blue"){for(int i=0; i<phiVal; i++){phi_val[i] = phi_val_blue[i];}}
        if(beam_input == "Yellow"){for(int i=0; i<phiVal; i++){phi_val[i] = phi_val_yellow[i];}}

	for(int i=0; i<bins; i++){
		
		phi_bins[i] = 0.5*(phi_val[i] + phi_val[i+1]);
	
	}

        double R, R_err, R_sys;
	if(beam_input == "Blue"){R = 0.990695; R_err = 0.00307404; R_sys = 0.0358484;}
        if(beam_input == "Yellow"){R = 0.992644; R_err = 0.00310132; R_sys = 0.0276967;}

	double phi_val_infile, totChi2NDF, totBgAmp, totBgAmpErr, totSigAmp, totSigAmpErr, upChi2NDF, upBgAmp, upBgAmpErr, upSigAmp, upSigAmpErr, downChi2NDF, downBgAmp, downBgAmpErr, downSigAmp, downSigAmpErr; 
	double phi_val_infile_arr[bins], totChi2NDF_arr[bins], totBgAmp_arr[bins], totBgAmpErr_arr[bins], totSigAmp_arr[bins], totSigAmpErr_arr[bins], upChi2NDF_arr[bins], upBgAmp_arr[bins], upBgAmpErr_arr[bins], upSigAmp_arr[bins], upSigAmpErr_arr[bins], downChi2NDF_arr[bins], downBgAmp_arr[bins], downBgAmpErr_arr[bins], downSigAmp_arr[bins], downSigAmpErr_arr[bins];

        ifstream infile1;
        infile1.open(Form("Par_BB_xF%d.txt",xF_input),std::ios::in);
         
	int i = 0;
        while(infile1 >> phi_val_infile >> totChi2NDF >> totBgAmp >> totBgAmpErr >> totSigAmp >> totSigAmpErr >> upChi2NDF >> upBgAmp >> upBgAmpErr >> upSigAmp >> upSigAmpErr >> downChi2NDF >> downBgAmp >> downBgAmpErr >> downSigAmp >> downSigAmpErr){

	phi_val_infile_arr[i] = phi_val_infile;
        totChi2NDF_arr[i] = totChi2NDF;
	totBgAmp_arr[i] = totBgAmp;
	totBgAmpErr_arr[i] = totBgAmpErr; 
	totSigAmp_arr[i] = totSigAmp;
	totSigAmpErr_arr[i] = totSigAmpErr;
	upChi2NDF_arr[i] = upChi2NDF;
	upBgAmp_arr[i] = upBgAmp;
	upBgAmpErr_arr[i] = upBgAmpErr;
	upSigAmp_arr[i] = upSigAmp;
	upSigAmpErr_arr[i] = upSigAmpErr;
	downChi2NDF_arr[i] = downChi2NDF;
	downBgAmp_arr[i] = downBgAmp;
	downBgAmpErr_arr[i] = downBgAmpErr;
	downSigAmp_arr[i] = downSigAmp;
	downSigAmpErr_arr[i] = downSigAmpErr;

	i++;
	}

	infile1.close();

        ofstream outfile2;
        outfile2.open(Form("auto_mod_relLum_%s_xF%d.txt",region_input.c_str(),xF_input),std::ios::out);

	//Calculation of sector by sector Relative Luminosity begins here

	double secBySec[bins];
        double secBySecStatErr[bins]; double secBySecSysErr[bins]; double secBySecTotErr[bins];
	double e[bins], f[bins], e_err[bins], f_err[bins];

	for(int i=0; i<bins; i++){

		if(region_input == "sig"){ e[i] = upSigAmp_arr[i]; e_err[i] = upSigAmpErr_arr[i];
					   f[i] = downSigAmp_arr[i]; f_err[i] = downSigAmpErr_arr[i];
		 }
		if(region_input == "sb"){ e[i] = upBgAmp_arr[i]; e_err[i] = upBgAmpErr_arr[i]; 
                                           f[i] = downBgAmp_arr[i]; f_err[i] = downBgAmpErr_arr[i];
		}

		// Asymmetry calculation: (e - R*f) / (e + R*f)
                secBySec[i] = (e[i] - (R * f[i])) / (e[i] + (R * f[i]));

                // --- Correct Error Propagation ---
                // Partial derivatives for the asymmetry function Y = (e - Rf) / (e + Rf)
                double dY_de = (2 * R * f[i]) / pow(e[i] + R * f[i], 2);
                double dY_df = (-2 * R * e[i]) / pow(e[i] + R * f[i], 2);
                double dY_dR = (-2 * e[i] * f[i]) / pow(e[i] + R * f[i], 2);

                // Statistical error propagation
                double term_e = pow(dY_de * e_err[i], 2);
                double term_f = pow(dY_df * f_err[i], 2);
                double term_R_stat = pow(dY_dR * R_err, 2);
                secBySecStatErr[i] = sqrt(term_e + term_f + term_R_stat);

                // Systematic error propagation (only depends on R_sys)
                double term_R_sys = pow(dY_dR * R_sys, 2);
                secBySecSysErr[i] = sqrt(term_R_sys);

                // Total error is the quadrature sum of statistical and systematic errors
                secBySecTotErr[i] = sqrt(pow(secBySecStatErr[i], 2) + pow(secBySecSysErr[i], 2));

                // Output results for debugging
        std::cout << "Sector " << i << ": Asymmetry = " << secBySec[i]
                  << ", Stat Err = " << secBySecStatErr[i]
                  << ", Sys Err = " << secBySecSysErr[i]
                  << ", Total Err = " << secBySecTotErr[i] << std::endl;

                if(i==11 || i==12){continue;}
                outfile2<<phi_bins[i]<<" "<<secBySec[i]<<" "<<secBySecStatErr[i]<<" "<<secBySecSysErr[i]<<" "<<secBySecTotErr[i]<<endl;


	}
	

}
