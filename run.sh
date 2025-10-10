#!/bin/bash

# Target dir
tar_dir=bin

# Save the current directory
orig_dir=$(pwd)

# Change to the target directory 
cd "$tar_dir" || exit 1

# Clean up prev compilation
make clean

# New compilation
make

# Return to original directory
cd "$orig_dir" || exit 1

# Execute the code
./thym test2

# Plotting 
# Change to tool dir
cd "./tools" || exit 1

# Plot model results
./plot_in_out.py ../test2/


