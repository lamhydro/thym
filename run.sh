#!/bin/bash

# To run 'run.sh' you must execute './run.sh [test_case]' (e.g './run.sh test_gr4j')

echo ""
echo "Excecuting $0 for $1"
echo ""

# INPUT DATA 
COMP=true # 'true' or 'false'
EXEC=true # 'true' or 'false'
PLOT=true # 'true' or 'false'
tar_dir=bin # Target dir
testcase=$1 # Test case
orig_dir=$(pwd) # Save the current directory

# GET MODEL NAME
cd "$testcase" || exit 1 # Change to the target directory 
mod_name=$(awk 'NR==15 {print $1}' ctrl.in) # Model name
case "$mod_name" in
    gr4j)
        mod_number=1
        ;;
    hbv)
        mod_number=2
        ;;
    hymod)
        mod_number=3
        ;;
    *)
        echo "Unknown option"
        ;;
esac
cd "$orig_dir" || exit 1 # Return to original directory

# ----- COMPILATION
if $COMP; then
    
    # Change to the target directory 
    cd "$tar_dir" || exit 1
    
    # Change model name within makefile
    sed -i "s/^MODEL =.*/MODEL = $mod_number/" makefile
    
    # Clean up prev compilation
    make clean
    
    # New compilation
    make
fi

# ----- EXECUTION
if $EXEC; then
    # Return to original directory
    cd "$orig_dir" || exit 1
    
    # Execute the code
    ./thym_$mod_name $testcase
fi

# ----- PLOTTING
if $PLOT; then
    # Change to tool dir
    cd "./tools" || exit 1
    
    # Plot model results
    ./plot_in_out.py ../$testcase
fi


