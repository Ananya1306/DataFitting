#include "TH1.h"
#include "TF1.h"
#include "TList.h"


//Skewed Gaussian Function
Double_t SkGausPeak(Double_t *x, Double_t *par) {

  //par[0] scale, par[1] mean, par[2] sigma, par[3] skewness
  //return par[0]*exp(-0.5*(pow(((x[0]-par[1])/(par[2]*(1+(par[3]*(x[0]-par[1]))))),2)));
  return par[0]*exp(-0.5*pow((x[0]-par[1])/par[2],2))*ROOT::Math::normal_cdf(par[3]*x[0],1,0);

}

void SkGausFuncData(){

   //TFile *f = new TFile("../EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et06pt0_xFTest_1.root");
   //TH1F *h = (TH1F*)f->Get("pi0M_BDown_xF0_phi20");
   TFile *f = new TFile("BlueBeamAllxF3.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BAll_xF3_phi10");
   TH1F *hSig = (TH1F*)h->Clone("hSig");
   //for(int i=10; i<30; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,100);}
   hSig->Draw("ep"); 
  
   TF1 *fsg = new TF1("fsg",SkGausPeak,0.13,0.19,4); 
  //fsg->SetParameters(2500,0.130,0.10,4); //pi0M_BDown_xF0_phi10 fit initial parameter
   //fsg->SetParameters(2500,0.125,0.008,4); //pi0M_BDown_xF1_phi10 fit initial parameter
  fsg->SetParameters(20,0.13,-1,-1); //pi0M_BDown_xF2_phi10 fit initial parameter
  //fsg->SetParameter(0,1000); fsg->SetParameter(1,0.13); fsg->SetParameter(2,0.05); fsg->SetParameter(3,4);
  fsg->Draw("same");
  hSig->Fit(fsg,"RW");

/*  TF1 *fsgTest1 = new TF1("fsgTest1",SkGausPeak,0.08,0.3,4);
  fsgTest1->SetParameters(1000,0.13,0.05,40); //pi0M_BDown_xF2_phi10 fit initial parameter
  fsgTest1->SetLineColor(kCyan); fsgTest1->Draw("same");
*/
/*  TF1 *fsgDraw = new TF1("fsgDraw",SkGausPeak,0.0,1.0,4);  //Defining a function for drawing purposes
  fsgDraw->SetParameters(fsg->GetParameter(0),fsg->GetParameter(1),fsg->GetParameter(2),fsg->GetParameter(3));
  fsgDraw->SetLineColor(kBlue); fsgDraw->Draw("same");
*/
/*  TCanvas *c2 = new TCanvas();
  TF1 *fsg_test1 = new TF1("fsg_test1",SkGausPeak,-1.0,1.0,4); fsg_test1->SetParameters(10,0,0.5,4); fsg_test1->SetLineColor(kBlue); fsg_test1->Draw();
  TF1 *fsg_test2 = new TF1("fsg_test2",SkGausPeak,-1.0,1.0,4); fsg_test2->SetParameters(10,0,0.5,1); fsg_test2->SetLineColor(kOrange); fsg_test2->Draw("same");
  TF1 *fsg_test3 = new TF1("fsg_test3",SkGausPeak,-1.0,1.0,4); fsg_test3->SetParameters(10,0,0.5,0); fsg_test3->SetLineColor(kMagenta); fsg_test3->Draw("same");
  TF1 *fsg_test4 = new TF1("fsg_test4",SkGausPeak,-1.0,1.0,4); fsg_test4->SetParameters(10,0,0.5,-1); fsg_test4->SetLineColor(kGreen); fsg_test4->Draw("same");
  TF1 *fsg_test5 = new TF1("fsg_test5",SkGausPeak,-1.0,1.0,4); fsg_test5->SetParameters(10,0,0.5,-4); fsg_test5->SetLineColor(kCyan); fsg_test5->Draw("same");
*/
} 
