#!/bin/sh

./mm_binarize 210 255 im-1.ppm im-1-bin.ppm

./mm_dil 0 3 im-1-bin.ppm im-1-dil.ppm 
./mm_ero 2 10 im-1-dil.ppm im-1-ero.ppm 