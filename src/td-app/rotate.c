#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void
process(char* ims_name, char* imd_name){
  //char * cmd = "./pre-op.sh ";
  //strcat(cmd, ims_name);
  system("sh pre-op.sh");
  


}

void usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 2
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);
  char *ims_name = argv[1];
  char *imd_name = argv[2];
  process(ims_name, imd_name);
 
  return EXIT_SUCCESS;
}
