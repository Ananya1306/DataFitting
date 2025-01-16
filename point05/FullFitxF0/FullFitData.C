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

void FullFitData(){

   TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF2_phi10");

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,1.0,8);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
/*   fitFcn->SetParameter(0,3.69751e-01); //weibull shape
   fitFcn->SetParameter(1,1.03390e+00); //weibull scale
   fitFcn->SetParameter(2,4.50058e-02);  //weibull location
   fitFcn->SetParameter(3,3.47827e+02); //weibull amplitude
   fitFcn->SetParameter(4,1.91231e+03); //skgaus amplitude
   fitFcn->SetParameter(5,1.21583e-01); //skgaus mean
   fitFcn->SetParameter(6,4.22959e-02); //skgaus sigma
   fitFcn->SetParameter(7,1.55093e+01); //skgaus skewness   
*/  
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
   fitFcn->SetParameter(0,7.63615e-01); //weibull shape
   fitFcn->SetParameter(1,8.75137e+00); //weibull scale
   fitFcn->SetParameter(2,6.50004e-02);  //weibull location
   fitFcn->SetParameter(3,5.38813e+02); //weibull amplitude
   fitFcn->SetParameter(4,1.92025e+03); //skgaus amplitude
   fitFcn->SetParameter(5,1.33428e-01); //skgaus mean
   fitFcn->SetParameter(6,4.56236e-02); //skgaus sigma
   fitFcn->SetParameter(7,-1.69004e-01); //skgaus skewness

   h->Fit("fitFcn","V+","ep");
   fitFcn->Draw("same");

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


} 
