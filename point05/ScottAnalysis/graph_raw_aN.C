void graph_raw_aN(string beam_input, string method_input){

//range_input = 0/1/2/3/4
//beam_input = Blue/Yellow
//method_input = crossRatio/relLum

	const double pol_blue = 0.5785;
	const double pol_yellow = 0.5872;
	double pol; double R_sys;
	const double R_sys_blue = 0.035848;
	const double R_sys_yellow = 0.0276967;
	//double xF_val[4] = {0.075, 0.125, 0.20, 0.30};
	int marker_color; int marker_style_sig, marker_style_sb; double marker_size;

	if(beam_input == "Blue"){pol = pol_blue; R_sys = R_sys_blue; marker_color = 600;}
	else if(beam_input == "Yellow"){pol = pol_yellow; R_sys = R_sys_yellow; marker_color = 807;}
	if(method_input == "crossRatio"){marker_style_sig = 20; marker_style_sb = 24; marker_size = 1.2;}
	else if(method_input == "relLum"){marker_style_sig = 21; marker_style_sb = 25; marker_size = 1.2;}

	cout<<"BlueBeam signal region"<<endl;
	int n0 = 0;
	fstream file0;
	file0.open(Form("aN_mod_%s_sig_goodChi2.txt",method_input.c_str()),std::ios::in); 
        double xF_val, aN_sig_phi, aN_sig_phi_err, aN_sig_cosphi, aN_sig_cosphi_err;
        auto gr_phi = new TGraphAsymmErrors();
        gr_phi->SetTitle("Raw A_{N} for sideband region; xF; A^{raw}_{N}");
        gr_phi->SetMarkerColor(marker_color); gr_phi->SetLineColor(marker_color); gr_phi->SetMarkerStyle(marker_style_sig); gr_phi->SetMarkerSize(marker_size);
	
	while(1){
		file0>>xF_val>>aN_sig_phi>>aN_sig_phi_err>>aN_sig_cosphi>>aN_sig_cosphi_err;
		
		gr_phi->SetPoint(n0, xF_val, aN_sig_phi);
                gr_phi->SetPointError(n0, 0.0, 0.0,(aN_sig_phi_err),(aN_sig_phi_err));

                cout<<n0<<" "<<xF_val<<" "<<aN_sig_phi<<" "<<aN_sig_phi_err<<" "<<aN_sig_phi<<endl;        	

	n0++;
		if(file0.eof()) break;
        }
        file0.close();

	cout<<"Blue Beam sideband region"<<endl;
        int n1 = 0; //const double y_offset = 0.005;
        fstream file1;
	file1.open(Form("aN_mod_%s_sb_goodChi2.txt",method_input.c_str()),std::ios::in);
        double xF_val_y, aN_sb_phi, aN_sb_phi_err, aN_sb_cosphi, aN_sb_cosphi_err;
        auto gr_phi_sb = new TGraphAsymmErrors();
        gr_phi_sb->SetTitle("Raw A_{N} for sideband region; xF; A^{raw}_{N}");
        gr_phi_sb->SetMarkerColor(marker_color); gr_phi_sb->SetLineColor(marker_color); gr_phi_sb->SetMarkerStyle(marker_style_sb); gr_phi_sb->SetMarkerSize(marker_size);

        while(1){
                file1>>xF_val_y>>aN_sb_phi>>aN_sb_phi_err>>aN_sb_cosphi>>aN_sb_cosphi_err;
		 
                gr_phi_sb->SetPoint(n1, xF_val_y, aN_sb_phi);
		gr_phi_sb->SetPointError(n1, 0.0, 0.0,(aN_sb_phi_err),(aN_sb_phi_err));
                cout<<n1<<" "<<xF_val_y<<" "<<aN_sb_phi<<" "<<aN_sb_phi_err<<" "<<aN_sb_phi<<endl;
                n1++;
                if(file1.eof()) break;
        }
        file1.close();

	TCanvas *c1 = new TCanvas();
	gr_phi->Draw("AP");
	gr_phi_sb->Draw("P SAME");
	
	auto *mg = new TMultiGraph();
	mg->Add(gr_phi); mg->Add(gr_phi_sb);
	gr_phi->SetTitle("#pi^{0} A_{N}"); gr_phi_sb->SetTitle("Bg A_{N}"); 
	//gr_phi->SetTitle(Form("signal %s", method_input.c_str())); gr_phi_sb->SetTitle(Form("sideband %s", method_input.c_str()));
	mg->SetTitle(Form("A_{N} for %s Beam using Weibull Bg, Sk Gaus Sig; xF; A_{N}",beam_input.c_str()));
	TCanvas *c2 = new TCanvas();
	mg->Draw("AP"); c2->BuildLegend();
	
	// Get the x-axis range of the TGraph
	Double_t x_min = mg->GetHistogram()->GetXaxis()->GetXmin();
	Double_t x_max = mg->GetHistogram()->GetXaxis()->GetXmax();

	// Set the y-coordinate for your horizontal line
	Double_t y_value = 0.0; // Example y-value

	// Create and draw the TLine with the adjusted x-coordinates
	TLine *line = new TLine(x_min, y_value, x_max, y_value);
	//line.SetLineColor(kRed); // Optional: Set line color
	//line.SetLineWidth(2);    // Optional: Set line width
	line->Draw("same");

/*	TFile *f = TFile::Open(Form("aN_all_range%d.root",range_input),"UPDATE");
	f->cd();
 	mg->Write(Form("graph_raw_aN_%sBeam_%s",beam_input.c_str(),method_input.c_str()));
*/
}
