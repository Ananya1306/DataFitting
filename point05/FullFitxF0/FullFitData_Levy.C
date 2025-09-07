#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"

//Skewed Gaussian Function
Double_t SkGausPeak(Double_t *x, Double_t *par) {


  return par[0]*exp(-0.5*pow((x[0]-par[1])/par[2],2))*ROOT::Math::normal_cdf(par[3]*x[0],1,0);

}

Double_t levy(Double_t *x, Double_t *par){

         if (x[0] <= par[2]){
                   return 0.0;
           }

   return (par[0])*(sqrt(par[1]/2*TMath::Pi())*exp(-par[1]/(2*(x[0]-par[2])))*(1/(pow((x[0]-par[2]),1.5)))); //Levy Distribution
}

// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  return levy(x,par) + SkGausPeak(x,&par[3]);
}

void FullFitData_Levy(){

   TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et06pt0_xFTest_1.root");
  TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi20");

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,1.0,7);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
   fitFcn->SetParameter(0,1.18289e+01); //levy amplitude
   fitFcn->SetParameter(1,9.87408e-02); //levy scale
   fitFcn->SetParameter(2,2.81545e-02);  //levy location
   fitFcn->SetParameter(3,3.55116e+02); //skgaus amplitude
   fitFcn->SetParameter(4,1.43528e-01); //skgaus mean
   fitFcn->SetParameter(5,-3.71306e-02); //skgaus sigma
   fitFcn->SetParameter(6,5.59420e+01); //skgaus skewness   
  
//For xF1 Data
/*   fitFcn->SetParameter(0,8.95512e+01); //levy amplitude
   fitFcn->SetParameter(1,2.08074e-01); //levy scale
   fitFcn->SetParameter(2,2.47977e-02);  //levy location
   fitFcn->SetParameter(3,2.44366e+03); //skgaus amplitude
   fitFcn->SetParameter(4,1.28733e-01); //skgaus mean
   fitFcn->SetParameter(5,4.09378e-02); //skgaus sigma
   fitFcn->SetParameter(6,1.44953e+01); //skgaus skewness 
*/
//For xF2 Data
/*   fitFcn->SetParameter(0,6.56128e+01); //levy amplitude
   fitFcn->SetParameter(1,1.80251e-01); //levy scale
   fitFcn->SetParameter(2,5.31019e-02);  //levy location
   fitFcn->SetParameter(3,1.69577e+03); //skgaus amplitude
   fitFcn->SetParameter(4,1.31567e-01); //skgaus mean
   fitFcn->SetParameter(5,4.56549e-02); //skgaus sigma
   fitFcn->SetParameter(6,1.06439e+00); //skgaus skewness 
*/


   h->Draw("ep");
   h->Fit("fitFcn","V+","ep");
   fitFcn->Draw("same");

   TF1* fitFcnDraw = (TF1*)fitFcn->Clone("fitFcnDraw");
   fitFcnDraw->SetRange(0., 0.2);
   fitFcnDraw->SetFillColor(14); 
   fitFcnDraw->SetFillStyle(3001);
   fitFcnDraw->Draw("SAME FC");

   TF1* fitFcnDraw1 = (TF1*)fitFcn->Clone("fitFcnDraw1");
   fitFcnDraw1->SetRange(0.18, 0.5); 
   fitFcnDraw1->SetFillColor(17);
   fitFcnDraw1->SetFillStyle(3001);
   fitFcnDraw1->Draw("SAME FC");

//improve the picture

   TF1 *backFcn = new TF1("backFcn",levy,0.0,1.0,3);
   backFcn->SetLineColor(kRed);
   TF1 *signalFcn = new TF1("signalFcn",SkGausPeak,0.0,1.0,4);
   signalFcn->SetLineColor(kBlue);

   double back0 = fitFcn->GetParameter(0);
   double back1 = fitFcn->GetParameter(1);
   double back2 = fitFcn->GetParameter(2);
   //double back3 = fitFcn->GetParameter(3);

   double scale = fitFcn->GetParameter(3);
   double mean = fitFcn->GetParameter(4);
   double sigma = fitFcn->GetParameter(5);
   double skew = fitFcn->GetParameter(6);
 
   //h->Draw("ep");
   backFcn->SetParameters(back0,back1,back2);
   backFcn->Draw("same");

   signalFcn->SetParameters(scale,mean,sigma,skew);
   signalFcn->Draw("same");

   TF1* backFcnDraw = (TF1*)backFcn->Clone("backFcnDraw");
   backFcnDraw->SetRange(0.0, 0.18);
   backFcnDraw->SetFillColor(2);
   backFcnDraw->SetFillStyle(3344);
   backFcnDraw->Draw("SAME FC");

   TF1* backFcnDraw1 = (TF1*)backFcn->Clone("backFcnDraw1");
   backFcnDraw1->SetRange(0.18, 0.5);
   backFcnDraw1->SetFillColor(2);
   backFcnDraw1->SetFillStyle(3001);
   backFcnDraw1->Draw("SAME FC");

   //draw the legend
   TLegend *legend=new TLegend(0.4,0.55,0.88,0.95);
   legend->SetTextFont(72);
   legend->SetTextSize(0.04);
   legend->AddEntry(h,"Data","lpe");
   legend->AddEntry(backFcn,"Background fit","l");
   legend->AddEntry(signalFcn,"Signal fit","l");
   legend->AddEntry(fitFcn,"Global Fit","l");
  // legend->AddEntry(fitFcnDraw,"Signal Region","f");
  // legend->AddEntry(fitFcnDraw1,"Sideband Region","f");
   legend->AddEntry(backFcnDraw,"BG in Signal Region","f");
   legend->AddEntry(backFcnDraw1,"BG in Sideband Region","f");
   legend->Draw();

gStyle->SetOptStat(0);
gStyle->SetOptFit(1111);
} 
