#!/bin/bash
#40 41 42 43 44 45
# Define the parameter values as arrays
declare -a ranges=(50 51 52 53 54 55)
declare -a xF_values=(0 1 2 3 4)
declare -a beams=("Blue" "Yellow")
declare -a regions=("sig" "sb")
#declare -a blue_spins=("BUp" "BDown")
#declare -a yellow_spins=("YUp" "YDown")


# Loop through each parameter to generate all combinations
for range_val in "${ranges[@]}"; do
    for xF_val in "${xF_values[@]}"; do
        for beam_val in "${beams[@]}"; do
            # Conditional logic for spin_input
            #if [ "$beam_val" == "Blue" ]; then
            #    spins=("${blue_spins[@]}")
            #else
            #    spins=("${yellow_spins[@]}")
            #fi

            for region_val in "${regions[@]}"; do
                # Construct the command to run the ROOT macro
                # The assumption is that your macro is named 'my_macro.C'
                # and takes the parameters as command-line arguments.
                # Adjust this command to match how your macro actually receives parameters.
                echo "Running macro with parameters: range=$range_val, xF=$xF_val, beam=$beam_val, region=$region_val"

                # Replace the following line with the actual command to run your ROOT macro.
                 root -l -b -q 'aN_crossRatio.C('$range_val','$xF_val',"'$beam_val'","'$region_val'")'
                # For a C++ executable: ./my_macro $range_val $xF_val $beam_val $spin_val
                #root -l -b -q 'pi0yields.C($range_val,$xF_val,\"$beam_val\",\"$spin_val\")'
                #echo '$range_val','$xF_val',"$beam_val","$spin_val"
            done
        done
    done
done

echo "Automation script finished."
