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

double d(int u, int v){
  return sqrt(u*u + v*v);
}

double d1(int u, int v, int u0, int v0){
  return sqrt((u-u0)*(u-u0) + (v-v0)*(v-v0));
}

double d2(int u, int v, int u0, int v0){
  return sqrt((u+u0)*(u+u0) + (v+v0)*(v+v0));
}

double lp(double u, double v, double u0, double v0, int n, int w, int d0){
  return 1/ (1 + pow(d(u,v)/d0, 2*n));
}

double hp(double u, double v, double u0, double v0, int n, int w, int d0){
  return 1/ (1 + pow(d0/d(u,v), 2*n));
}

double br(double u, double v, double u0, double v0, int n, int w, int d0){
  return 1/ (1 + pow( (d(u,v)*w)/(pow(d(u,v),2) - pow(d0,2)), 2*n));
}

double bp(double u, double v, double u0, double v0, int n, int w, int d0){
  return 1- br(u,v,u0,v0,n,w,d0);
}

double no(double u, double v, double u0, double v0, int n, int w, int d0){
  return 1/ (1 + pow( pow(d0,2)/ (d1(u,v,u0,v0)*d2(u,v,u0,v0)), 2*n));
}


void process(char* ims_name, char* imd_name, char* filter, int d0, int n, int w, int u0, int v0) {
  
  /*
  float (*function_pointer) (double, double, double, double, int, int, int);
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
  }
  */
  pnm ims = pnm_load(ims_name);
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);

  pnm imd = pnm_new(width, height, PnmRawPgm);
  pnm as_repr = pnm_new(width, height, PnmRawPgm);
  pnm ps_repr = pnm_new(width, height, PnmRawPgm);

  double* as =(double *) malloc(width * height * sizeof(double));
  double* ps =(double *) malloc(width * height * sizeof(double));
  unsigned short* res =(unsigned short *) malloc(width * height * sizeof(unsigned short));

  res = pnm_get_channel(ims,res,PnmGreen);
  fftw_complex* my_fft_plane1 =  fft_forward(width, height, res);
  fft_fr_to_spectra(width, height,my_fft_plane1, as, ps);

  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      //as[i*width+j] *= function_pointer((j-width/2),(i-height/2),u0,v0,n,w,d0);
       if(strcmp(filter,"lp") == 0){
	 as[i*width+j] *= lp((j-width/2), (i-height/2), u0, v0, n, w, d0);
       }
       else if(strcmp(filter,"hp") == 0){
	 as[i*width+j] *= hp((j-width/2), (i-height/2), u0, v0, n, w, d0);
       }
       else if(strcmp(filter,"br") == 0){
	 as[i*width+j] *= br((j-width/2), (i-height/2), u0, v0, n, w, d0);
       }
       else if(strcmp(filter,"bp") == 0){
	 as[i*width+j] *= bp((j-width/2), (i-height/2), u0, v0, n, w, d0);
       }
       else if(strcmp(filter,"no") == 0){
	 as[i*width+j] *= no((j-width/2), (i-height/2), u0, v0, n, w, d0);
       }

    }
  }
  fftw_complex* my_fft_plane2 = fftw_malloc(width * height * sizeof(fftw_complex));
  fft_spectra_to_fr(width, height, as, ps, my_fft_plane2);
  spectral_normalization(width, height, as, ps);

  free(res);
  res = fft_backward(width, height, my_fft_plane2);

  for (int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){     

      if(res[i*width + j] > 255)
	res[i*width + j] = -res[i*width + j];

      pnm_set_component(as_repr, i, j, PnmRed, (unsigned short) as[i*width + j]);
      pnm_set_component(as_repr, i, j, PnmGreen, (unsigned short) as[i*width+j]);
      pnm_set_component(as_repr, i, j, PnmBlue, (unsigned short) as[i*width+j]);

      pnm_set_component(ps_repr, i, j, PnmRed, (unsigned short) ps[i*width + j]);
      pnm_set_component(ps_repr, i, j, PnmGreen, (unsigned short) ps[i*width+j]);
      pnm_set_component(ps_repr, i, j, PnmBlue, (unsigned short) ps[i*width+j]);

      pnm_set_component(imd, i, j, PnmRed, res[i*width + j]);
      pnm_set_component(imd, i, j, PnmGreen, res[i*width+j]);
      pnm_set_component(imd, i, j, PnmBlue, res[i*width+j]);
    }
  }

  free(my_fft_plane1);
  free(my_fft_plane2);
  free(as);
  free(ps);
  free(res);

  pnm_save(as_repr, PnmRawPpm, "as_img.ppm");
  pnm_save(ps_repr, PnmRawPpm, "ps_img.ppm");
  pnm_save(imd, PnmRawPpm, imd_name);

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
