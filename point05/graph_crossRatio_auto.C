void graph_crossRatio_auto(int range_input, string beam_input, string region_input){


	std::ofstream outfile;
	outfile.open(Form("sig_sb_range%d/%sBeam/aN_mod_crossRatio_%s_goodChi2.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::out);
        const int xFBins = 5; //4
        double pol;
        const double pol_blue = 0.5785;
        const double pol_yellow = 0.5872;
        if(beam_input == "Blue"){pol = pol_blue;}
        else if(beam_input == "Yellow"){pol = pol_yellow;}
	
	double aN_sb_phi[xFBins]; double aN_sb_phi_err[xFBins];
	double aN_sb_cosphi[xFBins]; double aN_sb_cosphi_err[xFBins];
	//double xF_val[xFBins] = {0.075, 0.125, 0.20, 0.30};
        double xF_val[xFBins] = {0.0875,0.1125,0.15,0.2,0.3};
	gStyle->SetOptFit(1111);

// Global style setting to show fit parameters on the canvas
    gStyle->SetOptFit(1111);

    // Arrays to store the graphs and fit parameters for all bins
    auto graphs = new TGraphErrors*[xFBins];
    auto graphs1 = new TGraphErrors*[xFBins];

    // The main loop
    for (int i = 0; i < xFBins; ++i) {
        int n = 0;
        fstream file;
        
        // Open the correct file for this iteration
        file.open(Form("sig_sb_range%d/%sBeam/auto_mod_crossRatio_%s_xF%d.txt", range_input, beam_input.c_str(), region_input.c_str(), i), std::ios::in);
        if (!file.is_open()) {
            cout << "Error: Could not open file for xF bin " << i << endl;
            continue; // Skip to next iteration if file not found
        }

        double phi_val_xF, crossRatio_xF, crossRatioErr_xF;

        // Create new TGraphErrors objects for this iteration and store their pointers
        graphs[i] = new TGraphErrors();
        graphs1[i] = new TGraphErrors();

        // Set titles and styles dynamically
        graphs[i]->SetTitle(Form("Cross Ratio for xF%d bin xF = %.3f; cos #phi; Cross_Ratio", i, xF_val[i]));
        graphs[i]->SetMarkerColor(800 + 10 - i); 
        graphs[i]->SetMarkerStyle(20);
        
        graphs1[i]->SetTitle(Form("Cross Ratio for xF%d bin xF = %.3f; #phi; Cross_Ratio", i, xF_val[i]));
        graphs1[i]->SetMarkerColor(800 + 10 - i);
        graphs1[i]->SetMarkerStyle(20);

        // Read data and populate the graphs
        while (file >> phi_val_xF >> crossRatio_xF >> crossRatioErr_xF) {
            graphs[i]->SetPoint(n, TMath::Cos(phi_val_xF), crossRatio_xF);
            graphs[i]->SetPointError(n, 0, crossRatioErr_xF);
            graphs1[i]->SetPoint(n, phi_val_xF, crossRatio_xF);
            graphs1[i]->SetPointError(n, 0, crossRatioErr_xF);
            n++;
        }
        file.close();

        // Create a new canvas for the cos(phi) fit and draw
        TCanvas *c = new TCanvas(Form("c%d", 2*i + 1), Form("Canvas %d (cos phi)", 2*i + 1));
        TF1 *f_cos = new TF1(Form("f_cos%d", i), "pol1", -1.5, 1.5);
        // Set initial parameters based on the manual code
        switch(i) {
            case 0:
                f_cos->SetParameters(0, 1);
                break;
            case 1:
                f_cos->SetParameters(0, 1);
                break;
            case 2:
                f_cos->SetParameters(0, 1);
                break;
            case 3:
                f_cos->SetParameters(0, 1);
                break;
        }
	graphs[i]->Fit(f_cos, "RV+");
        graphs[i]->Draw("AP");

        // Store the fit parameters
        aN_sb_phi[i] = f_cos->GetParameter(1);
        aN_sb_phi_err[i] = f_cos->GetParError(1);

        // Create a new canvas for the phi fit and draw
        TCanvas *c_phi = new TCanvas(Form("c%d_phi", 2*i + 2), Form("Canvas %d (phi)", 2*i + 2));
        TF1 *f_phi = new TF1(Form("f_phi%d", i), "[0] + [1]*cos(x)", -3.5, 3.5);
        // Set initial parameters based on the manual code
        switch(i) {
            case 0:
                f_phi->SetParameters(0, 1);
                break;
            case 1:
                f_phi->SetParameters(0, 1);
                break;
            case 2:
                f_phi->SetParameters(0, 1);
                break;
            case 3:
                f_phi->SetParameters(0, 1);
                break;
        }
	graphs1[i]->Fit(f_phi, "RV+");
        graphs1[i]->Draw("AP"); // Corrected this line to draw the phi graph

        // Store the fit parameters
        aN_sb_cosphi[i] = f_phi->GetParameter(1);
        aN_sb_cosphi_err[i] = f_phi->GetParError(1);
    }

	for(int i=0; i<xFBins; i++){
		cout<<"xF = "<<xF_val[i]<<" aN_sb_phi = "<<aN_sb_phi[i]/pol<<" aN_sb_phi_err = "<<aN_sb_phi_err[i]/pol<<endl;
		cout<<"xF = "<<xF_val[i]<<" aN_sb_cosphi = "<<aN_sb_cosphi[i]/pol<<" aN_sb_cosphi_err = "<<aN_sb_cosphi_err[i]/pol<<endl;
		outfile<<xF_val[i]<<" "<<aN_sb_phi[i]/pol<<" "<<aN_sb_phi_err[i]/pol<<" "<<aN_sb_cosphi[i]/pol<<" "<<aN_sb_cosphi_err[i]/pol<<endl; 

	}

}
