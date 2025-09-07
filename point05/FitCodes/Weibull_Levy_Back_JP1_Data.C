#include "TH1.h"
#include "TF1.h"
#include "TList.h"

double weibull_pdf(double *x, double *par) {
    if (x[0] <= par[2]) {//par[0]=shape; par[1]=scale; par[2]=location; par[3] = amplitude;
        return 0.0;
    }

    //double z = (x - par[2]) / par[1];
    return (par[3]) * (par[0] / par[1]) * pow(((x[0] - par[2]) / par[1]), par[0] - 1) * exp(-pow(((x[0] - par[2]) / par[1]), par[0]));
}

Double_t levy(Double_t *x, Double_t *par){

         if (x[0] <= par[2]){
                   return 0.0;
           }

   return (par[0])*(sqrt(par[1]/2*TMath::Pi())*exp(-par[1]/(2*(x[0]-par[2])))*(1/(pow((x[0]-par[2]),1.5)))); //Levy Distribution

//par[0] is Ampltitude, par[1] is Scale, par[2] is location
}

//Sum of Weibull and Levy for Background Distribution
Double_t fitFunction(Double_t *x, Double_t *par){
	return weibull_pdf(x, par) + levy(x, &par[4]);
}

void Weibull_Levy_Back_JP1_Data(){


   TFile *f = new TFile("../fitHists.root");
   TH1F *h = (TH1F*)f->Get("pi0M_BUp_xF0_phi0");
   TH1F *hBack = (TH1F*)h->Clone("hBack");
   TH1F *hBackFit = (TH1F*)h->Clone("hBackFit");
   for(int i=9; i<25; i++)
	{hBack->SetBinContent(i,0); hBack->SetBinError(i,0.04);
	 hBackFit->SetBinContent(i,0); hBackFit->SetBinError(i,0.04);
	}
   hBack->Draw();

   TF1 *fw = new TF1("fw",weibull_pdf,0.0,0.31,4);
   fw->SetParameters(0.8,0.9,0.03,0.008);
   fw->Draw("same");
   hBack->Fit(fw,"R");

   TF1 *fl = new TF1("fl",levy,0.31,1,3);
   fl->SetParameters(0.0008,0.07,0.02); //pi0MxFPhi_0 fit initial parameters
   fl->Draw("same");
   hBack->Fit(fl,"R+");

   //Once the individual fits are done, do the total fit

   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0.0,1.0,7);
   fitFcn->SetLineColor(kMagenta);
   
   

//Parameters are set from the values received from the individual fit codes
//For xF0 Data
   fitFcn->SetParameter(0,fw->GetParameter(0)); //weibull shape
   fitFcn->SetParameter(1,fw->GetParameter(1)); //weibull scale
   fitFcn->SetParameter(2,fw->GetParameter(2));  //weibull location
   fitFcn->SetParameter(3,fw->GetParameter(3)); //weibull amplitude
   fitFcn->SetParameter(4,fl->GetParameter(0)); //levy amplitude
   fitFcn->SetParameter(5,fl->GetParameter(1)); //levy scale
   fitFcn->SetParameter(6,fl->GetParameter(2)); //levy location 

   TCanvas *c2 = new TCanvas();
   hBackFit->Fit("fitFcn","R+","ep");
// hBackFit->Draw();  
// fitFcn->Draw("same");

}
