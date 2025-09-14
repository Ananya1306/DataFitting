#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

void Diff_Levy_Weibull() {
    // Define the file paths for the two input files
    const std::string levy_file = "aN_pi0_bkg_Levy.txt";
    const std::string weibull_file = "aN_pi0_bkg_Weibull.txt";

    // Open the input files
    std::ifstream levy_data(levy_file);
    std::ifstream weibull_data(weibull_file);

    // Check if both files were opened successfully
    if (!levy_data.is_open()) {
        std::cerr << "Error: Could not open file " << levy_file << std::endl;
        //return 1;
    }
    if (!weibull_data.is_open()) {
        std::cerr << "Error: Could not open file " << weibull_file << std::endl;
        //return 1;
    }

    // Define variables to store the data from each line
    double xF_val_Levy, aN_pi0_Levy, aN_pi0_err_Levy, aN_bkg_Levy, aN_bkg_err_Levy;
    double xF_val_Weibull, aN_pi0_Weibull, aN_pi0_err_Weibull, aN_bkg_Weibull, aN_bkg_err_Weibull;

    // Set output formatting for the console
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "xF_val\t\tStd Dev(pi0)\tStd Dev(bkg)\tDiff(pi0)\tDiff(bkg)\n";
    std::cout << "------------------------------------------------------------------------------------\n";

    // Read the files line by line (row by row)
    // The loop continues as long as data can be successfully read from both files
    while (levy_data >> xF_val_Levy >> aN_pi0_Levy >> aN_pi0_err_Levy >> aN_bkg_Levy >> aN_bkg_err_Levy &&
           weibull_data >> xF_val_Weibull >> aN_pi0_Weibull >> aN_pi0_err_Weibull >> aN_bkg_Weibull >> aN_bkg_err_Weibull) {

        // --- Calculation for aN_pi0 ---
        // Calculate the standard deviation of the difference between aN_pi0_Levy and aN_pi0_Weibull
        // This is done by propagating their respective statistical errors in quadrature.
        double aN_pi0_diff_std_dev = std::sqrt(std::pow(aN_pi0_err_Levy, 2) + std::pow(aN_pi0_err_Weibull, 2));

        // Calculate the actual difference between the central values
        double aN_pi0_diff_val = aN_pi0_Levy - aN_pi0_Weibull;

        // --- Calculation for aN_bkg ---
        // Calculate the standard deviation of the difference between aN_bkg_Levy and aN_bkg_Weibull
        // Propagate their respective statistical errors.
        double aN_bkg_diff_std_dev = std::sqrt(std::pow(aN_bkg_err_Levy, 2) + std::pow(aN_bkg_err_Weibull, 2));
        
        // Calculate the actual difference between the central values
        double aN_bkg_diff_val = aN_bkg_Levy - aN_bkg_Weibull;

        // Output the results for the current row (xF_val)
        std::cout << xF_val_Levy << "\t\t" << aN_pi0_diff_std_dev << "\t" << aN_bkg_diff_std_dev << "\t" << aN_pi0_diff_val << "\t" << aN_bkg_diff_val << std::endl;
    }

    // Close the files after processing
    levy_data.close();
    weibull_data.close();

//    return 0;
}

