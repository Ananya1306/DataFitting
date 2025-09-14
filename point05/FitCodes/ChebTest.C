#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom.h"

void ChebTest() {
    // === 1. Create dummy data to fit ===
    // In a real application, you would load your histogram from a file.
    TCanvas *c1 = new TCanvas("c1", "5th-order Chebyshev fit", 800, 600);
    TH1F *hBackground = new TH1F("hBackground", "Background with Chebyshev fit", 100, -1, 1);
    gRandom->SetSeed(0);
    
    // Fill the histogram with a function and add noise
    auto true_func = new TF1("true_func", "cheb5", -1, 1);
    true_func->SetParameters(10, 5, 2, 8, 1, 3);
    hBackground->FillRandom("true_func", 50000);
    
    // === 2. Define the TF1 for the Chebyshev polynomial ===
    // "cheb5" is the predefined 5th-order Chebyshev function.
    // We pass 6 parameters: p0 to p5.
    TF1 *f_cheb5 = new TF1("f_cheb5", "cheb5", -1, 1);
    f_cheb5->SetLineColor(kRed);


/*    TF1 *f_cheb5_custom = new TF1("f_cheb5_custom", "[0]*TMath::Chebyshev5(2*x-1) + [1]*TMath::Chebyshev4(2*x-1) + [2]*TMath::Chebyshev3(2*x-1) + [3]*TMath::Chebyshev2(2*x-1) + [4]*TMath::Chebyshev1(2*x-1) + [5]*TMath::Chebyshev0(2*x-1)", 0, 1);
    f_cheb5_custom->SetNpx(500); // Increase points for a smoother plot
    f_cheb5_custom->SetLineColor(kRed);
*/
    // === 3. Fit the histogram ===
    // The "R" option restricts the fit to the function's range (-1, 1).
    hBackground->Fit(f_cheb5, "R");

    // Print the fitted parameters
    std::cout << "Fitted Parameters:" << std::endl;
    for (int i = 0; i < 6; ++i) {
        std::cout << "p[" << i << "] = " << f_cheb5->GetParameter(i) << std::endl;
    }
    
    // Draw the histogram and the fit result
    hBackground->Draw();
    
    // Clean up
    delete true_func;

cheb2
Fitted Parameters:
p[0] = 49846.7
p[1] = -46412.8
p[2] = 42444.3
cheb3
Fitted Parameters:
p[0] = -436448
p[1] = 1.071e+06
p[2] = -433572
p[3] = 301507
}
