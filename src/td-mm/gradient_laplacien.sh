#!/bin/sh

./mm_dil 2 5 lena.ppm lena_dildisk.ppm
./mm_ero 2 5 lena.ppm lena_erodisk.ppm
./mm_substract lena_dildisk.ppm lena_erodisk.ppm lena_gradient.ppm
./mm_add lena.ppm lena.ppm lena_add.ppm
./mm_add lena_dildisk.ppm lena_erodisk.ppm lena_add1.ppm
./mm_substract lena_add1.ppm lena_add.ppm lena_lapl.ppm
