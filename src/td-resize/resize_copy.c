#include <stdlib.h>
#include <stdio.h>

#include <pnm.h>

void  
process(int coeff, char* ims_name, char* imd_name){
  pnm ims = pnm_load(ims_name);
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  pnm imd = pnm_new(width * coeff, height * coeff, PnmRawPpm);

  for(int y=0; y<height * coeff; y+=coeff)
    for(int x=0; x<width * coeff; x+=coeff)
      for(int cptY=0; cptY<coeff; cptY++)
	for(int cptX=0; cptX<coeff; cptX++){
	  pnm_set_component(imd, y + cptY, x + cptX, PnmRed, pnm_get_component(ims, y/coeff, x/coeff, PnmRed));
	  pnm_set_component(imd, y + cptY, x + cptX, PnmGreen, pnm_get_component(ims, y/coeff, x/coeff, PnmGreen));
	  pnm_set_component(imd, y + cptY, x + cptX, PnmBlue, pnm_get_component(ims, y/coeff, x/coeff, PnmBlue));
	}
  
  
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
  pnm_free(ims);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <coeff> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int 
main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);  
  int coeff = atoi(argv[1]);
  char *ims_name = argv[2];
  char *imd_name = argv[3];
  process(coeff, ims_name, imd_name);
  return EXIT_SUCCESS;
}