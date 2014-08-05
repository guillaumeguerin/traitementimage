#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <pnm.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "fft.h"
#include <fftw3.h>

float d(double i, double j){
  return sqrt((float)j*j+(float)i*i);
}

float d1(double u, double v, double u0, double v0){
  return sqrtf((u-u0)*(u-u0) + (v-v0)*(v-v0));
}

float d2(double u, double v, double u0, double v0){
  return sqrtf((u+u0)*(u+u0) + (v+v0)*(v+v0));
}

float lp(double u, double v, double u0, double v0, int n, int w, int d0){
  return 1/ (1 + pow((float)(d(u,v)/d0), 2*n));
}

float hp(double u, double v, double u0, double v0, int n, int w, int d0){
  //float tmp = d0/d(u,v);
  //float tmp = pow((float)(d0/d(u,v)), 2 *n);
  //printf("%f %f %f \n", d(u,v), tmp, 1 / pow((float)(d0/d(u,v)), 2 *n));
  return 1 / pow((float)(d0/d(u,v)), 2 *n);
}

float br(double u, double v, double u0, double v0, int n, int w, int d0){
  return 1/ (1 + pow((d(u,v)*w)/(d2(u,v,u0,v0)-d0*d0), 2*n));
}

float bp(double u, double v, double u0, double v0, int n, int w, int d0){
  return 1 - br(u,v,u0,v0,n,w,d0);
}

float no(double u, double v, double u0, double v0, int n, int w, int d0){
  return 1/ (1+ pow((d0*d0)/ (d1(u,v,u0,v0)*d2(u,v,u0,v0)), 2*n));
}

/*float low_pass(int d0, int ordre_n, float d_u_v) {
  return 1/(1 +  pow((d_u_v/d0), 2*ordre_n));
  }*/

void process(char* ims_name, char* imd_name, char* filter, int d0, int n, int w, int u0, int v0) {

  
  /* Selection du filtre */
  /*float (*function_pointer) (double, double, double, double, int, int, int);
  if(strcmp(filter,"lp") == 0){
    printf("low pass filter\n");
    function_pointer = lp;
  }
  else if(strcmp(filter,"hp") == 0){
    printf("high pass filter\n");
    function_pointer = hp;
  }
  else if(strcmp(filter,"br") == 0){
    printf("band reject filter\n");
    function_pointer = br;
  }
  else if(strcmp(filter,"bp") == 0){
    printf("band pass filter\n");
    function_pointer = br;
  }
  else if(strcmp(filter,"no") == 0){
    printf("rejet d'encoche \n");
    function_pointer = no;
  }
  else {
    printf("unknown filter,\n filters avalaible: lp, hp, br, bp, no");
    assert(false);
    }*/


  pnm ims = pnm_load(ims_name);
  int width  = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);
  
  unsigned short * image = (unsigned short *) malloc(height * width * sizeof(unsigned short));
  fftw_complex * freq_repr = (fftw_complex *) fftw_malloc(height* width * sizeof(fftw_complex));

  image = pnm_get_channel(ims, image, PnmRed);
  freq_repr = fft_forward(height, width, image);

  float * as = (float *) malloc(sizeof(float) * height * width);
  float * ps = (float *) malloc(sizeof(float) * height * width);
  fft_fr_to_spectra(width, height, freq_repr, as, ps);

  
  pnm imd2 = pnm_new(width, height, PnmRawPpm);
  for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++)
      for(int z = 0; z < 3; z++)
	pnm_set_component(imd2,y,x,z,as[x+y*height]);  
  pnm_save(imd2, PnmRawPpm, "toto.ppm");

  //float d_u_v;

  for(int j=0; j<height; j++){
    for(int i=0; i<width; i++){
      //d_u_v  = sqrt((float)(j-height/2)*(j-height/2)+(float)(i-width/2)*(i-width/2));       
      //as[i+j*width] = function_pointer(i, j, u0, v0, n, w, d0) * as[i+j*width];
      as[i+j*width] = low_pass(d0, n, d((float) i-width/2,(float) j-height/2)) * as[i+j*width];
      //printf("%f \n", hp(i, j, u0, v0, n, w, d0));
      //printf("%d \n", function_pointer(i, j, u0, v0, n, w, d0));
    }
  }
  
  pnm imd3 = pnm_new(width, height, PnmRawPpm);
  for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++)
      for(int z = 0; z < 3; z++)
	pnm_set_component(imd3,y,x,z,as[x+y*height]); 
  pnm_save(imd3, PnmRawPpm, "toto2.ppm");
  

  fft_spectra_to_fr(height,width,as,ps, freq_repr);
  
  image = fft_backward(height, width, freq_repr);
  
  for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++)
      for(int z = 0; z < 3; z++)
	pnm_set_component(imd,y,x,z,image[x+y*height]);
  
  pnm_save(imd, PnmRawPpm, imd_name);
  
  free(image);

}

void usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imd> <filter> <d0> <n> <w> <u0> <v0>\n", s);
  exit(EXIT_FAILURE);
}

#define param 8
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);
  char* ims_name = argv[1];
  char* imd_name = argv[2];
  char* filter_name = argv[3];
  int d0 = atoi(argv[4]);
  int n = atoi(argv[5]);
  int w = atoi(argv[6]);
  int u0 = atoi(argv[7]);
  int v0 = atoi(argv[8]);
  
  process(ims_name, imd_name, filter_name, d0, n, w, u0, v0); 
  
  return EXIT_SUCCESS;
}
