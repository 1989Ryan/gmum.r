#!/bin/bash

cd ../../inst/dev/datasets/svm
wget http://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/binary/mushrooms
cut -d " " -f 2- mushrooms > mushrooms.x
cut -d " " -f -1 mushrooms > mushrooms.y
rm mushrooms
