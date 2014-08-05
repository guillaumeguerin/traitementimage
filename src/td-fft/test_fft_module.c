#include<stdlib.h>
#include<stdio.h>
#include "fft.h"
#include "pnm.h"

/**
 * @file test_fft_module.c
 * @brief test the behaviors of functions in fft module
 */


/**
 * @brief test the forward and backward functions
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_for_backward(pnm ims, char* name)
{
  int w  = pnm_get_width(ims);
  int h = pnm_get_height(ims);
  fftw_complex * machin = fftw_malloc(w*h*sizeof(fftw_complex));
  unsigned short * image = malloc(w*h*sizeof(unsigned short));
  image = pnm_get_channel(ims, pnm_get_image(ims),0); 
  machin =  fft_forward(h,w,image);
  image = fft_backward(h,w,machin);

  /*
  for(int i=0; i<w*h; i++){
    pnm_set_component(ims, i%w,((i-i%w))/w,0,image[i]);
    pnm_set_component(ims, i%w,((i-i%w))/w,1,image[i]);
    pnm_set_component(ims, i%w,((i-i%w))/w,2,image[i]);
  }*/

  pnm_set_channel(ims, image, 0);
  pnm_set_channel(ims, image,1);
  pnm_set_channel(ims, image,2);
  pnm_save(ims,PnmRawPpm,"FB_lena.ppm");
}

/**
 * @brief test the creation of amplitude and phase spectrum
 *        and construct their images in ppm files
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_spectrum(pnm ims, char* name)
{
  int w  = pnm_get_width(ims);
  int h = pnm_get_height(ims);
  fftw_complex * machin = fftw_malloc(w*h*sizeof(fftw_complex));
  unsigned short * image = malloc(w*h*sizeof(unsigned short));
  image = pnm_get_channel(ims, pnm_get_image(ims), 0); 
  machin =  fft_forward(h,w,image);
  
  float * as = (float *) malloc(sizeof(float) * h * w);
  float * ps = (float *) malloc(sizeof(float) * h * w);
  fft_fr_to_spectra(w, h, machin, as, ps);
    
  int i;
  unsigned short * imageAS = malloc(w*h*sizeof(unsigned short));
  unsigned short * imagePS = malloc(w*h*sizeof(unsigned short));
  for(i = 0; i < h * w; i++){
    imageAS[i] = (unsigned short) as[i];
    imagePS[i] = (unsigned short) ps[i];
  }

  pnm_set_channel(ims, imageAS, 0);
  pnm_set_channel(ims, imageAS, 1);
  pnm_set_channel(ims, imageAS, 2);
  pnm_save(ims,PnmRawPpm,"AS_lena.ppm");

  pnm_set_channel(ims, imagePS, 0);
  pnm_set_channel(ims, imagePS, 1);
  pnm_set_channel(ims, imagePS, 2);
  pnm_save(ims,PnmRawPpm,"PS_lena.ppm");

  fft_spectra_to_fr(h,w,imageAS,imagePS,machin);
  
  image = fft_backward(h,w,machin);

  pnm_set_channel(ims, image, 0);
  pnm_set_channel(ims, image,1);
  pnm_set_channel(ims, image,2);
  pnm_save(ims,PnmRawPpm,"FB_ASPS_lena.ppm");
}

/**
 * @brief test the modification of amplitude and phase spectrum and 
 *        construct output images
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_modification(pnm ims, char* name)
{
  
}


static void 
run(pnm ims, char* name)
{
  test_for_backward(ims, name);
  test_spectrum(ims, name);
  test_modification(ims, name);
}


void 
usage (char *s)
{
  fprintf(stderr, "Usage: %s <input_name> \n", s);
  exit(EXIT_FAILURE);
}

int 
main(int argc, char *argv[])
{
  if (argc != 2)
    usage(argv[0]);
  pnm ims = pnm_load(argv[1]);
  run(ims, argv[1]);
  return EXIT_SUCCESS;
}
