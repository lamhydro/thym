#!/bin/bash

set -e

MODEL_DIR=test_gr4j

echo "saving input files for the best solution found ..."

if [ ! -e best ] ; then
    mkdir best
fi

cp model/$MODEL_DIR/results.out                     best/results.out
cp model/$MODEL_DIR/diagnostic.out                  best/diagnostic.out
# cp model/output/Irondequoit_Diagnostics.csv best/Irondequoit_Diagnostics.csv 
# cp model/output/Irondequoit_Hydrographs.csv best/Irondequoit_Hydrographs.csv

exit 0

