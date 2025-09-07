void graph_crossRatio(){


	std::ofstream outfile;
	outfile.open("aN_crossRatio_sb.txt");
        const int xFBins = 4;
	double pol_val = 58.18/100; 
	double aN_sb_phi[4]; double aN_sb_phi_err[4];
	double aN_sb_cosphi[4]; double aN_sb_cosphi_err[4];
	double xF_val[4] = {0.1, 0.15, 0.225, 0.325};

	int n0=0;
	fstream file0;
	file0.open("mod_crossRatio_sb_xF0.txt");
	double phi_val_xF0, crossRatio_xF0, crossRatioErr_xF0;
	auto gr0 = new TGraphErrors(); auto gr01 = new TGraphErrors();
        gr0->SetTitle("Cross Ratio for xF0 bin xF = 0.075; cos #phi; Cross_Ratio"); 
        gr0->SetMarkerColor(2); gr0->SetMarkerStyle(21);
	gr01->SetTitle("Cross Ratio for xF0 bin xF = 0.075; #phi; Cross_Ratio"); 
        gr01->SetMarkerColor(2); gr01->SetMarkerStyle(21);
	while(1){
		file0>>phi_val_xF0>>crossRatio_xF0>>crossRatioErr_xF0;
		gr0->SetPoint(n0, TMath::Cos(phi_val_xF0), crossRatio_xF0);
		gr0->SetPointError(n0, 0, crossRatioErr_xF0);
		gr01->SetPoint(n0, phi_val_xF0, crossRatio_xF0);
                gr01->SetPointError(n0, 0, crossRatioErr_xF0);

	    n0++;
	    if(file0.eof()) break;
	}
	file0.close();

	TCanvas *c1 = new TCanvas();
	TF1 *f1 = new TF1("f1","pol1",-1.5,1.5);
        f1->SetParameters(0,1); gr0->Fit("f1","RW"); gr0->Draw("AP");
	aN_sb_phi[0] = f1->GetParameter(1); aN_sb_phi_err[0] = f1->GetParError(1);

	TCanvas *c2 = new TCanvas();
	TF1 *f2 = new TF1("f2", "[0] + [1]*cos(x)", -3.5,3.5);
	f2->SetParameters(0,1); gr01->Fit("f2", "RW"); gr01->Draw("AP");
	aN_sb_cosphi[0] = f2->GetParameter(1); aN_sb_cosphi_err[0] = f2->GetParError(1);

	int n1=0;
        fstream file1;
        file1.open("mod_crossRatio_sb_xF1.txt");
        double phi_val_xF1, crossRatio_xF1, crossRatioErr_xF1;
        auto gr1 = new TGraphErrors(); auto gr11 = new TGraphErrors();
        gr1->SetTitle("Cross Ratio for xF1 bin xF = 0.125; cos #phi; Cross_Ratio");
        gr1->SetMarkerColor(3); gr1->SetMarkerStyle(21);
        gr11->SetTitle("Cross Ratio for xF0 bin xF = 0.125; #phi; Cross_Ratio"); 
        gr11->SetMarkerColor(3); gr11->SetMarkerStyle(21);     
	 while(1){                
                file1>>phi_val_xF1>>crossRatio_xF1>>crossRatioErr_xF1;
                gr1->SetPoint(n1, TMath::Cos(phi_val_xF1), crossRatio_xF1);
                gr1->SetPointError(n1, 0, crossRatioErr_xF1);
            	gr11->SetPoint(n1, phi_val_xF1, crossRatio_xF1);
                gr11->SetPointError(n1, 0, crossRatioErr_xF1);
	    n1++;
            if(file1.eof()) break;
        }
        file1.close();
        TCanvas *c3 = new TCanvas(); //gr1->Draw("AP");
	TF1 *f3 = new TF1("f3","pol1",-1.5,1.5);
        f3->SetParameters(0.00203,0.000612); 
	gr1->Fit("f3","RW"); gr1->Draw("AP");
        aN_sb_phi[1] = f3->GetParameter(1); aN_sb_phi_err[1] = f3->GetParError(1);

	TCanvas *c4 = new TCanvas();
        TF1 *f4 = new TF1("f4", "[0] + [1]*cos(x)", -3.5,3.5);
        f4->SetParameters(0.00203,0.000612); gr11->Fit("f4", "RW"); gr11->Draw("AP");
	aN_sb_cosphi[1] = f4->GetParameter(1); aN_sb_cosphi_err[1] = f4->GetParError(1);

	int n2=0;
        fstream file2;
        file2.open("mod_crossRatio_sb_xF2.txt");
        double phi_val_xF2, crossRatio_xF2, crossRatioErr_xF2;
        auto gr2 = new TGraphErrors(); auto gr21 = new TGraphErrors();
        gr2->SetTitle("Cross Ratio for xF2 bin xF = 0.2; cos #phi; Cross_Ratio");
        gr2->SetMarkerColor(6); gr2->SetMarkerStyle(21);
	gr21->SetTitle("Cross Ratio for xF0 bin xF = 0.2; #phi; Cross_Ratio"); 
        gr21->SetMarkerColor(6); gr21->SetMarkerStyle(21);    
        while(1){
                file2>>phi_val_xF2>>crossRatio_xF2>>crossRatioErr_xF2;
                gr2->SetPoint(n2, TMath::Cos(phi_val_xF2), crossRatio_xF2);
                gr2->SetPointError(n2, 0, crossRatioErr_xF2);
		gr21->SetPoint(n2, phi_val_xF2, crossRatio_xF2);
                gr21->SetPointError(n2, 0, crossRatioErr_xF2);
            n2++;
            if(file2.eof()) break;
        }
        file2.close();
	TCanvas *c5 = new TCanvas(); //gr1->Draw("AP");
        TF1 *f5 = new TF1("f5","pol1",-1.5,1.5);
        f5->SetParameters(0.00203,0.000612); 
        gr2->Fit("f5","RW"); gr2->Draw("AP");
	aN_sb_phi[2] = f5->GetParameter(1); aN_sb_phi_err[2] = f5->GetParError(1);

        TCanvas *c6 = new TCanvas();
        TF1 *f6 = new TF1("f6", "[0] + [1]*cos(x)", -3.5,3.5);
        f6->SetParameters(0.00203,0.000612); gr21->Fit("f6", "RW"); gr21->Draw("AP");
	aN_sb_cosphi[2] = f6->GetParameter(1); aN_sb_cosphi_err[2] = f6->GetParError(1);

	int n3=0;
        fstream file3;
        file3.open("mod_crossRatio_sb_xF3.txt");
        double phi_val_xF3, crossRatio_xF3, crossRatioErr_xF3;
        auto gr3 = new TGraphErrors(); auto gr31 = new TGraphErrors();
        gr3->SetTitle("Cross Ratio for xF3 bin xF = 0.3; cos phi; Cross_Ratio");
        gr3->SetMarkerColor(7); gr3->SetMarkerStyle(21);
        gr31->SetTitle("Cross Ratio for xF3 bin xF = 0.3; #phi; Cross_Ratio"); 
        gr31->SetMarkerColor(7); gr31->SetMarkerStyle(21);    
	while(1){
                file3>>phi_val_xF3>>crossRatio_xF3>>crossRatioErr_xF3;
                gr3->SetPoint(n3, TMath::Cos(phi_val_xF3), crossRatio_xF3);
                gr3->SetPointError(n3, 0, crossRatioErr_xF3);
            	gr31->SetPoint(n3, phi_val_xF3, crossRatio_xF3);
                gr31->SetPointError(n3, 0, crossRatioErr_xF3);
	    n3++;
            if(file3.eof()) break;
        }
        file3.close();
	TCanvas *c7 = new TCanvas();
	TF1 *f7 = new TF1("f7","pol1",-1.5,1.5);
        f7->SetParameters(0.00203,0.000612);
	gr3->Draw("AP");
	gr3->Fit("f7","RW");
        aN_sb_phi[3] = f7->GetParameter(1); aN_sb_phi_err[3] = f7->GetParError(1);

	TCanvas *c8 = new TCanvas();
        TF1 *f8 = new TF1("f8", "[0] + [1]*cos(x)", -3.5,3.5);
        f8->SetParameters(0.00203,0.000612); gr31->Fit("f8", "RW"); 
	gr31->Draw("AP");
	aN_sb_cosphi[3] = f8->GetParameter(1); aN_sb_cosphi_err[3] = f8->GetParError(1);


	auto mg = new TMultiGraph();
	mg->Add(gr01); gr01->SetTitle("Cross Ratio for xF0 bin xF = 0.075");
	mg->Add(gr11); gr11->SetTitle("Cross Ratio for xF1 bin xF = 0.125");
	mg->Add(gr21); gr21->SetTitle("Cross Ratio for xF2 bin xF = 0.2");
	mg->Add(gr31); gr31->SetTitle("Cross Ratio for xF3 bin xF = 0.3");

	mg->SetTitle("Cross Ratio for xF bins; #phi; Cross_Ratio");
	TCanvas *c9 = new TCanvas(); mg->Draw("AP");
	c9->BuildLegend();


	gStyle->SetOptFit(1111);

	for(int i=0; i<4; i++){
		cout<<"xF = "<<xF_val[i]<<" aN_sb_phi = "<<aN_sb_phi[i]<<" aN_sb_phi_err = "<<aN_sb_phi_err[i]<<endl;
		cout<<"xF = "<<xF_val[i]<<" aN_sb_cosphi = "<<aN_sb_cosphi[i]<<" aN_sb_cosphi_err = "<<aN_sb_cosphi_err[i]<<endl;
		outfile<<xF_val[i]<<" "<<aN_sb_phi[i]<<" "<<aN_sb_phi_err[i]<<" "<<aN_sb_cosphi[i]<<" "<<aN_sb_cosphi_err[i]<<endl; 

	}

}
