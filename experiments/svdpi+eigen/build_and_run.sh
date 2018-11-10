#!/bin/bash -x
EIGEN_HOME=/home/Daniil/eigen/eigen-git-mirror

rm -rf  ./obj_dir/
rm  -f *.log

verilator --cc test.sv qubits.cc qubits_dpi.c --exe main.cc -CFLAGS "-g -DDPI_DLLISPEC= -DPI_DLLESPEC= -I. -I$EIGEN_HOME"  2>&1 | tee verilation.log

make -C ./obj_dir/ -f Vtest.mk Vtest 2>&1 | tee compilation.log

./obj_dir/Vtest 2>&1 | tee simulation.log

