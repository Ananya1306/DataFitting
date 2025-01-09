#include "TH1.h"
#include "TF1.h"
#include "TList.h"


Double_t levy(Double_t *x, Double_t *par){
	
	 if (x[0] <= par[2]){
                   return 0.0;
           }

   return (par[0])*(sqrt(par[1]/2*TMath::Pi())*exp(-par[1]/(2*(x[0]-par[2])))*(1/(pow((x[0]-par[2]),1.5)))); //Levy Distribution


}

void LevyFuncData(){

   TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi10");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   for(int i=8; i<30; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   hBack->Draw(); 
  
   TF1 *fl = new TF1("fl",levy,0,1,3); 
   fl->SetParameters(1600,0.05,0.001); //par[0] is Ampltitude, par[1] is Scale, par[2] is location
   fl->Draw("same");
   hBack->Fit(fl,"R");

} 
