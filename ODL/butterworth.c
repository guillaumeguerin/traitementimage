/**
 * @author Joris Valette
 * @file butterworth.c
 * @brief Apply frequency filter on a picture
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pnm.h"
#include "fft.h"

int w;
int u0;
int v0;

float lp(int u, int v, int w, int h, int d0, int n)
{
    return 1 / (1 + pow(hypot(v - h / 2, u - w / 2) / d0, 2 * n));
}

float hp(int u, int v, int w, int h, int d0, int n)
{
    return 1 / (1 + pow(d0 / hypot(v - h / 2, u - w / 2), 2 * n));
}

float br(int u, int v, int w, int h, int d0, int n)
{
    return 1 / (1 + pow(hypot(v - h / 2, u - w / 2) * w / ((hypot(v - h / 2, u - w / 2) * hypot(v - h / 2, u - w / 2)) - d0 * d0), 2 * n));
}

float bp(int u, int v, int w, int h, int d0, int n)
{
    return 1 - br(u, v, w, h, d0, n);
}

float no(int u, int v, int w, int h, int d0, int n)
{
    return 1 / (1 + pow(d0 * d0 / (hypot(v - h / 2 - v0, u - w / 2 - u0) * hypot(v - h / 2 + v0, u - w / 2 + u0)), 2 * n));
}

void process(pnm ims, pnm imd, int d0, int n, float(*filter)(int, int, int, int, int, int))
{
    const int srcWidth = pnm_get_width(ims);
    const int srcHeight = pnm_get_height(ims);
    const int srcSize = srcWidth * srcHeight;
    unsigned short *pIms = (unsigned short*)malloc(3 * srcSize * sizeof(unsigned short));
    pIms = pnm_get_image(ims);
    unsigned short *pImd = (unsigned short*)malloc(3 * srcSize * sizeof(unsigned short));
    pImd = pnm_get_image(imd);
    
    fftw_complex *freq_repr = fft_forward(srcHeight, srcWidth, pIms);

    for(int v = 0; v < srcHeight; v++) {
    	for(int u = 0; u < srcWidth; u++) {
	    freq_repr[v * srcWidth + u] *= filter(u, v, srcWidth, srcHeight, d0, n);
    	}
    }

    unsigned short *imRecons = fft_backward(srcHeight, srcWidth, freq_repr);
    pnm_set_channel(imd, (unsigned short*)imRecons, PnmRed);
    pnm_set_channel(imd, (unsigned short*)imRecons, PnmGreen);
    pnm_set_channel(imd, (unsigned short*)imRecons, PnmBlue);
}

void usage (char *s)
{
    fprintf(stderr, "Usage: %s <ims> <imd> <filter> <d0> <n> <filter_specific_args...>\n", s);
    fprintf(stderr, "With <filter_specific_args...> being :\n");
    fprintf(stderr, "\tWith <filter> = \"lp\" or \"hp\" : \t\"none\"\n");
    fprintf(stderr, "\tWith <filter> = \"br\" or \"bp\" : \t<w>\n");
    fprintf(stderr, "\tWith <filter> = \"no\" : \t\t<u0> <v0>\n");

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if(argc < 6)
	usage(argv[0]);

    void *filter;
    
    if(strcmp(argv[3], "lp") == 0) {
	if(argc != 6)
	    usage(argv[0]);
	filter = lp;
    }   
    else if(strcmp(argv[3], "hp") == 0) {
	if(argc != 6)
	    usage(argv[0]);
	filter = hp;
    }  
    else if(strcmp(argv[3], "br") == 0) {
	if(argc != 7)
	    usage(argv[0]);
	filter = br;
	w = atoi(argv[6]);	
    }
    else if(strcmp(argv[3], "bp") == 0) {
	if(argc != 7)
	    usage(argv[0]);
	filter = bp;
	w = atoi(argv[6]);
    }
    else if(strcmp(argv[3], "no") == 0) {
	if(argc != 8)
	    usage(argv[0]);
	filter = no;
	u0 = atoi(argv[6]);
	v0 = atoi(argv[7]);
    }
    else
	usage(argv[0]);

    int d0 = atoi(argv[4]);
    int n = atoi(argv[5]);

    pnm ims = pnm_load(argv[1]);
    pnm imd = pnm_new(pnm_get_width(ims), pnm_get_height(ims), PnmRawPpm);

    process(ims, imd, d0, n, filter);

    pnm_save(imd, PnmRawPpm, argv[2]);
    // Free
    return EXIT_SUCCESS;
}
