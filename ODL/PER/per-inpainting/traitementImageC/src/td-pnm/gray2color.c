#include <stdlib.h>
#include <stdio.h>

#include <pnm.h>

void  
process(char* ims0_name, char* ims1_name, char* ims2_name, char* imd_name){

  pnm ims0 = pnm_load(ims0_name);
  pnm ims1 = pnm_load(ims1_name);
  pnm ims2 = pnm_load(ims2_name);
  int height = pnm_get_height(ims0);
  int width = pnm_get_width(ims0);
  pnm imd = pnm_new(width, height, PnmRawPpm);

  unsigned short r,g,b;

  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      for(int z=0; z<3; z++){
	r = pnm_get_component(ims0, y, x, PnmRed);
	g = pnm_get_component(ims1, y, x, PnmGreen);
	b = pnm_get_component(ims2, y, x, PnmBlue);
	pnm_set_component(imd, y, x, PnmRed, r);
	pnm_set_component(imd, y, x, PnmGreen, g);
       	pnm_set_component(imd, y, x,PnmBlue, b);
	
      }
    }
  }
  
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims0);
  pnm_free(ims1);
  pnm_free(ims2);
  
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <ims0> <ims1> <ims2> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 4
int 
main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);  
  char *ims0_name = argv[1];
  char *ims1_name = argv[2];
  char *ims2_name = argv[3];
  char *imd_name = argv[4];
  process(ims0_name, ims1_name, ims2_name, imd_name);
  return EXIT_SUCCESS;
}
