void aN_raw_graph(){


	const double pol = 57.85/100;

	//double xF_val[4] = {0.075, 0.125, 0.20, 0.30};

	int n0 = 0;
	
	fstream file0;
	file0.open("aN_crossRatio_sig_point05.txt");
	
        double xF_val, aN_sig_phi, aN_sig_phi_err, aN_sig_cosphi, aN_sig_cosphi_err;
	auto gr_phi = new TGraphErrors(); auto gr_cosphi = new TGraphErrors();
	gr_phi->SetTitle("Raw #A_N for signal region - par #phi");
	gr_phi->SetMarkerColor(kBlue); gr_phi->SetMarkerStyle(20);
	gr_cosphi->SetTitle("Raw #A_N for signal region - par cos#phi");
	gr_cosphi->SetMarkerColor(kBlue); gr_cosphi->SetMarkerStyle(25);

	while(1){
		file0>>xF_val>>aN_sig_phi>>aN_sig_phi_err>>aN_sig_cosphi>>aN_sig_cosphi_err;
		gr_phi->SetPoint(n0, xF_val, aN_sig_phi*pol);
		gr_phi->SetPointError(n0, 0, aN_sig_phi_err*pol);
		gr_cosphi->SetPoint(n0, xF_val, aN_sig_cosphi*pol);
		gr_cosphi->SetPointError(n0, 0, aN_sig_cosphi_err*pol);

		cout<<n0<<" "<<xF_val<<" "<<aN_sig_cosphi*pol<<" "<<aN_sig_cosphi_err*pol<<endl;
		n0++;
		if(file0.eof()) break;
        }
        file0.close();

	int n1 = 0;

        fstream file1;
        file1.open("aN_crossRatio_sig_point075.txt");

        double xF_val075, aN_sig_phi075, aN_sig_phi_err075, aN_sig_cosphi075, aN_sig_cosphi_err075;
        auto gr_phi075 = new TGraphErrors(); auto gr_cosphi075 = new TGraphErrors();
        gr_phi075->SetTitle("Raw #A_N for signal region - par #phi");
        gr_phi075->SetMarkerColor(kBlue); gr_phi075->SetMarkerStyle(20);
        gr_cosphi075->SetTitle("Raw #A_N for signal region - par cos#phi");
        gr_cosphi075->SetMarkerColor(kBlue); gr_cosphi075->SetMarkerStyle(25);

        while(1){
                file1>>xF_val075>>aN_sig_phi075>>aN_sig_phi_err075>>aN_sig_cosphi075>>aN_sig_cosphi_err075;
                gr_phi075->SetPoint(n1, xF_val075, aN_sig_phi075*pol);
                gr_phi075->SetPointError(n1, 0, aN_sig_phi_err075*pol);
                gr_cosphi075->SetPoint(n0, xF_val075, aN_sig_cosphi075*pol);
                gr_cosphi075->SetPointError(n1, 0, aN_sig_cosphi_err075*pol);

                cout<<n1<<" "<<xF_val075<<" "<<aN_sig_phi075*pol<<" "<<aN_sig_phi_err075*pol<<" "<<aN_sig_cosphi075*pol<<" "<<aN_sig_cosphi_err075*pol<<endl;
                n1++;
                if(file1.eof()) break;
        }
        file1.close();

	TCanvas *c0 = new TCanvas();
	gr_phi075->Draw("AP"); gr_cosphi075->Draw("P");
	c0->BuildLegend();

	//TGraph *line = new TGraph(2);
   	//line->SetPoint(0,0,0);
   	//line->SetPoint(1,0,3);

	auto mg = new TMultiGraph();
	mg->Add(gr_phi); mg->Add(gr_phi075); //mg->Add(gr_cosphi); mg->Add(gr_cosphi075); 
	mg->SetTitle("Raw A_{N} for signal region; xF; A_{N}^{raw}");
	TCanvas *c1 = new TCanvas(); mg->Draw("AP"); 
	TLine *line = new TLine(0.0, 0.0, 3.0, 0.0); line->Draw("same");

        c1->BuildLegend();

}
