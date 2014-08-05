#ifndef FFT_H
#define FFT_H

#include <complex.h>
#include <fftw3.h>


fftw_complex *fft_forward(int width, int height, 
                          unsigned short *channel);

unsigned short *fft_backward(int width, int height, 
                             fftw_complex *spatial);

void fft_fr_to_spectra(int width, int height,
                       fftw_complex *frequency_repr,
                       double *amplitude_spectrum,
                       double *phase_spectrum);

void fft_spectra_to_fr(int width, int height,
                       double *amplitude_spectrum,
                       double *phase_spectrum,
                       fftw_complex *frequency_repr);

void spectral_normalization(int width, int height, 
                   double *as, double *ps);
#endif
