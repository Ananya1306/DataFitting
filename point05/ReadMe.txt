if(range_input == 10){sb_Low_1 = 0; sb_High_1 = 0; sig_Low = 11; sig_High = 21; sb_Low_2 = 21; sb_High_2 = 40;}Data Files :

Analysis Files : 

1. pi0yields.C - Calculates pi0 yields in inv mass hists for various sig & sb region for Up and Down . Input - Data files from -  EEmcNanoTreeQA_schedRun15_YellowBeam(BlueBeam)_160All_Et06pt0_xFTest_1(2).root. Output - point05/sig_sb_range0(1,2)/BlueBeam(YellowBeam)/pi0_xF0(1,2,3)_Up(Down).txt


2. aN_crossRatio.C - Calculates cross ratio and relative luminosity(using rel lum value) . Input - point05/sig_sb_range0(1,2)/BlueBeam(YellowBeam)/pi0_xF0(1,2,3)_Up(Down).txt. Output - crossRatio_sb(sig)_xF0(1,2,3).txt. Tip : If you need to rotate the azimuthal axis for better plots, change the way that outfiles are stored. To Do : calculate this for signal and sideband region at once.
Tip: For plotting the good sectors, the output files are changed to "mod*"

3.  FitCodes/ or FullFitxF0/FullFit_Data_BBeam_modRange_xF0_Levy(LogNormal,Weibull) : change the codes for different fits, and also for calculating the fractional forms for the different sig and sb regions. Input :YellowBeam_xF_hists_modRange.root . Output : sig_sb_range0(1,2)/BlueBeam(YellowBeam)/fSig_Levy(LogNotmal,Weibull).txt



4. Revision, keep using graph_crossRatio.C and graph_relLum.C 
graph_crossRatio_auto.C and graph_relLum_auto.C : Plots graphs of the crossRatio values and relative luminosity values. Also, fits the graphs to find the parameter values for plotting raw aN. Input : sig_sb_range0(1,2)/BlueBeam(YellowBeam)/mod_crossRatio_sb(sig)_xF0(1,2,3).txt. Output : aN_crossRatio_sb(sig).txt NOTE : graph_crossRatio.C and graph_relLum.C is redundant now

5. graph_raw_aN.C : Plots graphs of the raw aN. Input : aN_crossRatio_sb(sig).txt. Output : Just a graph

6. aN_bkg_pi0.C : Plots graphs of the final aN for both pi0 and the background region. Input : aN_crossRatio_sb(sig).txt, fSig_BB_Levy(LogNormal, Weibull ).txt. Output : aN_pi0_bkg_Levy(Weibull, LogNormal).txt

7. graph_aN_bkg_pi0_v1.C : Plots all graphs together
