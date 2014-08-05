#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>
#include <math.h>



/**
 * @file color_transfert
 * @brief transfert color from reference image to source image.
 *        Method from Reinhard et al. : 
 *        Erik Reinhard, Michael Ashikhmin, Bruce Gooch and Peter Shirley, 
 *        'Color Transfer between Images', IEEE CGA special issue on 
 *        Applied Perception, Vol 21, No 5, pp 34-41, September - October 2001
 */

#define D 3

static float RGB2LMS[D][D] = {
  {0.3811, 0.5783, 0.0402}, 
  {0.1967, 0.7244, 0.0782},  
  {0.0241, 0.1288, 0.8444}
};

static float LMS2RGB[D][D] = {
  {4.4679,  -3.5873, 0.1193},
  {-1.2186, 2.3809,  -0.1624},
  {0.0497,  -0.2439, 1.2045}
};

void rgb_to_lms(unsigned short r, unsigned short g, unsigned short b, float * l, float * m, float * s){
  
    *l = RGB2LMS[0][0] * r + RGB2LMS[1][0] * g + RGB2LMS[2][0] * b;
    *m = RGB2LMS[0][1] * r + RGB2LMS[1][1] * g + RGB2LMS[2][1] * b;
    *s = RGB2LMS[0][2] * r + RGB2LMS[1][2] * g + RGB2LMS[2][2] * b;
}

void lms_to_rgb(float l, float m, float s, unsigned short * r, unsigned short * g,  unsigned short * b){
  
  *r = (unsigned short) LMS2RGB[0][0] * l + LMS2RGB[1][0] * m + LMS2RGB[2][0] * s;
  *g = (unsigned short) LMS2RGB[0][1] * l + LMS2RGB[1][1] * m + LMS2RGB[2][1] * s;
  *b = (unsigned short) LMS2RGB[0][2] * l + LMS2RGB[1][2] * m + LMS2RGB[2][2] * s;
}

void process(char* ims_name, char* imt_name, char* imd_name){
  
  pnm ims = pnm_load(ims_name);
  pnm imt = pnm_load(imt_name);
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);

  unsigned short * image_r = (unsigned short *) malloc(width * height * sizeof(unsigned short));
  unsigned short * image_g = (unsigned short *) malloc(width * height * sizeof(unsigned short));
  unsigned short * image_b = (unsigned short *) malloc(width * height * sizeof(unsigned short));
  image_r = pnm_get_channel(ims, image_r, PnmRed);
  image_g = pnm_get_channel(ims, image_g, PnmGreen);
  image_b = pnm_get_channel(ims, image_b, PnmBlue);

  float * image_l = (float *) malloc(width * height * sizeof(float));
  float * image_m = (float *) malloc(width * height * sizeof(float));
  float * image_s = (float *) malloc(width * height * sizeof(float));
 

  /* passage du repere RGB au repere LMS */
  for(int j=0; j<height; j++){
    for(int i=0; i<width; i++){
      rgb_to_lms(image_r[i+j*width], image_g[i+j*width], image_b[i+j*width], &image_l[i+j*width], &image_m[i+j*width], &image_s[i+j*width]);
    }
  }

  unsigned short * image2_r = (unsigned short *) malloc(width * height * sizeof(unsigned short));
  unsigned short * image2_g = (unsigned short *) malloc(width * height * sizeof(unsigned short));
  unsigned short * image2_b = (unsigned short *) malloc(width * height * sizeof(unsigned short));

  /* passage du repere LMS au repere RGB */
  for(int j=0; j<height; j++){
    for(int i=0; i<width; i++){
      lms_to_rgb(image_l[i+j*width], image_m[i+j*width], image_s[i+j*width], &image2_r[i+j*width], &image2_g[i+j*width], &image2_b[i+j*width]);
    }
  }
  
  pnm_set_channel(imd, image2_r, PnmRed);
  pnm_set_channel(imd, image2_g, PnmGreen);
  pnm_set_channel(imd, image2_b, PnmBlue);

  pnm_save(imd, PnmRawPpm, imd_name);

}

/**
 * @brief usage function : 
 *        <ims>: the source image as reference
 *        <imt>: the target image to maodify
 *        <imd>: the result
 */
void usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define param 3

int main(int argc, char ** argv){
  if (argc != param+1) usage(argv[0]);
  char *ims_name = argv[1];
  char *imt_name = argv[2];
  char *imd_name = argv[3];
  process(ims_name, imt_name, imd_name);
  return EXIT_SUCCESS;
}
