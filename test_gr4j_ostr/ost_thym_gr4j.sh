#!/bin/bash

set -e

# cp ./Irondequoit.rvp model/Irondequoit.rvp
cp ./ctrl.in model/test_gr4j/ctrl.in

cd model

#./Raven_v28_MacOS.exe Irondequoit -o output/
./thym_gr4j test_gr4j

cd ..

exit 0

