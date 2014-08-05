#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pnm.h>

float gaussian(float sigma, float k) {
  return exp(-k*k/(2*sigma*sigma));
}

void process(char* ims_name, char* imd_name, int sigma_s, int sigma_g) {
  
  pnm ims = pnm_load(ims_name);
  int width  = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);

  unsigned short * image = (unsigned short *) malloc(height * width * sizeof(unsigned short));
  float * res = (float *) malloc(height * width * sizeof(float));
  float * tmp = (float *) malloc(height * width * sizeof(float));

  image = pnm_get_channel(ims, image, PnmRed);
  for(int i=0; i<width*height; i++){
    res[i] = (float) image[i];
    tmp[i] = (float) image[i];
  }

  float numerator, denominator, tmp2;

  for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++) {
      denominator = 0;
      numerator = 0;
      for(int j=-sigma_s; j < sigma_s; j++)
	for(int i=-sigma_s; i < sigma_s; i++) {
	  if((i+x) < width  && (x+i) > 0 && (j+y) < height && (y+j) > 0) {
	    tmp2 = gaussian(sigma_s, hypotf(i,j)) * gaussian(sigma_g, abs(tmp[x+y*width]-tmp[(x+i)+(y+j)*width]));
	    numerator += tmp2*tmp[(x+i)+(y+j)*width];
	    denominator += tmp2;
	  }
	}
      res[x+y*width] = numerator/denominator;
    }

  for(int y = 0; y < height; y++)
     for(int x = 0; x < width; x++) 
       for(int z = 0; z < 3; z++)
	 pnm_set_component(imd,y,x,z,res[y*height+x]);

  pnm_save(imd, PnmRawPpm, imd_name);

  free(image);
  free(res);
  free(tmp);
}

void usage (char *s){
  fprintf(stderr, "Usage: %s <sigma_s> <sigma_g> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 4
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);
  float sigma_s =(float) atoi(argv[1]);
  float sigma_g =(float) atoi(argv[2]);
  char* ims_name = argv[3];
  char* imd_name = argv[4];
  
  process(ims_name, imd_name, sigma_s, sigma_g); 
  
  return EXIT_SUCCESS;
}
