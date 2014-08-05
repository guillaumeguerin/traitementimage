#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

#include "mm.h"

void  
process(char* ima_name, char* imb_name, char* imd_name){
  pnm ima = pnm_load(ima_name);
  pnm imb = pnm_load(imb_name);
  pnm imd = pnm_new(pnm_get_width(ima), pnm_get_height(ima), PnmRawPpm);
  int height = pnm_get_height(ima);
  int width = pnm_get_width(ima);
  
  unsigned short * image = (unsigned short *) malloc(height * width * sizeof(unsigned short));
  unsigned short * image2 = (unsigned short *) malloc(height * width * sizeof(unsigned short));
  unsigned short * image3 = (unsigned short *) malloc(height * width * sizeof(unsigned short));

  image = pnm_get_channel(ima, image, PnmRed);
  image2 = pnm_get_channel(imb, image2, PnmRed);
  
  int tmp;
  for(int j=0; j<height; j++)
    for(int i=0; i<width; i++){
      tmp = image[i+j*width]-image2[i+j*width];
      if(tmp < 0)
	tmp = 0;
      image3[i+j*width] = tmp;
    }
 
  pnm_set_channel(imd, image3, PnmRed);
  pnm_set_channel(imd, image3, PnmGreen);
  pnm_set_channel(imd, image3, PnmBlue);

  pnm_save(imd, PnmRawPpm, imd_name);  
}

void usage (char *s){
  fprintf(stderr, "Usage: %s <ima> <imb> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);
  char *ima_name = argv[1];
  char *imb_name = argv[2];
  char *imd_name = argv[3];
  process(ima_name, imb_name, imd_name);
 
  return EXIT_SUCCESS;
}
