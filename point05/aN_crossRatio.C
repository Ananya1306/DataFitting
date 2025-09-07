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

void aN_crossRatio(int range_input, int xF_input, string beam_input, string region_input){

//range_input = 0/1/2
//xF_input = 0/1/2/3
//beam_input = Blue/Yellow
//region_input = sig/sb
//spin_input = YUp, YDown, BUp, BDown

	const int bins = 24; const int halfbins = 12; const double pi = TMath::Pi();
	double phiBUp, pi0sigErrBUp, pi0sbErrBUp; int pi0sigBUp, pi0sbBUp;
	double phiBUpArray[bins], pi0sigErrBUpArray[bins], pi0sbErrBUpArray[bins]; int pi0sigBUpArray[bins], pi0sbBUpArray[bins];

	double phiBDown, pi0sigErrBDown, pi0sbErrBDown; int pi0sigBDown, pi0sbBDown;
        double phiBDownArray[bins], pi0sigErrBDownArray[bins], pi0sbErrBDownArray[bins]; int pi0sigBDownArray[bins], pi0sbBDownArray[bins];

	double phi_val[bins], rot_phi_val[bins], a[halfbins], b[halfbins], c[halfbins], d[halfbins], a_err[halfbins], b_err[halfbins], c_err[halfbins], d_err[halfbins];

	string spin_input1, spin_input2;
	if(beam_input == "Blue"){spin_input1 = "BUp"; spin_input2 = "BDown";}
	if(beam_input == "Yellow"){spin_input1 = "YUp"; spin_input2 = "YDown";}
	
	int i = 0; int j = 0;

	ifstream infile1;
        infile1.open(Form("sig_sb_range%d/%sBeam/auto_pi0_xF%d_%s.txt",range_input,beam_input.c_str(),xF_input,spin_input1.c_str()),std::ios::in);

	while(infile1 >> phiBUp >> pi0sigBUp >> pi0sigErrBUp >> pi0sbBUp >> pi0sbErrBUp ){

		phiBUpArray[i] = phiBUp; pi0sigBUpArray[i] = pi0sigBUp; pi0sigErrBUpArray[i] = pi0sigErrBUp;
		pi0sbBUpArray[i] = pi0sbBUp; pi0sbErrBUpArray[i] = pi0sbErrBUp;	
		
		i++;
	}

	infile1.close();

	ifstream infile2;
        infile2.open(Form("sig_sb_range%d/%sBeam/auto_pi0_xF%d_%s.txt",range_input,beam_input.c_str(),xF_input,spin_input2.c_str()),std::ios::in);

        while(infile2 >> phiBDown >> pi0sigBDown >> pi0sigErrBDown >> pi0sbBDown >> pi0sbErrBDown ){

                phiBDownArray[j] = phiBDown; pi0sigBDownArray[j] = pi0sigBDown; pi0sigErrBDownArray[j] = pi0sigErrBDown;
                pi0sbBDownArray[j] = pi0sbBDown; pi0sbErrBDownArray[j] = pi0sbErrBDown;

                j++;
        }

        infile2.close();

	//change all sig to sb for calculating in the sb region

	cout<<"At first, printing all the pi0sig for all 24 bins"<<endl;

	for(int i=0; i<bins; i++){
		
		cout<<phiBUpArray[i]<<" "<<pi0sbBUpArray[i]<<" "<<pi0sbBDownArray[i]<<endl;

		if(i<halfbins)
		{
			if(region_input == "sb"){
			//cout<<phiBUpArray[i]<<" "<<pi0sigBUpArray[i]<<" "<<pi0sigBDownArray[i+12]<<endl;
			phi_val[i] = phiBUpArray[i]; a[i] = pi0sbBUpArray[i]; b[i] = pi0sbBDownArray[i+12];
			a_err[i] = pi0sbErrBUpArray[i]; b_err[i] = pi0sbErrBDownArray[i+12];}
			if(region_input == "sig"){
			phi_val[i] = phiBUpArray[i]; a[i] = pi0sigBUpArray[i]; b[i] = pi0sigBDownArray[i+12];
                        a_err[i] = pi0sigErrBUpArray[i]; b_err[i] = pi0sigErrBDownArray[i+12];}
		}
		if(i>=halfbins)
		{
			if(region_input == "sb"){
			//cout<<phiBUpArray[i]<<" "<<pi0sigBUpArray[i]<<" "<<pi0sigBDownArray[i-12]<<endl;
			phi_val[i] = phiBUpArray[i]; c[i-12] = pi0sbBUpArray[i]; d[i-12] = pi0sbBDownArray[i-12];
			c_err[i-12] = pi0sbErrBUpArray[i]; d_err[i-12] = pi0sbErrBDownArray[i-12];}
			if(region_input == "sig"){
			phi_val[i] = phiBUpArray[i]; c[i-12] = pi0sigBUpArray[i]; d[i-12] = pi0sigBDownArray[i-12];
                        c_err[i-12] = pi0sigErrBUpArray[i]; d_err[i-12] = pi0sigErrBDownArray[i-12];}
		}}

	cout<<"Now, printing the a, b arrays and sqrt(ab)"<<endl;

	for(int i=0; i<halfbins; i++){
		//cout<<phi_val[i]<<" "<<a[i]<<" "<<b[i]<<" "<<sqrt(a[i]*b[i])<<endl;
		cout<<a[i]<<" "<<b[i]<<" "<<sqrt(a[i]*b[i])<<endl;
	}
	cout<<"Printing the c, d arrays and sqrt (cd)"<<endl;
	for(int i=0; i<halfbins; i++){
		//cout<<phi_val[i+12]<<" "<<c[i]<<" "<<d[i]<<" "<<sqrt(c[i]*d[i])<<endl;
		cout<<c[i]<<" "<<d[i]<<" "<<sqrt(c[i]*d[i])<<endl;
	}
//Calculating Cross Ratio

	ofstream outfile;
        outfile.open(Form("sig_sb_range%d/%sBeam/auto_mod_crossRatio_%s_xF%d.txt",range_input,beam_input.c_str(),region_input.c_str(),xF_input),std::ios::out);

	cout<<"Finally, calculating cross ratio"<<endl;

	double CrossRatio[halfbins]; double CrossRatioErr[halfbins]; double CrossRatioErr_new[halfbins];

	for(int i=0; i<halfbins; i++){

/*		CrossRatio[i] = (sqrt(a[i]*b[i]) - sqrt(c[i]*d[i]))/(sqrt(a[i]*b[i]) + sqrt(c[i]*d[i]));
		
		double num = sqrt(a[i]*b[i]*c[i]*d[i]); double den = pow((sqrt(a[i]*b[i]) + sqrt(c[i]*d[i])),2);
		double term_a = pow((a_err[i]/a[i]),2); double term_b = pow((b_err[i]/b[i]),2);
		double term_c = pow((c_err[i]/c[i]),2); double term_d = pow((d_err[i]/d[i]),2);
		
		CrossRatioErr[i] = (num/den)*(sqrt (term_a + term_b + term_c + term_d) );

		cout<<phi_val[i + halfbins]<<" "<<a[i]<<" "<<b[i]<<" "<<c[i]<<" "<<d[i]<<" "<<CrossRatio[i]<<endl;
                cout<<"err "<<a_err[i]<<" "<<b_err[i]<<" "<<c_err[i]<<" "<<d_err[i]<<" "<<CrossRatioErr[i]<<endl;
		cout<<endl; //write into the outfile here itself if not rotated array
		
		if(i==0 || i == halfbins-1){continue;}
		outfile<<phi_val[i]<<" "<<CrossRatio[i]<<" "<<CrossRatioErr[i]<<endl;
*/

	// Calculate the cross ratio
        double term1 = sqrt(a[i] * b[i]);
        double term2 = sqrt(c[i] * d[i]);
        CrossRatio[i] = (term1 - term2) / (term1 + term2);

        // --- Correct Error Propagation using Partial Derivatives ---
        // Let Y = (sqrt(ab) - sqrt(cd)) / (sqrt(ab) + sqrt(cd))
        // We calculate the partial derivatives of Y with respect to a, b, c, d
        // dY/da = (sqrt(b*c*d)) / (a * (sqrt(a*b) + sqrt(c*d))^2)
        // dY/db = (sqrt(a*c*d)) / (b * (sqrt(a*b) + sqrt(c*d))^2)
        // dY/dc = -(sqrt(a*b*d)) / (c * (sqrt(a*b) + sqrt(c*d))^2)
        // dY/dd = -(sqrt(a*b*c)) / (d * (sqrt(a*b) + sqrt(c*d))^2)

        double denominator_sq = pow(term1 + term2, 2);

        double dY_da = (sqrt(b[i] * c[i] * d[i])) / (sqrt(a[i]) * denominator_sq);
        double dY_db = (sqrt(a[i] * c[i] * d[i])) / (sqrt(b[i]) * denominator_sq);
        double dY_dc = -(sqrt(a[i] * b[i] * d[i])) / (sqrt(c[i]) * denominator_sq);
        double dY_dd = -(sqrt(a[i] * b[i] * c[i])) / (sqrt(d[i]) * denominator_sq);

        // Sum the squared terms for the total error
        double term_a = pow(dY_da * a_err[i], 2);
        double term_b = pow(dY_db * b_err[i], 2);
        double term_c = pow(dY_dc * c_err[i], 2);
        double term_d = pow(dY_dd * d_err[i], 2);

        CrossRatioErr[i] = sqrt(term_a + term_b + term_c + term_d);

        // Output results
        std::cout << "Phi: " << phi_val[i] << " radian" << std::endl;
        std::cout << "Cross Ratio = " << CrossRatio[i] << std::endl;
        std::cout << "Correct Error = " << CrossRatioErr[i] << std::endl;
        std::cout << "-----------------------------------" << std::endl;

	if(i==0 || i == halfbins-1){continue;}
                outfile<<phi_val[i]<<" "<<CrossRatio[i]<<" "<<CrossRatioErr[i]<<endl;

		}

/*	std::rotate(CrossRatio, CrossRatio + 6, CrossRatio + halfbins);
	std::rotate(CrossRatioErr, CrossRatioErr + 6, CrossRatioErr + halfbins);
	cout<<"Rotated Array "<<endl;
	for(int i = 0; i<halfbins; i++){ cout<<phi_val[i+6]<<" "<<CrossRatio[i]<<" "<<CrossRatioErr[i]<<endl;
					//outfile1<<phi_val[i+6]<<" "<<CrossRatio[i]<<" "<<CrossRatioErr[i]<<endl;	
					}
	 outfile.close(); 
*/
//Calculating sector by sector using Relative luminosity

       cout<<" Rel Luminosity of Blue Beam = 0.990695 Stat Error = 0.00307404 Sys Err = 0.035848"<<endl;
        cout<<" Rel Luminosity of Yellow Beam = 0.992644 Stat Error = 0.00310132 Sys Err = 0.0276967"<<endl; 
	cout<<"Sector by Sector using Relative Luminosity"<<endl;
	cout<<endl;
	//double secBySecErr[bins];
	double secBySec[bins];
    	double secBySecStatErr[bins]; double secBySecSysErr[bins]; double secBySecTotErr[bins]; 

	double e[bins], f[bins], e_err[bins], f_err[bins];
	double R, R_err, R_sys;

	if(beam_input == "Blue"){R = 0.990695; R_err = 0.00307404; R_sys = 0.0358484;}
        if(beam_input == "Yellow"){R = 0.992644; R_err = 0.00310132; R_sys = 0.0276967;}

//	ofstream outfile2;
//        outfile2.open(Form("sig_sb_range%d/%sBeam/auto_mod_relLum_%s_xF%d.txt",range_input,beam_input.c_str(),region_input.c_str(),xF_input),std::ios::out);

	for(int i=0; i<bins; i++){

		if(region_input == "sb"){
               //       cout<<phiBUpArray[i]<<" "<<pi0sigBUpArray[i]<<" "<<pi0sigBDownArray[i+12]<<endl;
                        phi_val[i] = phiBUpArray[i]; e[i] = pi0sbBUpArray[i]; f[i] = pi0sbBDownArray[i];
                        e_err[i] = pi0sbErrBUpArray[i]; f_err[i] = pi0sbErrBDownArray[i];}
		if(region_input == "sig"){
                        phi_val[i] = phiBUpArray[i]; e[i] = pi0sigBUpArray[i]; f[i] = pi0sigBDownArray[i];
                        e_err[i] = pi0sigErrBUpArray[i]; f_err[i] = pi0sigErrBDownArray[i];}
/*		
		secBySec[i] = (e[i] - (R*f[i])) / (e[i] + (R*f[i]));
		double term_R = pow(R_err/R, 2); double term_f = pow(f_err[i]/f[i], 2);
		double term1 = sqrt( (pow(e_err[i],2)) + pow(e[i],2)*term_R*term_f );
		double num = 2*R*f[i]; double den = pow((e[i] + (R*f[i])), 2);
		secBySecErr[i] = (num/den)*term1;

		cout<<phi_val[i]<<" "<<e[i]<<" "<<f[i]<<" "<<R<<" "<<secBySec[i]<<endl;
		cout<<"err "<<term1<<" "<<num/den<<" "<<" "<<secBySecErr[i]<<endl;
		
		if(i==11 || i==12){continue;}
		outfile2<<phi_val[i]<<" "<<secBySec[i]<<" "<<secBySecErr[i]<<endl;
*/

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

//		if(i==11 || i==12){continue;}
//                outfile2<<phi_val[i]<<" "<<secBySec[i]<<" "<<secBySecStatErr[i]<<" "<<secBySecSysErr[i]<<" "<<secBySecTotErr[i]<<endl;

	}

//	outfile2.close();

/*	cout<<"Rotated RelLum Axes"<<endl;

	ofstream outfile3;
	outfile3.open(Form("sig_sb_range%d/RotAxis%sBeam/auto_relLum_%s_xF%d.txt",range_input,beam_input.c_str(),region_input.c_str(),xF_input),std::ios::out);

//	std::rotate(secBySec, secBySec + halfbins, secBySec + bins);
//	std::rotate(secBySecErr, secBySecErr + halfbins, secBySecErr + bins);
	for(int i=0; i<bins; i++){rot_phi_val[i] = phi_val[i];}
//	std::rotate(phi_val, phi_val + halfbins, phi_val + bins);
	std::rotate(rot_phi_val, rot_phi_val + halfbins, rot_phi_val + bins);
        cout<<"Rotated Array "<<endl;
        for(int i = 0; i<bins; i++){ cout<<i<<" "<<phi_val[i]<<" "<<rot_phi_val[i]<<" "<<rot_phi_val[i] + pi<<" "<<secBySec[i]<<" "<<secBySecErr[i]<<endl;
                                  outfile3<<rot_phi_val[i] + pi<<" "<<secBySec[i]<<" "<<secBySecErr[i]<<endl;      
                                        }
         outfile3.close(); 
*/
}

