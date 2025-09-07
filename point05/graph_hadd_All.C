void graph_hadd_All(int range_input, string region_input){

	TFile *infile = TFile::Open("Raw_aN_All_crossRatio_relLum.root","READ");
	
	auto *mg1 = (TMultiGraph*)infile->Get(Form("graph_mod_%s_range%d",region_input.c_str(),range_input));
	auto *mg2 = (TMultiGraph*)infile->Get(Form("graph_unmod_%s_range%d",region_input.c_str(),range_input));
	auto *mg3 = (TMultiGraph*)infile->Get(Form("graph_mod_%s_crossRatio_range%d",region_input.c_str(),range_input));
        auto *mg4 = (TMultiGraph*)infile->Get(Form("graph_unmod_%s_crossRatio_range%d",region_input.c_str(),range_input));

	auto *mgAll = new TMultiGraph();
	mgAll->Add(mg1); mgAll->Add(mg2); mgAll->Add(mg3); mgAll->Add(mg4);
	
	TCanvas *c0 = new TCanvas();
	mgAll->SetTitle("Raw A_{N} for sideband region; xF; A^{raw}_{N}");
	mgAll->Draw("AP");
	c0->BuildLegend();
	TLine *line1 = new TLine(0,0,1,0);
	line1->Draw();	
}
