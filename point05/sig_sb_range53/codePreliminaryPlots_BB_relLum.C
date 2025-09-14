void codePreliminaryPlots_BB_relLum(){
//TCanvas c1 for pi0, c2 for bkg, c3 for raw sb, c1 for raw sig
	TFile *f = TFile::Open("PreliminaryPlots_All.root");

	auto *mg1 = (TMultiGraph*)f->Get("pi0_AN_crossRatio");
	auto *mg2 = (TMultiGraph*)f->Get("pi0_AN_crossRatio_YB");

	auto *mgPi0 = new TMultiGraph();
	mgPi0->Add(mg1); 
        //mgPi0->Add(mg2);
	mgPi0->GetXaxis()->SetTitle("|x_{F}|");
	//mgPi0->GetYaxis()->SetTitle("A_{N}^{raw}");
        //mgPi0->GetYaxis()->SetTitle("Background A_{N}"); //background
        mgPi0->GetYaxis()->SetTitle("#pi^{0} A_{N}"); //pi0
	mgPi0->GetXaxis()->SetTitleSize(0.05);
        mgPi0->GetYaxis()->SetTitleSize(0.05);
	mgPi0->GetXaxis()->SetLabelSize(0.05);
        mgPi0->GetYaxis()->SetLabelSize(0.05);
//	mgPi0->GetYaxis()->SetRangeUser(-0.04, 0.12); //final pi0
//	mgPi0->GetYaxis()->SetRangeUser(-0.02, 0.04);	
        TCanvas *c5 = new TCanvas("c5", "Pi0 AN Plot crossRatio Blue Beam", 1000, 500);
    c5->SetLeftMargin(0.15);
    c5->SetBottomMargin(0.15);
    c5->SetRightMargin(0.05);
    c5->SetTopMargin(0.08);

	mgPi0->Draw("AP");

	// Create a TLegend instance
    //TLegend *legend = new TLegend(0.65, 0.70, 0.88, 0.88); //right side 
    TLegend *legend = new TLegend(0.25, 0.68, 0.45, 0.88); //left side pi0 and background
    legend->SetHeader("p^{#uparrow} + p #rightarrow #pi^{0} + X", "C"); // "C" centers the header

    TLegendEntry *le0 = legend->AddEntry((TObject*)0, "#sqrt{s} = 200 GeV", "C");
	
    //legend->AddEntry(nullptr,"#sqrt s = 200 GeV","h");
    legend->SetFillColor(kWhite);
    legend->SetBorderSize(0);

    // Add entries using NULL pointers for the object
    // This allows you to set the marker attributes directly on the entry.
    
    // Entry 1: Blue square
    TLegendEntry *le1 = legend->AddEntry((TObject*)0, "x_{F} > 0", "P");
    le1->SetMarkerStyle(kFullCircle);
    le1->SetMarkerColor(kBlue);
    le1->SetMarkerSize(1.5);
    
    // Entry 2: Red circle
/*    TLegendEntry *le2 = legend->AddEntry((TObject*)0, "x_{F} < 0", "P");
    le2->SetMarkerStyle(kFullSquare);
    le2->SetMarkerColor(807);
    le2->SetMarkerSize(1.5);
*/ 
    legend->Draw();

//For Raw AN plots
/*    TLegend *legend1 = new TLegend(0.57, 0.70, 0.80, 0.88);
    TLegendEntry *le3 = legend1->AddEntry((TObject*)0, "signal region", "C");
    legend1->SetFillColor(kWhite);
    legend1->SetBorderSize(0);
    legend1->Draw();
*/
//For pi0 AN plots
    //TLegend *legend1 = new TLegend(0.60, 0.70, 0.83, 0.88);
    TLegend *legend1 = new TLegend(0.50, 0.70, 0.81, 0.83);
//    TLegendEntry *le3 = legend1->AddEntry((TObject*)0, "#bf{STAR Preliminary}", "C");
    TLegendEntry *le4 = legend1->AddEntry((TObject*)0, " Beam pol. err. 3.5%", "C"); //Blue 3.02%
  //  TLegendEntry *le5 = legend1->AddEntry((TObject*)0, "Yellow Beam pol. err. 3.21%", "C");
    TLegendEntry *le6 = legend1->AddEntry((TObject*)0, "not shown in plot", "C");
    TLegendEntry *le7 = legend1->AddEntry((TObject*)0, "x_{F} < 0 points shifted manually", "C");
    legend1->SetFillColor(kWhite);
    legend1->SetBorderSize(0);
    legend1->SetTextSize(0.045);
    legend1->Draw();

	Double_t x_min = mgPi0->GetHistogram()->GetXaxis()->GetXmin();
        Double_t x_max = mgPi0->GetHistogram()->GetXaxis()->GetXmax();
	Double_t y_value = 0.0;
	TLine *line = new TLine(x_min, y_value, x_max, y_value);
	line->Draw("same");
	
	//c4->SaveAs("raw_sig_AN_direct.png");

    TFile *outfile = new TFile("PreliminaryPlots_All.root","UPDATE");
    c5->Write();
    outfile->Close();

    c5->Update();
    
}
