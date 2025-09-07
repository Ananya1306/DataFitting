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

// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  return weibull_pdf(x,par) + SkGausPeak(x,&par[4]);
}

void FullFit_Data_BBeam_modRange_xF0_Weibull(int phi_val){


   TFile *f = new TFile("BlueBeamAllxF0.root");
   TH1F *h = (TH1F*)f->Get(Form("pi0M_BAll_xF0_phi%d",phi_val));

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,0.4,8);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 Data

if(phi_val == 10 || phi_val == 14 || phi_val == 15){  
   fitFcn->SetParameter(0,0.861411); //weibull shape
   fitFcn->SetParameter(1,0.130309); //weibull scale
   fitFcn->SetParameter(2,0.0450138);  //weibull location
   fitFcn->SetParameter(3,29.2653); //weibull amplitude
   fitFcn->SetParameter(4,1466.52); //skgaus amplitude
   fitFcn->SetParameter(5, 0.147855); //skgaus mean
   fitFcn->SetParameter(6,0.0440151); //skgaus sigma
   fitFcn->SetParameter(7,1.10437); //skgaus skewness   
}
else if(phi_val == 22 || phi_val == 1 || phi_val == 17 || phi_val == 19){
   fitFcn->SetParameter(0,0.726123); //weibull shape
   fitFcn->SetParameter(1,0.244885); //weibull scale
   fitFcn->SetParameter(2,0.045011);  //weibull location
   fitFcn->SetParameter(3,48.1843); //weibull amplitude
   fitFcn->SetParameter(4,645.866); //skgaus amplitude
   fitFcn->SetParameter(5,0.145529); //skgaus mean
   fitFcn->SetParameter(6,0.0309976); //skgaus sigma
   fitFcn->SetParameter(7,3.68344); //skgaus skewness
}
else {

   fitFcn->SetParameter(0,0.581526); //weibull shape
   fitFcn->SetParameter(1,2.18422); //weibull scale
   fitFcn->SetParameter(2,0.0450332);  //weibull location
   fitFcn->SetParameter(3,325.377); //weibull amplitude
   fitFcn->SetParameter(4,1466.52); //skgaus amplitude
   fitFcn->SetParameter(5, 0.147855); //skgaus mean
   fitFcn->SetParameter(6,0.0440151); //skgaus sigma
   fitFcn->SetParameter(7,1.10437); //skgaus skewness 

}

   TCanvas *cAll = new TCanvas();
   h->Draw("ep");
   h->Fit("fitFcn","V+","ep"); //V+ for xF0, xF1 and xF3; RW for xF2
   fitFcn->Draw("same");

   TF1* fitFcnDraw = (TF1*)fitFcn->Clone("fitFcnDraw");
   fitFcnDraw->SetFillColor(kRed); 
   fitFcnDraw->Draw("SAME FC");
//improve the picture

   TF1 *backFcn = new TF1("backFcn",weibull_pdf,0.0,0.4,4);
   backFcn->SetLineColor(kRed);
   TF1 *signalFcn = new TF1("signalFcn",SkGausPeak,0.0,0.4,4);
   signalFcn->SetLineColor(kBlue);

   double back0 = fitFcn->GetParameter(0);
   double back1 = fitFcn->GetParameter(1);
   double back2 = fitFcn->GetParameter(2);
   double back3 = fitFcn->GetParameter(3);

   double scale = fitFcn->GetParameter(4);
   double mean = fitFcn->GetParameter(5);
   double sigma = fitFcn->GetParameter(6);
   double skew = fitFcn->GetParameter(7);
 
   //TCanvas *c2 = new TCanvas();

   //h->Draw("ep");
   backFcn->SetParameters(back0,back1,back2,back3);
   backFcn->Draw("same");

   signalFcn->SetParameters(scale,mean,sigma,skew);
   signalFcn->Draw("same");


    TFile *fUp = new TFile("BlueBeamUpxF0.root");
   TH1F *hUp = (TH1F*)fUp->Get(Form("pi0M_BUp_xF0_phi%d",phi_val));

   TF1 *fitFcnUp = new TF1("fitFcnUp",fitFunction,0.0,0.4,8);
   fitFcnUp->SetLineColor(kMagenta);

   //Parameters are set from the values received from the total fitFcn done above
//For xF0 Data
   fitFcnUp->SetParameter(0,fitFcn->GetParameter(0)); //weibull shape
   fitFcnUp->SetParameter(1,fitFcn->GetParameter(1)); //weibull scale
   fitFcnUp->SetParameter(2,fitFcn->GetParameter(2));  //weibull location
   fitFcnUp->SetParameter(3,fitFcn->GetParameter(3)); //weibull amplitude
   fitFcnUp->SetParameter(4,fitFcn->GetParameter(4)); //skgaus amplitude
   fitFcnUp->SetParameter(5,fitFcn->GetParameter(5)); //skgaus mean
   fitFcnUp->SetParameter(6,fitFcn->GetParameter(6)); //skgaus sigma
   fitFcnUp->SetParameter(7,fitFcn->GetParameter(7)); //skgaus skewness   

   //Only let the weibull amplitude and the Skewed Gaussian amplitude vary
   fitFcnUp->FixParameter(0,fitFcn->GetParameter(0));
   fitFcnUp->FixParameter(1,fitFcn->GetParameter(1));
   fitFcnUp->FixParameter(2,fitFcn->GetParameter(2));
  // fitFcnUp->FixParameter(3,fitFcn->GetParameter(3));
  // fitFcnUp->FixParameter(4,fitFcn->GetParameter(4));
   fitFcnUp->FixParameter(5,fitFcn->GetParameter(5));
   fitFcnUp->FixParameter(6,fitFcn->GetParameter(6));
   fitFcnUp->FixParameter(7,fitFcn->GetParameter(7));

   TCanvas *cUp = new TCanvas();
   hUp->Draw("ep");
   hUp->Fit("fitFcnUp","V+","ep"); //V+ for xF0, xF1 and xF3; RW for xF2
   fitFcnUp->Draw("same");

   TFile *fDown = new TFile("BlueBeamDownxF0.root");
   //TH1F *h = (TH1F*)f->Get("pi0M_YUp_xF3_phi0"); 
   TH1F *hDown = (TH1F*)fDown->Get(Form("pi0M_BDown_xF0_phi%d",phi_val));

   TF1 *fitFcnDown = new TF1("fitFcnDown",fitFunction,0.0,0.4,8);
   fitFcnDown->SetLineColor(kMagenta);

   //Parameters are set from the values received from the total fitFcn done above
//For xF0 Data
   fitFcnDown->SetParameter(0,fitFcn->GetParameter(0)); //weibull shape
   fitFcnDown->SetParameter(1,fitFcn->GetParameter(1)); //weibull scale
   fitFcnDown->SetParameter(2,fitFcn->GetParameter(2));  //weibull location
   fitFcnDown->SetParameter(3,fitFcn->GetParameter(3)); //weibull amplitude
   fitFcnDown->SetParameter(4,fitFcn->GetParameter(4)); //skgaus amplitude
   fitFcnDown->SetParameter(5,fitFcn->GetParameter(5)); //skgaus mean
   fitFcnDown->SetParameter(6,fitFcn->GetParameter(6)); //skgaus sigma
   fitFcnDown->SetParameter(7,fitFcn->GetParameter(7)); //skgaus skewness   

   //Only let the weibull amplitude and the Skewed Gaussian amplitude vary
   fitFcnDown->FixParameter(0,fitFcn->GetParameter(0));
   fitFcnDown->FixParameter(1,fitFcn->GetParameter(1));
   fitFcnDown->FixParameter(2,fitFcn->GetParameter(2));
  // fitFcnUp->FixParameter(3,fitFcn->GetParameter(3));
  // fitFcnUp->FixParameter(4,fitFcn->GetParameter(4));
   fitFcnDown->FixParameter(5,fitFcn->GetParameter(5));
   fitFcnDown->FixParameter(6,fitFcn->GetParameter(6));
   fitFcnDown->FixParameter(7,fitFcn->GetParameter(7));

   TCanvas *cDown = new TCanvas();
   hDown->Draw("ep");
   hDown->Fit("fitFcnDown","V+","ep"); //V+ for xF0, xF1 and xF3; RW for xF2
   fitFcnDown->Draw("same");

   ofstream outfile;
   outfile.open("Par_BB_xF0.txt",std::ios::app);
   outfile<<phi_val<<" "<<fitFcn->GetChisquare()/fitFcn->GetNDF()<<" "<<fitFcn->GetParameter(3)<<" "<<fitFcn->GetParError(3)<<" "<<fitFcn->GetParameter(4)<<" "<<fitFcn->GetParError(4)<<" "<<fitFcnUp->GetChisquare()/fitFcnUp->GetNDF()<<" "<<fitFcnUp->GetParameter(3)<<" "<<fitFcnUp->GetParError(3)<<" "<<fitFcnUp->GetParameter(4)<<" "<<fitFcnUp->GetParError(4)<<" "<<fitFcnDown->GetChisquare()/fitFcnDown->GetNDF()<<" "<<fitFcnDown->GetParameter(3)<<" "<<fitFcnDown->GetParError(3)<<" "<<fitFcnDown->GetParameter(4)<<" "<<fitFcnDown->GetParError(4)<<endl;
   outfile.close();

   gStyle->SetOptStat(0);
   gStyle->SetOptFit(1111);
} 
