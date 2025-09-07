#include "TH1.h"
#include "TF1.h"
#include "TList.h"


Double_t logN(Double_t *x, Double_t *par){

        /* if (x[0] <= par[2]){
                   return 0.0;
           }
*/
   return 1.0 * par[2] /( x[0]*par[1]* sqrt(2*TMath::Pi())) * exp( - pow((log(x[0]) - par[0]),2) / 2*pow(par[1],2) )   ; //Log-Normal Distribution

//par[0] is Mu, par[1] is Sigma, par[2] is Scale
}

void LogNormalFuncData(){


   TFile *f = new TFile("../BlueBeam_xF_hists_modRange.root");
   TH1F *h = (TH1F*)f->Get("BlueBeam_xF3");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   for(int i=13; i<25; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,5000);} //for xF0 and xF1 i starts from 8
   //for(int i=40; i<100; i++){hBack->SetBinContent(i,0); hBack->SetBinError(i,5000);}
   hBack->Draw();


	TF1 *f1 = new TF1("f1",logN,0,0.4,3);
        //f1->SetParameters(-3.0,1.0, 1000.0); //initial fit par xF0
        //f1->SetParameters(-2.0,1.5, 100.0); //initial fit par xF1
        //f1->SetParameters(-2.0,1.5, 100.0); //initial fit par xF2
        f1->SetParameters(-2.0,1.5, 100.0); //initial fit par xF3
	f1->Draw("same");


//        hBack->Draw("same");
	hBack->Fit(f1,"R+");

}
