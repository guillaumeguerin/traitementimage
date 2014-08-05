#include <stdlib.h>
#include <stdio.h>

#include <pnm.h>

void  
process(int min, int max, char* ims_name, char* imd_name){

  pnm ims = pnm_load(ims_name);
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);

  unsigned short r,g,b,min_r,min_g,min_b,max_r,max_g,max_b;

  min_r = 255;
  min_g = 255;
  min_b = 255;
  max_r = 0;
  max_g = 0;
  max_b = 0;

  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      for(int z=0; z<3; z++){
	r = pnm_get_component(ims, y, x, PnmRed);
	g = pnm_get_component(ims, y, x, PnmGreen);
	b = pnm_get_component(ims, y, x, PnmBlue);
	
	if(r < min_r){
	  min_r = r;
	}
	
	if(g < min_g){
	  min_g = g;
	}

	if(b < min_b){
	  min_b = b;
	}

	if(r > max_r){
	  max_r = r;
	}
	
	if(g > max_g){
	  max_g = g;
	}

	if(b > max_b){
	  max_b = b;
	}
      }
    }
  }

  unsigned short normalize_r, normalize_g, normalize_b;

  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      for(int z=0; z<3; z++){

	normalize_r = (max - min) / (max_r - min_r) * pnm_get_component(ims, y, x, PnmRed) + (min * max_r - max * min_r) / (max_r - min_r);

	normalize_g = (max - min) / (max_g - min_g) * pnm_get_component(ims, y, x, PnmGreen) + (min * max_g - max * min_g) / (max_g - min_g);
	
	normalize_b = (max - min) / (max_b - min_b) * pnm_get_component(ims, y, x, PnmBlue) + (min * max_b - max * min_b) / (max_b - min_b);
	
	pnm_set_component(imd, y, x, PnmRed, normalize_r);
	pnm_set_component(imd, y, x, PnmGreen, normalize_g);
       	pnm_set_component(imd, y, x,PnmBlue, normalize_b);
	
      }
    }
  }
  
  printf("****** valeurs min r g b ****** \n");
  printf("%u \n", min_r);
  printf("%u \n", min_g);
  printf("%u \n", min_b);
  printf("****** valeurs max r g b ****** \n");
  printf("%u \n", max_r);
  printf("%u \n", max_g);
  printf("%u \n", max_b);
  printf("****** valeurs normalize r g b ****** \n");
  printf("%u \n", normalize_r);
  printf("%u \n", normalize_g);
  printf("%u \n", normalize_b);

  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);
  
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <min> <max> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 4
int 
main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);  
  int min = atoi(argv[1]);
  int max = atoi(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];
  process(min, max, ims_name, imd_name);
  return EXIT_SUCCESS;
}
