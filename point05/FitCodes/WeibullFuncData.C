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


void WeibullFuncData(){

   TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF2_phi10");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   for(int i=10; i<30; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   hBack->Draw(); 
  
//for xF0
   TF1 *fw = new TF1("fw",weibull_pdf,0,1,4); 
   //fw->SetParameters(0.4,1.5,0.05,450); // pi0M_BDown_xF0_phi10 fit initial parameter
   //fw->SetParameters(10,15,0.06,800); //pi0M_BDown_xF1_phi10 fit initial parameter
   fw->SetParameters(10,15,0.06,800); //pi0M_BDown_xF2_phi10 fit initial parameter
   fw->Draw("same");
   hBack->Fit(fw,"R");

  double shape = fw->GetParameter(0);
  double scale = fw->GetParameter(1);
  double location = fw->GetParameter(2);
  double amplitude = fw->GetParameter(3);

  TF1 *fwfit = new TF1("fwfit",weibull_pdf,0,1,4); fwfit->SetParameters(shape,scale,location,amplitude);
  TCanvas *c2 = new TCanvas(); hBackFit->Draw("lpe"); fwfit->Draw("same");

} 
