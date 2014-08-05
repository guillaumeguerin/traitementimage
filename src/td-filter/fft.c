#include "fft.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pnm.h>

fftw_complex *fft_forward(int width, int height,
                          unsigned short *channel){
  fftw_complex *frequency = fftw_malloc(width*height*sizeof(fftw_complex));
  fftw_complex *spatial = fftw_malloc(width*height*sizeof(fftw_complex));

  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      frequency[i*width+j] = (double)(channel[i*width+j]*pow(-1,i+j));
    }
  }

  fftw_plan plan = fftw_plan_dft_2d(height, width, frequency, spatial,
                                    FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_destroy_plan(plan);
  fftw_free(frequency);

  return spatial;
}

unsigned short *fft_backward(int width, int height,
                             fftw_complex *spatial){

  fftw_complex *frequency = malloc(width*height*sizeof(fftw_complex));
  unsigned short *res = malloc(width*height*sizeof(unsigned short));

  fftw_plan plan = fftw_plan_dft_2d(height, width, spatial, frequency,
                                    FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(plan);

  double max = 0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      frequency[i*width+j] *= pow(-1,i+j);
      if(max < creal(frequency[i * width + j]))
        max = creal(frequency[i * width + j]);
    }
  }    
  for(int i = 0; i < width*height; i++){
    res[i] = creal(frequency[i]) / max * pnm_maxval;
  }

  fftw_free(frequency);
  fftw_destroy_plan(plan);
 
  return res;
}


void fft_fr_to_spectra(int width, int height,
                       fftw_complex *frequency_repr,
                       double *as, double *ps){
  for(int i = 0; i < width*height+1;  i++){
    as[i] = hypot(creal(frequency_repr[i]),cimag(frequency_repr[i]));
    ps[i] = atan2(cimag(frequency_repr[i]),creal(frequency_repr[i]));
  }
}


void fft_spectra_to_fr(int width, int height,
                       double *as, double *ps,
                       fftw_complex *frequency_repr){
  for(int i=0; i<width*height; i++)
      frequency_repr[i] = as[i] *  cos(ps[i]) + I * (as[i] * sin(ps[i]));
}


void spectral_normalization(int width, int height, double *as, double *ps){
  double max_as = 0;
  double max_ps = 0;
  for(int i = 0; i < width*height+1;  i++){
    if(as[i] > max_as)
      max_as = as[i];
    if(ps[i] > max_ps)
      max_ps = ps[i];
  }
  for(int i = 0; i < width*height;  i++){
    as[i] /= max_as;
    ps[i] /= max_ps;
    as[i] = pow(as[i], 0.15) * pnm_maxval;
    ps[i] = pow(ps[i], 0.15) * pnm_maxval;
  }
}

