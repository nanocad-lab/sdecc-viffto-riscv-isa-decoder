#!/bin/bash

#mex -v CXX="g++-5" CXXFLAGS="-std=c++14 -fPIC" LD_LIBRARY_PATH="/usr/local/MATLAB/R2015a/bin/glnxa64" src/MyRiscvDecoder.cc src/riscv-*.cc
mex -v CXX="g++-5" CXXFLAGS="-std=c++14 -O3 -Wall -fPIC" src/MyRiscvDecoder.cc src/riscv-*.cc src/mwg_decode.cc
