/// \file
/// \ingroup tutorial_fit
/// \notebook -js
/// Illustrates how to fit excluding points in a given range.
///
/// \macro_image
/// \macro_output
/// \macro_code
///
/// \author Rene Brun

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

Bool_t reject;
Double_t fRejWeibull(Double_t *x, Double_t *par)
{
    if (reject && x[0] > 0.07 && x[0] < 0.25) {
      TF1::RejectPoint();
      return 0;
   }
   return weibull_pdf(x, par);
}

void fitExclude_Weibull_Data() {
   
   //Get the Histogram

   TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi10");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   //hBack->Draw();

   TF1 *fw = new TF1("fw",fRejWeibull,0,1,4);
   //fw->SetParameters(0.9,0.8,0.03,400);
   fw->SetParameters(0.3,1.5,0.05,450);
   //fit only the linear background excluding the signal area
   reject = kTRUE;
   hBack->Fit(fw,"0");
   reject = kFALSE;
   //store 2 separate functions for visualization
   TF1 *fleft = new TF1("fleft",fRejWeibull,0,0.087,4);
   fleft->SetParameters(fw->GetParameters());
   hBack->GetListOfFunctions()->Add(fleft);
   gROOT->GetListOfFunctions()->Remove(fleft);
   TF1 *fright = new TF1("fright",fRejWeibull,0.25,1.0,4);
   fright->SetParameters(fw->GetParameters());
   hBack->GetListOfFunctions()->Add(fright);
   gROOT->GetListOfFunctions()->Remove(fright);
   hBack->Draw("ep");
   gStyle->SetOptFit(1011);
}
