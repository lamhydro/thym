#!/bin/bash

set -e

echo ""
echo "Cleaning up current dir from Ostrich output files and dirs"
echo ""

# Removing Ostrich output files
rm OstDDSP*
rm OstErrors*
rm OstModel*
rm OstOutput*
rm OstProgress*
rm OstStatus*

# Removing Ostrich output dirs
rm -r processor_*


echo ""
echo "Launching Ostrich ..."
echo ""

# Set up control file for Ostrich
if [ -f "ostIn_Unix.txt" ]; then
    cp ostIn_Unix.txt ostIn.txt
fi

# Running Ostrich
PARAL=false
NPROC=4
if $PARAL; then
    mpirun -np $NPROC OstrichMPI
    exit 0
else
    ./Ostrich
fi


