void graph_relLum(int range_input, string beam_input, string region_input){


	std::ofstream outfile;
       outfile.open(Form("sig_sb_range%d/%sBeam/aN_mod_indv_relLum_%s_goodChi2.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::out);
        const int xFBins = 5; //4

	double pol;        
	const double pol_blue = 0.5785;
        const double pol_yellow = 0.5872;
        if(beam_input == "Blue"){pol = pol_blue;}
        else if(beam_input == "Yellow"){pol = pol_yellow;}

	double aN_sb_phi[xFBins]; double aN_sb_phi_err[xFBins];
        double aN_sb_cosphi[xFBins]; double aN_sb_cosphi_err[xFBins];
	double xF_val[xFBins] = {0.0875,0.1125,0.15,0.2,0.3};
        //double xF_val[xFBins] = {0.075, 0.125, 0.20, 0.30};

	int n0=0;
        fstream file0;
        file0.open(Form("sig_sb_range%d/%sBeam/auto_mod_relLum_%s_xF0.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::in);
        double phi_val_xF0, relLum_xF0, relLumStatErr_xF0, relLumSysErr_xF0, relLumTotErr_xF0;
        auto gr0 = new TGraphErrors(); auto gr00 = new TGraphErrors();
        gr0->SetTitle("xF0 bin xF = 0.075;  #phi; RelLum_Ratio"); 
        gr0->SetMarkerColor(2); gr0->SetMarkerStyle(21);
	gr00->SetTitle("xF0 bin xF = 0.075; cos #phi; RelLum_Ratio");
        gr00->SetMarkerColor(2); gr00->SetMarkerStyle(21);
        while(file0>>phi_val_xF0>>relLum_xF0>>relLumStatErr_xF0>>relLumSysErr_xF0>>relLumTotErr_xF0)
              {  gr0->SetPoint(n0, phi_val_xF0, relLum_xF0);
                gr0->SetPointError(n0, 0, relLumStatErr_xF0);
		gr00->SetPoint(n0, cos(phi_val_xF0), relLum_xF0);
                gr00->SetPointError(n0, 0, relLumStatErr_xF0);

		cout<<phi_val_xF0<<" relLum "<<relLum_xF0<<" statErr = "<<relLumStatErr_xF0<<" sysErr = "<<relLumSysErr_xF0<<" "<<" totErr = "<<relLumTotErr_xF0<<endl;
            n0++;
        }

        file0.close();
	TCanvas *c0 = new TCanvas();
	gr0->Draw("AP");
        TF1 *f1 = new TF1("f1","[0] + [1]*cos(x)",-3.5,3.5); //0.0 to 6.3
        f1->SetParameters(0,1); gr0->Fit("f1","RV+"); gr0->Draw("AP");
	TCanvas *c1 = new TCanvas(); gr00->Draw("AP");
	TF1 *f2 = new TF1("f2","pol1",-1.5,1.5);
	f2->SetParameters(0,1); gr00->Fit("f2", "RV+"); gr00->Draw("AP");
	aN_sb_phi[0] = f1->GetParameter(1); aN_sb_phi_err[0] = f1->GetParError(1);
	aN_sb_cosphi[0] = f2->GetParameter(1); aN_sb_cosphi_err[0] = f2->GetParError(1);


	int n1=0;
        fstream file1;
	file1.open(Form("sig_sb_range%d/%sBeam/auto_mod_relLum_%s_xF1.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::in);
        double phi_val_xF1, relLum_xF1, relLumStatErr_xF1, relLumSysErr_xF1, relLumTotErr_xF1;
        auto gr1 = new TGraphErrors(); auto gr11 = new TGraphErrors();
        gr1->SetTitle("xF1 bin xF = 0.125;  #phi; RelLum_Ratio");
        gr1->SetMarkerColor(3); gr1->SetMarkerStyle(21);
        gr11->SetTitle("xF1 bin xF = 0.125; cos #phi; RelLum_Ratio");
        gr11->SetMarkerColor(3); gr11->SetMarkerStyle(21);
        while(file1>>phi_val_xF1>>relLum_xF1>>relLumStatErr_xF1>>relLumSysErr_xF1>>relLumTotErr_xF1){
//                file1>>phi_val_xF1>>relLum_xF1>>relLumStatErr_xF1>>relLumSysErr_xF1>>relLumTotErr_xF1;
                gr1->SetPoint(n1, phi_val_xF1, relLum_xF1);
                gr1->SetPointError(n1, 0, relLumStatErr_xF1);
            	gr11->SetPoint(n1, cos(phi_val_xF1), relLum_xF1);
                gr11->SetPointError(n1, 0, relLumStatErr_xF1);
	    n1++;
        }
        file1.close();
	TCanvas *c2 = new TCanvas();
        gr1->Draw("AP");
	TF1 *f3 = new TF1("f3","[0] + [1]*cos(x)",-3.5,3.5);
        f3->SetParameters(0,1); gr1->Fit("f3","RV+"); gr1->Draw("AP");
        TCanvas *c3 = new TCanvas(); gr11->Draw("AP");
        TF1 *f4 = new TF1("f4","pol1",-1.5,1.5);
        f4->SetParameters(0,1); gr11->Fit("f4", "RV+"); gr11->Draw("AP");
	aN_sb_phi[1] = f3->GetParameter(1); aN_sb_phi_err[1] = f3->GetParError(1);
	aN_sb_cosphi[1] = f4->GetParameter(1); aN_sb_cosphi_err[1] = f4->GetParError(1);

	int n2=0;
        fstream file2;
        file2.open(Form("sig_sb_range%d/%sBeam/auto_mod_relLum_%s_xF2.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::in);
        double phi_val_xF2, relLum_xF2, relLumStatErr_xF2, relLumSysErr_xF2, relLumTotErr_xF2;
        auto gr2 = new TGraphErrors(); auto gr22 = new TGraphErrors();
        gr2->SetTitle("xF2 bin xF = 0.20;  #phi; RelLum_Ratio");
        gr2->SetMarkerColor(6); gr2->SetMarkerStyle(21);
	gr22->SetTitle("xF2 bin xF = 0.20; cos #phi; RelLum_Ratio");
        gr22->SetMarkerColor(6); gr22->SetMarkerStyle(21);
        while(file2>>phi_val_xF2>>relLum_xF2>>relLumStatErr_xF2>>relLumSysErr_xF2>>relLumTotErr_xF2){
            //    file2>>phi_val_xF2>>relLum_xF2>>relLumStatErr_xF2>>relLumSysErr_xF2>>relLumTotErr_xF2;
                gr2->SetPoint(n2, phi_val_xF2, relLum_xF2);
                gr2->SetPointError(n2, 0, relLumStatErr_xF2);
            	gr22->SetPoint(n2, cos(phi_val_xF2), relLum_xF2);
                gr22->SetPointError(n2, 0, relLumStatErr_xF2);
	    n2++;
            
        }
        file2.close();
	TCanvas *c4 = new TCanvas();
        gr2->Draw("AP");
	TF1 *f5 = new TF1("f5","[0] + [1]*cos(x)",-3.5,3.5);
        f5->SetParameters(0,1); gr2->Fit("f5","RV+"); gr2->Draw("AP");
        TCanvas *c5 = new TCanvas(); gr22->Draw("AP");
        TF1 *f6 = new TF1("f6","pol1",-1.5,1.5);
        f6->SetParameters(0,1); gr22->Fit("f6","RV+"); gr22->Draw("AP");
	aN_sb_phi[2] = f5->GetParameter(1); aN_sb_phi_err[2] = f5->GetParError(1);
	aN_sb_cosphi[2] = f6->GetParameter(1); aN_sb_cosphi_err[2] = f6->GetParError(1);


	int n3=0;
        fstream file3;
        file3.open(Form("sig_sb_range%d/%sBeam/auto_mod_relLum_%s_xF3.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::in);
	double phi_val_xF3, relLum_xF3, relLumStatErr_xF3, relLumSysErr_xF3, relLumTotErr_xF3;
        auto gr3 = new TGraphErrors(); auto gr33 = new TGraphErrors();
        gr3->SetTitle("xF3 bin xF = 0.3;  #phi; RelLum_Ratio");
        gr3->SetMarkerColor(7); gr3->SetMarkerStyle(21);
        gr33->SetTitle("xF3 bin xF = 0.3; cos #phi; RelLum_Ratio");
        gr33->SetMarkerColor(7); gr33->SetMarkerStyle(21);
	while(file3>>phi_val_xF3>>relLum_xF3>>relLumStatErr_xF3>>relLumSysErr_xF3>>relLumTotErr_xF3){
            //    file3>>phi_val_xF3>>relLum_xF3>>relLumStatErr_xF3>>relLumSysErr_xF3>>relLumTotErr_xF3;
                gr3->SetPoint(n3, phi_val_xF3, relLum_xF3);
                gr3->SetPointError(n3, 0, relLumStatErr_xF3);
		gr33->SetPoint(n3, cos(phi_val_xF3), relLum_xF3);
                gr33->SetPointError(n3, 0, relLumStatErr_xF3);
            n3++;
            
        }
        file3.close();
	TCanvas *c6 = new TCanvas();
        gr3->Draw("AP");
	TF1 *f7 = new TF1("f7","[0] + [1]*cos(x)",-3.5,3.5);
        f7->SetParameters(0,1); gr3->Fit("f7","RV+"); gr3->Draw("AP");
        TCanvas *c7 = new TCanvas(); gr33->Draw("AP");
        TF1 *f8 = new TF1("f8","pol1",-1.5,1.5);
        f8->SetParameters(0,1); gr33->Fit("f8","RV+"); gr33->Draw("AP");
	aN_sb_phi[3] = f7->GetParameter(1); aN_sb_phi_err[3] = f7->GetParError(1);
	aN_sb_cosphi[3] = f8->GetParameter(1); aN_sb_cosphi_err[3] = f8->GetParError(1);

	int n4=0;
        fstream file4;
        file4.open(Form("sig_sb_range%d/%sBeam/auto_mod_relLum_%s_xF4.txt",range_input,beam_input.c_str(),region_input.c_str()),std::ios::in);
        double phi_val_xF4, relLum_xF4, relLumStatErr_xF4, relLumSysErr_xF4, relLumTotErr_xF4;
        auto gr4 = new TGraphErrors(); auto gr44 = new TGraphErrors();
        gr4->SetTitle("xF4 bin xF = 0.3;  #phi; RelLum_Ratio");
        gr4->SetMarkerColor(7); gr4->SetMarkerStyle(21);
        gr44->SetTitle("xF4 bin xF = 0.3; cos #phi; RelLum_Ratio");
        gr44->SetMarkerColor(7); gr44->SetMarkerStyle(21);
        while(file4>>phi_val_xF4>>relLum_xF4>>relLumStatErr_xF4>>relLumSysErr_xF4>>relLumTotErr_xF4){
            //    file4>>phi_val_xF4>>relLum_xF4>>relLumStatErr_xF4>>relLumSysErr_xF4>>relLumTotErr_xF4;
                gr4->SetPoint(n4, phi_val_xF4, relLum_xF4);
                gr4->SetPointError(n4, 0, relLumStatErr_xF4);
                gr44->SetPoint(n4, cos(phi_val_xF4), relLum_xF4);
                gr44->SetPointError(n4, 0, relLumStatErr_xF4);
            n4++;
            
        }
        file4.close();
        TCanvas *c8 = new TCanvas();
        gr4->Draw("AP");
        TF1 *f9 = new TF1("f9","[0] + [1]*cos(x)",-3.5,3.5);
        f9->SetParameters(0,1); gr4->Fit("f9","RV+"); gr4->Draw("AP");
        TCanvas *c9 = new TCanvas(); gr44->Draw("AP");
        TF1 *f10 = new TF1("f10","pol1",-1.5,1.5);
        f10->SetParameters(0,1); gr44->Fit("f10","RV+"); gr44->Draw("AP");
        aN_sb_phi[4] = f9->GetParameter(1); aN_sb_phi_err[4] = f9->GetParError(1);
        aN_sb_cosphi[4] = f10->GetParameter(1); aN_sb_cosphi_err[4] = f10->GetParError(1);

	auto mg = new TMultiGraph();
        mg->Add(gr0); gr0->SetTitle(" xF0 bin xF = 0.0875");
        mg->Add(gr1); gr1->SetTitle(" xF1 bin xF = 0.1125");
        mg->Add(gr2); gr2->SetTitle(" xF2 bin xF = 0.15");
        mg->Add(gr3); gr3->SetTitle(" xF3 bin xF = 0.2");
//        mg->Add(gr4); gr4->SetTitle(" xF3 bin xF = 0.3");

        mg->SetTitle("A_{N} RelLum for xF bins;  #phi; Raw A_{N}");
        TCanvas *c10 = new TCanvas(); mg->Draw("AP");
        c10->BuildLegend();

	gStyle->SetOptFit(1111);

	for(int i=0; i<xFBins; i++){
                cout<<"xF = "<<xF_val[i]<<" aN_sb_phi = "<<aN_sb_phi[i]<<" aN_sb_phi_err = "<<aN_sb_phi_err[i]<<endl;
                cout<<"xF = "<<xF_val[i]<<" aN_sb_phi/pol = "<<aN_sb_phi[i]/pol<<" aN_sb_phi_err/pol = "<<aN_sb_phi_err[i]/pol<<endl;
                outfile<<xF_val[i]<<" "<<aN_sb_phi[i]/pol<<" "<<aN_sb_phi_err[i]/pol<<" "<<aN_sb_cosphi[i]/pol<<" "<<aN_sb_cosphi_err[i]/pol<<endl;

        }

}
