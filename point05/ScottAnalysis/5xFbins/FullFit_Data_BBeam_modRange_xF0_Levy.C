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

void FullFit_Data_BBeam_modRange_xF0_Levy(){

//   std::ofstream outfile;
//   outfile.open("../sig_sb_range0/BlueBeam/fSig_Levy.txt", std::ios::app);

   //sig_sb region 0
   //double sig_Low = 0.10; double sig_High = 0.25; double sb_Low = 0.25; double sb_High = 0.40;  
   //sig_sb region 1
   //double sig_Low = 0.10; double sig_High = 0.20; double sb_Low = 0.20; double sb_High = 0.40;
   //sig_sb region 2
   //double sig_Low = 0.09; double sig_High = 0.17; double sb_Low = 0.17; double sb_High = 0.40;
   //sig_sb region 3
   double sig_Low = 0.11; double sig_High = 0.21; double sb_Low = 0.21; double sb_High = 0.40;

/*	double sig_Low, sig_High, sb_Low, sb_High;
        if(range_input == 0){sig_Low = 0.10; sig_High = 0.25; sb_Low = 0.25; sb_High = 0.40;}
        else if(range_input == 1){sig_Low = 0.10; sig_High = 0.20; sb_Low = 0.20; sb_High = 0.40;}
        else if(range_input == 2){sig_Low = 0.09; sig_High = 0.17; sb_Low = 0.17; sb_High = 0.40;}
        else if(range_input == 3){sig_Low = 0.11; sig_High = 0.21; sb_Low = 0.21; sb_High = 0.40;}
*/
  //TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et06pt0_xFTest_1.root");
  //TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi20");

	//TH1F *h = nullptr;
   TFile *f = new TFile("BlueBeamAllxF0.root.root");
   //TH1F* h = (TH1F*)f->Get("pi0M_YUp_xF3_phi0");
   TH1F* h = (TH1F*)f->Get("pi0M_BAll_xF0_phi0");

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,0.4,7);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
//if(range_input == 0){   
   fitFcn->SetParameter(0,366.558); //levy amplitude
   fitFcn->SetParameter(1,0.0555105); //levy scale
   fitFcn->SetParameter(2,0.0359502);  //levy location
   fitFcn->SetParameter(3,17382.6); //skgaus amplitude
   fitFcn->SetParameter(4,0.143537); //skgaus mean
   fitFcn->SetParameter(5,0.0410467); //skgaus sigma
   fitFcn->SetParameter(6,14.1532); //skgaus skewness   

//}  
//For xF1 Data
//else if(range_input == 1){
/*   fitFcn->SetParameter(0,5.02849e+03); //levy amplitude
   fitFcn->SetParameter(1,4.82356e-01); //levy scale
   fitFcn->SetParameter(2,4.90052e-03);  //levy location
   fitFcn->SetParameter(3,1.49866e+05); //skgaus amplitude
   fitFcn->SetParameter(4,1.46147e-01); //skgaus mean
   fitFcn->SetParameter(5,4.24205e-02); //skgaus sigma
   fitFcn->SetParameter(6,1.02038e+00); //skgaus skewness ); //skgaus amplitude
*/
//}
//For xF2 Data
//else if(range_input == 2){
/*   fitFcn->SetParameter(0,5399.85); //levy amplitude
   fitFcn->SetParameter(1,0.152805); //levy scale
   fitFcn->SetParameter(2,0.0678691);  //levy location
   fitFcn->SetParameter(3,6.95451e+04); //skgaus amplitude
   fitFcn->SetParameter(4,1.49289e-01); //skgaus mean
   fitFcn->SetParameter(5,6.58667e-02); //skgaus sigma
   fitFcn->SetParameter(6,1.38106e+01); //skgaus skewness 
*/
//}
//For xF3 Data
//else if(range_input == 3){ 
/*  fitFcn->SetParameter(0,548.11); //levy amplitude
   fitFcn->SetParameter(1,0.171663); //levy scale
   fitFcn->SetParameter(2,0.164373);  //levy location
   fitFcn->SetParameter(3,5581.98); //skgaus amplitude
   fitFcn->SetParameter(4,0.190204); //skgaus mean
   fitFcn->SetParameter(5,0.0477432); //skgaus sigma
   fitFcn->SetParameter(6,13.0748); //skgaus skewness 
*/
//}


   h->Draw("ep");
   h->Fit("fitFcn","V+","ep"); //V+ for xF0, xF1 and xF3; RW for xF2
   fitFcn->Draw("same");

   TF1* fitFcnDraw = (TF1*)fitFcn->Clone("fitFcnDraw");
   fitFcnDraw->SetRange(sig_Low, sig_High);
   fitFcnDraw->SetFillColor(kRed); 
   fitFcnDraw->SetFillStyle(3001);
   fitFcnDraw->Draw("SAME FC");

   TF1* fitFcnDraw1 = (TF1*)fitFcn->Clone("fitFcnDraw1");
   fitFcnDraw1->SetRange(sb_Low, sb_High); 
   fitFcnDraw1->SetFillColor(kGray);
   fitFcnDraw1->SetFillStyle(3001);
   fitFcnDraw1->Draw("SAME FC");

//improve the picture

   TF1 *backFcn = new TF1("backFcn",levy,0.0,0.4,3);
   backFcn->SetLineColor(kRed);
   TF1 *signalFcn = new TF1("signalFcn",SkGausPeak,0.0,0.4,4);
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
   //signalFcn->SetRange(sig_Low/100.0, sig_High/100.0);
   signalFcn->SetFillColor(kBlue);
   signalFcn->SetFillStyle(3344);
   signalFcn->Draw("SAME FC");

   double fsig_sig = signalFcn->Integral(sig_Low,sig_High)/fitFcn->Integral(sig_Low,sig_High);
   double fsig_sb = signalFcn->Integral(sb_Low,sb_High)/fitFcn->Integral(sb_Low, sb_High);

   double fbg_sig = backFcn->Integral(sig_Low,sig_High)/fitFcn->Integral(sig_Low,sig_High);
   double fbg_sb = backFcn->Integral(sb_Low,sb_High)/fitFcn->Integral(sb_Low,sb_High);


   cout<<fsig_sig<<" "<<fsig_sb<<endl;
//   outfile<<fsig_sig<<" "<<fsig_sb<<endl;
//   outfile.close();

//   cout<<"hist integral = "<<h->Integral(10,25)<<endl;
//   cout<<"num = "<<num_fsig<<" den = "<<den_ffit<<" fsig_sig = "<<num_fsig/den_ffit<<endl;


   TF1* backFcnDraw = (TF1*)backFcn->Clone("backFcnDraw");
/*   backFcnDraw->SetRange(sig_Low/100.0, sig_High/100.0);
   backFcnDraw->SetFillColor(2);
   backFcnDraw->SetFillStyle(3344);
   backFcnDraw->Draw("SAME FC");
*/
   TF1* backFcnDraw1 = (TF1*)backFcn->Clone("backFcnDraw1");
/*   backFcnDraw1->SetRange(sb_Low/100.0, sb_high/100.0);
   backFcnDraw1->SetFillColor(2);
   backFcnDraw1->SetFillStyle(3001);
   backFcnDraw1->Draw("SAME FC");
*/
   //draw the legend
   TLegend *legend=new TLegend(0.4,0.55,0.88,0.95);
/*   legend->SetTextFont(42);
   legend->SetTextSize(0.);
   legend->AddEntry(h,"Data","lpe");
   legend->AddEntry(backFcn,"Background fit","l");
   legend->AddEntry(signalFcn,"Signal fit","l");
   legend->AddEntry(fitFcn,"Global Fit","l");
   legend->AddEntry(fitFcnDraw,"Signal Region","f");
   legend->AddEntry(fitFcnDraw1,"Sideband Region","f");
   //legend->AddEntry(backFcnDraw,"BG in Signal Region","f");
   //legend->AddEntry(backFcnDraw1,"BG in Sideband Region","f");
   legend->AddEntry(signalFcn, "Sig in Signal region", "f");
  */
    legend->AddEntry(h, Form("fsig_sig = %f",fsig_sig),"p");
   legend->AddEntry(h, Form("fsig_sb = %f",fsig_sb),"p");
   //legend->AddEntry(h, Form("fbg_sig = %f",fbg_sig),"p");
   //legend->AddEntry(h, Form("fbg_sb = %f",fbg_sb),"p");
   legend->Draw();

gStyle->SetOptStat(0);
gStyle->SetOptFit(1111);

} 
