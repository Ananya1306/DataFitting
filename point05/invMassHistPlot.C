void invMassHistPlot(){

	gStyle->SetOptStat(0);

	TFile *f = TFile::Open("BlueBeam_xF5bins_hists_modRange.root");
	
//Gettng histograms

        TH1F *xF0_h0 = (TH1F*)f->Get("pi0M_BAll_xF0_phi0"); xF0_h0->SetStats(kFALSE); xF0_h0->GetXaxis()->SetTitle("M_{#gamma#gamma} GeV/c^{2}");
        TH1F *xF1_h0 = (TH1F*)f->Get("pi0M_BAll_xF1_phi0"); xF1_h0->SetStats(kFALSE); xF1_h0->GetXaxis()->SetTitle("M_{#gamma#gamma} GeV/c^{2}");
	TH1F *xF2_h0 = (TH1F*)f->Get("pi0M_BAll_xF2_phi0"); xF2_h0->SetStats(kFALSE); xF2_h0->GetXaxis()->SetTitle("M_{#gamma#gamma} GeV/c^{2}");
        TH1F *xF3_h0 = (TH1F*)f->Get("pi0M_BAll_xF3_phi0"); xF3_h0->SetStats(kFALSE); xF3_h0->GetXaxis()->SetTitle("M_{#gamma#gamma} GeV/c^{2}");
	TH1F *xF4_h0 = (TH1F*)f->Get("pi0M_BAll_xF4_phi0"); xF4_h0->SetStats(kFALSE); xF4_h0->GetXaxis()->SetTitle("M_{#gamma#gamma} GeV/c^{2}");

//Cloning histograms

	TH1F *xF0_h1 = (TH1F*)xF0_h0->Clone();
	TH1F *xF0_h2 = (TH1F*)xF0_h0->Clone();
	TH1F *xF0_h3 = (TH1F*)xF0_h0->Clone();

	TH1F *xF1_h1 = (TH1F*)xF1_h0->Clone();
        TH1F *xF1_h2 = (TH1F*)xF1_h0->Clone();
        TH1F *xF1_h3 = (TH1F*)xF1_h0->Clone();

	TH1F *xF2_h1 = (TH1F*)xF2_h0->Clone();
        TH1F *xF2_h2 = (TH1F*)xF2_h0->Clone();
        TH1F *xF2_h3 = (TH1F*)xF2_h0->Clone();

	TH1F *xF3_h1 = (TH1F*)xF3_h0->Clone();
        TH1F *xF3_h2 = (TH1F*)xF3_h0->Clone();
        TH1F *xF3_h3 = (TH1F*)xF3_h0->Clone();
	
	TH1F *xF4_h1 = (TH1F*)xF4_h0->Clone();
        TH1F *xF4_h2 = (TH1F*)xF4_h0->Clone();
        TH1F *xF4_h3 = (TH1F*)xF4_h0->Clone();

//Setting range of histograms
	
	xF0_h1->GetXaxis()->SetRangeUser(0.0,0.11);
	xF0_h2->GetXaxis()->SetRangeUser(0.11, 0.22);
        xF0_h3->GetXaxis()->SetRangeUser(0.22, 0.4);	
	xF0_h1->SetFillColorAlpha(kGray,0.4);
        xF0_h2->SetFillColorAlpha(kRed,0.3);
        xF0_h3->SetFillColorAlpha(kGray,0.4);

	xF1_h1->GetXaxis()->SetRangeUser(0.0,0.11);
        xF1_h2->GetXaxis()->SetRangeUser(0.11, 0.22);
        xF1_h3->GetXaxis()->SetRangeUser(0.22, 0.4);
        xF1_h1->SetFillColorAlpha(kGray,0.4);
        xF1_h2->SetFillColorAlpha(kRed,0.3);
        xF1_h3->SetFillColorAlpha(kGray,0.4);

	xF2_h1->GetXaxis()->SetRangeUser(0.0,0.11);
        xF2_h2->GetXaxis()->SetRangeUser(0.11, 0.22);
        xF2_h3->GetXaxis()->SetRangeUser(0.22, 0.4);
        xF2_h1->SetFillColorAlpha(kGray,0.4);
        xF2_h2->SetFillColorAlpha(kRed,0.3);
        xF2_h3->SetFillColorAlpha(kGray,0.4);
	
	xF3_h1->GetXaxis()->SetRangeUser(0.0,0.11);
        xF3_h2->GetXaxis()->SetRangeUser(0.11, 0.22);
        xF3_h3->GetXaxis()->SetRangeUser(0.22, 0.4);
        xF3_h1->SetFillColorAlpha(kGray,0.4);
        xF3_h2->SetFillColorAlpha(kRed,0.3);
        xF3_h3->SetFillColorAlpha(kGray,0.4);
	
	xF4_h1->GetXaxis()->SetRangeUser(0.0,0.11);
        xF4_h2->GetXaxis()->SetRangeUser(0.11, 0.22);
        xF4_h3->GetXaxis()->SetRangeUser(0.22, 0.4);
        xF4_h1->SetFillColorAlpha(kGray,0.4);
        xF4_h2->SetFillColorAlpha(kRed,0.3);
        xF4_h3->SetFillColorAlpha(kGray,0.4);

/*
	TCanvas *c0 = new TCanvas();
	c0->SetLeftMargin(0.15); // Adjust left margin to prevent y-axis title from being cut
        c0->SetBottomMargin(0.15); // Adjust bottom margin to prevent x-axis title from being cut
	xF0_h0->Draw();
	xF0_h1->Draw("same");
	xF0_h2->Draw("same");
	xF0_h3->Draw("same");
	TLegend *legend0 = new TLegend(0.56, 0.70, 0.79, 0.88); //right side 
	legend0->SetTextSize(0.05);
	legend0->AddEntry(xF0_h2, "Signal Region", "f");
	legend0->AddEntry(xF0_h1, "Sideband Region", "f");
	TLegendEntry *le0 = legend0->AddEntry((TObject*)0, "0.075 < xF < 0.1", "C");
	legend0->SetBorderSize(0);
	legend0->Draw();

	TCanvas *c1 = new TCanvas();
	c1->SetLeftMargin(0.15); // Adjust left margin to prevent y-axis title from being cut
	c1->SetBottomMargin(0.15); // Adjust bottom margin to prevent x-axis title from being cut
        xF1_h0->Draw();
        xF1_h1->Draw("same");
        xF1_h2->Draw("same");
        xF1_h3->Draw("same");
        TLegend *legend1 = new TLegend(0.56, 0.70, 0.79, 0.88); //right side 
        legend1->SetTextSize(0.05);
	legend1->AddEntry(xF0_h2, "Signal Region", "f");
        legend1->AddEntry(xF0_h1, "Sideband Region", "f");
	TLegendEntry *le1 = legend1->AddEntry((TObject*)0, "0.1 < xF < 0.125", "C");
        legend1->SetBorderSize(0);
        legend1->Draw();

	TCanvas *c2 = new TCanvas();
	c2->SetLeftMargin(0.15); // Adjust left margin to prevent y-axis title from being cut
    	c2->SetBottomMargin(0.15); // Adjust bottom margin to prevent x-axis title from being cut
        xF2_h0->Draw();
        xF2_h1->Draw("same");
        xF2_h2->Draw("same");
        xF2_h3->Draw("same");
        TLegend *legend2 = new TLegend(0.56, 0.70, 0.79, 0.88); //right side 
        legend2->SetTextSize(0.05);
	legend2->AddEntry(xF0_h2, "Signal Region", "f");
        legend2->AddEntry(xF0_h1, "Sideband Region", "f");
	TLegendEntry *le2 = legend2->AddEntry((TObject*)0, "0.125 < xF < 0.175", "C");
        legend2->SetBorderSize(0);
        legend2->Draw();

	TCanvas *c3 = new TCanvas();
	c3->SetLeftMargin(0.15); // Adjust left margin to prevent y-axis title from being cut
    	c3->SetBottomMargin(0.15); // Adjust bottom margin to prevent x-axis title from being cut
        xF3_h0->Draw();
        xF3_h1->Draw("same");
        xF3_h2->Draw("same");
        xF3_h3->Draw("same");
        TLegend *legend3 = new TLegend(0.56, 0.70, 0.79, 0.88); //right side 
        legend3->SetTextSize(0.05);
	legend3->AddEntry(xF0_h2, "Signal Region", "f");
        legend3->AddEntry(xF0_h1, "Sideband Region", "f");
	TLegendEntry *le3 = legend3->AddEntry((TObject*)0, "0.175 < xF < 0.225", "C");
        legend3->SetBorderSize(0);
        legend3->Draw();

	TCanvas *c4 = new TCanvas();
	c4->SetLeftMargin(0.15); // Adjust left margin to prevent y-axis title from being cut
    	c4->SetBottomMargin(0.15); // Adjust bottom margin to prevent x-axis title from being cut
        xF4_h0->Draw();
        xF4_h1->Draw("same");
        xF4_h2->Draw("same");
        xF4_h3->Draw("same");
        TLegend *legend4 = new TLegend(0.56, 0.70, 0.79, 0.88); //right side 
        legend4->SetTextSize(0.05);
	legend4->AddEntry(xF0_h2, "Signal Region", "f");
        legend4->AddEntry(xF0_h1, "Sideband Region", "f");
	TLegendEntry *le4 = legend4->AddEntry((TObject*)0, "0.225 < xF < 0.375", "C");
        legend4->SetBorderSize(0);
        legend4->Draw();
*/	

    // Create a single canvas and divide it into a 3x2 grid
    TCanvas *cAll = new TCanvas("cAll", "Combined Plots", 1200, 800);
    cAll->Divide(3, 2);

    // Canvas 0
    cAll->cd(1);
    gPad->SetLeftMargin(0.15); // Adjust left margin to prevent y-axis title from being cut
    gPad->SetBottomMargin(0.15); // Adjust bottom margin to prevent x-axis title from being cut
    xF0_h0->Draw();
    xF0_h1->Draw("same");
    xF0_h2->Draw("same");
    xF0_h3->Draw("same");
    TLegend *legend0 = new TLegend(0.56, 0.70, 0.79, 0.88);
    legend0->SetHeader("0.075 < xF < 0.1", "C");
    legend0->SetTextSize(0.05);
    legend0->AddEntry(xF0_h2, "Signal", "f");
    legend0->AddEntry(xF0_h3, "Sideband", "f");
    legend0->SetBorderSize(0);
    legend0->Draw();

    // Canvas 1
    cAll->cd(2);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    xF1_h0->Draw();
    xF1_h1->Draw("same");
    xF1_h2->Draw("same");
    xF1_h3->Draw("same");
    TLegend *legend1 = new TLegend(0.56, 0.70, 0.79, 0.88);
    legend1->SetHeader("0.1 < xF < 0.125", "C");
    legend1->SetTextSize(0.05);
    legend1->AddEntry(xF1_h2, "Signal", "f");
    legend1->AddEntry(xF1_h3, "Sideband", "f");
    legend1->SetBorderSize(0);
    legend1->Draw();

    // Canvas 2
    cAll->cd(3);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    xF2_h0->Draw();
    xF2_h1->Draw("same");
    xF2_h2->Draw("same");
    xF2_h3->Draw("same");
    TLegend *legend2 = new TLegend(0.56, 0.70, 0.79, 0.88);
    legend2->SetHeader("0.125 < xF < 0.175", "C");
    legend2->SetTextSize(0.05);
    legend2->AddEntry(xF2_h2, "Signal", "f");
    legend2->AddEntry(xF2_h3, "Sideband", "f");
    legend2->SetBorderSize(0);
    legend2->Draw();

    // Canvas 3
    cAll->cd(4);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    xF3_h0->Draw();
    xF3_h1->Draw("same");
    xF3_h2->Draw("same");
    xF3_h3->Draw("same");
    TLegend *legend3 = new TLegend(0.56, 0.70, 0.79, 0.88);
    legend3->SetHeader("0.175 < xF < 0.225", "C");
    legend3->SetTextSize(0.05);
    legend3->AddEntry(xF3_h2, "Signal", "f");
    legend3->AddEntry(xF3_h3, "Sideband", "f");
    legend3->SetBorderSize(0);
    legend3->Draw();

    // Canvas 4
    cAll->cd(5);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    xF4_h0->Draw();
    xF4_h1->Draw("same");
    xF4_h2->Draw("same");
    xF4_h3->Draw("same");
    TLegend *legend4 = new TLegend(0.56, 0.70, 0.79, 0.88);
    legend4->SetHeader("0.225 < xF < 0.375", "C");
    legend4->SetTextSize(0.05);
    legend4->AddEntry(xF4_h2, "Signal", "f");
    legend4->AddEntry(xF4_h3, "Sideband", "f");
    legend4->SetBorderSize(0);
    legend4->Draw();

    // The sixth pad (bottom right) is intentionally left empty as requested.
    cAll->cd(6);

}
