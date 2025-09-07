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

//   TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
//   TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi10");

   TFile *f = new TFile("BlueBeamAllxF4.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BAll_xF4_phi0");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   for(int i=13; i<22; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,500);}
   hBack->Draw(); 
  
//for xF0 and xF1
   TF1 *fw = new TF1("fw",weibull_pdf,0,0.4,4); 
   //fw->SetParameters(0.8,2.0,0.05,500); // pi0M_BDown_xF0_phi10 fit initial parameter
   //fw->SetParameters(0.08,0.5,0.08,100); //pi0M_BDown_xF1_phi10 fit initial parameter
//for xF2 define the ranges of the function
   //TF1 *fw = new TF1("fw",weibull_pdf,0.2,1,4);  
//   fw->SetParameters(1.10732,2.24815,0.0642311,1274); //pi0M_BDown_xF2_phi10 fit initial parameter
  fw->SetParameters(1.75553,0.190207,0.155009,813.631);  
 fw->Draw("same");
   hBack->Fit(fw,"V+");

  double shape = fw->GetParameter(0);
  double scale = fw->GetParameter(1);
  double location = fw->GetParameter(2);
  double amplitude = fw->GetParameter(3);

  TF1 *fwfit = new TF1("fwfit",weibull_pdf,0,1,4); fwfit->SetParameters(shape,scale,location,amplitude);
  TCanvas *c2 = new TCanvas(); hBackFit->Draw("lpe"); fwfit->Draw("same");

} 
