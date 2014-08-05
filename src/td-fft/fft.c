#include <math.h>
#include <complex.h>
#include "fft.h"

fftw_complex *fft_forward(int width, int height, unsigned short* image){

  fftw_complex *freq_repr = fftw_malloc(width * height * sizeof(fftw_complex));
  fftw_complex *spatial_repr = fftw_malloc(width * height * sizeof(fftw_complex));

  for(int i=0; i < height * width; i++){
    spatial_repr[i] = image[i] + I * 0;
    // recentrage : I'(x,y) = -1^(x+y)*I(x,y)
    spatial_repr[i] = pow(-1, (i % width) + ((i - (i % width)) / width)) * spatial_repr[i];
  }
  
  fftw_plan plan = fftw_plan_dft_2d(height, width, spatial_repr, freq_repr, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(plan);
  
  fftw_free(spatial_repr);
  fftw_destroy_plan(plan);
  return freq_repr;
}

unsigned short *fft_backward(int width, int height, fftw_complex* freq_repr){

  fftw_complex *spatial_repr = fftw_malloc(width * height * sizeof(fftw_complex)); 
  fftw_plan plan = fftw_plan_dft_2d(height, width, freq_repr, spatial_repr, FFTW_BACKWARD, FFTW_ESTIMATE);
  unsigned short *image = fftw_malloc(width * height * sizeof(unsigned short));
  
  fftw_execute(plan); 
  
  for(int i=0; i < height * width; i++){
      image[i] = creal(spatial_repr[i]) / (width * height);
      // on remet le centre comme avant
      image[i] = pow(-1, (i % width) + ((i - (i % width)) / width)) * image[i];
  }

  fftw_free(freq_repr); 
  fftw_free(spatial_repr);
  return image;
}

void fft_fr_to_spectra(int w, int h, fftw_complex* freq_repr, float * as, float * ps){

  float asMax = 0;
  for(int i=0; i < h * w; i++){
    as[i] = hypot(creal(freq_repr[i]), cimag(freq_repr[i])) / h;
    if(asMax < as[i])
      asMax = as[i];
    ps[i] = atan2(creal(freq_repr[i]), cimag(freq_repr[i]));
  }

  //normalisation avec la constante k fixee a la valeur 0.1 (cours 1 page 16)
  float k = 0.1;
  for(int i = 0; i < h * w; i++){
    as[i]=pow((as[i] / asMax), k) * 255;
  }
}

void fft_spectra_to_fr(int h, int w, float* as, float* ps, fftw_complex* freq_repr){
  freq_repr = fftw_malloc(w * h * sizeof(fftw_complex));

  for(int i = 0; i < h * w; i ++){
    freq_repr[i] = (as[i] * cos(ps[i])) + I * (as[i] * sin(ps[i]));
  }
}
