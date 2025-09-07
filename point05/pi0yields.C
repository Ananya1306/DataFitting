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
#include <string>
#include "Math/ProbFuncMathCore.h"

using namespace std;

void pi0yields(int range_input, int xF_input, string beam_input, string spin_input){
//range_input = 0/1/2/3/4
//xF_input = 0/1/2/3
//beam_input = Blue/Yellow
//spin_input = YUp, YDown, BUp, BDown

	double sig_Low, sig_High, sb_Low_1, sb_High_1, sb_Low_2, sb_High_2;
/*	if(range_input == 10){sb_Low_1 = 0; sb_High_1 = 0; sig_Low = 11; sig_High = 21; sb_Low_2 = 21; sb_High_2 = 40;}
	else if(range_input == 11){sb_Low_1 = 0; sb_High_1 = 11; sig_Low = 11; sig_High = 21; sb_Low_2 = 21; sb_High_2 = 40;}
	else if(range_input == 12){sb_Low_1 = 0; sb_High_1 = 11; sig_Low = 11; sig_High = 21; sb_Low_2 = 0; sb_High_2 = 0;}
	else if(range_input == 3){sb_Low_1 = 0; sb_High_1 = 0; sig_Low = 11; sig_High = 21; sb_Low_2 = 22; sb_High_2 = 40;}
	else if(range_input == 4){sb_Low_1 = 0; sb_High_1 = 11; sig_Low = 11; sig_High = 21; sb_Low_2 = 21; sb_High_2 = 40;}
*/
/*if(range_input == 1){sb_Low_1 = 0; sb_High_1 = 0; sig_Low = 11; sig_High = 21; sb_Low_2 = 21; sb_High_2 = 40;}
//The above is for test	
else if(range_input == 50){sb_Low_1 = 0; sb_High_1 = 11; sig_Low = 11; sig_High = 21; sb_Low_2 = 21; sb_High_2 = 40;}
*/
//for 4 xF bins, ranges are 40, 41, 42 etc. data file is EEmc....xFTest_1.root
//for 5 xF bins, ranges are 50, 51, 52 etc. data file is EEmc....xFTest_2.root

	if(range_input == 40){sb_Low_1 = 0; sb_High_1 = 8; sig_Low = 8; sig_High = 19; sb_Low_2 = 19; sb_High_2 = 40;}
	if(range_input == 41){sb_Low_1 = 0; sb_High_1 = 9; sig_Low = 9; sig_High = 20; sb_Low_2 = 20; sb_High_2 = 40;}
	if(range_input == 42){sb_Low_1 = 0; sb_High_1 = 10; sig_Low = 10; sig_High = 21; sb_Low_2 = 21; sb_High_2 = 40;}
	if(range_input == 43){sb_Low_1 = 0; sb_High_1 = 11; sig_Low = 11; sig_High = 22; sb_Low_2 = 22; sb_High_2 = 40;}
	if(range_input == 44){sb_Low_1 = 0; sb_High_1 = 8; sig_Low = 8; sig_High = 21; sb_Low_2 = 21; sb_High_2 = 40;}
	if(range_input == 45){sb_Low_1 = 0; sb_High_1 = 9; sig_Low = 9; sig_High = 22; sb_Low_2 = 22; sb_High_2 = 40;}

	const int phiVal = 25; const int phi = 24; const int spin = 2; const int phihalf = 12; const double pi = 3.14159; const int xF = 5;
										
	double phi_val_blue[25]={-1.0*pi,-0.916667*pi,-0.833333*pi,-0.75*pi,-0.666667*pi,-0.583333*pi,-0.5*pi,-0.416667*pi,-0.33333*pi,-0.25*pi,-0.16667*pi,-0.08333*pi,0.0*pi,0.08333*pi,0.16667*pi,0.25*pi,0.33333*pi,0.416667*pi,0.5*pi,0.583333*pi,0.66667*pi,0.75*pi,0.83333*pi,0.916667*pi,1.0*pi };
	

	double phi_val_yellow[25], phi_val[25];

	for(int i=0; i<phiVal; i++){phi_val_yellow[i] = phi_val_blue[ phiVal - 1 - i];}
	
	if(beam_input == "Blue"){for(int i=0; i<phiVal; i++){phi_val[i] = phi_val_blue[i];}}
	if(beam_input == "Yellow"){for(int i=0; i<phiVal; i++){phi_val[i] = phi_val_yellow[i];}}

	
	TFile *f = new TFile(Form("EEmcNanoTreeQA_schedRun15_%sBeam_160All_Et06pt0_xFTest_1.root",beam_input.c_str()));
	TH1F *histo[phi];

	cout<<"pi0 yields for Blue Beam Up xF (0.05-0.1) or xF = 0.075"<<endl;
	cout<<"phi "<<" pi0sig "<<" pi0sigErr "<<" pi0sb "<<" pi0sbErr "<<endl;

	ofstream outfile;
        outfile.open(Form("sig_sb_range%d/%sBeam/auto_pi0_xF%d_%s.txt",range_input,beam_input.c_str(),xF_input,spin_input.c_str()),std::ios::out);

	for(int i=0; i<phi; i++){
//Need to set variable names	
		histo[i] = (TH1F*)f->Get(Form("pi0M_%s_xF%d_phi%d",spin_input.c_str(),xF_input,i));

		cout<<0.5*(phi_val[i] + phi_val[i+1])<<" "<<histo[i]->Integral(sig_Low, sig_High)<<" "<<TMath::Sqrt(histo[i]->Integral(sig_Low, sig_High))<<" "<<histo[i]->Integral(sb_Low_1, sb_High_1) + histo[i]->Integral(sb_Low_2, sb_High_2)<<" "<<TMath::Sqrt(histo[i]->Integral(sb_Low_1, sb_High_1) + histo[i]->Integral(sb_Low_2, sb_High_2))<<endl;

		outfile<<0.5*(phi_val[i] + phi_val[i+1])<<" "<<histo[i]->Integral(sig_Low, sig_High)<<" "<<TMath::Sqrt(histo[i]->Integral(sig_Low, sig_High))<<" "<<histo[i]->Integral(sb_Low_1, sb_High_1) + histo[i]->Integral(sb_Low_2, sb_High_2)<<" "<<TMath::Sqrt(histo[i]->Integral(sb_Low_1, sb_High_1) + histo[i]->Integral(sb_Low_2, sb_High_2))<<endl;
	}


	outfile.close();
}


