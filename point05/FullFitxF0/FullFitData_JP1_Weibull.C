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

void FullFitData_JP1_Weibull(){

   TFile *f = new TFile("../fitHists.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BUp_xF0_phi0");

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,1.0,8);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
   fitFcn->SetParameter(0,1.37794e+00); //weibull shape
   fitFcn->SetParameter(1,7.00109e-01); //weibull scale
   fitFcn->SetParameter(2,-1.51763e+00);  //weibull location
   fitFcn->SetParameter(3,1.62125e-01); //weibull amplitude
   fitFcn->SetParameter(4,4.61557e-02); //skgaus amplitude
   fitFcn->SetParameter(5,1.36138e-01); //skgaus mean
   fitFcn->SetParameter(6,6.58167e-02); //skgaus sigma
   fitFcn->SetParameter(7,1.27916e+01); //skgaus skewness   
  
//For xF1 Data
/*   fitFcn->SetParameter(0,6.31695e-01); //weibull shape
   fitFcn->SetParameter(1,5.18902e-01); //weibull scale
   fitFcn->SetParameter(2,5.50082e-02);  //weibull location
   fitFcn->SetParameter(3,2.51605e+02); //weibull amplitude
   fitFcn->SetParameter(4,2.44366e+03); //skgaus amplitude
   fitFcn->SetParameter(5,1.28733e-01); //skgaus mean
   fitFcn->SetParameter(6,4.09378e-02); //skgaus sigma
   fitFcn->SetParameter(7,1.44953e+01); //skgaus skewness
*/
//For xF2 Data
//   fitFcn->SetParameter(0,9.08661e-01); //weibull shape
//   fitFcn->SetParameter(1,5.16092e-01); //weibull scale
//   fitFcn->SetParameter(2,7.52973e-02);  //weibull location
//   fitFcn->SetParameter(3,1.32086e+02); //weibull amplitude
  
/*   fitFcn->SetParameter(0,6.07931e-01); //weibull shape
   fitFcn->SetParameter(1,9.27480e-01); //weibull scale
   fitFcn->SetParameter(2,9.63899e-02);  //weibull location
   fitFcn->SetParameter(3,1.99215e+02); //weibull amplitude
*/
/*   fitFcn->SetParameter(0,9.54581e-01); //weibull shape
   fitFcn->SetParameter(1,6.50360e-01); //weibull scale
   fitFcn->SetParameter(2,8.49998e-02);  //weibull location
   fitFcn->SetParameter(3,1.29145e+02); //weibull amplitude
*/  
/*   fitFcn->SetParameter(4,9.23950e+02); //skgaus amplitude
   fitFcn->SetParameter(5,1.30225e-01); //skgaus mean
   fitFcn->SetParameter(6,5.37411e-02); //skgaus sigma
   fitFcn->SetParameter(7,1.56325e+01); //skgaus skewness
*/
/*   fitFcn->SetParameter(4,1.69577e+03); //skgaus amplitude
   fitFcn->SetParameter(5,1.31567e-01); //skgaus mean
   fitFcn->SetParameter(6,4.56549e-02); //skgaus sigma
   fitFcn->SetParameter(7,1.06439e+00); //skgaus skewness
*/
/*   fitFcn->SetParameter(4,7.82475e+02); //skgaus amplitude
   fitFcn->SetParameter(5,1.27669e-01); //skgaus mean
   fitFcn->SetParameter(6,7.54396e-02); //skgaus sigma
   fitFcn->SetParameter(7,1.41958e+01); //skgaus skewness
*/
   h->Draw("ep");
   h->Fit("fitFcn","V+","ep");
   fitFcn->Draw("same");

   TF1* fitFcnDraw = (TF1*)fitFcn->Clone("fitFcnDraw");
   fitFcnDraw->SetRange(0., 0.9);
   fitFcnDraw->SetFillColor(15);
   fitFcnDraw->SetFillStyle(3001);
   fitFcnDraw->Draw("SAME FC");

//improve the picture

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

   gStyle->SetOptFit(1111);
} 
