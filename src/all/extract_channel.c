#include <stdlib.h>
#include <stdio.h>

#include <pnm.h>

void  
process(int num, char* ims_name, char* imd_name){

  pnm ims = pnm_load(ims_name);
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);

  unsigned short r,g,b;

  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      for(int z=0; z<3; z++){
	switch (num){
	case 1:
	  r = pnm_get_component(ims, y, x,PnmRed);
	  pnm_set_component(imd,y,x,PnmRed, r);
	  pnm_set_component(imd,y,x,PnmGreen, 0);
	  pnm_set_component(imd,y,x,PnmBlue, 0);
	  break;
	
	case 2:
	  g = pnm_get_component(ims, y, x,PnmGreen);
	  pnm_set_component(imd,y,x,PnmRed, 0);
	  pnm_set_component(imd,y,x,PnmGreen, g);
	  pnm_set_component(imd,y,x,PnmBlue, 0);
	  break;
	
	case 3:
	  b = pnm_get_component(ims, y, x,PnmBlue);
	  pnm_set_component(imd,y,x,PnmRed, 0);
	  pnm_set_component(imd,y,x,PnmGreen, 0);
	  pnm_set_component(imd,y,x,PnmBlue, b);
	  break;
	}
	
      }
    }
  }
  
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);
  
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <num> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int 
main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);  
  int num = atoi(argv[1]);
  char *ims_name = argv[2];
  char *imd_name = argv[3];
  process(num, ims_name, imd_name);
  return EXIT_SUCCESS;
}
