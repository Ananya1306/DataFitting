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

Double_t levy(Double_t *x, Double_t *par){

         if (x[0] <= par[2]){
                   return 0.0;
           }

   return (par[0])*(sqrt(par[1]/2*TMath::Pi())*exp(-par[1]/(2*(x[0]-par[2])))*(1/(pow((x[0]-par[2]),1.5)))); //Levy Distribution

//par[0] is Ampltitude, par[1] is Scale, par[2] is location
}

// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  return weibull_pdf(x,par) + SkGausPeak(x,&par[4]) + levy(x, &par[8]);
}

void FullFitEmb_JP1_Weibull_Levy(){

   TFile *f = new TFile("../fitHists.root");
   TH1F *h = (TH1F*)f->Get("pi0MxFPhi_0");

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,1.0,11);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
   fitFcn->SetParameter(0,1.06056e+00); //weibull shape
   fitFcn->SetParameter(1,3.54694e-01); //weibull scale
   fitFcn->SetParameter(2,4.49980e-02);  //weibull location
   fitFcn->SetParameter(3,8.73695e-03); //weibull amplitude
   fitFcn->SetParameter(4,4.61557e-02); //skgaus amplitude
   fitFcn->SetParameter(5,1.36138e-01); //skgaus mean
   fitFcn->SetParameter(6,6.58167e-02); //skgaus sigma
   fitFcn->SetParameter(7,1.27916e+01); //skgaus skewness   
   fitFcn->SetParameter(8,-1.85181e-04); //levy amplitude  
   fitFcn->SetParameter(9,1.11362e-01); //levy scale
   fitFcn->SetParameter(10,2.77390e-01); //levy location
   h->Draw("ep");
   h->Fit("fitFcn","V+","ep");
   fitFcn->Draw("same");

 /*  TF1* fitFcnDraw = (TF1*)fitFcn->Clone("fitFcnDraw");
   fitFcnDraw->SetRange(0., 0.9);
   fitFcnDraw->SetFillColor(15);
   fitFcnDraw->SetFillStyle(3001);
   fitFcnDraw->Draw("SAME FC");
*/
//improve the picture
/*
   TF1 *backFcn = new TF1("backFcn",weibull_pdf,0.0,1.0,4);
   backFcn->SetLineColor(kRed);
   TF1 *signalFcn = new TF1("signalFcn",SkGausPeak,0.0,1.0,4);
   signalFcn->SetLineColor(kBlue);

   double back0 = fitFcn->GetParameter(0);
   double back1 = fitFcn->GetParameter(1);
   double back2 = fitFcn->GetParameter(2);
   double back3 = fitFcn->GetParameter(3);

   double scale = fitFcn->GetParameter(4);
   double mean = fitFcn->GetParameter(5);
   double sigma = fitFcn->GetParameter(6);
   double skew = fitFcn->GetParameter(7);

   backFcn->SetParameters(back0,back1,back2,back3);
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
*/
   gStyle->SetOptFit(1111);
} 
