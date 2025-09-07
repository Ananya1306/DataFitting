#include "TH1.h"
#include "TF1.h"
#include "TList.h"


Double_t levy(Double_t *x, Double_t *par){
	
	 if (x[0] <= par[2]){
                   return 0.0;
           }

   return (par[0])*(sqrt(par[1]/2*TMath::Pi())*exp(-par[1]/(2*(x[0]-par[2])))*(1/(pow((x[0]-par[2]),1.5)))); //Levy Distribution

//par[0] is Ampltitude, par[1] is Scale, par[2] is location
}

void LevyFuncData(){

   TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi10");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   //The folllowing for loops are written in order to avoid the shoulder
   for(int i=0; i<8; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);} 
   for(int i=11; i<20; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);} 
   hBack->Draw(); 
  
   TF1 *fl = new TF1("fl",levy,0.06,1,3); 
   fl->SetParameters(50,0.1,0.06); //pi0M_BDown_xF0_phi10 fit initial parameters
   //fl->SetParameters(50,0.1,0.06); //pi0M_BDown_xF1_phi10 fit initial parameters
   fl->Draw("same");
   hBack->Fit(fl,"R");

   double amplitude = fl->GetParameter(0);
   double scale = fl->GetParameter(1);
   double location = fl->GetParameter(2);

   TF1 *flfit = new TF1("fwfit",levy,0,1,4); flfit->SetParameters(amplitude,scale,location);
  TCanvas *c2 = new TCanvas(); hBackFit->Draw("lpe"); flfit->Draw("same");

} 
