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

Bool_t reject;
Double_t fReject(Double_t *x, Double_t *par)
{
    if (reject && x[0] > 0.00 && x[0] < 0.11) { //upper limit 0.09 gives one kind f analysis method, while upper limit 0.11 gives anoher kind of analysis method
      TF1::RejectPoint();
      return 0;
   }
   return fitFunction(x, par);
}


void FullFitEmbedding_Levy(){

   TFile *f = new TFile("../BothTreeOutAll_XfTestpoint05.root");
   TH1F *h = (TH1F*)f->Get("pi0MxFPhi_0");

   TF1 *fitFcn = new TF1("fitFcn",fReject,0.0,1.0,7);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 fit    

   fitFcn->SetParameter(0,1.07227e+02); //levy amplitude
   fitFcn->SetParameter(1,3.93470e-01); //levy scale
   fitFcn->SetParameter(2,-9.58480e-04);  //levy location
   fitFcn->SetParameter(3,1.20102e+03); //skgaus amplitude
   fitFcn->SetParameter(4,1.46149e-01); //skgaus mean
   fitFcn->SetParameter(5,4.99566e-02); //skgaus sigma
   fitFcn->SetParameter(6,-2.56764e-01); //skgaus skewness   
  
//For xF1 fit
/*  
   fitFcn->SetParameter(0,1.37715e+02); //levy amplitude
   fitFcn->SetParameter(1,5.02110e-01); //levy scale
   fitFcn->SetParameter(2,5.55608e-03);  //levy location
   fitFcn->SetParameter(3,1.08243e+03); //skgaus amplitude
   fitFcn->SetParameter(4,1.42599e-01); //skgaus mean
   fitFcn->SetParameter(5,-4.59123e-02); //skgaus sigma
   fitFcn->SetParameter(6,5.53844e+00); //skgaus skewness 
*/
//For xF2 fit
/*   fitFcn->SetParameter(0,1.68947e+02); //levy amplitude
   fitFcn->SetParameter(1,6.22293e-01); //levy scale
   fitFcn->SetParameter(2,1.29054e-02);  //levy location
   fitFcn->SetParameter(3,1.52321e+03); //skgaus amplitude
   fitFcn->SetParameter(4,1.51929e-01); //skgaus mean
   fitFcn->SetParameter(5,4.41792e-02); //skgaus sigma
   fitFcn->SetParameter(6,-1.74061e-01); //skgaus skewnes
*/
   //h->Draw("ep");
   reject=kTRUE; h->Fit("fitFcn","V+","ep"); reject=kFALSE;
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
  // backFcn->Draw("same");

   signalFcn->SetParameters(scale,mean,sigma,skew);
 //  signalFcn->Draw("same");

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
