void tprofile_crossRatio(){

	double aN_sb_phi[4]; double aN_sb_phi_err[4];
        double aN_sb_cosphi[4]; double aN_sb_cosphi_err[4];
        double xF_val[4] = {0.075, 0.125, 0.20, 0.30};

        int n0=0;
        fstream file0;
        //file0.open("mod_crossRatio_sig_xF1.txt");
        file0.open("sig_sb_range0/BlueBeam/crossRatio_BBsig_xF1.txt");
        double phi_val_xF0, crossRatio_xF0, crossRatioErr_xF0;

	TProfile *hprof = new TProfile("hprof","Cross Ratio for xF1 bin; #phi; Cross_ratio",100,-2,2);

	while(1){
		file0>>phi_val_xF0>>crossRatio_xF0>>crossRatioErr_xF0;
		hprof->Fill(phi_val_xF0, crossRatio_xF0);

		n0++;

	}

	hprof->Draw();
}
