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
#include "Math/ProbFuncMathCore.h"

using namespace std;

void aN_crossRatio(){

	const int bins = 24; const int halfbins = 12;
	double phiBUp, pi0sigErrBUp, pi0sbErrBUp; int pi0sigBUp, pi0sbBUp;
	double phiBUpArray[bins], pi0sigErrBUpArray[bins], pi0sbErrBUpArray[bins]; int pi0sigBUpArray[bins], pi0sbBUpArray[bins];

	double phiBDown, pi0sigErrBDown, pi0sbErrBDown; int pi0sigBDown, pi0sbBDown;
        double phiBDownArray[bins], pi0sigErrBDownArray[bins], pi0sbErrBDownArray[bins]; int pi0sigBDownArray[bins], pi0sbBDownArray[bins];

	double phi_val[bins], a[halfbins], b[halfbins], c[halfbins], d[halfbins], a_err[halfbins], b_err[halfbins], c_err[halfbins], d_err[halfbins];
	
	int i = 0; int j = 0;

	fstream fileBUp;
	fileBUp.open("pi0_xF3_BUp.txt",ios::in);	

	if (!fileBUp.is_open()) {
        std::cerr << "Error opening file: pi0_xF0_BUp.txt" << std::endl;
        // Indicate an error
    }

	while(fileBUp >> phiBUp >> pi0sigBUp >> pi0sigErrBUp >> pi0sbBUp >> pi0sbErrBUp ){

		phiBUpArray[i] = phiBUp; pi0sigBUpArray[i] = pi0sigBUp; pi0sigErrBUpArray[i] = pi0sigErrBUp;
		pi0sbBUpArray[i] = pi0sbBUp; pi0sbErrBUpArray[i] = pi0sbErrBUp;	
		
		i++;
	}

	fileBUp.close();

	fstream fileBDown;
        fileBDown.open("pi0_xF3_BDown.txt",ios::in);

        if (!fileBDown.is_open()) {
        std::cerr << "Error opening file: pi0_xF0_BDown.txt" << std::endl;
        // Indicate an error
    }

        while(fileBDown >> phiBDown >> pi0sigBDown >> pi0sigErrBDown >> pi0sbBDown >> pi0sbErrBDown ){

                phiBDownArray[j] = phiBDown; pi0sigBDownArray[j] = pi0sigBDown; pi0sigErrBDownArray[j] = pi0sigErrBDown;
                pi0sbBDownArray[j] = pi0sbBDown; pi0sbErrBDownArray[j] = pi0sbErrBDown;

                j++;
        }

        fileBDown.close();

	//change all sig to sb for calculating in the sb region

	cout<<"At first, printing all the pi0sig for all 24 bins"<<endl;

	for(int i=0; i<bins; i++){
		
		cout<<phiBUpArray[i]<<" "<<pi0sigBUpArray[i]<<" "<<pi0sigBDownArray[i]<<endl;

		if(i<halfbins)
		{
			//cout<<phiBUpArray[i]<<" "<<pi0sigBUpArray[i]<<" "<<pi0sigBDownArray[i+12]<<endl;
			phi_val[i] = phiBUpArray[i]; a[i] = pi0sigBUpArray[i]; b[i] = pi0sigBDownArray[i+12];
			a_err[i] = pi0sigErrBUpArray[i]; b_err[i] = pi0sigErrBDownArray[i+12];
		}
		if(i>=halfbins)
		{
			//cout<<phiBUpArray[i]<<" "<<pi0sigBUpArray[i]<<" "<<pi0sigBDownArray[i-12]<<endl;
			phi_val[i] = phiBUpArray[i]; c[i-12] = pi0sigBUpArray[i]; d[i-12] = pi0sigBDownArray[i-12];
			c_err[i-12] = pi0sigErrBUpArray[i]; d_err[i-12] = pi0sigErrBDownArray[i-12];
		}

	}

	cout<<"Now, printing the a, b arrays"<<endl;

	for(int i=0; i<halfbins; i++){

		cout<<phi_val[i]<<" "<<a[i]<<" "<<b[i]<<endl;

	}
	cout<<"Printing the c, d arrays"<<endl;
	for(int i=0; i<halfbins; i++){

		cout<<phi_val[i+12]<<" "<<c[i]<<" "<<d[i]<<endl;
	}
//Calculating Cross Ratio
	cout<<"Finally, calculating cross ratio"<<endl;

	double CrossRatio[halfbins]; double CrossRatioErr[halfbins]; double CrossRatioErr_new[halfbins];
	ofstream outfile;
	outfile.open("crossRatio_sig_xF3.txt");

	for(int i=0; i<halfbins; i++){

		CrossRatio[i] = (sqrt(a[i]*b[i]) - sqrt(c[i]*d[i]))/(sqrt(a[i]*b[i]) + sqrt(c[i]*d[i]));
		
		double num = sqrt(a[i]*b[i]*c[i]*d[i]); double den = pow((sqrt(a[i]*b[i]) + sqrt(c[i]*d[i])),2);
		double term_a = pow((a_err[i]/a[i]),2); double term_b = pow((b_err[i]/b[i]),2);
		double term_c = pow((c_err[i]/c[i]),2); double term_d = pow((d_err[i]/d[i]),2);
		
		CrossRatioErr[i] = (num/den)*(sqrt (term_a + term_b + term_c + term_d) );

		cout<<phi_val[i]<<" "<<a[i]<<" "<<b[i]<<" "<<c[i]<<" "<<d[i]<<" "<<CrossRatio[i]<<endl;
                cout<<"err "<<a_err[i]<<" "<<b_err[i]<<" "<<c_err[i]<<" "<<d_err[i]<<" "<<CrossRatioErr[i]<<endl;
                outfile<<phi_val[i]<<" "<<CrossRatio[i]<<" "<<CrossRatioErr[i]<<endl;
	}
	outfile.close();

//Calculating sector by sector using Relative luminosity
	cout<<endl;
	cout<<"Sector by Sector using Relative Luminosity"<<endl;
	cout<<endl;
	double secBySec[bins]; double secBySecErr[bins]; double R = 1.011; double R_err = sqrt(R); //RelLum
	double e[bins], f[bins], e_err[bins], f_err[bins];

	ofstream outfile2; outfile2.open("relLum_sig_xF3.txt");
	for(int i=0; i<bins; i++){

		e[i] = pi0sigBUpArray[i]; f[i] = pi0sigBDownArray[i];
		e_err[i] = pi0sigErrBUpArray[i]; f_err[i] = pi0sigErrBDownArray[i];

		secBySec[i] = (e[i] - (R*f[i])) / (e[i] + (R*f[i]));
		
		double term_R = pow(R_err/R, 2); double term_f = pow(f_err[i]/f[i], 2);
		double term1 = sqrt( (pow(e_err[i],2)) + pow(e[i],2)*term_R*term_f );
		double num = 2*R*f[i]; double den = pow((e[i] + (R*f[i])), 2);
		secBySecErr[i] = (num/den)*term1;

		cout<<phi_val[i]<<" "<<e[i]<<" "<<f[i]<<" "<<R<<" "<<secBySec[i]<<endl;
		cout<<"err "<<term1<<" "<<num/den<<" "<<" "<<secBySecErr[i]<<endl;

		outfile2<<phi_val[i]<<" "<<secBySec[i]<<" "<<secBySecErr[i]<<endl;
	}


}
