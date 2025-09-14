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

void FullFit_Data_BBeam_modRange_xF4_Weibull(int phi_val){

   double phi_val_blue[24] = {-3.01069, -2.74889, -2.48709, -2.22529, -1.96349, -1.70169, -1.4399, -1.17809, -0.916292, -0.654503, -0.392699, -0.130894, 0.130894, 0.392699, 0.654503, 0.916292, 1.17809, 1.4399, 1.70169, 1.9635, 2.2253, 2.48709, 2.74889, 3.01069};

   int range[6] = {40, 41, 42, 43, 44, 45};
   double sig_Low[6] = {0.08, 0.09, 0.10, 0.11, 0.08, 0.09};
   double sig_High[6] = {0.19, 0.20, 0.21, 0.22, 0.21, 0.22};

   TFile *f = new TFile("YellowBeamAllxF4.root");
   TH1F *h = (TH1F*)f->Get(Form("pi0M_BAll_xF4_phi%d",phi_val));

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,0.4,8);
    fitFcn->SetLineColor(kMagenta);

//Parameters are set from the values received from the individual fit codes
//For xF4 Data
   
   fitFcn->SetParameter(0,1.13618e+00); //weibull shape
   fitFcn->SetParameter(1,1.59102e-01); //weibull scale
   fitFcn->SetParameter(2,1.15000e-01);  //weibull location
   fitFcn->SetParameter(3,2.46930e+02); //weibull amplitude
   fitFcn->SetParameter(4,2958.96); //skgaus amplitude
   fitFcn->SetParameter(5,0.173404); //skgaus mean
   fitFcn->SetParameter(6,0.0374878); //skgaus sigma
   fitFcn->SetParameter(7,-0.676158); //skgaus skewness 

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


    TFile *fUp = new TFile("YellowBeamUpxF4.root");
   TH1F *hUp = (TH1F*)fUp->Get(Form("pi0M_BUp_xF4_phi%d",phi_val));

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

    TF1 *backFcnUp = new TF1("backFcnUp",weibull_pdf,0.0,0.4,4);
   backFcnUp->SetLineColor(kRed);
   backFcnUp->SetParameter(0,fitFcnUp->GetParameter(0));
   backFcnUp->SetParameter(1,fitFcnUp->GetParameter(1));
   backFcnUp->SetParameter(2,fitFcnUp->GetParameter(2));
   backFcnUp->SetParameter(3,fitFcnUp->GetParameter(3));
   backFcnUp->Draw("same");

   TFile *fDown = new TFile("YellowBeamDownxF4.root");
   TH1F *hDown = (TH1F*)fDown->Get(Form("pi0M_BDown_xF4_phi%d",phi_val));

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

   TF1 *backFcnDown = new TF1("backFcnDown",weibull_pdf,0.0,0.4,4);
   backFcnDown->SetLineColor(kRed);
   backFcnDown->SetParameter(0,fitFcnDown->GetParameter(0));
   backFcnDown->SetParameter(1,fitFcnDown->GetParameter(1));
   backFcnDown->SetParameter(2,fitFcnDown->GetParameter(2));
   backFcnDown->SetParameter(3,fitFcnDown->GetParameter(3));
   backFcnDown->Draw("same");


   ofstream outfile;
   outfile.open("Par_YB_xF4_5xFBins.txt",std::ios::app);
   outfile<<phi_val<<" "<<fitFcn->GetChisquare()/fitFcn->GetNDF()<<" "<<fitFcn->GetParameter(3)<<" "<<fitFcn->GetParError(3)<<" "<<fitFcn->GetParameter(4)<<" "<<fitFcn->GetParError(4)<<" "<<fitFcnUp->GetChisquare()/fitFcnUp->GetNDF()<<" "<<fitFcnUp->GetParameter(3)<<" "<<fitFcnUp->GetParError(3)<<" "<<fitFcnUp->GetParameter(4)<<" "<<fitFcnUp->GetParError(4)<<" "<<fitFcnDown->GetChisquare()/fitFcnDown->GetNDF()<<" "<<fitFcnDown->GetParameter(3)<<" "<<fitFcnDown->GetParError(3)<<" "<<fitFcnDown->GetParameter(4)<<" "<<fitFcnDown->GetParError(4)<<endl;
   outfile.close();

   int num_files = 6;
    std::vector<std::ofstream> outfile_array_Up(num_files);
    std::vector<std::ofstream> outfile_array_Down(num_files);

    for (int i = 0; i < num_files; ++i) {
        std::string filename = "YUp_pi0_bkg_xF4_range" + std::to_string(i) + ".txt";
        outfile_array_Up[i].open(filename, std::ios::app);

        if (outfile_array_Up[i].is_open()) {
            std::cout << "Successfully opened " << filename << std::endl;
            //outfile_array[i] << "This is content for file " << i << std::endl;
        } else {
            std::cerr << "Failed to open " << filename << std::endl;
        }
    }

    // Don't forget to close the files when you're done.
    for (int i = 0; i < num_files; ++i) {
        if (outfile_array_Up[i].is_open()) {
            outfile_array_Up[i]<<phi_val_blue[phi_val]<<" "<<fitFcnUp->Integral(sig_Low[i],sig_High[i])*100 - backFcnUp->Integral(sig_Low[i],sig_High[i])*100<<" "<<sqrt(fitFcnUp->Integral(sig_Low[i],sig_High[i])*100 - backFcnUp->Integral(sig_Low[i],sig_High[i])*100)<<" "<<backFcnUp->Integral(sig_Low[i],sig_High[i])*100<<" "<<sqrt(backFcnUp->Integral(sig_Low[i],sig_High[i])*100)<<endl;
            outfile_array_Up[i].close();
        }
    }

    for (int i = 0; i < num_files; ++i) {
        std::string filename = "YDown_pi0_bkg_xF4_range" + std::to_string(i) + ".txt";
        outfile_array_Down[i].open(filename, std::ios::app);

        if (outfile_array_Down[i].is_open()) {
            std::cout << "Successfully opened " << filename << std::endl;
            //outfile_array[i] << "This is content for file " << i << std::endl;
        } else {
            std::cerr << "Failed to open " << filename << std::endl;
        }
    }
  
    // Don't forget to close the files when you're done.
    for (int i = 0; i < num_files; ++i) {
        if (outfile_array_Down[i].is_open()) {
            outfile_array_Down[i]<<phi_val_blue[phi_val]<<" "<<fitFcnDown->Integral(sig_Low[i],sig_High[i])*100 - backFcnDown->Integral(sig_Low[i],sig_High[i])*100<<" "<<sqrt(fitFcnDown->Integral(sig_Low[i],sig_High[i])*100 - backFcnDown->Integral(sig_Low[i],sig_High[i])*100)<<" "<<backFcnDown->Integral(sig_Low[i],sig_High[i])*100<<" "<<sqrt(backFcnDown->Integral(sig_Low[i],sig_High[i])*100)<<endl;
            outfile_array_Down[i].close();
        }
    }

   gStyle->SetOptStat(0);
   gStyle->SetOptFit(1111);
} 
