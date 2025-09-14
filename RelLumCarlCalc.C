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

void RelLumCarlCalc(){

	const int bins = 24; const int halfbins = 12; const double pi = TMath::Pi();

	ifstream infile1;
        infile1.open("RelLumBlueSpinPhiVal_Jets.txt",std::ios::in);
	double phi_val, nBUp, nBDown;
	double phi_val_arr[24], nBUp_arr[24], nBDown_arr[24], nBUpErr_arr[24], nBDownErr_arr[24];
	
	int i = 0;
	while(infile1 >> phi_val >> nBUp >> nBDown ){

		phi_val_arr[i] = phi_val;
		nBUp_arr[i] = nBUp; nBDown_arr[i] = nBDown;
		nBUpErr_arr[i] = sqrt(nBUp); nBDownErr_arr[i] = sqrt(nBDown); 
		i++;
	}

	double a[halfbins], b[halfbins], c[halfbins], d[halfbins];
	double a_err[halfbins], b_err[halfbins], c_err[halfbins], d_err[halfbins];

	for(int i=0; i<halfbins; i++){
		
		a[i] = nBUp_arr[i]; c[i] = nBDown_arr[i];
		a_err[i] = nBUpErr_arr[i]; c_err[i] = nBDownErr_arr[i];
		
	}

	for(int i=0; i<halfbins; i++){

		b[i] = nBUp_arr[i+12]; d[i] = nBDown_arr[i+12];
		b_err[i] = nBUpErr_arr[i+12]; d_err[i] = nBDownErr_arr[i+12];
	}


        cout<<"Calculating Relative Luminosity using cross Ratio"<<endl;

	double CrossRatio[halfbins]; double CrossRatioErr[halfbins];
	for(int i=0; i<halfbins; i++){

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
        std::cout << "Phi: " << phi_val_arr[i] << " radian" << std::endl;
        std::cout << "Cross Ratio = " << CrossRatio[i] << std::endl;
        std::cout << "Correct Error = " << CrossRatioErr[i] << std::endl;
        std::cout << "-----------------------------------" << std::endl;


	}
 	
	TGraphErrors *gr = new TGraphErrors(halfbins, phi_val_arr, CrossRatio, 0, CrossRatioErr);
	gr->SetTitle("Relative Luminosity Diff Blue Spin with jets ; #phi [rad]; Rel. Lum.");
	gr->SetMarkerStyle(20); 
	//gr->SetMarkerColor(kOrange + 7);
	gr->SetMarkerColor(kBlue);
	TCanvas *c1 = new TCanvas();
	TF1 *f1 = new TF1("f1","pol0",0.0,3.5); //-3.5 - 0 for Blue; 0 - 3.5 for Yellow
	f1->SetParameter(0,1);
	gr->Fit("f1","V+");
	gr->Draw("AP");

	double RelLum; double RelLumErr;
	double fitPar = f1->GetParameter(0);
	double fitParErr = f1->GetParError(0);

	RelLum = (1.0 + 2.0*(fitPar));
	RelLumErr = (2.0*(fitParErr)); 

	cout<<"Relative Luminosity = "<<RelLum<<" Error = "<<std::setprecision(8)<<RelLumErr<<endl; 

 gStyle->SetOptFit(1111);

//Yellow Beam 
//Relative Luminosity = 0.997356 Error = 0.00018987674

//Blue Beam
//Relative Luminosity = 1.00199 Error = 0.00019223273

}
