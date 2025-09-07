void aN_bkg_pi0(){

	const int xFBins = 4;
	double pol = 58.18/100;
	double aN_sig[4], aN_sig_err[4], aN_sb[4], aN_sb_err[4];
	double xF_val[4]; double fsig_sig[4], fsig_sb[4];

	//Read aN_crossRatio_sig

	//cout<<"aN_crossRatio_sig"<<endl;
	int n0 = 0;
	fstream file0;
	file0.open("aN_crossRatio_sig.txt");
	double xF, aN_phi, aN_phierr, aN_cosphi, aN_cosphierr;
	while(file0>>xF>>aN_phi>>aN_phierr>>aN_cosphi>>aN_cosphierr){
		
		xF_val[n0] = xF; aN_sig[n0] = aN_phi; aN_sig_err[n0] = aN_phierr;
		
	//	cout<<"aN_sig = "<<aN_sig[n0]<<endl;
	    n0++;
            //if(file0.eof()) break;
	}

	file0.close();

	//cout<<endl;
	//cout<<"aN_crossRatio_sb"<<endl;

	//Read aN_crossRatio_sb
	int n1 = 0;
        fstream file1;
        file1.open("aN_crossRatio_sb.txt");
        double xF1, aN_phi1, aN_phierr1, aN_cosphi1, aN_cosphierr1;
        while(file1>>xF1>>aN_phi1>>aN_phierr1>>aN_cosphi1>>aN_cosphierr1){
                
		 aN_sb[n1] = aN_phi1; aN_sb_err[n1] = aN_phierr1;
        
	//	 cout<<"aN_sb = "<<aN_sb[n1]<<endl;
            n1++;
           // if(file1.eof()) break;
        }

        file1.close();

	//cout<<endl;
	//cout<<"fsig_sig and fsig_sb"<<endl;

	//Read fsig_sig and fsig_sb for Levy Func

	int n2 = 0;
	fstream file2;
	file2.open("fSig_BB_LogNormal.txt");
	double fsig_sig_val, fsig_sb_val;
	while(file2>>fsig_sig_val>>fsig_sb_val){

		fsig_sig[n2] = fsig_sig_val; fsig_sb[n2] = fsig_sb_val;
	  //  	cout<<"fsig_sig = "<<fsig_sig[n2]<<" fsig_sb = "<<fsig_sb[n2]<<endl;
	    n2++;
	    //if(file2.eof()) break;

	}

	file2.close();



	//cout<<endl;
	cout<<"aN_pi0 and aN_bkg"<<endl;
	//Calculation of aN_pi0 and aN_bkg starts here
	ofstream outfile; outfile.open("aN_pi0_bkg_LogNormal.txt");
	double aN_pi0[4], aN_bkg[4], aN_pi0_err[4], aN_bkg_err[4];

	for(int i=0; i<xFBins; i++){

//	cout<<"aN_sig = "<<aN_sig[i]<<" aN_sb = "<<aN_sb[i]<<endl;
//	cout<<"fsig_sig = "<<fsig_sig[i]<<" fsig_sb = "<<fsig_sb[i]<<endl;
		double num_1 = aN_sig[i] * (1 - fsig_sb[i]); double num_2 = aN_sb[i] * (1 - fsig_sig[i]);
		double den_1 = fsig_sig[i] * (1 - fsig_sb[i]); double den_2 = fsig_sb[i] * (1 - fsig_sig[i]);
		double num_pi0 = pol * (num_1 - num_2); double den_pi0 = den_1 - den_2;
		aN_pi0[i] = num_pi0/den_pi0;

		double num_3 = aN_sig_err[i] * (1 - fsig_sb[i]); double num_4 = aN_sb_err[i] * (1 - fsig_sig[i]);		      double num_pi0_err = pol * sqrt( pow(num_3,2) + pow(num_4,2) ); 
		aN_pi0_err[i] = num_pi0_err/den_pi0;

		
		double num_5 = aN_sig[i] * fsig_sb[i]; double num_6 = aN_sb[i] * fsig_sig[i];
		double den_3 = (1 - fsig_sig[i]) * fsig_sb[i]; double den_4 = (1 - fsig_sb[i]) * fsig_sig[i];
		double num_bkg = pol * (num_5 - num_6); double den_bkg = den_3 - den_4;
		aN_bkg[i] = num_bkg/den_bkg;

//		cout<<"num5 = "<<num_5<<" num6 = "<<num_6<<" num_bkg = "<<num_bkg<<endl;
//               cout<<"den3 = "<<den_3<<" den4 = "<<den_4<<" den_bkg = "<<den_bkg<<endl;
//                cout<<"aN_bkg = "<<num_bkg/den_bkg<<endl;

		double num_7 = aN_sig_err[i] * fsig_sb[i]; double num_8 = aN_sb_err[i] * fsig_sig[i];
		double num_bkg_err = pol * sqrt( pow(num_7,2) + pow(num_8,2) );
		aN_bkg_err[i] = num_bkg_err/den_bkg;

		cout<<"aN_pi0 = "<<aN_pi0[i]<<" aN_pi0_err = "<<aN_pi0_err[i]<<" aN_bkg = "<<aN_bkg[i]<<" aN_bkg_err = "<<aN_bkg_err[i]<<endl;
		outfile<<xF_val[i]<<" "<<aN_pi0[i]<<" "<<aN_pi0_err[i]<<" "<<aN_bkg[i]<<" "<<aN_bkg_err[i]<<endl;
	}


	


}
