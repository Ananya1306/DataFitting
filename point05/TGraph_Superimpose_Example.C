#include <TCanvas.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TStyle.h>
#include <TLegend.h>

void TGraph_Superimpose_Example() {

    // Set a clean style for the canvas and graphs.
    gStyle->SetOptStat(0);
    gStyle->SetGridStyle(1);
    gStyle->SetTitleOffset(1.2, "Y");

    // Create a canvas.
    TCanvas *c1 = new TCanvas("c1", "Superimposed Graphs", 800, 600);
    c1->SetGrid();

    // --- Create and fill the first TGraph with filled errors (for the band) ---

    // Number of data points.
    const Int_t n = 5;

    // Define arrays for data points and asymmetric errors.
    Double_t x[n] = {0.5, 1.5, 2.5, 3.5, 4.5};
    Double_t y[n] = {10, 8.5, 12, 10.5, 9};

    // Asymmetric errors (for a TGraphAsymmErrors).
    Double_t exl[n] = {0.2, 0.2, 0.2, 0.2, 0.2};
    Double_t exh[n] = {0.3, 0.3, 0.3, 0.3, 0.3};
    Double_t eyl[n] = {1.0, 0.8, 1.2, 1.0, 0.9};
    Double_t eyh[n] = {1.2, 1.0, 1.5, 1.1, 1.0};

    TGraphAsymmErrors *graphWithBand = new TGraphAsymmErrors(n, x, y, exl, exh, eyl, eyh);
    graphWithBand->SetTitle("Data with Systematic and Statistical Errors");

    // Set the fill color and transparency for the error band.
    graphWithBand->SetFillColorAlpha(kBlue, 0.35); // 35% opaque blue

    // Create a TMultiGraph for the error band.
    TMultiGraph *mgBand = new TMultiGraph();
    mgBand->Add(graphWithBand, "a e2"); // "a": axes, "e2": filled error band
    
    // Draw the multi-graph with the filled error band first.
    // The "a" option ensures the axes are drawn correctly for both graphs.
    mgBand->Draw("a e2");

    // --- Create and fill the second TGraph with markers (for the points) ---

    // Define arrays for the data points and symmetric errors.
    Double_t x2[n] = {0.5, 1.5, 2.5, 3.5, 4.5};
    Double_t y2[n] = {10.2, 8.2, 12.1, 10.8, 9.3};
    Double_t ey2[n] = {0.3, 0.2, 0.4, 0.3, 0.2};

    TGraphErrors *graphWithPoints = new TGraphErrors(n, x2, y2, 0, ey2);
    graphWithPoints->SetTitle("Measured Data Points");
    
    // Set attributes for the markers and error bars.
    graphWithPoints->SetMarkerStyle(kFullCircle);
    graphWithPoints->SetMarkerSize(1.5);
    graphWithPoints->SetMarkerColor(kRed);
    graphWithPoints->SetLineColor(kRed);

    // Create a TMultiGraph for the points and their error bars.
    TMultiGraph *mgPoints = new TMultiGraph();
    mgPoints->Add(graphWithPoints, "pe"); // "p": points only

    // Draw the second multi-graph on top of the first using "SAME".
    // "SAME" ensures no new axes or frames are drawn.
    mgPoints->Draw("pe SAME");

    // --- Create and position a legend ---
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->AddEntry(graphWithBand, "Systematic Error Band", "F");
    legend->AddEntry(graphWithPoints, "Statistical Error Bars", "LEP");
    legend->Draw();

    // Update the canvas to show the final result.
    c1->Update();
}

