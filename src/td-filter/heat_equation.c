#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>
#include <string.h>

#define TEMPORAL_DISCRETIZATION 0.25

void  
process(int n, char* ims_name, char* imd_name){
  pnm ims = pnm_load(ims_name);
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);
  unsigned short * image = (unsigned short *) malloc(height * width * sizeof(unsigned short));
  float * res = (float *) malloc(height * width * sizeof(float));
  float * tmp = (float *) malloc(height * width * sizeof(float));

  image = pnm_get_channel(ims, image, PnmRed);
  for(int i=0; i<width*height; i++){
    res[i] = (float) image[i];
    tmp[i] = (float) image[i];
  }

  float up, down, left, right, laplace;

  for(int c=0; c<n; c++){
    for(int j=0; j<height; j++){
      for(int i=0; i<width; i++){
	if(i == 0)
	  left = (float)tmp[i + j*width];
	else
	  left = (float)tmp[i-1 + j*width];
	if(i == width-1)
	  right = (float)tmp[i + j*width];
	else
	  right = (float)tmp[i+1 + j*width];
	if(j == 0)
	  up = (float)tmp[i + j*width];
	else
	  up = (float)tmp[i + (j-1)*width];
	if(j == height-1)
	  down = (float)tmp[i + j*width];
	else
	  down = (float)tmp[i + (j+1)*width];
	
	laplace = up + down + left + right - (4 * tmp[i + j*width]);
	res[i + j*width] = tmp[i + j*width] + (TEMPORAL_DISCRETIZATION * laplace);
      }
    }
    memcpy(tmp, res, sizeof(float) * height * width);
  }
  
  for(int y = 0; y<height; y++)
     for(int x = 0; x<width; x++) 
       for(int z = 0; z<3; z++)
       pnm_set_component(imd,y,x,z,res[y*height+x]);

  pnm_save(imd, PnmRawPpm, imd_name); 

  free(image);
  free(res);
}

void usage (char *s){
  fprintf(stderr, "Usage: %s <n> <imb> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);
  int n = atoi(argv[1]);
  char *ims_name = argv[2];
  char *imd_name = argv[3];
  process(n, ims_name, imd_name);
 
  return EXIT_SUCCESS;
}
