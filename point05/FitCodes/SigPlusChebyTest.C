#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom.h"
#include "TMath.h"
#include <iostream>

/**
 * @brief Combines a Skewed Gaussian function with a 3rd-order Chebyshev polynomial.
 *
 * This function handles all 8 parameters:
 * Parameters par[0] to par[3] are for the Skewed Gaussian peak:
 * par[0] = scale
 * par[1] = mean
 * par[2] = sigma
 * par[3] = skewness
 *
 * Parameters par[4] to par[7] are for the 3rd-order Chebyshev background:
 * par[4] = p0 (constant term)
 * par[5] = p1
 * par[6] = p2
 * par[7] = p3
 *
 * @param x The independent variable (x-coordinate).
 * @param par The array of all 8 parameters.
 * @return The value of the combined function.
 */
Double_t CombinedFitFunction(Double_t *x, Double_t *par) {
    // === Skewed Gaussian Peak ===
    // We use TMath functions for better compatibility.
    // The normal_cdf can be implemented using the error function (erf).
    Double_t skewed_gaussian = par[0] * TMath::Exp(-0.5 * TMath::Power((x[0] - par[1]) / par[2], 2)) * 0.5 * (1 + TMath::Erf(par[3] * x[0] / TMath::Sqrt(2.0)));
    
    // === 3rd-order Chebyshev Background ===
    // The standard Chebyshev polynomials are defined on [-1, 1].
    // We need to map the fit range [0.0, 0.4] to [-1, 1].
    // The transformation is x' = (2*x - a - b) / (b - a)
    // Here, a = 0.0 and b = 0.4, so x' = (2*x - 0.4) / 0.4 = 5*x - 1
    Double_t x_prime = 5 * x[0] - 1;

    // Manually evaluate the Chebyshev polynomials using the transformed variable
    Double_t T0 = 1.0;
    Double_t T1 = x_prime;
    Double_t T2 = 2.0 * TMath::Power(x_prime, 2) - 1.0;
    Double_t T3 = 4.0 * TMath::Power(x_prime, 3) - 3.0 * x_prime;

    Double_t chebyshev_background = par[4] * T0 +
                                   par[5] * T1 +
                                   par[6] * T2 +
                                   par[7] * T3;
    
    // Return the sum of the two functions
    return skewed_gaussian + chebyshev_background;
}

/**
 * @brief ROOT macro to fit a combined function on a custom range.
 *
 * This example:
 * 1. Creates a dummy histogram with a peak and background.
 * 2. Defines a new TF1 using the custom combined C++ function.
 * 3. Fits the histogram with the new TF1 on the specified range [0.0, 0.4].
 * 4. Prints the fit parameters and draws the result.
 */
void SigPlusChebyTest() {
    // === 1. Create dummy data to fit ===
    // In a real application, you would load your histogram from a file.
    TCanvas *c1 = new TCanvas("c1", "Combined Skewed Gaussian and Chebyshev Fit", 800, 600);
    TH1F *hCombined = new TH1F("hCombined", "Combined Fit Example;x;Events", 100, 0.0, 0.4);
    gRandom->SetSeed(0);
    
    // Define a "true" combined function to fill the histogram.
    TF1 *true_func = new TF1("true_func", CombinedFitFunction, 0.0, 0.4, 8);
    // Parameters for Skewed Gaussian
    true_func->SetParameters(100, 0.25, 0.03, 5);
    // Parameters for Chebyshev Background
    true_func->SetParameters(100, -200, 150, -50);
    
    // Fill the histogram with the function and add noise
    hCombined->FillRandom("true_func", 1000000);
    
    // === 2. Define the TF1 for the combined function to fit ===
    // We pass the C++ function name, the fitting range, and the number of parameters (8).
    TF1 *fitFcn = new TF1("fitFcn", CombinedFitFunction, 0.0, 0.4, 8);
    fitFcn->SetLineColor(kRed);
    fitFcn->SetNpx(500); // Increase points for a smoother plot

    // Set initial parameter values to help the fit converge
    fitFcn->SetParameters(100, 0.25, 0.03, 5, 100, -200, 150, -50);

    // === 3. Fit the histogram ===
    // The "R" option restricts the fit to the function's defined range [0.0, 0.4].
    hCombined->Fit(fitFcn, "R");

    // Print the fitted parameters
    std::cout << "\nFitted Parameters:" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << "p[" << i << "] = " << fitFcn->GetParameter(i) << " +/- " << fitFcn->GetParError(i) << std::endl;
    }
    
    // Draw the histogram and the fit result
    hCombined->Draw();
    gPad->Update(); // Ensure the canvas is updated
}
