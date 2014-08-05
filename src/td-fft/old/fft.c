#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include "fft.h"

fftw_complex *fft_forward(int height, int width, unsigned short* gray_image){
  
  fftw_complex * spatial_repr = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * height * width);
  fftw_complex * frequency_repr = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * height * width);

  int i;  
  for(i = 0; i < height * width; i++){
    spatial_repr[i] = gray_image[i] + I * 0;
  }

  fftw_plan plan = fftw_plan_dft_2d(height, width, spatial_repr, frequency_repr, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(plan);

  fftw_destroy_plan(plan);
  fftw_free(spatial_repr);
  return frequency_repr;
}

unsigned short *fft_backward(int height, int width, fftw_complex* frequency_repr){
  
  fftw_complex * spatial_repr = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * height * width);
  fftw_plan plan = fftw_plan_dft_2d(height, width, frequency_repr, spatial_repr, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(plan);
  
  unsigned short * res = fftw_malloc(sizeof(unsigned short) * height * width);
  int i;
  for(i = 0; i < height * width; i++){
    res[i] = creal(spatial_repr[i])/(width * height);
    //printf("%u \n", res[i]);
  }
  return res;
}

void fft_fr_to_spectra(int height, int width, fftw_complex* freq_repr, float* as, float* ps){
  
}

void fft_spectra_to_fr(int height, int width, float* as, float* ps, fftw_complex* freq_repr){

}

