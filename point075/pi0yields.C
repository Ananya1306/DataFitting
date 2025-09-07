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

void pi0yields(){

	ofstream outfile;
	outfile.open("pi0_xF0_BUp.txt");

	//signal region 10 - 25; sideband region 26 - 40;

	const int phi = 24; const int spin = 2; const int phihalf = 12; const double pi = 3.14159; const int xF = 5;
														
	double phi_val[25]={-1.0*pi,-0.916667*pi,-0.833333*pi,-0.75*pi,-0.666667*pi,-0.583333*pi,-0.5*pi,-0.416667*pi,-0.33333*pi,-0.25*pi,-0.16667*pi,-0.08333*pi,0.0*pi,0.08333*pi,0.16667*pi,0.25*pi,0.33333*pi,0.416667*pi,0.5*pi,0.583333*pi,0.66667*pi,0.75*pi,0.83333*pi,0.916667*pi,1.0*pi };

	TFile *f = new TFile("EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et06pt0_xFTest_2.root");
	TH1F *histo[spin][xF][phi];

	cout<<"pi0 yields for Blue Beam Up xF (0.05-0.1) or xF = 0.075"<<endl;
	cout<<"phi "<<" pi0sig "<<" pi0sigErr "<<" pi0sb "<<" pi0sbErr "<<endl;


	for(int i=0; i<phi; i++){
//Need to set variable names	
		histo[0][0][i] = (TH1F*)f->Get(Form("pi0M_BUp_xF0_phi%d",i));

		cout<<0.5*(phi_val[i] + phi_val[i+1])<<" "<<histo[0][0][i]->Integral(10,25)<<" "<<TMath::Sqrt(histo[0][0][i]->Integral(10,25))<<" "<<histo[0][0][i]->Integral(26,40)<<" "<<TMath::Sqrt(histo[0][0][i]->Integral(26,40))<<endl;

		outfile<<0.5*(phi_val[i] + phi_val[i+1])<<" "<<histo[0][0][i]->Integral(10,25)<<" "<<TMath::Sqrt(histo[0][0][i]->Integral(10,25))<<" "<<histo[0][0][i]->Integral(26,40)<<" "<<TMath::Sqrt(histo[0][0][i]->Integral(26,40))<<endl;
	}


	outfile.close();
}
