#include "TH1.h"
#include "TF1.h"
#include "TList.h"


//Skewed Gaussian Function
Double_t SkGausPeak(Double_t *x, Double_t *par) {//par[0]=scale; par[1]=mean; par[2]=sigma; par[3]=skewness

  return par[0]*exp(-0.5*pow((x[0]-par[1])/par[2],2))*ROOT::Math::normal_cdf(par[3]*x[0],1,0);

}

double weibull_pdf(double *x, double *par) {
    if (x[0] <= par[2]) {//par[0]=shape; par[1]=scale; par[2]=location; par[3] = amplitude;
        return 0.0;
    }

    //double z = (x - par[2]) / par[1];
    return (par[3]) * (par[0] / par[1]) * pow(((x[0] - par[2]) / par[1]), par[0] - 1) * exp(-pow(((x[0] - par[2]) / par[1]), par[0]));
}

//Chebyshev background function
Double_t Cheb(Double_t *x, Double_t *par){
  return   ( par[0] + par[1]*x[0] + par[2]*(2*x[0]*x[0] - 1) + par[3]*(4*x[0]*x[0]*x[0] - 3*x[0]));

}

//Sum of Gaussain and Weibull for the Background
Double_t fitFunction(Double_t *x, Double_t *par) {
  //return SkGausPeak(x,par) + weibull_pdf(x,&par[4]) ;
    return SkGausPeak(x,par) + Cheb(x,&par[4]);
}


void SigPlusChebyBack(){

TFile *f = new TFile("../BlueBeam_xF5bins_hists_modRange.root");
   //TH1F *h = (TH1F*)f->Get("BlueBeam_xF0");
   TH1F *h = (TH1F*)f->Get("pi0M_BAll_xF2_phi0");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   for(int i=11; i<22; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,500);}
   hBack->Draw();

   TF1 *fw = new TF1("fw",Cheb,0,0.4,4);
   fw->SetParameters(0.6,3.0,0.1,100); //pi0M_BDown_xF2_phi10 fit initial parameter
   fw->Draw("same");
   hBack->Fit(fw,"V+");

   double shape = fw->GetParameter(0);
  double scale = fw->GetParameter(1);
  double location = fw->GetParameter(2);
  double amplitude = fw->GetParameter(3);

  TF1 *fwfit = new TF1("fwfit",Cheb,0,1,4); fwfit->SetParameters(shape,scale,location,amplitude);
  TCanvas *c2 = new TCanvas(); hBackFit->Draw("lpe"); fwfit->Draw("same");

  TH1F *hSig = (TH1F*)h->Clone("hSig");
   TF1 *fsg = new TF1("fsg",SkGausPeak,0.15,0.18,4);
  //fsg->SetParameters(2500,0.130,0.008,4); //pi0M_BDown_xF0_phi10 fit initial parameter
   //fsg->SetParameters(2500,0.130,0.008,4); //pi0M_BDown_xF1_phi10 fit initial parameter
  fsg->SetParameters(2000,0.13,0.03,4); //pi0M_BDown_xF2_phi10 fit initial parameter
  //fsg->SetParameter(0,1000); fsg->SetParameter(1,0.13); fsg->SetParameter(2,0.05); fsg->SetParameter(3,4);
  TCanvas *c3 = new TCanvas();
  fsg->Draw("same");
  hSig->Fit(fsg,"R");

  double gausAmp = fsg->GetParameter(0);
  double gausMean = fsg->GetParameter(1);
  double gausSigma = fsg->GetParameter(2);
  double gausSkew = fsg->GetParameter(3);

  TH1F *hAll = (TH1F*)h->Clone("hAll");
  TCanvas *c4 = new TCanvas();
  TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,0.4,8);
  fitFcn->SetParameter(4,shape);
  fitFcn->SetParameter(5,scale);
  fitFcn->SetParameter(6, location);
  fitFcn->SetParameter(7, amplitude);
  fitFcn->SetParameter(0, gausAmp);
  fitFcn->SetParameter(1, gausMean);
  fitFcn->SetParameter(2, gausSigma);
  fitFcn->SetParameter(3, gausSkew);

  hAll->Fit(fitFcn,"RW");

}
