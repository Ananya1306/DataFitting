void graph_relLum_auto(int range_input, string beam_input, string region_input){


	std::ofstream outfile;
       outfile.open(Form("sig_sb_range%d/%sBeam/aN_mod_relLum_%s_goodChi2.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::out);
        const int xFBins = 5; //4
        double pol;
        const double pol_blue = 0.5785;
        const double pol_yellow = 0.5872;
        if(beam_input == "Blue"){pol = pol_blue;}
        else if(beam_input == "Yellow"){pol = pol_yellow;}


        double aN_sb_phi[xFBins]; double aN_sb_phi_err[xFBins];
        double aN_sb_cosphi[xFBins]; double aN_sb_cosphi_err[xFBins];


	gStyle->SetOptFit(1111);

	// Arrays to store the graphs and fit parameters for all bins
    auto graphs_phi = new TGraphErrors*[xFBins];
    auto graphs_cosphi = new TGraphErrors*[xFBins];

    // Array of xF values for dynamic title generation
    //const double xF_val[] = {0.075, 0.125, 0.2, 0.3};
    double xF_val[xFBins] = {0.0875,0.1125,0.15,0.2,0.3};
    // The main loop to process all four xF bins
    for (int i = 0; i < xFBins; ++i) {
        int n = 0;
        fstream file;
        
        // Open the correct file for this iteration using a dynamic file path
        file.open(Form("sig_sb_range%d/%sBeam/auto_mod_relLum_%s_xF%d.txt", range_input, beam_input.c_str(), region_input.c_str(), i), std::ios::in);
        if (!file.is_open()) {
            cout << "Error: Could not open file for xF bin " << i << endl;
            continue; // Skip to next iteration if file not found
        }

        double phi_val, relLum, relLumStatErr, relLumSysErr, relLumTotErr;

        // Create new TGraphErrors objects for this iteration and store their pointers
        graphs_phi[i] = new TGraphErrors();
        graphs_cosphi[i] = new TGraphErrors();

        // Set titles and styles dynamically for phi graph
        graphs_phi[i]->SetTitle(Form("xF%d bin xF = %.3f; #phi; RelLum_Ratio", i, xF_val[i]));
        graphs_phi[i]->SetMarkerColor(800 + i); 
        graphs_phi[i]->SetMarkerStyle(21);
        
        // Set titles and styles dynamically for cos(phi) graph
        graphs_cosphi[i]->SetTitle(Form("xF%d bin xF = %.3f; cos #phi; RelLum_Ratio", i, xF_val[i]));
        graphs_cosphi[i]->SetMarkerColor(800 + i);
        graphs_cosphi[i]->SetMarkerStyle(21);

        // Read data and populate the graphs in a more robust loop
        while (file >> phi_val >> relLum >> relLumStatErr >> relLumSysErr >> relLumTotErr ) {
            graphs_phi[i]->SetPoint(n, phi_val, relLum);
            graphs_phi[i]->SetPointError(n, 0, relLumTotErr);
            graphs_cosphi[i]->SetPoint(n, TMath::Cos(phi_val), relLum);
            graphs_cosphi[i]->SetPointError(n, 0, relLumTotErr);
            n++;
        }
        file.close();

        // Create a new canvas for the phi fit
        TCanvas *c_phi = new TCanvas(Form("c_phi%d", i), Form("Canvas %d (phi)", i));
        graphs_phi[i]->Draw("AP");
        TF1 *f_phi = new TF1(Form("f_phi%d", i), "[0] + [1]*cos(x)", -3.5, 3.5);
        f_phi->SetParameters(0, 1);
        graphs_phi[i]->Fit(f_phi, "RV+");
        graphs_phi[i]->Draw("AP");

        // Store the fit parameters
        aN_sb_phi[i] = f_phi->GetParameter(1);
        aN_sb_phi_err[i] = f_phi->GetParError(1);

        // Create a new canvas for the cos(phi) fit
        TCanvas *c_cosphi = new TCanvas(Form("c_cosphi%d", i), Form("Canvas %d (cos phi)", i));
        graphs_cosphi[i]->Draw("AP");
        TF1 *f_cosphi = new TF1(Form("f_cosphi%d", i), "pol1", -1.5, 1.5);
        f_cosphi->SetParameters(0, 1);
        graphs_cosphi[i]->Fit(f_cosphi, "RV+");
        graphs_cosphi[i]->Draw("AP");

        // Store the fit parameters
        aN_sb_cosphi[i] = f_cosphi->GetParameter(1);
        aN_sb_cosphi_err[i] = f_cosphi->GetParError(1);

        // Clean up TF1 objects
        delete f_phi;
        delete f_cosphi;
    }
    

	for(int i=0; i<xFBins; i++){
        	cout<<"xF = "<<xF_val[i]<<" aN_sb_phi = "<<aN_sb_phi[i]<<" aN_sb_phi_err = "<<aN_sb_phi_err[i]<<endl;
                cout<<"xF = "<<xF_val[i]<<" aN_sb_phi/pol = "<<aN_sb_phi[i]/pol<<" aN_sb_phi_err/pol = "<<aN_sb_phi_err[i]/pol<<endl; 
	       outfile<<xF_val[i]<<" "<<aN_sb_phi[i]/pol<<" "<<aN_sb_phi_err[i]/pol<<" "<<aN_sb_cosphi[i]/pol<<" "<<aN_sb_cosphi_err[i]/pol<<endl;

        }


}
