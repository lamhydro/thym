#!/bin/bash

set -e

echo "saving input files for the best solution found ..."

if [ ! -e best ] ; then
    mkdir best
fi

cp model/test_gr4j/results.out                     best/results.out
cp model/test_gr4j/diagnostic.out                  best/diagnostic.out
# cp model/output/Irondequoit_Diagnostics.csv best/Irondequoit_Diagnostics.csv 
# cp model/output/Irondequoit_Hydrographs.csv best/Irondequoit_Hydrographs.csv

exit 0

