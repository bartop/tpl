#!/bin/bash

mkdir $HOME/usr
export PATH="$HOME/usr/bin:$PATH"
wget https://cmake.org/files/v3.9/cmake-3.9.1-Linux-x86_64.sh
chmod +x cmake-3.9.1-Linux-x86_64.sh
./cmake-3.9.1-Linux-x86_64.sh --prefix=$HOME/usr --exclude-subdir --skip-license
cmake --version
eval "${MATRIX_EVAL}"
export CPLUS_INCLUDE_PATH=/usr/local/include
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=$COMPILER -DCMAKE_BUILD_TYPE=Release ..
make all
ctest .
