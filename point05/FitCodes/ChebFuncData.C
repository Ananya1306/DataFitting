#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"

//Chebyshev background function
Double_t Cheb(Double_t *x, Double_t *par){
  return   ( par[0] + par[1]*x[0] + par[2]*(2*x[0]*x[0] - 1) + par[3]*(4*x[0]*x[0]*x[0] - 3*x[0]));

}

void ChebFuncData(){

TFile *f = new TFile("../BlueBeam_xF5bins_hists_modRange.root");
   //TH1F *h = (TH1F*)f->Get("BlueBeam_xF0");
   TH1F *h = (TH1F*)f->Get("pi0M_BAll_xF0_phi0");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   for(int i=11; i<22; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,500);}
   hBack->Draw();

//for xF0 and xF1
   //User defined function
   TF1 *fw = new TF1("fw",Cheb,0.0,0.4,5);
   fw->SetParameters(-1500,3700,-1500,1000,300); // pi0M_BDown_xF0_phi10 fit initial parameter
   hBack->Draw();
   fw->Draw("same");
   hBack->Fit(fw,"V+");  
 
/* Predefined function
   TF1 *f_cheb5 = new TF1("f_cheb5", "cheb3", 0.0,0.4);;
    f_cheb5->SetLineColor(kRed);
*/

//fw->SetParameters(10,15,0.06,800); //pi0M_BDown_xF1_phi10 fit initial parameter
//for xF2 define the ranges of the function
   //TF1 *fw = new TF1("fw",weibull_pdf,0.2,1,4);  
  // fw->SetParameters(0.6,3.0,0.1,100); //pi0M_BDown_xF2_phi10 fit initial parameter
/*   	hBack->Draw();
	f_cheb5->Draw("same");
   hBack->Fit(f_cheb5,"V+");

   std::cout << "Fitted Parameters:" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << "p[" << i << "] = " << f_cheb5->GetParameter(i) << std::endl;
    }
*/
/*
  double shape = fw->GetParameter(0);
  double scale = fw->GetParameter(1);
  double location = fw->GetParameter(2);
  double amplitude = fw->GetParameter(3);

  TF1 *fwfit = new TF1("fwfit",Cheb,0,1,4); fwfit->SetParameters(shape,scale,location,amplitude);
  TCanvas *c2 = new TCanvas(); hBackFit->Draw("lpe"); fwfit->Draw("same");
*/

}
