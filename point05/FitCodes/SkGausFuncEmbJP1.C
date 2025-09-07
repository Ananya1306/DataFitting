#include "TH1.h"
#include "TF1.h"
#include "TList.h"


//Skewed Gaussian Function
Double_t SkGausPeak(Double_t *x, Double_t *par) {

//par[0] = scale; par[1] = mean; par[2] = sigma; par[3] = skewness;
  return par[0]*exp(-0.5*pow((x[0]-par[1])/par[2],2))*ROOT::Math::normal_cdf(par[3]*x[0],1,0);

}

void SkGausFuncEmbJP1(){

   TFile *f = new TFile("../fitHists.root");
   TH1F *h = (TH1F*)f->Get("pi0MxFPhi_0");
   TH1F *hSig = (TH1F*)h->Clone("hSig");
   //for(int i=10; i<30; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   hSig->Draw(); 
  
  // TF1 *fsg = new TF1("fsg",SkGausPeak,0.09,0.2,4);//pi0MxFPhi_0 fit function 
   TF1 *fsg = new TF1("fsg",SkGausPeak,0.09,0.2,4);//pi0MxFPhi_1 fit function 
  //fsg->SetParameters(2000,0.130,0.01,4); //pi0MxFPhi_0 fit initial parameters
  // fsg->SetParameters(2000,0.130,0.01,4); //pi0MxFPhi_1 fit initial parameters
    fsg->SetParameters(0.1,0.130,1,5); //pi0MxFPhi_2 fit initial parameters
  fsg->Draw("same");
  hSig->Fit(fsg,"R");

} 
