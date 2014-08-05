#!/bin/sh

./mm_binarize 220 255 gear.ppm gear_binarize.ppm
pvisu gear_binarize.ppm &
./mm_open 2 35 gear_binarize.ppm gear2.ppm
pvisu gear2.ppm &
./mm_substract gear_binarize.ppm gear2.ppm gear3.ppm
pvisu gear3.ppm &
./mm_label gear3.ppm gear_color.ppm
pvisu gear_color.ppm &
