#include "TH1.h"
#include "TF1.h"
#include "TList.h"

double weibull_pdf(double *x, double *par) {
    if (x[0] <= par[2]) {//par[0]=shape; par[1]=scale; par[2]=location; par[3] = amplitude;
        return 0.0;
    }

    //double z = (x - par[2]) / par[1];
    return (par[3]) * (par[0] / par[1]) * pow(((x[0] - par[2]) / par[1]), par[0] - 1) * exp(-pow(((x[0] - par[2]) / par[1]), par[0]));
}


void WeibullFuncDef(){

  
//for xF0 and xF1
   TF1 *fw = new TF1("(par[3]) * (par[0] / par[1]) * pow(((x[0] - par[2]) / par[1]), par[0] - 1) * exp(-pow(((x[0] - par[2]) / par[1]), par[0]))",weibull_pdf,0,3,4); 
   fw->SetParameters(0.5,3.0,0.1,100); 
   fw->SetLineColor(kRed); fw->Draw();
 
   TF1 *fw1 = new TF1("fw1",weibull_pdf,0,3,4);
   fw1->SetParameters(1.0,3.0,0.1,100);
   fw1->SetLineColor(kBlack); fw1->Draw("same");
 
   TF1 *fw2 = new TF1("fw2",weibull_pdf,0,3,4);
   fw2->SetParameters(2.0,3.0,0.1,100);
   fw2->SetLineColor(kCyan); fw2->Draw("same");

   TF1 *fw3 = new TF1("fw3",weibull_pdf,0,3,4);
   fw3->SetParameters(5.0,3.0,0.1,100);
   fw3->SetLineColor(kBlue); fw3->Draw("same");

} 
