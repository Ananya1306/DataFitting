#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom.h"
#include "TMath.h"
#include <iostream>

/**
 * @brief Custom C++ function for a 5th-order Chebyshev polynomial fit.
 *
 * The standard TMath::Chebyshev functions are defined on the interval [-1, 1].
 * To use them on a custom range, we must perform a change of variables.
 * For the interval [0, 1], the transformation is x' = 2*x - 1.
 *
 * @param x The independent variable (x-coordinate).
 * @param par The array of parameters (p0 to p5).
 * @return The value of the 5th-order Chebyshev polynomial at x.
 */
Double_t cheb5_custom(Double_t *x, Double_t *par) {
    // Perform the linear transformation from [0, 1] to [-1, 1]
    Double_t x_prime = 2 * x[0] - 1;

    // Return the linear combination of the Chebyshev basis functions
    // The TMath::Chebyshev(order, x) function is used for each term.
    return par[0] * TMath::Chebyshev(0, x_prime) +
           par[1] * TMath::Chebyshev(1, x_prime) +
           par[2] * TMath::Chebyshev(2, x_prime) +
           par[3] * TMath::Chebyshev(3, x_prime) +
           par[4] * TMath::Chebyshev(4, x_prime) +
           par[5] * TMath::Chebyshev(5, x_prime);
}

/**
 * @brief ROOT macro to fit a 5th-order Chebyshev polynomial on the [0, 1] range.
 *
 * This example:
 * 1. Creates a dummy histogram filled with a Chebyshev function.
 * 2. Defines a new TF1 using a custom C++ function.
 * 3. Fits the histogram with the new TF1 on the specified range.
 * 4. Prints the fit parameters and draws the result.
 */
void fitChebyshev5_customRange() {
    // === 1. Create dummy data to fit ===
    // In a real application, you would load your histogram from a file.
    TCanvas *c1 = new TCanvas("c1", "5th-order Chebyshev fit on [0,1] range", 800, 600);
    TH1F *hBackground = new TH1F("hBackground", "Background with Chebyshev fit;x;Events", 100, 0, 1);
    gRandom->SetSeed(0);
    
    // Define a "true" function on the [0, 1] range to fill the histogram.
    TF1 *true_func = new TF1("true_func", cheb5_custom, 0, 1, 6);
    true_func->SetParameters(1000, 500, 200, 800, 100, 300);
    
    // Fill the histogram with the function and add noise
    hBackground->FillRandom("true_func", 500000);
    
    // === 2. Define the TF1 for the Chebyshev polynomial to fit ===
    // We pass the C++ function name, the fitting range [0, 1], and the number of parameters.
    TF1 *f_cheb5_custom = new TF1("f_cheb5_custom", cheb5_custom, 0, 1, 6);
    f_cheb5_custom->SetLineColor(kRed);
    f_cheb5_custom->SetNpx(500); // Increase points for a smoother plot

    // === 3. Fit the histogram ===
    // The "R" option restricts the fit to the function's defined range [0, 1].
    hBackground->Fit(f_cheb5_custom, "R");

    // Print the fitted parameters
    std::cout << "\nFitted Parameters:" << std::endl;
    for (int i = 0; i < 6; ++i) {
        std::cout << "p[" << i << "] = " << f_cheb5_custom->GetParameter(i) << " +/- " << f_cheb5_custom->GetParError(i) << std::endl;
    }
    
    // Draw the histogram and the fit result
    hBackground->Draw();
    gPad->Update(); // Ensure the canvas is updated
    
    // Clean up
    delete true_func;
}

