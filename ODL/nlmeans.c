/**
 * @author Joris Valette
 * @file nlmeans.c
 * @brief Apply NL-Means filter on a picture
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pnm.h"

// TODO : Supprimer les trucs inutiles

#define R 5
#define N 2
#define DIV (2 * N + 1) * (2 * N + 1)
float sigma;

void process(pnm ims, pnm imd)
{
    const int srcWidth = pnm_get_width(ims);
    const int srcHeight = pnm_get_height(ims);
    const int srcSize = srcWidth * srcHeight;
    unsigned short *pIms = (unsigned short*)malloc(3 * srcSize * sizeof(unsigned short));
    pIms = pnm_get_image(ims);
    unsigned short *pImd = (unsigned short*)malloc(3 * srcSize * sizeof(unsigned short));
    pImd = pnm_get_image(imd);
    float *currImg = (float*)malloc(srcSize * 3 * sizeof(float));
    float *nextImg = (float*)malloc(srcSize * 3 * sizeof(float));

    for(int i = 0; i < srcSize; i++) {
	currImg[i] = pIms[i*3];
    }

    for(int y = 0; y < srcHeight; y++) {
	for(int x = 0; x < srcWidth; x++) {
	    const int offset = y * srcWidth + x;
	    float numerator = 0.;
	    float denominator = 0.;

	    for(int y0 = y - R; y0 < y + R + 1; y0++) {
		for(int x0 = x - R; x0 < x + R + 1; x0++) {
		    if(y0 >= 0 && y0 < srcHeight && x0 >= 0 && x0 < srcWidth) {
			const int offset0 = y0 * srcWidth + x0;
			float dist = 0.;

			for(int patch_y = -N; patch_y < N + 1; patch_y++) {
			    for(int patch_x = -N; patch_x < N + 1; patch_x++) {

				// Check boundaries for patch around p and patch around q
				if(y + patch_y >= 0 && y + patch_y < srcHeight && x + patch_x >= 0 && x + patch_x < srcWidth
				   && y0 + patch_y >= 0 && y0 + patch_y < srcHeight && x0 + patch_x >= 0 && x0 + patch_x < srcWidth) {

				    const int offset_patch = patch_y * srcWidth + patch_x;
				    float value_p = currImg[offset + offset_patch];
				    float value_q = currImg[offset0 + offset_patch];
				    //dist += sqrt((value_p - value_q) * (value_p - value_q));
				    dist += abs(value_p - value_q);
				    //dist += (value_p - value_q) * (value_p - value_q);
				    //dist += value_p - value_q;
				}
			    }
			}
			//dist /= DIV;
			float weight = exp(-dist/(2*sigma*sigma));
			numerator += weight * currImg[offset0];
			denominator += weight;
		    }
		}
	    }
	    nextImg[offset] = numerator/denominator;
	}
    }
   
    for(int i = 0; i < srcSize; i++) {
	pImd[i*3] = nextImg[i];
	pImd[i*3+1] = nextImg[i];
	pImd[i*3+2] = nextImg[i];
    }

    free(currImg);
    free(nextImg);
}

void usage (char *s)
{
    fprintf(stderr, "Usage: %s <sigma> <ims> <imd>\n", s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
	usage(argv[0]);

    sigma = atoi(argv[1]);
    pnm ims = pnm_load(argv[2]);
    pnm imd = pnm_new(pnm_get_width(ims), pnm_get_height(ims), PnmRawPpm);

    process(ims, imd);

    pnm_save(imd, PnmRawPpm, argv[3]);
    // Free
    return EXIT_SUCCESS;
}
