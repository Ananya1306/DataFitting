#include "TH1.h"
void ReadHist()
{
	      //TFile *f1 = TFile::Open("/star/u/ananyapaul/Analysis/EEmcAnalysis/BrightSTAR/jobResults/EEmcEmbedding/OutputRootFiles/EEmcNanoTreeQA_EmbeddingAllParts_withWeights_xF1.root");
	      //TFile *f1 = TFile::Open("/star/u/ananyapaul/Analysis/EEmcAnalysis/BrightSTAR/EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
	      //TH1F *h1 = (TH1F*)f1->Get("pi0MxFPhi_0_w");
	      TFile *f1 = TFile::Open("EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et04pt2_xFTest_1.root");
	      TH1F *h1 = (TH1F*)f1->Get("pi0M_BUp_xF0_phi10");
	      TCanvas *c1 = new TCanvas();
              //c1->SetLogy();
     	      h1->SetLineColor(kRed);
	      //h1->DrawNormalized();
	      h1->Scale(1./h1->Integral());
	      h1->Draw(); h1->Draw("C same");

	      //TFile *f2 = TFile::Open("/star/u/ananyapaul/Analysis/EEmcAnalysis/BrightSTAR/EEmcNanoTreeQA_schedRun15_BlueBeam_LumPol_160All.root");
//	      TFile *f2 = TFile::Open("/star/u/ananyapaul/Analysis/EEmcAnalysis/BrightSTAR/EEmcNanoTreeQA_AllRuns_v7.root");
	      //TFile *f1 = TFile::Open("/star/u/ananyapaul/Analysis/EEmcAnalysis/BrightSTAR/EEmcNanoTreeQA_PhotonTest.root")
              //TH1F *h2 = (TH1F*)f2->Get("pi0M_bDown_xF0_phi15");
	      TFile *f2 = TFile::Open("BothTreeOutAll_XfTestpoint05.root");
              TH1F *h2 = (TH1F*)f2->Get("pi0MxFPhi_0");
              TCanvas *c2 = new TCanvas();
              //c2->SetLogy();
              h2->SetLineColor(kBlue);
              //h2->DrawNormalized();
	      h2->Scale(1./h2->Integral());
              h2->Draw(); h2->Draw("C same");

/*
   	      //TFile *f3 = TFile::Open("/star/u/ananyapaul/Analysis/EEmcAnalysis/BrightSTAR/jobResults/EEmcEmbedding/OutputRootFiles/EEmcNanoTreeQA_EmbeddingAll.root");
              TFile *f3 = TFile::Open("/star/u/ananyapaul/Analysis/EEmcAnalysis/BrightSTAR/jobResults/schedEEmcRun15ReadEEmcOutputFiles/EEmcNanoTreeQA_schedRun15_BlueBeam_160All_Et03pt2_xFTest_2.root");
              TH1F *h3 = (TH1F*)f3->Get("pi0M_BDown_xF0_phi20");
              TCanvas *c3 = new TCanvas();
	      //c3->SetLogy();
              h3->SetLineColor(kCyan);
              //h3->DrawNormalized();
              h3->Scale(1./h3->Integral());
              h3->Draw();


              //TFile *f3 = TFile::Open("/star/u/ananyapaul/Analysis/EEmcAnalysis/BrightSTAR/jobResults/EEmcEmbedding/OutputRootFiles/EEmcNanoTreeQA_EmbeddingAllParts_withWeights_test.root");
              TH1F *h4 = (TH1F*)f1->Get("pi0M_YDown_xF3_phi15");
              TCanvas *c4 = new TCanvas();
              //c4->SetLogy();
              h4->SetLineColor(kCyan);
              //h4->DrawNormalized();
              //h3->Scale(1./h3->Integral());
              h4->Draw();
*/

	      //Double_t KTest = h1->KolmogorovTest(h2); cout<<"Compatibility = "<<KTest<<endl;
	      TCanvas *c5 = new TCanvas(); //c5->SetLogy(); 
              h2->SetTitle("Invariant Mass Spectra of Diphotons for 0.05 < xF < 0.1");
	      //h1->SetTitle("#pi0 Inv Mass Histogram for 0.08<xF<0.12");
              h1->Draw(); h1->Draw("c same"); h2->Draw(" same");h2->Draw("c same"); //h2->Draw("same"); //h4->Draw("same");
              auto legend = new TLegend(0.7,0.7,0.98,0.9);
	      //legend->SetHeader(" ");
	      legend->AddEntry(h1,"Run15 data");
	      legend->AddEntry(h2,"Embedding events triggered by EHT0");
	      //legend->AddEntry(h3,"Run15 Data, photons triggered by EHT0");
	      //legend->AddEntry(h4,"0.03 < xF < 0.07");
	      legend->Draw();
	      //c5->SaveAs("PionxF1Test.pdf");

	      /*c5->Divide(2,2);
	      h1->SetTitle("Invariant Mass Spectra of Diphotons");
     	      c5->cd(1); h1->Draw(); c5->cd(2); h2->Draw(); c5->cd(3); h3->Draw(); c5->cd(4); h4->Draw();
	      auto legend = new TLegend(0.7,0.7,0.98,0.9);
	      legend->AddEntry(h1,"0.08 < xF < 0.12");
              legend->AddEntry(h2,"0.05 < xF < 0.09");
              legend->AddEntry(h3,"0.10 < xF < 0.15");
              legend->AddEntry(h4,"0.03 < xF < 0.07");
              legend->Draw();*/

	      gStyle->SetOptStat(0);
}
