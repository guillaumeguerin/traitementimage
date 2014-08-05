#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

#include "mm.h"

void  
process(int min, int max, char* ims_name, char* imd_name){
  pnm ims = pnm_load(ims_name);
  pnm imd = pnm_new(pnm_get_width(ims), pnm_get_height(ims), PnmRawPpm);
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  
  unsigned short * image = (unsigned short *) malloc(height * width * sizeof(unsigned short));
  unsigned short * image2 = (unsigned short *) malloc(height * width * sizeof(unsigned short));

  image = pnm_get_channel(ims, image, PnmRed);
  
  int tmp;
  for(int j=0; j<height; j++)
    for(int i=0; i<width; i++){
      tmp = image[i+j*width];
      if(tmp >= min && tmp <= max)	
	image2[i+j*width] = 255;
      else
	image2[i+j*width] = 0;
    }
 
  pnm_set_channel(imd, image2, PnmRed);
  pnm_set_channel(imd, image2, PnmGreen);
  pnm_set_channel(imd, image2, PnmBlue);

  pnm_save(imd, PnmRawPpm, imd_name);  
}

void usage (char *s){
  fprintf(stderr, "Usage: %s <min> <max> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 4
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);
  int min = atoi(argv[1]);
  int max = atoi(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];
  process(min, max, ims_name, imd_name);
 
  return EXIT_SUCCESS;
}
