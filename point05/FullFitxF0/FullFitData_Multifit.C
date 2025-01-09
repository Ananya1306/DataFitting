#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"

//Skewed Gaussian Function
Double_t SkGausPeak(Double_t *x, Double_t *par) {


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
Double_t fitBackground(Double_t *x, Double_t *par) {
  return weibull_pdf(x,par) + SkGausPeak(x,&par[4]);
}

// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  return fitBackground(x,par) + SkGausPeak(x,&par[8]);
}

void FullFitData_Multifit(){

   TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi10");

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,1.0,12);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
   fitFcn->SetParameter(0,9.54518e-01); //weibull shape
   fitFcn->SetParameter(1,3.45811e-01); //weibull scale
   fitFcn->SetParameter(2,2.50755e-02);  //weibull location
   fitFcn->SetParameter(3,1.54742e+02); //weibull amplitude

   fitFcn->SetParameter(4,1.14003e+03); //bg skgaus amplitude
   fitFcn->SetParameter(5,5.91274e-02); //bg skgaus mean
   fitFcn->SetParameter(6,1.37819e-02); //bg skgaus sigma
   fitFcn->SetParameter(7,5.17982e+01); //bg skgaus skewness   
  
   fitFcn->SetParameter(8,1.89946e+03); //sig skgaus amplitude
   fitFcn->SetParameter(9,1.22966e-01); //sig skgaus mean
   fitFcn->SetParameter(10,4.13624e-02); //sig skgaus sigma
   fitFcn->SetParameter(11,1.58433e+01); //sig skgaus skewness
 
   //h->Draw();
   h->Fit("fitFcn","V+","ep");
   fitFcn->Draw("same");

//improve the picture

   TF1 *backFcn = new TF1("backFcn",fitBackground,0.0,1.0,8);
   backFcn->SetLineColor(kGreen);
   TF1 *signalFcn = new TF1("signalFcn",SkGausPeak,0.0,1.0,4);
   signalFcn->SetLineColor(kBlue);

   double back0 = fitFcn->GetParameter(0);
   double back1 = fitFcn->GetParameter(1);
   double back2 = fitFcn->GetParameter(2);
   double back3 = fitFcn->GetParameter(3);
   double back4 = fitFcn->GetParameter(4);
   double back5 = fitFcn->GetParameter(5);
   double back6 = fitFcn->GetParameter(6);
   double back7 = fitFcn->GetParameter(7);

   double scale = fitFcn->GetParameter(8);
   double mean = fitFcn->GetParameter(9);
   double sigma = fitFcn->GetParameter(10);
   double skew = fitFcn->GetParameter(11);

   backFcn->SetParameters(back0,back1,back2,back3,back4,back5,back6,back7);
   backFcn->Draw("same");

   signalFcn->SetParameters(scale,mean,sigma,skew);
   signalFcn->Draw("same");

   //draw the legend
   TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
   legend->SetTextFont(72);
   legend->SetTextSize(0.04);
   legend->AddEntry(h,"Data","lpe");
   legend->AddEntry(backFcn,"Background fit","l");
   legend->AddEntry(signalFcn,"Signal fit","l");
   legend->AddEntry(fitFcn,"Global Fit","l");
   legend->Draw();


} 
