void graph_aN_bkg_pi0_v1(int range_input, string region_input, string method_input)

{
//range_input = 0/1/2/3/4
//region_input = sig/sb
//method_input = crossRatio/relLum

//This only works for the signal region of Blue Beam as of now

	const double pol = 0.5785;
        const double pol_yellow = 0.5872;
        //double xF_val[4] = {0.075, 0.125, 0.20, 0.30};
        int colorBB, colorYB; int marker_style; double marker_size;
        /*if(range_input == 1){colorBB = 432; colorYB = 807; marker = 29;} //blue = 600; cyan = 432; yellow = 800; orange = 807;
        else if(range_input == 0){colorBB = 432; colorYB = 807; marker = 29;} //cross = 29; square = 21
        else if(range_input == 2){colorBB = 432; colorYB = 807; marker = 29;}*/

        if(method_input == "crossRatio"){colorBB = 600; colorYB = 807; marker_style = 20; marker_size = 1.2; }
        else if(method_input == "relLum"){colorBB = 600; colorYB = 807; marker_style = 21; marker_size = 1.2;}
//colorBB = 600 for Blue;

	double offset_0 = 0.005; double offset_1 = 0.007; double offset_2 = 0.009; 	

        cout<<"BlueBeam"<<endl; //Yellow
        int n0 = 0;
        fstream file0;
        file0.open(Form("sig_sb_range%d/BlueBeam/aN_mod_indv_%s_%s_goodChi2.txt",range_input,method_input.c_str(),region_input.c_str()),std::ios::in);
        double xF_val, aN_sig_phi, aN_sig_phi_err, aN_sig_cosphi, aN_sig_cosphi_err;
        auto gr_phi = new TGraphAsymmErrors(); auto gr_cosphi = new TGraphErrors();
        gr_phi->SetTitle("Raw A_{N} for signal region; xF; A^{raw}_{N}");
        gr_phi->SetMarkerColor(colorBB); gr_phi->SetLineColor(colorBB); //Uncomment for Blue Beam
	//gr_phi->SetMarkerColor(colorYB); gr_phi->SetLineColor(colorYB); //Uncomment for Yellow Beam
	gr_phi->SetMarkerStyle(marker_style); gr_phi->SetMarkerSize(marker_size);
        gr_cosphi->SetTitle("Raw A_N for signal region - par cos#phi");
        gr_cosphi->SetMarkerColor(kBlue); gr_cosphi->SetMarkerStyle(20); //Uncomment for Blue Beam
//	gr_cosphi->SetMarkerColor(colorYB); gr_cosphi->SetMarkerStyle(20); //Uncomment for Yellow Beam
        cout<<"Raw AN"<<endl;
        while(1){
                file0>>xF_val>>aN_sig_phi>>aN_sig_phi_err>>aN_sig_cosphi>>aN_sig_cosphi_err;
		gr_phi->SetPoint(n0, xF_val, aN_sig_phi);
                gr_phi->SetPointError(n0, 0.0, 0.0, aN_sig_phi_err, aN_sig_phi_err );

                cout<<n0<<" "<<xF_val<<" "<<aN_sig_phi<<" "<<aN_sig_phi_err<<endl;
                n0++;
                if(file0.eof()) break;
        }
        file0.close();

	int m0 = 0;
	fstream fnfile0;
	fnfile0.open(Form("sig_sb_range%d/BlueBeam/aN_pi0_bkg_Levy_%s.txt",range_input,method_input.c_str()), std::ios::in);
	double xF_val_0, aN_pi0_0, aN_pi0_err_0, aN_bkg_0, aN_bkg_err_0;
	auto gr_levy = new TGraphAsymmErrors(); gr_levy->SetMarkerStyle(23); gr_levy->SetMarkerSize(1.2); 
	gr_levy->SetMarkerColor(colorBB); gr_levy->SetLineColor(colorBB); //Uncomment for Blue Beam
        //gr_levy->SetMarkerColor(colorYB); //Uncommnet for Yellow Beam
        cout<<"Levy"<<endl;
	while(1){
		fnfile0>>xF_val_0>>aN_pi0_0>>aN_pi0_err_0>>aN_bkg_0>>aN_bkg_err_0;
		gr_levy->SetPoint(m0, xF_val_0 + offset_0, aN_pi0_0);
  		gr_levy->SetPointError(m0, 0.0, 0.0, aN_pi0_err_0, aN_pi0_err_0);
		cout<<xF_val_0<<" "<<aN_pi0_0<<endl;
		m0++;
		if(fnfile0.eof()) break;
	} fnfile0.close();

	int m1 = 0;
        fstream fnfile1;
        fnfile1.open(Form("sig_sb_range%d/BlueBeam/aN_pi0_bkg_Cheb_%s.txt",range_input,method_input.c_str()), std::ios::in);
 	double xF_val_1, aN_pi0_1, aN_pi0_err_1, aN_bkg_1, aN_bkg_err_1;
        auto gr_lognormal = new TGraphAsymmErrors(); gr_lognormal->SetMarkerStyle(22); gr_lognormal->SetMarkerColor(colorBB); gr_lognormal->SetMarkerSize(1.2); gr_lognormal->SetLineColor(colorBB);
	cout<<"Chebyshev"<<endl;
        while(1){
                fnfile1>>xF_val_1>>aN_pi0_1>>aN_pi0_err_1>>aN_bkg_1>>aN_bkg_err_1;
                gr_lognormal->SetPoint(m1, xF_val_1 + offset_1, aN_pi0_1);
		gr_lognormal->SetPointError(m1, 0.0, 0.0, aN_pi0_err_1, aN_pi0_err_1);
                cout<<xF_val_1<<" "<<aN_pi0_1<<endl;
		m1++;
                if(fnfile1.eof()) break;
        } fnfile1.close();

	int m2 = 0;
        fstream fnfile2;
        fnfile2.open(Form("sig_sb_range%d/BlueBeam/aN_pi0_bkg_Weibull_%s.txt",range_input,method_input.c_str()), std::ios::in);
        double xF_val_2, aN_pi0_2, aN_pi0_err_2, aN_bkg_2, aN_bkg_err_2;
        auto gr_weibull = new TGraphAsymmErrors(); gr_weibull->SetMarkerStyle(29); gr_weibull->SetMarkerSize(1.2); gr_weibull->SetLineColor(colorBB);
	gr_weibull->SetMarkerColor(colorBB); //Uncomment for Blue Beam
        //gr_weibull->SetMarkerColor(colorYB); //Uncomment for Yellow Beam
	cout<<"Weibull"<<endl;
        while(1){
                fnfile2>>xF_val_2>>aN_pi0_2>>aN_pi0_err_2>>aN_bkg_2>>aN_bkg_err_2;
                gr_weibull->SetPoint(m2, xF_val_2 + offset_2, aN_pi0_2);
                gr_weibull->SetPointError(m2, 0.0, 0.0, aN_pi0_err_2, aN_pi0_err_2);
		cout<<xF_val_2<<" "<<aN_pi0_2<<endl;
                m2++;
                if(fnfile2.eof()) break;
        } fnfile2.close();
/*
	cout<<"YellowBeam"<<endl;
        int n1 = 0; const double y_offset = 0.005;
        fstream file1;
        file1.open(Form("sig_sb_range%d/YellowBeam/aN_mod_%s_%s_goodChi2.txt",range_input,method_input.c_str(),region_input.c_str()),std::ios::in);
        double xF_val_y, aN_sig_phi_y, aN_sig_phi_err_y, aN_sig_cosphi_y, aN_sig_cosphi_err_y;
        auto gr_phi_y = new TGraphAsymmErrors(); auto gr_cosphi_y = new TGraphErrors();
        gr_phi_y->SetTitle("Raw A_{N} for signal region; xF; A^{raw}_{N}");
        gr_phi_y->SetMarkerColor(colorYB); gr_phi_y->SetLineColor(colorYB); gr_phi_y->SetMarkerStyle(marker_style); gr_phi_y->SetMarkerSize(marker_size);
        gr_cosphi_y->SetTitle("Raw A_N for signal region - par cos#phi");
        gr_cosphi_y->SetMarkerColor(kOrange); gr_cosphi_y->SetMarkerStyle(20);

        while(1){
                file1>>xF_val_y>>aN_sig_phi_y>>aN_sig_phi_err_y>>aN_sig_cosphi_y>>aN_sig_cosphi_err_y;
                //gr_phi_y->SetPoint(n1, xF_val_y + y_offset, aN_sig_phi_y/pol_yellow);
                gr_phi_y->SetPoint(n1, xF_val_y + y_offset, aN_sig_phi_y/pol_yellow);
                gr_phi_y->SetPointError(n1, 0.0, 0.0,(aN_sig_phi_err_y/pol_yellow),(aN_sig_phi_err_y/pol_yellow));
                gr_cosphi_y->SetPoint(n1, xF_val_y + y_offset, aN_sig_cosphi_y/pol_yellow);
                gr_cosphi_y->SetPointError(n1, 0, aN_sig_cosphi_err_y/pol_yellow);

                cout<<n1<<" "<<xF_val_y<<" "<<aN_sig_phi_y/pol_yellow<<" "<<aN_sig_phi_err_y/pol_yellow<<endl;
                n1++;
                if(file1.eof()) break;
        }
        file1.close();

	int m3 = 0;
        fstream fnfile3;
        fnfile3.open(Form("sig_sb_range%d/YellowBeam/aN_pi0_bkg_Levy.txt",range_input),std::ios::in);
        double xF_val_y0, aN_pi0_y0, aN_pi0_err_y0, aN_bkg_y0, aN_bkg_err_y0;
        auto gry_levy = new TGraph(); gry_levy->SetMarkerStyle(23); gry_levy->SetMarkerColor(colorYB);

        while(1){
                fnfile3>>xF_val_y0>>aN_pi0_y0>>aN_pi0_err_y0>>aN_bkg_y0>>aN_bkg_err_y0;
                gry_levy->SetPoint(m3, xF_val_y0 + + y_offset, aN_pi0_y0);
                m3++;
                if(fnfile3.eof()) break;
        } fnfile3.close();

        int m4 = 0;
        fstream fnfile4;
        fnfile4.open(Form("sig_sb_range%d/YellowBeam/aN_pi0_bkg_LogNormal.txt",range_input), std::ios::in);
        double xF_val_y1, aN_pi0_y1, aN_pi0_err_y1, aN_bkg_y1, aN_bkg_err_y1;
        auto gry_lognormal = new TGraph(); gry_lognormal->SetMarkerStyle(22); gry_lognormal->SetMarkerColor(colorYB);

        while(1){
                fnfile4>>xF_val_y1>>aN_pi0_y1>>aN_pi0_err_y1>>aN_bkg_y1>>aN_bkg_err_y1;
                gry_lognormal->SetPoint(m4, xF_val_y1 + y_offset, aN_pi0_y1);
                m4++;
                if(fnfile4.eof()) break;
        } fnfile4.close();

        int m5 = 0;
        fstream fnfile5;
        fnfile5.open(Form("sig_sb_range%d/YellowBeam/aN_pi0_bkg_Weibull.txt",range_input), std::ios::in);

        while(1){
                fnfile5>>xF_val_y2>>aN_pi0_y2>>aN_pi0_err_y2>>aN_bkg_y2>>aN_bkg_err_y2;
                gry_weibull->SetPoint(m5, xF_val_y2 + y_offset, aN_pi0_y2);
                m5++;
                if(fnfile5.eof()) break;
        } fnfile5.close();
*/	

	auto mg = new TMultiGraph();
	mg->Add(gr_phi); gr_phi->SetTitle("Incl. pi0 A_{N} sig. region"); 
//	mg->Add(gr_levy); gr_levy->SetTitle("pi0 A_{N} levy bg");
//	mg->Add(gr_lognormal); gr_lognormal->SetTitle("pi0 A_{N} cheb3 bg"); 
//	mg->Add(gr_weibull); gr_weibull->SetTitle("pi0 A_{N} weibull bg");
/*	mg->GetXaxis()->SetTitle("x_{F}");
        mg->GetYaxis()->SetTitle("A_{N}");
	mg->GetXaxis()->SetLabelSize(0.05);
        mg->GetYaxis()->SetLabelSize(0.05);
        mg->SetTitle(Form("pi0 A_{N} %s",method_input.c_str()));
	mg->Add(gr_phi_y); gr_phi_y->SetTitle("Raw A_N");
	mg->Add(gry_levy); gry_levy->SetTitle("levy");
	mg->Add(gry_lognormal); gry_lognormal->SetTitle("lognormal"); 
	mg->Add(gry_weibull); gry_weibull->SetTitle("weibull");
*/

	TCanvas *c9 = new TCanvas(); mg->Draw("AP");
// Now that the axes are created, you can access them and set their properties.
    mg->GetXaxis()->SetTitle("x_{F}");
    mg->GetYaxis()->SetTitle("A_{N}");
    
    mg->GetXaxis()->SetLabelSize(0.05);
    mg->GetYaxis()->SetLabelSize(0.05);
    
    mg->SetTitle("pi0 A_{N} method");
    
    // --- Now SetRangeUser() will work correctly ---
    mg->GetYaxis()->SetRangeUser(-0.007, 0.03);

	c9->BuildLegend();

	// Assume 'gr' is your TGraph object and it has been drawn
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
/*
	TFile *f = TFile::Open(Form("aN_all_range%d.root",range_input),"UPDATE");
        f->cd();
        mg->Write(Form("graph_rawaN_pluspi0_test_indvFit_BlueBeam_%s",method_input.c_str()));

*/
}
