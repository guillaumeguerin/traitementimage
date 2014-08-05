#include <stdlib.h>
#include <stdio.h>

#include <pnm.h>

void  
process(int x0, int y0, int width, int height, char* ims_name, char* imd_name){

  pnm imd = pnm_new(width, height, PnmRawPpm);
  pnm ims = pnm_load(ims_name);

  unsigned short r,g,b;

  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      for(int z=0; z<3; z++){	
	r = pnm_get_component(ims, y+y0, x+x0,PnmRed);
	g = pnm_get_component(ims, y+y0, x+x0,PnmGreen);
	b = pnm_get_component(ims, y+y0, x+x0,PnmBlue);
	pnm_set_component(imd,y,x,PnmRed, r);
	pnm_set_component(imd,y,x,PnmGreen, g);
       	pnm_set_component(imd,y,x,PnmBlue, b);
	
      }
    }
  }
  
  
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);
  
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <x0> <y0> <w> <h> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 6
int 
main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);  
  int x0 = atoi(argv[1]);
  int y0 = atoi(argv[2]);
  int w = atoi(argv[3]);
  int h = atoi(argv[4]);
  char *ims_name = argv[5];
  char *imd_name = argv[6];
  process(x0, y0, w, h, ims_name, imd_name);
  return EXIT_SUCCESS;
}
