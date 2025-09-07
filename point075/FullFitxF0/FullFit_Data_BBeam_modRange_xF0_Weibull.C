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

void FullFit_Data_BBeam_modRange_xF0_Weibull(){

  std::ofstream outfile;
   outfile.open("fSig_YB_Weibull.txt", std::ios::app);

  //TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et06pt0_xFTest_1.root");
  //TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi20");
   TFile *f = new TFile("../YellowBeam_xF_hists_modRange.root");
   TH1F *h = (TH1F*)f->Get("pi0M_YUp_xF3_phi0");

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,0.4,8);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
/*   fitFcn->SetParameter(0,7.85529e-01); //weibull shape
   fitFcn->SetParameter(1,2.04986e-01); //weibull scale
   fitFcn->SetParameter(2,4.50006e-02);  //weibull location
   fitFcn->SetParameter(3,1.52711e+03); //weibull amplitude
   fitFcn->SetParameter(4,17382.6); //skgaus amplitude
   fitFcn->SetParameter(5,0.143537); //skgaus mean
   fitFcn->SetParameter(6,0.0410467); //skgaus sigma
   fitFcn->SetParameter(7,14.1532); //skgaus skewness   
  */
//For xF1 Data 
/*   fitFcn->SetParameter(0,1.06007e+00); //weibull shape
   fitFcn->SetParameter(1,3.28539e-01); //weibull scale
   fitFcn->SetParameter(2,5.49591e-02);  //weibull location
   fitFcn->SetParameter(3,8.14295e+03); // weibull amplitude
   fitFcn->SetParameter(4,1.49866e+05); //skgaus amplitude
   fitFcn->SetParameter(5,1.46147e-01); //skgaus mean
   fitFcn->SetParameter(6,4.24205e-02); //skgaus sigma
   fitFcn->SetParameter(7,1.02038e+00); //skgaus skewness ); //skgaus amplitude
*/
//For xF2 Data
/*   fitFcn->SetParameter(0,1.06007e+00); //weibull shape
   fitFcn->SetParameter(1,3.28539e-01); //weibull scale
   fitFcn->SetParameter(2,5.49591e-02);  //weibull location
   fitFcn->SetParameter(3,8.14295e+03); //weibull amplitude
   fitFcn->SetParameter(4,1.49866e+05); //skgaus amplitude
   fitFcn->SetParameter(5,1.46147e-01); //skgaus mean
   fitFcn->SetParameter(6,4.24205e-02); //skgaus sigma
   fitFcn->SetParameter(7,1.02038e+00); //skgaus skewness 
*/
//For xF3 Data
   fitFcn->SetParameter(0,1.75553); //weibull shape
   fitFcn->SetParameter(1,0.190207); //weibull scale
   fitFcn->SetParameter(2,0.155009);  //weibull location
   fitFcn->SetParameter(3,813.631); //weibull amplitude
   fitFcn->SetParameter(4,9325.2); //skgaus amplitude
   fitFcn->SetParameter(5,0.16671); //skgaus mean
   fitFcn->SetParameter(6,0.0305421); //skgaus sigma
   fitFcn->SetParameter(7,0.00439185); //skgaus skewness 



   h->Draw("ep");
   h->Fit("fitFcn","RW","ep");
   fitFcn->Draw("same");

   TF1* fitFcnDraw = (TF1*)fitFcn->Clone("fitFcnDraw");
   fitFcnDraw->SetRange(0.10, 0.25);
   fitFcnDraw->SetFillColor(kRed); 
   fitFcnDraw->SetFillStyle(3001);
   fitFcnDraw->Draw("SAME FC");

   TF1* fitFcnDraw1 = (TF1*)fitFcn->Clone("fitFcnDraw1");
   fitFcnDraw1->SetRange(0.25, 0.4); 
   fitFcnDraw1->SetFillColor(kGray);
   fitFcnDraw1->SetFillStyle(3001);
   fitFcnDraw1->Draw("SAME FC");

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
   signalFcn->SetRange(0.10, 0.25);
   signalFcn->SetFillColor(kBlue);
   signalFcn->SetFillStyle(3344);
   signalFcn->Draw("SAME FC");

   double fsig_sig = signalFcn->Integral(0.10,0.25)/fitFcn->Integral(0.10,0.25);
   double fsig_sb = signalFcn->Integral(0.25,0.40)/fitFcn->Integral(0.25,0.40);

   double fbg_sig = backFcn->Integral(0.10,0.25)/fitFcn->Integral(0.10,0.25);
   double fbg_sb = backFcn->Integral(0.25,0.40)/fitFcn->Integral(0.25,0.40);

   outfile<<fsig_sig<<" "<<fsig_sb<<endl;
   outfile.close();

   TF1* backFcnDraw = (TF1*)backFcn->Clone("backFcnDraw");
/*   backFcnDraw->SetRange(0.10, 0.25);
   backFcnDraw->SetFillColor(2);
   backFcnDraw->SetFillStyle(3344);
   backFcnDraw->Draw("SAME FC");
*/
   TF1* backFcnDraw1 = (TF1*)backFcn->Clone("backFcnDraw1");
/*   backFcnDraw1->SetRange(0.25, 0.4);
   backFcnDraw1->SetFillColor(2);
   backFcnDraw1->SetFillStyle(3001);
   backFcnDraw1->Draw("SAME FC");
*/


//draw the legend
   TLegend *legend=new TLegend(0.4,0.55,0.88,0.95);
   legend->SetTextFont(42);
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
   legend->AddEntry(h, Form("fsig_sig = %f",fsig_sig),"p");
   legend->AddEntry(h, Form("fsig_sb = %f",fsig_sb),"p");
   //legend->AddEntry(h, Form("fbg_sig = %f",fbg_sig),"p");
   //legend->AddEntry(h, Form("fbg_sb = %f",fbg_sb),"p");
   legend->Draw();
gStyle->SetOptStat(0);
gStyle->SetOptFit(1111);
} 
