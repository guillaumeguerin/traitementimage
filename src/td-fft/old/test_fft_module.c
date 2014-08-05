#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>
#include "fft.h"

/**
 * @file test_fft_module.c
 * @brief test the behaviors of functions in fft module
 */
void process(int num, char* ims_name, char* imd_name);
unsigned short * extract_grey(pnm ims_name, unsigned short * grey_image);
void do_grey(unsigned short * r, pnm imd);
/**
 * @brief test the forward and backward functions
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_for_backward(pnm ims, char* name)
{
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);
  unsigned short * res = (unsigned short *) malloc(sizeof(unsigned short *) * height * width);
  unsigned short * res2 = (unsigned short *) malloc(sizeof(unsigned short *) * height * width);
  
  res2 = extract_grey(ims, res);
  fftw_complex * frequency_repr = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * height * width);
  frequency_repr = fft_forward(height, width, res);
  fft_backward(height, width, frequency_repr);

  do_grey(res2, imd);

  pnm_save(imd, PnmRawPpm, "FB_lena.ppm");
  pnm_free(imd);
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
  run(ims, "lena.ppm");
  return EXIT_SUCCESS;
}

unsigned short *
extract_grey(pnm ims, unsigned short * grey_image){
  unsigned short r;
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      for(int z=0; z<3; z++){
	  r = pnm_get_component(ims, y, x,PnmRed);
	  grey_image[x+y] = r;
      }
    }
  }
  pnm_free(ims);
  return grey_image;
}

void
do_grey(unsigned short * r, pnm imd){
  int height = pnm_get_height(imd);
  int width = pnm_get_width(imd);
  
  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      for(int z=0; z<3; z++){
	  pnm_set_component(imd,y,x,PnmRed, r[x]);
	  pnm_set_component(imd,y,x,PnmGreen, r[x]);
	  pnm_set_component(imd,y,x,PnmBlue, r[x]);
      }
    }
  }
}
