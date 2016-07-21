#!/bin/bash

#mex -v CXX="g++-5" CXXFLAGS="-std=c++14 -fPIC" LD_LIBRARY_PATH="/usr/local/MATLAB/R2015a/bin/glnxa64" src/MyRv64gDecoder.cc src/riscv-*.cc
#mex -v CXX="g++-5" CXXFLAGS="-std=c++14 -O3 -Wall -fPIC" src/MyRv64gDecoder.cc src/riscv-*.cc src/mwg_decode.cc # For MWG-Desktop-UbuntuVM
mex -v CXX="$GCC5/bin/g++" CXXFLAGS="-std=c++14 -O3 -Wall -fPIC -L$GCC5/lib64" LDFLAGS="-L$GCC5/lib64" src/MyRv64gDecoder.cc src/riscv-*.cc src/mwg_decode.cc # For Hoffman2
