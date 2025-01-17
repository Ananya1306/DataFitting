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

   TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
  TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF2_phi10");

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,1.0,7);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
/*   fitFcn->SetParameter(0,7.97643e+01); //levy amplitude
   fitFcn->SetParameter(1,7.04444e-02); //levy scale
   fitFcn->SetParameter(2,2.96776e-02);  //levy location
   fitFcn->SetParameter(3,1.89946e+03); //skgaus amplitude
   fitFcn->SetParameter(4,1.22966e-01); //skgaus mean
   fitFcn->SetParameter(5,4.13624e-02); //skgaus sigma
   fitFcn->SetParameter(6,1.58433e+01); //skgaus skewness   
*/  
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
   fitFcn->SetParameter(0,6.56128e+01); //levy amplitude
   fitFcn->SetParameter(1,1.80251e-01); //levy scale
   fitFcn->SetParameter(2,5.31019e-02);  //levy location
   fitFcn->SetParameter(3,1.69577e+03); //skgaus amplitude
   fitFcn->SetParameter(4,1.31567e-01); //skgaus mean
   fitFcn->SetParameter(5,4.56549e-02); //skgaus sigma
   fitFcn->SetParameter(6,1.06439e+00); //skgaus skewness 



   h->Draw("ep");
   h->Fit("fitFcn","V+","ep");
   fitFcn->Draw("same");

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
