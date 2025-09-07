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

void FullFit_Data_BBeam_modRange_xF0_Levy(int range_input, int xF_input, string beam_input){

   std::ofstream outfile;
   outfile.open(Form("../sig_sb_range%d/%sBeam/fSig_Levy.txt",range_input,beam_input.c_str()),std::ios::app);

   double sig_Low, sig_High, sb_Low_1, sb_High_1, sb_Low_2, sb_High_2;
   if(range_input == 40){sb_Low_1 = 0.00; sb_High_1 = 0.08; sig_Low = 0.08; sig_High = 0.19; sb_Low_2 = 0.19; sb_High_2 = 0.40;}
   if(range_input == 41){sb_Low_1 = 0.00; sb_High_1 = 0.09; sig_Low = 0.09; sig_High = 0.20; sb_Low_2 = 0.20; sb_High_2 = 0.40;}
   if(range_input == 42){sb_Low_1 = 0.00; sb_High_1 = 0.10; sig_Low = 0.10; sig_High = 0.21; sb_Low_2 = 0.21; sb_High_2 = 0.40;}
   if(range_input == 43){sb_Low_1 = 0.00; sb_High_1 = 0.11; sig_Low = 0.11; sig_High = 0.22; sb_Low_2 = 0.22; sb_High_2 = 0.40;}
   if(range_input == 44){sb_Low_1 = 0.00; sb_High_1 = 0.08; sig_Low = 0.08; sig_High = 0.21; sb_Low_2 = 0.21; sb_High_2 = 0.40;}
   if(range_input == 45){sb_Low_1 = 0.00; sb_High_1 = 0.09; sig_Low = 0.09; sig_High = 0.22; sb_Low_2 = 0.22; sb_High_2 = 0.40;}

   TFile *f = new TFile(Form("../%sBeam_xF_hists_modRange.root",beam_input.c_str()));
   TH1F* h = (TH1F*)f->Get(Form("%sBeam_xF%d",beam_input.c_str(),xF_input));

    

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,0.4,7);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
if(xF_input == 0){   
   fitFcn->SetParameter(0,366.558); //levy amplitude
   fitFcn->SetParameter(1,0.0555105); //levy scale
   fitFcn->SetParameter(2,0.0359502);  //levy location
   fitFcn->SetParameter(3,17382.6); //skgaus amplitude
   fitFcn->SetParameter(4,0.143537); //skgaus mean
   fitFcn->SetParameter(5,0.0410467); //skgaus sigma
   fitFcn->SetParameter(6,14.1532); //skgaus skewness   
}  
//For xF1 Data
else if(xF_input == 1){
   fitFcn->SetParameter(0,5.02849e+03); //levy amplitude
   fitFcn->SetParameter(1,4.82356e-01); //levy scale
   fitFcn->SetParameter(2,4.90052e-03);  //levy location
   fitFcn->SetParameter(3,1.49866e+05); //skgaus amplitude
   fitFcn->SetParameter(4,1.46147e-01); //skgaus mean
   fitFcn->SetParameter(5,4.24205e-02); //skgaus sigma
   fitFcn->SetParameter(6,1.02038e+00); //skgaus skewness ); //skgaus amplitude

}
//For xF2 Data
else if(xF_input == 2){
   fitFcn->SetParameter(0,5399.85); //levy amplitude
   fitFcn->SetParameter(1,0.152805); //levy scale
   fitFcn->SetParameter(2,0.0678691);  //levy location
   fitFcn->SetParameter(3,6.95451e+04); //skgaus amplitude
   fitFcn->SetParameter(4,1.49289e-01); //skgaus mean
   fitFcn->SetParameter(5,6.58667e-02); //skgaus sigma
   fitFcn->SetParameter(6,1.38106e+01); //skgaus skewness 

}
//For xF3 Data
else if(xF_input == 3){ 
  fitFcn->SetParameter(0,7.89815e+02); //levy amplitude
   fitFcn->SetParameter(1,2.62014e-01); //levy scale
   fitFcn->SetParameter(2,1.14481e-01);  //levy location
   fitFcn->SetParameter(3,5621.41); //skgaus amplitude
   fitFcn->SetParameter(4,0.175239); //skgaus mean
   fitFcn->SetParameter(5,0.0381244); //skgaus sigma
   fitFcn->SetParameter(6,13.3106); //skgaus skewness 

}
else {cout<<"invalid xF"<<endl;}

   h->Draw("ep");
   if(xF_input == 2) {h->Fit("fitFcn","RW","ep");}
   else{h->Fit("fitFcn","V+","ep");} //V+ for xF0, xF1 and xF3; RW for xF2
   fitFcn->Draw("same");

   TF1* fitFcnDraw = (TF1*)fitFcn->Clone("fitFcnDraw");
   fitFcnDraw->SetRange(sig_Low, sig_High);
   fitFcnDraw->SetFillColor(kRed); 
   fitFcnDraw->SetFillStyle(3001);
   fitFcnDraw->Draw("SAME FC");

   TF1* fitFcnDraw1 = (TF1*)fitFcn->Clone("fitFcnDraw1");
   fitFcnDraw1->SetRange(sb_Low_1, sb_High_1); 
   fitFcnDraw1->SetFillColor(kGray);
   fitFcnDraw1->SetFillStyle(3001);
   fitFcnDraw1->Draw("SAME FC");

   TF1* fitFcnDraw2 = (TF1*)fitFcn->Clone("fitFcnDraw2");
   fitFcnDraw2->SetRange(sb_Low_2, sb_High_2);
   fitFcnDraw2->SetFillColor(kGray);
   fitFcnDraw2->SetFillStyle(3001);
   fitFcnDraw2->Draw("SAME FC");

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
   double fsig_sb = (signalFcn->Integral(sb_Low_1,sb_High_1) + signalFcn->Integral(sb_Low_2,sb_High_2))/(fitFcn->Integral(sb_Low_1, sb_High_1) + fitFcn->Integral(sb_Low_2, sb_High_2));

   double fbg_sig = backFcn->Integral(sig_Low,sig_High)/fitFcn->Integral(sig_Low,sig_High);
   double fbg_sb = (backFcn->Integral(sb_Low_1,sb_High_1) + backFcn->Integral(sb_Low_2,sb_High_2))/(fitFcn->Integral(sb_Low_1, sb_High_1) + fitFcn->Integral(sb_Low_2, sb_High_2));


   cout<<fsig_sig<<" "<<fsig_sb<<endl;
   outfile<<fsig_sig<<" "<<fsig_sb<<endl;
   outfile.close();

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
