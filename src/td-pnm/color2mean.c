#include <stdlib.h>
#include <stdio.h>

#include <pnm.h>

unsigned short
moyenne_channel(signed short r, signed short g, signed short b){
  return (r + g + b)/3;
}

void  
process(char* ims_name, char* imd_name){

  pnm ims = pnm_load(ims_name);
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);

  unsigned short r,g,b,m;

  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      for(int z=0; z<3; z++){
	r = pnm_get_component(ims, y, x, PnmRed);
	g = pnm_get_component(ims, y, x, PnmGreen);
	b = pnm_get_component(ims, y, x, PnmBlue);

	m = moyenne_channel(r,g,b);

	pnm_set_component(imd, y, x, PnmRed, m);
	pnm_set_component(imd, y, x, PnmGreen, m);
       	pnm_set_component(imd, y, x,PnmBlue, m);
      }
    }
  }
  
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);
  
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 2
int 
main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);  
  char *ims_name = argv[1];
  char *imd_name = argv[2];
  process(ims_name, imd_name);
  return EXIT_SUCCESS;
}
