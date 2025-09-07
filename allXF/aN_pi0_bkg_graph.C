void aN_pi0_bkg_graph(){

	int n0 = 0;
	fstream file0;
	file0.open("aN_pi0_bkg_Levy_point05.txt");
	
	double xF_val0, aN_pi0_0, aN_pi0_err0, aN_bkg0, aN_bkg_err0;
	auto gr_pi0_05 = new TGraphErrors(); auto gr_bkg_05 = new TGraphErrors();
	gr_pi0_05->SetTitle("A_{N} for #pi^0"); gr_pi0_05->SetMarkerColor(kBlue); gr_pi0_05->SetMarkerStyle(21);
	gr_bkg_05->SetTitle("A_{N} for background"); gr_bkg_05->SetMarkerColor(kBlue); gr_bkg_05->SetMarkerStyle(21);

	while(1){
		file0>>xF_val0>>aN_pi0_0>>aN_pi0_err0>>aN_bkg0>>aN_bkg_err0;
		gr_pi0_05->SetPoint(n0, xF_val0, aN_pi0_0);
		gr_pi0_05->SetPointError(n0, 0, aN_pi0_err0);
		gr_bkg_05->SetPoint(n0, xF_val0, aN_bkg0);
		gr_bkg_05->SetPointError(n0, 0, abs(aN_bkg_err0));
		
		cout<<xF_val0<<" "<<aN_pi0_0<<" "<<aN_pi0_err0<<" "<<aN_bkg0<<" "<<aN_bkg_err0<<endl;
		n0++;
		if(file0.eof()) break;
	}
	

	int n1 = 0;
        fstream file1;
        file1.open("aN_pi0_bkg_Levy_point075.txt");

        double xF_val1, aN_pi0_1, aN_pi0_err1, aN_bkg1, aN_bkg_err1;
        auto gr_pi0_075 = new TGraphErrors(); auto gr_bkg_075 = new TGraphErrors();
        gr_pi0_075->SetTitle("A_{N} for #pi^0"); gr_pi0_075->SetMarkerColor(kBlue); gr_pi0_075->SetMarkerStyle(21);
        gr_bkg_075->SetTitle("A_{N} for background"); gr_bkg_075->SetMarkerColor(kBlue); gr_bkg_075->SetMarkerStyle(21);

        while(1){
                file1>>xF_val1>>aN_pi0_1>>aN_pi0_err1>>aN_bkg1>>aN_bkg_err1;
                gr_pi0_075->SetPoint(n1, xF_val1, aN_pi0_1);
                gr_pi0_075->SetPointError(n1, 0, aN_pi0_err1);
                gr_bkg_075->SetPoint(n1, xF_val1, aN_bkg1);
                gr_bkg_075->SetPointError(n1, 0, abs(aN_bkg_err1));

                n1++;
                if(file1.eof()) break;
        }


	int n2 = 0;
        fstream file2;
        file2.open("aN_pi0_bkg_Weibull_point05.txt");

        double xF_val2, aN_pi0_2, aN_pi0_err2, aN_bkg2, aN_bkg_err2;
        auto gr_pi0_05_Wbl = new TGraphErrors(); auto gr_bkg_05_Wbl = new TGraphErrors();
        gr_pi0_05_Wbl->SetTitle("A_{N} for #pi^0"); gr_pi0_05_Wbl->SetMarkerColor(kCyan); gr_pi0_05_Wbl->SetMarkerStyle(22);
        gr_bkg_05_Wbl->SetTitle("A_{N} for background"); gr_bkg_05_Wbl->SetMarkerColor(kCyan); gr_bkg_05_Wbl->SetMarkerStyle(22);

        while(1){
                file2>>xF_val2>>aN_pi0_2>>aN_pi0_err2>>aN_bkg2>>aN_bkg_err2;
                gr_pi0_05_Wbl->SetPoint(n2, xF_val2, aN_pi0_2);
                gr_pi0_05_Wbl->SetPointError(n2, 0, aN_pi0_err2);
                gr_bkg_05_Wbl->SetPoint(n2, xF_val2, aN_bkg2);
                gr_bkg_05_Wbl->SetPointError(n2, 0, abs(aN_bkg_err2));

                //cout<<xF_val0<<" "<<aN_pi0_0<<" "<<aN_pi0_err0<<" "<<aN_bkg0<<" "<<aN_bkg_err0<<endl;
                n2++;
                if(file2.eof()) break;
        }


        int n3 = 0;
        fstream file3;
        file3.open("aN_pi0_bkg_Weibull_point075.txt");

        double xF_val3, aN_pi0_3, aN_pi0_err3, aN_bkg3, aN_bkg_err3;
        auto gr_pi0_075_Wbl = new TGraphErrors(); auto gr_bkg_075_Wbl = new TGraphErrors();
        gr_pi0_075_Wbl->SetTitle("A_{N} for #pi^0"); gr_pi0_075_Wbl->SetMarkerColor(kCyan); gr_pi0_075_Wbl->SetMarkerStyle(22);
        gr_bkg_075_Wbl->SetTitle("A_{N} for background"); gr_bkg_075_Wbl->SetMarkerColor(kCyan); gr_bkg_075_Wbl->SetMarkerStyle(22);

        while(1){
                file3>>xF_val3>>aN_pi0_3>>aN_pi0_err3>>aN_bkg3>>aN_bkg_err3;
                gr_pi0_075_Wbl->SetPoint(n3, xF_val3, aN_pi0_3);
                gr_pi0_075_Wbl->SetPointError(n3, 0, aN_pi0_err3);
                gr_bkg_075_Wbl->SetPoint(n3, xF_val3, aN_bkg3);
                gr_bkg_075_Wbl->SetPointError(n3, 0, abs(aN_bkg_err3));

                n3++;
                if(file3.eof()) break;
        }

	
	int n4 = 0;
        fstream file4;
        file4.open("aN_pi0_bkg_LogNormal_point05.txt");

        double xF_val4, aN_pi0_4, aN_pi0_err4, aN_bkg4, aN_bkg_err4;
        auto gr_pi0_05_Ln = new TGraphErrors(); auto gr_bkg_05_Ln = new TGraphErrors();
        gr_pi0_05_Ln->SetTitle("A_{N} for #pi^0"); gr_pi0_05_Ln->SetMarkerColor(kBlue); gr_pi0_05_Ln->SetMarkerStyle(24);
        gr_bkg_05_Ln->SetTitle("A_{N} for background"); gr_bkg_05_Ln->SetMarkerColor(kBlue); gr_bkg_05_Ln->SetMarkerStyle(24);

        while(1){
                file4>>xF_val4>>aN_pi0_4>>aN_pi0_err4>>aN_bkg4>>aN_bkg_err4;
                gr_pi0_05_Ln->SetPoint(n4, xF_val4, aN_pi0_4);
                gr_pi0_05_Ln->SetPointError(n4, 0, aN_pi0_err4);
                gr_bkg_05_Ln->SetPoint(n4, xF_val4, aN_bkg4);
                gr_bkg_05_Ln->SetPointError(n4, 0, abs(aN_bkg_err4));

                //cout<<xF_val0<<" "<<aN_pi0_0<<" "<<aN_pi0_err0<<" "<<aN_bkg0<<" "<<aN_bkg_err0<<endl;
                n4++;
                if(file4.eof()) break;
        }


        int n5 = 0;
        fstream file5;
        file5.open("aN_pi0_bkg_LogNormal_point075.txt");

        double xF_val5, aN_pi0_5, aN_pi0_err5, aN_bkg5, aN_bkg_err5;
        auto gr_pi0_075_Ln = new TGraphErrors(); auto gr_bkg_075_Ln = new TGraphErrors();
        gr_pi0_075_Ln->SetTitle("A_{N} for #pi^0"); gr_pi0_075_Ln->SetMarkerColor(kBlue); gr_pi0_075_Ln->SetMarkerStyle(24);
        gr_bkg_075_Ln->SetTitle("A_{N} for background"); gr_bkg_075_Ln->SetMarkerColor(kBlue); gr_bkg_075_Ln->SetMarkerStyle(24);

        while(1){
                file5>>xF_val5>>aN_pi0_5>>aN_pi0_err5>>aN_bkg5>>aN_bkg_err5;
                gr_pi0_075_Ln->SetPoint(n5, xF_val5, aN_pi0_5);
                gr_pi0_075_Ln->SetPointError(n5, 0, aN_pi0_err5);
                gr_bkg_075_Ln->SetPoint(n5, xF_val5, aN_bkg5);
                gr_bkg_075_Ln->SetPointError(n5, 0, abs(aN_bkg_err5));

                n5++;
                if(file5.eof()) break;
        }


	TCanvas *c0 = new TCanvas(); gr_pi0_05->Draw("AP"); gr_pi0_05_Wbl->Draw("AP"); gr_pi0_05_Ln->Draw("AP");
	TCanvas *c1 = new TCanvas(); gr_pi0_075->Draw("AP"); gr_pi0_075_Wbl->Draw("AP"); gr_pi0_075_Ln->Draw("AP");

	TCanvas *c4 = new TCanvas(); gr_bkg_05->Draw("AP"); gr_bkg_05_Wbl->Draw("AP"); gr_bkg_05_Ln->Draw("AP");
	TCanvas *c5 = new TCanvas(); gr_bkg_075->Draw("AP"); gr_bkg_075_Wbl->Draw("AP"); gr_bkg_075_Ln->Draw("AP");

	auto mg0 = new TMultiGraph();
	mg0->Add(gr_pi0_05); mg0->Add(gr_pi0_075); mg0->Add(gr_pi0_05_Wbl); mg0->Add(gr_pi0_075_Wbl); mg0->Add(gr_pi0_05_Ln); mg0->Add(gr_pi0_075_Ln);
	mg0->SetTitle("A_{N} for #pi^{0}; xF; A_{N}");
	TCanvas *c2 = new TCanvas(); mg0->Draw("AP");
	TLine *line0 = new TLine(0.0, 0.0, 3.0, 0.0); line0->Draw("same");

	TLegend *legend=new TLegend(0.4,0.55,0.88,0.95);
        legend->SetTextFont(42);
        legend->SetTextSize(0.);
        legend->AddEntry(gr_pi0_05,"Levy bg fn","lpe");
        legend->AddEntry(gr_pi0_05_Wbl,"Weibull bg fn","lpe");
        legend->AddEntry(gr_pi0_05_Ln,"LogNormal bg fn","lpe");
        legend->Draw();

	auto mg1 = new TMultiGraph();
        mg1->Add(gr_bkg_05); mg1->Add(gr_bkg_075); mg1->Add(gr_bkg_05_Wbl); mg1->Add(gr_bkg_075_Wbl); mg1->Add(gr_bkg_05_Ln); mg1->Add(gr_bkg_075_Ln);
        mg1->SetTitle("A_{N} for background; xF; A_{N}");
        TCanvas *c3 = new TCanvas(); mg1->Draw("AP");
        TLine *line1 = new TLine(0.0, 0.0, 3.0, 0.0); line1->Draw("same");

	TLegend *legend1=new TLegend(0.4,0.55,0.88,0.95);
   	legend1->SetTextFont(42);
   	legend1->SetTextSize(0.);
	legend1->AddEntry(gr_bkg_05,"Levy bg fn","lpe");
	legend1->AddEntry(gr_bkg_05_Wbl,"Weibull bg fn","lpe");
	legend1->AddEntry(gr_bkg_05_Ln,"LogNormal bg fn","lpe");
	legend1->Draw();
}
