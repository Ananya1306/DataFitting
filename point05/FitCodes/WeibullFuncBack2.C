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


void WeibullFuncBack2(){

   TFile *f = new TFile("../BothTreeOutAll_XfTestpoint05.root");
   TH1F *h = (TH1F*)f->Get("pi0MxFPhi_0");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   for(int i=11; i<30; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   hBack->Draw(); 
  
//for xF0
   TF1 *fw = new TF1("fw",weibull_pdf,0.25,1,4); 
   //fw->SetParameters(0.9,1,0.04,300); //Shape, scale, location, amplitude
   fw->SetParameters(0.8,1.4,0.06,295);

/*   TF1 *fw = new TF1("fw",weibull_pdf,0,1,4);  
   fw->SetParameters(0.8,1,0.02,350); //Shape, scale, location, amplitude
*/    
  fw->Draw("same");
  hBack->Fit(fw,"R");

} 
