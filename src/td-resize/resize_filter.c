#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <pnm.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <fftw3.h>
#include "fft.h"

#define PI (3.141592653589793)


#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

float box(float x) {
  if( x < 0.5 && x >= -0.5)
    return 1;
  return 0;
}

float tent(float x) {
  if(x < 0)
    x = -x;
  if(x >= 0 && x <= 1)
    return 1 - x;
  else
    return 0;
}

float gaussian (float x) {
  return 1/sqrt(PI*2)*exp(-x*x/2);
}

float bell (float x) {
  if(fabsf(x) <= 0.5)
    return -x*x + 0.75;
  if(fabsf(x) > 0.5 && fabsf(x) <= 1.5)
    return 0.5*(abs(x)-1.5)*(abs(x)-1.5);
  return 0;
}

float mitchell_netravali (float x){
  if(x >= -1 && x<=1)
    return (float)7/6 * abs(x*x*x) -2*x*x + (float)8/9;

  if((x >= -2 && x <= -1) || (x >= 1 && x <= 2))
    return -(float)7/18 * abs(x*x*x) + 2*x*x - (float)10/3*abs(x) + (float)16/9;

  return 0;
}

void flip(int width, int height, unsigned short *image1, unsigned short *image2) {
  for(int y=0; y<height; y++) 
    for(int x=0; x<width; x++)
      image1[width * y + x] = image2[height * x + y];
}

void process(int factor, char* filter, char* ims_name, char* imd_name){

  /* Selection du filtre */
  float (*function_pointer) (float);
  if(strcmp(filter,"box") == 0){
    printf("box filter\n");
    function_pointer = box;
  }
  else if(strcmp(filter,"tent") == 0){
    printf("tent filter\n");
    function_pointer = tent;
  }
  else if(strcmp(filter,"gauss") == 0){
    printf("gaussian filter\n");
    function_pointer = gaussian;
  }
  else if(strcmp(filter,"bell") == 0){
    printf("bell filter\n");
    function_pointer = bell;
  }
  else if(strcmp(filter,"mitch") == 0){
    printf("Mitchell-Netravali filter\n");
    function_pointer = mitchell_netravali;
  }
  else {
    printf("unknown filter,\n filters avalaible: box, tent, gauss, bell, mitch");
    assert(false);
  }

  pnm ims = pnm_load(ims_name);

  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  int height2 = height * factor;
  int width2 = width * factor;
  float wf = factor / 2;
  pnm imd = pnm_new(width2, height2, PnmRawPpm);
  
  /* image contiendra les valeur des pixels de l'image source, image2 contiendra l'image en cours de modification et image3 contiendra les images pivotées dont l'image finale*/
  unsigned short * image = (unsigned short *) malloc(width * height * sizeof(unsigned short));
  unsigned short * image2 = (unsigned short *) malloc(width2 * height2 * sizeof(unsigned short));
  unsigned short * image3 = (unsigned short *) malloc(width2 * height2 * sizeof(unsigned short));
  image = pnm_get_channel(ims, image, PnmRed);

  
  float left, right, s, l;
  int i_origin;

  /* On applique le filtre sur les lignes */
  for(int i=0; i < height2; i++){ 
    i_origin = i / factor;

    for(int l2=0; l2 < width2; l2++){
      l = l2 / factor;
      left = (l - wf) >= 0 ? (l - wf) : 0;     
      right = (l + wf) <= width ? (l + wf) : width; 
      s = 0;
      
      for(int k=left; k <= right; k++)
	s += image[k + i_origin * width] * function_pointer(k-l);  
      
      if(s > 255)
	s = 255;
      else if(s < 0)
	s = 0;
      image2[i_origin * width2 + l2] = s;
      }
  }

  /* retourne l'image pour que l'on puisse travailler sur les colonnes  */
  flip(width2, height2, image3, image2);

  /* On applique le filtre sur les colonnes */
  for(int i=0; i < height2; i++){
  
    for(int l2=0; l2 < width2; l2++){
      l = l2 / factor;
      left = (l - wf) >= 0 ? (l - wf) : 0;     
      right = (l + wf) <= width ? (l + wf) : width; 
      s = 0;

      for(int k=left; k <= right; k++)
	s += image3[k + i * width2] * function_pointer(k-l);

      if(s > 255)
	s = 255;
      else if(s < 0)
	s = 0;
      image2[i * width2 + l2] = s;
      }
  }

  flip(width2, height2, image3, image2);

  pnm_set_channel(imd, image3, PnmRed);
  pnm_set_channel(imd, image3, PnmGreen);
  pnm_set_channel(imd, image3, PnmBlue);

  pnm_save(imd, PnmRawPpm, imd_name); 
  free(image);
  free(image2);
  free(image3); 
  pnm_free(imd);
  pnm_free(ims); 
}

void usage (char *s){
  fprintf(stderr, "Usage: %s <factor> <filter> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 4
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);  
  int factor = atoi(argv[1]);
  char *filter = argv[2];
  char *ims_name = argv[3];
  char *imd_name = argv[4];
  process(factor, filter, ims_name, imd_name);
  return EXIT_SUCCESS;
}
