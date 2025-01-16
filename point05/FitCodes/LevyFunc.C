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

void LevyFunc(){

   TFile *f = new TFile("../BothTreeOutAll_XfTestpoint05.root");
   TH1F *h = (TH1F*)f->Get("pi0MxFPhi_2");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   for(int i=10; i<30; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   hBack->Draw(); 
  
   TF1 *fl = new TF1("fl",levy,0,1,3); 
   //fl->SetParameters(1500,0.02,0.0); //pi0MxFPhi_0 fit initial parameters
   //fl->SetParameters(150,0.9,0.01); //pi0MxFPhi_1 fit initial parameters
   fl->SetParameters(150,0.9,0.005); //pi0MxFPhi_2 fit initial parameters
   fl->Draw("same");
   hBack->Fit(fl,"R");

   double amplitude = fl->GetParameter(0);
   double scale = fl->GetParameter(1);
   double location = fl->GetParameter(2);

   TF1 *flfit = new TF1("fwfit",levy,0,1,4); flfit->SetParameters(amplitude,scale,location);
  TCanvas *c2 = new TCanvas(); hBackFit->Draw("lpe"); flfit->Draw("same");



} 
