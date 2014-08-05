#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void process(int r, int g, int b, int w, int h){
  printf("P3\n%d %d\n255\n",w,h);
  int i, j;
  for(j=0; j<h; j++){
    for(i=0; i<w; i++){
      printf("%d %d %d ",r,g,b);
    }
    printf("\n");
  }
}



void usage(char * s){
  char * use = "Usage: %s <r[0,255]> <g[0,255]> <b[0,255]> <width> <height> <name>\n";
  fprintf(stderr, use, s);
  exit(EXIT_FAILURE);
}

#define param 5

int main(int argc, char * argv[]){
  if(argc != (param+1)) usage(argv[0]);
  int r=atoi(argv[1]), g=atoi(argv[2]), b=atoi(argv[3]), w=atoi(argv[4]), h=atoi(argv[5]);
  process(r,g,b,w,h);

  return EXIT_SUCCESS;
}
