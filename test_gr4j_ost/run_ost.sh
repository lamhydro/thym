#!/bin/bash

set -e


# Set up variables
OST_SERIAL_EXE=/home/alejandro/Documents/github/ostrich/make/Ostrich
OST_PARALLEL_EXE=/home/alejandro/Documents/github/ostrich/make/OstrichMPI
PARAL=false
NPROC=4
MODEL_EXE=/home/alejandro/Documents/github/thym/bin/thym_gr4j
MODEL_DIR=/home/alejandro/Documents/github/thym/test_gr4j

echo ""
echo "Cleaning up current dir from Ostrich output files and dirs"
echo ""
# Removing Ostrich output files
rm -f OstDDSP*
rm -f OstErrors*
rm -f OstModel*
rm -f OstOutput*
rm -f OstProgress*
rm -f OstStatus*
# Removing Ostrich output dirs
rm -rf mod*

echo ""
echo "Set up model execution"
echo ""
# Create model dir
if [ ! -d "model" ]; then
    mkdir model
fi

# Create model directory and copy model input files within that directory
DIR_NAME=$(basename "$MODEL_DIR")
if [ ! -d "model/$DIR_NAME" ]; then
    mkdir model/$DIR_NAME
    cp $MODEL_DIR/*.in model/$DIR_NAME
fi
# Create symboly link to model executable
MODEL_NAME=$(basename "$MODEL_EXE")
if [ ! -e "$MODEL_NAME" ]; then
    ln -s "$MODEL_EXE" model/"$MODEL_NAME"
fi

# exit 0

echo ""
echo "Launching Ostrich ..."
echo ""
# Running Ostrich
if $PARAL; then
    if [ -f "ostIn_Parallel.txt" ]; then # Set up ostIn.txt for parallel run
        cp ostIn_Parallel.txt ostIn.txt
        mpirun -np $NPROC $OST_PARALLEL_EXE
    else
        exit 1
    fi

else

    if [ -f "ostIn_Serial.txt" ]; then # Set up ostIn.txt for serial run
        cp ostIn_Serial.txt ostIn.txt
        $OST_SERIAL_EXE
    else
        exit 1
    fi
fi


