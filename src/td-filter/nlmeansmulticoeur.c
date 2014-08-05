#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pnm.h>

float gaussian(float sigma, float k) {
  return exp(-k*k/(2*sigma*sigma));
}
/*
float euclidian_distance(float xA, float yA, float xB, float yB, int filter_size, float * img, int width){
  float tmp;
  int n2 = filter_size * filter_size;
  tmp=0;
  for(int i=0; i<n2; i++){
    tmp += (img[xA+yA*width] - img[xB+yB*width]) * (img[xA+yA*width] - img[xB+yB*width]);
  }
  return 1 / n2 * tmp;
  }*/

void process(char* ims_name, char* imd_name, int sigma) {
  
  pnm ims = pnm_load(ims_name);
  int width  = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);

  int halfsize = 1;
  int filter_size = 2*halfsize + 1;
  int n2 = filter_size * filter_size;


  unsigned short * image = (unsigned short *) malloc(height * width * sizeof(unsigned short));
  float * res = (float *) malloc(height * width * sizeof(float));
  float * tmp = (float *) malloc(height * width * sizeof(float));

  image = pnm_get_channel(ims, image, PnmRed);
  for(int i=0; i<width*height; i++){
    res[i] = (float) image[i];
    tmp[i] = (float) image[i];
  }

  float c, c2, euclidian_distance, weight;
  
  for(int j=0; j<height; j++){
    for(int i=0; i<width; i++){ //Pour tout pixel p de l'image
      
      c = 0;
      c2 = 0;

        for(int l=0; l<height; l++){
	  for(int k=0; k<width; k++){ //Pour tout pixel q de l'image
	    
	    if(i-halfsize > 0 && i+halfsize < width && j-halfsize > 0 && j+halfsize < height &&
	       k-halfsize > 0 && k+halfsize < width && l-halfsize > 0 && l+halfsize < height){
	      
	      //c += euclidian_distance(i,j,k,l,filter_size,tmp,width);
	      euclidian_distance = 0;
	      
 	      for(int n=-halfsize; n<halfsize; n++){
		for(int m=-halfsize; m<halfsize; m++){
		  euclidian_distance += (tmp[(i+n)+(j+m)*width] - tmp[(k+n)+(l+m)*width]) * (tmp[(i+n)+(j+m)*width] - tmp[(k+n)+(l+m)*width]);
		}
	      }
	      euclidian_distance = 1/n2 * euclidian_distance;
	      weight = gaussian(sigma, euclidian_distance);

	      c2 += weight;
	      c += weight * tmp[k+l*width];
	      //weight_sum = euclidian_distance(i,j,k,l,filter_size,tmp,width);
	    }
	    
	  }
	  //printf("ligneQ : %d \n", l);
	}

	res[i+j*width] = (float) 1/c * c2;

    }
    printf("ligneP : %d \n", j);
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
  fprintf(stderr, "Usage: %s <sigma> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);
  float sigma =(float) atoi(argv[1]);
  char* ims_name = argv[2];
  char* imd_name = argv[3];
  
  process(ims_name, imd_name, sigma); 
  
  return EXIT_SUCCESS;
}
