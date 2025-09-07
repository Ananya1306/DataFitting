#include "TH1.h"
#include "TF1.h"
#include "TList.h"


Double_t cheby(Double_t *x, Double_t *par){


  // return (par[0])*(sqrt(par[1]/2*TMath::Pi())*exp(-par[1]/(2*(x[0]-par[2])))*(1/(pow((x[0]-par[2]),1.5)))); //Levy Distribution

     return ( par[0]*1 + par[1]*x[0] + par[2]*( (2*pow(x[0],2)) - 1) + par[3]*( (4*pow(x[0],3)) - 3*x[0] )  );	

}

void ChebyFuncData(){

   TFile *f = new TFile("../BlueBeam_xF_hists_modRange.root");
   TH1F *h = (TH1F*)f->Get("BlueBeam_xF0");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   for(int i=11; i<20; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,500);} //for xF0 and xF1 i starts from 8
   hBack->Draw();

   TF1 *fl = new TF1("fl",cheby,0,0.4,4);
   fl->SetParameters(1,-1,1,-1); //pi0M_BDown_xF0_phi10 fit initial parameters 
   //fl->SetParameters(900,0.09,0.02); ////pi0M_BDown_xF1_phi10 fit initial parameters 
   //fl->SetParameters(50,0.1,0.06); ////pi0M_BDown_xF2_phi10 fit initial parameters
   fl->Draw("same");
   hBack->Fit(fl,"R");	

}
