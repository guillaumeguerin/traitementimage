#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

#include "mm.h"

int 
main(int argc, char *argv[]){

  pnm imf = mm_se(2, 20);
  pnm ims = pnm_load("lena.ppm");
  pnm imd = pnm_new(pnm_get_width(ims), pnm_get_height(ims), PnmRawPpm);
  mm_process(1, 10, ims, imd, mm_greater);
  pnm_save(imd, PnmRawPpm, "lena_test.ppm");
  return EXIT_SUCCESS;
}
