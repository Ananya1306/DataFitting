#include "TH1.h"
#include "TF1.h"
#include "TList.h"

double weibull_pdf(double *x, double *par) {
    if (x[0] <= par[2]) {//par[0]=shape; par[1]=scale; par[2]=location; par[3] = amplitude;
        return 0.0;
    }

    //double z = (x - par[2]) / par[1];
    return (par[3]) * (par[0] / par[1]) * pow(((x[0] - par[2]) / par[1]), par[0] - 1) * exp(-pow(((x[0] - par[2]) / par[1]), par[0]));
}


void WeibullFunc(){

   TFile *f = new TFile("../BothTreeOutAll_XfTestpoint05.root");
   TH1F *h = (TH1F*)f->Get("pi0MxFPhi_0");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   //The following for loops are written in order to avoid the shoulder
   for(int i=0; i<8; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   for(int i=11; i<20; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   hBack->Draw(); 
  
//for xF0
   TF1 *fw = new TF1("fw",weibull_pdf,0.06,1,4); 
   fw->SetParameters(0.8,1.4,0.06,295); //pi0MxFPhi_0 fit initial parameter
    //fw->SetParameters(2.5,1.5,0.06,400); //pi0MxFPhi_1 fit initial parameter
    //fw->SetParameters(2.5,1.5,0.01,400); //pi0MxFPhi_2 fit initial parameter
  fw->Draw("same");
  hBack->Fit(fw,"R");

  double shape = fw->GetParameter(0);
  double scale = fw->GetParameter(1);
  double location = fw->GetParameter(2);
  double amplitude = fw->GetParameter(3);

  TF1 *fwfit = new TF1("fwfit",weibull_pdf,0,1,4); fwfit->SetParameters(shape,scale,location,amplitude);
  TCanvas *c2 = new TCanvas(); hBackFit->Draw("lpe"); fwfit->Draw("same");

} 
