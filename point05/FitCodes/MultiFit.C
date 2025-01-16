#include "TH1.h"
#include "TF1.h"
#include "TList.h"


//Skewed Gaussian Function
Double_t SkGausPeak(Double_t *x, Double_t *par) {

//par[0]=scale; par[1]=mean; par[2]=sigma; par[3]=skewness
  return par[0]*exp(-0.5*pow((x[0]-par[1])/par[2],2))*ROOT::Math::normal_cdf(par[3]*x[0],1,0);

}

double weibull_pdf(double *x, double *par) {
    if (x[0] <= par[2]) {//par[0]=shape; par[1]=scale; par[2]=location; par[3] = amplitude;
        return 0.0;
    }

    //double z = (x - par[2]) / par[1];
    return (par[3]) * (par[0] / par[1]) * pow(((x[0] - par[2]) / par[1]), par[0] - 1) * exp(-pow(((x[0] - par[2]) / par[1]), par[0]));
}

//Sum of Gaussain and Weibull for the Background
Double_t fitFunction(Double_t *x, Double_t *par) {
  return weibull_pdf(x,par) + SkGausPeak(x,&par[4]);
}

Bool_t reject;
Double_t fRejBack(Double_t *x, Double_t *par)
{
    if (reject && x[0] > 0.08 && x[0] < 0.3) {
      TF1::RejectPoint();
      return 0;
   }
   return fitFunction(x, par);
}


void MultiFit(){

   TFile *f = new TFile("../BothTreeOutAll_XfTestpoint05.root");
   TH1F *h = (TH1F*)f->Get("pi0MxFPhi_1");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   for(int i=10; i<30; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   hBack->Draw("lpe");

   TF1 *fsg = new TF1("fsg",SkGausPeak,0.0,0.08,4);
   //fsg->SetParameters(300,0.015,0.001,4); //pi0MxFPhi_0 fit initial parameter
   fsg->SetParameters(300,0.07,0.02,4); //pi0MxFPhi_1 fit initial parameter
   fsg->SetLineColor(kBlue); fsg->Draw("same");
   hBack->Fit(fsg,"R");
   double GausScale = fsg->GetParameter(0); double Mean = fsg->GetParameter(1); 
   double Sigma = fsg->GetParameter(2); double Skewness = fsg->GetParameter(3); 

   //TF1 *fw = new TF1("fw",weibull_pdf,0.250,1,4);///pi0MxFPhi_0 fit initial function
   //fw->SetParameters(0.8,1.4,0.06,295); //pi0MxFPhi_0 fit initial parameter
    TF1 *fw = new TF1("fw",weibull_pdf,0.08,1,4);//pi0MxFPhi_1 fit initial function
   fw->SetParameters(0.8,1.4,0.06,295); //pi0MxFPhi_1 fit initial parameter
   fw->SetLineColor(kBlue); fw->Draw("same");
   hBack->Fit(fw,"R+");   
   double Shape = fw->GetParameter(0); double WeibullScale = fw->GetParameter(1);
   double Location = fw->GetParameter(2); double Amplitude = fw->GetParameter(3);


//   TCanvas *c2 = new TCanvas();
   TF1 *ftot = new TF1("ftot",fitFunction,0,1,8);
   ftot->SetParameters(Shape,WeibullScale,Location,Amplitude,GausScale, Mean,Sigma,Skewness);
   ftot->SetLineColor(kRed); //hBack->Fit(ftot,"R+");

   TCanvas *c2 = new TCanvas(); hBackFit->Draw("lpe"); fsg->Draw("same"); fw->Draw("same");
   TF1 *ftotFit = new TF1("ftotFit",fRejBack,0,1,8);
   ftotFit->SetParameters(Shape,WeibullScale,Location,Amplitude,GausScale, Mean,Sigma,Skewness);
   reject = kTRUE; hBackFit->Fit(ftotFit,"R+"); reject = kFALSE;
}

