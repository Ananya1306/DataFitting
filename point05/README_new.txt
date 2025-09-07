#Update : 4 xFBins ranges - 40... xF Bins - 0, 1, 2, 3
	: 5 xFBins ranges - 50..  xF Bins - 0, 1, 2, 3, 4

#New instructions with codes :

1. pi0yields.C(range, xF, beam, spin) - Calculates pi0 yields in inv mass hists for various sig & sb region for Up and Down . Input - Data files from -  EEmcNanoTreeQA_schedRun15_YellowBeam(BlueBeam)_160All_Et06pt0_xFTest_1(2).root. Output - point05/sig_sb_range0(1,2)/BlueBeam(YellowBeam)/auto_pi0_xF0(1,2,3)_Up(Down).txt

2. aN_crossRatio.C(range, xF, Beam, region) - Calculates cross ratio and relative luminosity(using rel lum value). For the Luminosity Calculation, everything regarding the Relative Luminosity value and the systematic error has been included. Code automatically skips the bad sectors, and are saved in files using mod* value.
Input - sig_sb_range(40/60)/(Blue/Yellow)Beam/auto_pi0_xF(0/1/2/3)_(BUp/BDown/YUp/YDown).txt
Output - sig_sb_range(40/60)/(Blue/Yellow)Beam/auto_mod_crossRatio_(sig/sb)_xF(0/1/2/3).txt
	 sig_sb_range(40/60)/(Blue/Yellow)Beam/auto_mod_relLum_(sig/sb)_xF(0/1/2/3).txt

3. graph_crossRatio_auto.C(range, Beam, region) - Plots the cross ratio values and fits them. Records the p1 parameter of the fit, saves the fit parameters divided by the polarisation values, here itself!
Input - sig_sb_range(40/60)/(Blue/Yellow)Beam/auto_mod_crossRatio_(sig/sb)_xF(0/1/2/3).txt
Output - sig_sb_range(40/60)/(Blue/Yellow)Beam/aN_mod_crossRatio_(sig/sb)_goodChi2.txt

4. graph_relLum_auto.C(range, beam, region) - Plots the relative luminosity values and fits them. Records the p1 parameter of the fit, saves the fit parameters divided by the polarisation values, here itself!
Input - sig_sb_range(40/60)/(Blue/Yellow)Beam/auto_mod_relLum_(sig/sb)_xF(0/1/2/3).txt
Output - sig_sb_range(40/60)/(Blue/Yellow)Beam/aN_mod_relLum_(sig/sb)_goodChi2.txt

5. graph_raw_aN.C(range, beam, method) - Quick check to plot the raw asymmetries of signal and sideband region side-by-side for both the methods. Now that the polarisation values have been set, you don't really need to divide the inputs by pol. Only plots the statistical errors, and not the systematic errors of relLum
Input - sig_sb_range(40/60)/(Blue/Yellow)Beam/aN_mod_(crossRatio/relLum)_sig_goodChi2.txt
        sig_sb_range(40/60)/(Blue/Yellow)Beam/aN_mod_(crossRatio/relLum)_sb_goodChi2.txt 

6. graph_raw_aN_sys.C(range, beam, region) - Takes both crossRatio and relLum inputs and calculates the systematics between the two different measurements. Plots the combined value of cross ratio, rel lum and the combined statistical error, plots only the systematics error due to the two measuremets.
Input - sig_sb_range(40/60)/(Blue/Yellow)Beam/aN_mod_crossRatio_(sig/sb)_goodChi2.txt
        sig_sb_range(40/60)/(Blue/Yellow)Beam/aN_mod_relLum_(sig/sb)_goodChi2.txt
Output - sig_sb_range(40/60)/(Blue/Yellow)Beam/aN_raw_sys_bothMethods_(sig/sb).txt

-------------------

7. FitCodes/ or FullFitxF0/FullFit_Data_BBeam_modRange_xF0_Levy(LogNormal,Weibull) : change the codes for different fits, and also for calculating the fractional forms for the different sig and sb regions. Input :YellowBeam_xF_hists_modRange.root . Output : sig_sb_range0(1,2)/BlueBeam(YellowBeam)/fSig_Levy(LogNotmal,Weibull).txt -- This is a separate thing altogether that you have to do.

