#include "fft.h"
#include <math.h>

// TODO : corriger le 1 canal à un endroit, 3 à l'autre...

// Centrage : inverse les quadrants 1-3 et 2-4 de l'image
int center(int i, int width)
{
    //return pow((-1), i / width + i % width); 
    int res = i / width + i % width;
    return res % 2 == 0 ? 1 : -1;
}

fftw_complex *fft_forward(int height, int width, unsigned short* gray_image)
{
    fftw_complex *spatialRepr = (fftw_complex*)malloc(width * height * sizeof(fftw_complex));

    /* Initialisation */
    // On enregistre un canal à la fois puisque l'image est grise (d'où le 3*i), et on centre
    for(int i = 0; i < width * height; i++)
	spatialRepr[i] = gray_image[3*i] * center(i, width) + I*0;

    /* FFT */
    fftw_complex *freqRepr = (fftw_complex*)malloc(width * height * sizeof(fftw_complex));
    fftw_plan plan = fftw_plan_dft_2d(height, width, spatialRepr, freqRepr, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // Libération mémoire
    fftw_free(spatialRepr);
    fftw_free(plan);

    return freqRepr;
}

unsigned short *fft_backward(int height, int width, fftw_complex* freq_repr)
{
    fftw_complex *spatial_repr = (fftw_complex*)malloc(width * height * sizeof(fftw_complex));
  
    fftw_plan plan = fftw_plan_dft_2d(height, width, freq_repr, spatial_repr, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
  
    unsigned short *gray_image = (unsigned short *)malloc(width * height * sizeof(unsigned short));
    for(int i = 0; i < width * height; i++) {
	unsigned short r = (unsigned short)(creal(spatial_repr[i]) / (width * height));
	gray_image[i] = r / center(i, width);
	if(gray_image[i] > 255)
	  gray_image[i] = 0;
    }

    fftw_free(spatial_repr);
    fftw_free(plan);
    return gray_image;
}

void fft_fr_to_spectra(int height, int width, fftw_complex* freq_repr, float* as, float* ps)
{
    for(int i = 0; i < width * height; i++) {
	as[i] = hypot(creal(freq_repr[i]), cimag(freq_repr[i]));
	ps[i] = atan2(cimag(freq_repr[i]), creal(freq_repr[i]));
    }
}

void fft_spectra_to_fr(int height, int width, float* as, float* ps, fftw_complex* freq_repr)
{
    for(int i = 0; i < width * height; i++) {
	freq_repr[i] = as[i] * cos(ps[i]) + I * as[i] * sin(ps[i]);
    }
}
