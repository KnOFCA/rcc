#!/bin/bash

rm -rf generated
bash ./generate.sh
rm -rf ANTLR/.antlr
rm -rf build
mkdir build
cd build
cmake ..
make
