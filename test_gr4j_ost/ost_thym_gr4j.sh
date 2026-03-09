#!/bin/bash

set -e

MODEL_DIR=test_gr4j
MODEL_EXE=thym_gr4j

cp ctrl.in model/$MODEL_DIR/ctrl.in

cd model
./$MODEL_EXE $MODEL_DIR
cd ..

exit 0

