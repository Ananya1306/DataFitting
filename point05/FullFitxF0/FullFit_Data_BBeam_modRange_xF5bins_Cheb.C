#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"

//Skewed Gaussian Function
Double_t SkGausPeak(Double_t *x, Double_t *par) {


  return par[0]*exp(-0.5*pow((x[0]-par[1])/par[2],2))*ROOT::Math::normal_cdf(par[3]*x[0],1,0);

}

/*double weibull_pdf(double *x, double *par) {
    if (x[0] <= par[2]) {//par[0]=shape; par[1]=scale; par[2]=location; par[3] = amplitude;
        return 0.0;
    }

    //double z = (x - par[2]) / par[1];
    return (par[3]) * (par[0] / par[1]) * pow(((x[0] - par[2]) / par[1]), par[0] - 1) * exp(-pow(((x[0] - par[2]) / par[1]), par[0]));
}
*/

//Chebyshev background function
Double_t Cheb(Double_t *x, Double_t *par){
  return   ( par[0] + par[1]*x[0] + par[2]*(2*x[0]*x[0] - 1) + par[3]*(4*x[0]*x[0]*x[0] - 3*x[0]));

}

// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
//  return weibull_pdf(x,par) + SkGausPeak(x,&par[4]);
    return Cheb(x,par) + SkGausPeak(x,&par[4]);
}

void FullFit_Data_BBeam_modRange_xF5bins_Cheb(int range_input, int xF_val, string beam_input){


  std::ofstream outfile;
  outfile.open(Form("../sig_sb_range%d/%sBeam/fSig_Cheb.txt",range_input,beam_input.c_str()),std::ios::app);

  double sig_Low, sig_High, sb_Low_1, sb_High_1, sb_Low_2, sb_High_2;
   if(range_input == 50){sb_Low_1 = 0.00; sb_High_1 = 0.08; sig_Low = 0.08; sig_High = 0.19; sb_Low_2 = 0.19; sb_High_2 = 0.40;}
   if(range_input == 51){sb_Low_1 = 0.00; sb_High_1 = 0.09; sig_Low = 0.09; sig_High = 0.20; sb_Low_2 = 0.20; sb_High_2 = 0.40;}
   if(range_input == 52){sb_Low_1 = 0.00; sb_High_1 = 0.10; sig_Low = 0.10; sig_High = 0.21; sb_Low_2 = 0.21; sb_High_2 = 0.40;}
   if(range_input == 53){sb_Low_1 = 0.00; sb_High_1 = 0.11; sig_Low = 0.11; sig_High = 0.22; sb_Low_2 = 0.22; sb_High_2 = 0.40;}
   if(range_input == 54){sb_Low_1 = 0.00; sb_High_1 = 0.08; sig_Low = 0.08; sig_High = 0.21; sb_Low_2 = 0.21; sb_High_2 = 0.40;}
   if(range_input == 55){sb_Low_1 = 0.00; sb_High_1 = 0.09; sig_Low = 0.09; sig_High = 0.22; sb_Low_2 = 0.22; sb_High_2 = 0.40;}
  

//TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et06pt0_xFTest_1.root");
  //TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi20");
   TFile *f = new TFile(Form("../%sBeam_xF5bins_hists_modRange.root",beam_input.c_str()));
   //TH1F *h = (TH1F*)f->Get("pi0M_YUp_xF3_phi0"); 
   //TH1F *h = (TH1F*)f->Get("BlueBeam_xF3");
   TH1F *h = (TH1F*)f->Get(Form("pi0M_BAll_xF%d_phi0",xF_val)); h->GetXaxis()->SetTitle("M_{#gamma#gamma} GeV/c^{2}");


   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,0.4,8);
    fitFcn->SetLineColor(kMagenta);

    TCanvas *c = new TCanvas();

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
if(xF_val == 0){
   fitFcn->SetParameter(0,-393425); //cheb par0
   fitFcn->SetParameter(1,1.00151e+06); //cheb par1
   fitFcn->SetParameter(2,-394316);  //cheb par2
   fitFcn->SetParameter(3,289077); //cheb par3
   fitFcn->SetParameter(4,21074.7); //skgaus amplitude
   fitFcn->SetParameter(5,0.146042); //skgaus mean
   fitFcn->SetParameter(6,0.0233096); //skgaus sigma
   fitFcn->SetParameter(7,0.392946); //skgaus skewness   
}
//For xF1 Data
else if(xF_val == 1){
   fitFcn->SetParameter(0,-1.46603e+06); //cheb par0
   fitFcn->SetParameter(1,3.59763e+06); //cheb par1
   fitFcn->SetParameter(2,-1.45317e+06);  //cheb par2
   fitFcn->SetParameter(3,1.00952e+06); // cheb par3
   fitFcn->SetParameter(4,461651); //skgaus amplitude
   fitFcn->SetParameter(5,0.161713); //skgaus mean
   fitFcn->SetParameter(6,0.025265); //skgaus sigma
   fitFcn->SetParameter(7,0.025265); //skgaus skewness
}
//For xF2 Data
else if(xF_val == 2){
   fitFcn->SetParameter(0,-1.33873e+06); //cheb par0
   fitFcn->SetParameter(1,2.92996e+06); //cheb par1
   fitFcn->SetParameter(2,-1.30827e+06);  //cheb par2
   fitFcn->SetParameter(3,754536); // cheb par3
   fitFcn->SetParameter(4,233209); //skgaus amplitude
   fitFcn->SetParameter(5,0.154275); //skgaus mean
   fitFcn->SetParameter(6,0.0416931); //skgaus sigma
   fitFcn->SetParameter(7,-5.31277); //skgaus skewness ); //skgaus amplitude
}
//For xF3 Data
else if(xF_val == 3){
   fitFcn->SetParameter(0,-1.57955e+06); //cheb par0
   fitFcn->SetParameter(1,3.50517e+06); //cheb par1
   fitFcn->SetParameter(2,-1.53646e+06);  //cheb par2
   fitFcn->SetParameter(3,910301); //cheb par3
   fitFcn->SetParameter(4,45071); //skgaus amplitude
   fitFcn->SetParameter(5,0.141341); //skgaus mean
   fitFcn->SetParameter(6,0.0318866); //skgaus sigma
   fitFcn->SetParameter(7,-0.00430498); //skgaus skewness 
}
//For xF4 Data
else if(xF_val == 4){
   fitFcn->SetParameter(0,-444155); //cheb par0
   fitFcn->SetParameter(1,916656); //cheb par1
   fitFcn->SetParameter(2,-428266);  //cheb par2
   fitFcn->SetParameter(3,222052); //cheb par3
   fitFcn->SetParameter(4,6680.2); //skgaus amplitude
   fitFcn->SetParameter(5,0.162931); //skgaus mean
   fitFcn->SetParameter(6,0.0288585); //skgaus sigma
   fitFcn->SetParameter(7,14.3062); //skgaus skewness 
}

   h->Draw("ep");
   h->Fit("fitFcn","RW","ep");
//   if(xF_val == 2 || xF_val == 3){h->Fit("fitFcn","RW","ep");}
//   else{h->Fit("fitFcn","V+","ep");} //V+ for xF0, xF1 and xF3; RW for xF2
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

   TF1 *backFcn = new TF1("backFcn",Cheb,0.0,0.4,4);
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
   //signalFcn->SetRange(sig_Low, sig_High);
   signalFcn->SetFillColor(kBlue);
   signalFcn->SetFillStyle(3344);
   signalFcn->Draw("SAME FC");

   double fsig_sig = signalFcn->Integral(sig_Low, sig_High)/fitFcn->Integral(sig_Low, sig_High);
   double fsig_sb = (signalFcn->Integral(sb_Low_1, sb_High_1)+ signalFcn->Integral(sb_Low_2, sb_High_2))/(fitFcn->Integral(sb_Low_1, sb_High_1) + fitFcn->Integral(sb_Low_2, sb_High_2)) ;

   double fbg_sig = backFcn->Integral(sig_Low, sig_High)/fitFcn->Integral(sig_Low, sig_High);
   double fbg_sb = (backFcn->Integral(sb_Low_1, sb_High_1) + backFcn->Integral(sb_Low_2, sb_High_2))/(fitFcn->Integral(sb_Low_1, sb_High_1) + fitFcn->Integral(sb_Low_2, sb_High_2));

     cout<<fsig_sig<<" "<<fsig_sb<<endl;
   outfile<<fsig_sig<<" "<<fsig_sb<<endl;
   outfile.close();

   TF1* backFcnDraw = (TF1*)backFcn->Clone("backFcnDraw");
   //backFcnDraw->SetRange(0.11, 0.21);
   //backFcnDraw->SetFillColor(2);
   //backFcnDraw->SetFillStyle(3344);
   backFcnDraw->Draw("SAME FC");
/*
   TF1* backFcnDraw1 = (TF1*)backFcn->Clone("backFcnDraw1");
   backFcnDraw1->SetRange(0.21, 0.40);
   backFcnDraw1->SetFillColor(2);
   backFcnDraw1->SetFillStyle(3001);
   backFcnDraw1->Draw("SAME FC");
*/



//draw the legend
//   TLegend *legend=new TLegend(0.4,0.55,0.88,0.95);
//     TLegend *legend = new TLegend(0.05, 0.75, 0.3, 0.9);
     TLegend *legend = new TLegend(0.09, 0.68, 0.29, 0.78); 
   legend->SetTextSize(0.04);
   legend->SetBorderSize(0);
   legend->SetFillStyle(0); 
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
*/   legend->AddEntry((TObject*)0, Form("fsig_{sig} = %f",fsig_sig),"");
   legend->AddEntry((TObject*)0, Form("fsig_{sb} = %f",fsig_sb),"");
   //legend->AddEntry(h, Form("fbg_sig = %f",fbg_sig),"p");
   //legend->AddEntry(h, Form("fbg_sb = %f",fbg_sb),"p");
   legend->Draw();

/*   TFile *fout = TFile::Open("ChebFits.root","UPDATE");
   fout->cd(); c->Write(Form("xF_%d",xF_val));
   fout->Close();
*/
gStyle->SetOptStat(0);
gStyle->SetOptFit(1111);
} 
