#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <pnm.h>
#include <fftw3.h>

#include "fft.h"

void  
process(int coeff, char* ims_name, char* imd_name){
  pnm ims = pnm_load(ims_name);
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  int wh = width * height;
  int halfWidth =  ((coeff * width ) - width)/2;
  int halfHeight =  ((coeff * height ) - height)/2;
  int width2 = halfWidth +  width;
  int height2 = halfHeight +  height;
  int finalWidth = width * coeff;
  int finalHeight = height * coeff;
  pnm imd = pnm_new(width * coeff, height * coeff, PnmRawPpm);

  unsigned short * image = (unsigned short *) malloc(wh * sizeof(unsigned short));
  unsigned short * image2 = (unsigned short *) malloc(wh * sizeof(unsigned short) * coeff * coeff);
  image = pnm_get_channel(ims, pnm_get_image(ims),0); 
 
  fftw_complex * freq_repr = (fftw_complex *) fftw_malloc(wh * sizeof(fftw_complex));
  fftw_complex * freq_repr2 = (fftw_complex *) fftw_malloc(wh * coeff * coeff * sizeof(fftw_complex));
  freq_repr = fft_forward(height,width,image);

  /* On agrandi la transformée de Fourier en la remplissant de zéro au niveau des hautes fréquences */
  for(int j=0; j < finalHeight; j++)
    for(int i=0; i < finalWidth; i++){
      freq_repr2[i + j * width] = 0;
    }

  /* On met les valeurs de la fft au milieu dans le nouveau tableau */
  for(int j=halfHeight; j < height2; j++)
    for(int i=halfWidth; i < width2; i++){
      freq_repr2[i + j * finalWidth] = freq_repr[i - halfWidth + (j-halfHeight) * width];
    }

  image2 = fft_backward(finalHeight, finalWidth, freq_repr2);  
  
  /* dans fft_backward on divise les valeurs par le nombre d'échantillon, or celui ci a augmenté de coeff * coeff, il faut renormaliser */
  for(int i=0; i < finalWidth * finalHeight; i++){
    image2[i] *= coeff * coeff;
    if(image2[i] > 255){
      image2[i] = image2[i] % 255;
    }
  }
  
  pnm_set_channel(imd, image2, PnmRed);
  pnm_set_channel(imd, image2, PnmGreen);
  pnm_set_channel(imd, image2, PnmBlue);

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <coeff> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int 
main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);  
  int coeff = atoi(argv[1]);
  char *ims_name = argv[2];
  char *imd_name = argv[3];
  process(coeff, ims_name, imd_name);
  return EXIT_SUCCESS;
}
