#include "TH1.h"
#include "TF1.h"
#include "TList.h"


//Skewed Gaussian Function
Double_t SkGausPeak(Double_t *x, Double_t *par) {

  //return (0.5*par[0]*par[1]/TMath::Pi()) /
  //TMath::Max( 1.e-10,(x[0]-par[2])*(x[0]-par[2])
  //+ .25*par[1]*par[1]); //Landau Peak

  //return par[0]*exp(-0.5*(pow(((x[0]-par[1])/(par[2]*(1+(par[3]*(x[0]-par[1]))))),2)));

  return par[0]*exp(-0.5*pow((x[0]-par[1])/par[2],2))*ROOT::Math::normal_cdf(par[3]*x[0],1,0);

}

void SkGausFunc(){

   TFile *f = new TFile("../BothTreeOutAll_XfTestpoint05.root");
   TH1F *h = (TH1F*)f->Get("pi0MxFPhi_0");
   TH1F *hSig = (TH1F*)h->Clone("hSig");
   //for(int i=10; i<30; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   hSig->Draw(); 
  
   TF1 *fsg = new TF1("fsg",SkGausPeak,0.09,0.2,4); 
   fsg->SetParameters(2000,0.130,0.01,4); //Scale, Mean, Sigma, Skewness
    
  fsg->Draw("same");
  hSig->Fit(fsg,"R");

} 
