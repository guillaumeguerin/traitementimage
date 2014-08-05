#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

#include "mm.h"

void usage (char *s){
  fprintf(stderr, "Usage: %s <se> <hs> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 4
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);
  int se = atoi(argv[1]);
  int hs = atoi(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];

  pnm ims = pnm_load(ims_name);
  pnm imd = pnm_new(pnm_get_width(ims), pnm_get_height(ims), PnmRawPpm);
  pnm imd2 = pnm_new(pnm_get_width(ims), pnm_get_height(ims), PnmRawPpm);

  mm_process(se, hs, ims, imd, mm_lesser);
  mm_process(se, hs, imd, imd2, mm_greater);

  pnm_save(imd2, PnmRawPpm, imd_name);
  return EXIT_SUCCESS;
}
