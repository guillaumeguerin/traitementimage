#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pnm.h>
#include "mm.h"

pnm 
mm_se(int shape, int halfsize){
  
  int size = 2*halfsize+1;
  pnm imd = pnm_new(size, size, PnmRawPpm);
  unsigned short * image = (unsigned short *) malloc(size * size * sizeof(unsigned short));
  
  float norme;
  int cpt;

  switch(shape){
  case 0:

    for(int j=0; j<size; j++)
      for(int i=0; i<size; i++)
	image[i+j*size]=255;
    break;

  case 1:

    cpt=0;
    for(int j=0; j<size; j++){
      for(int i=0; i<size; i++){
	
	if(j<=halfsize){
	  if(i<=halfsize+cpt && i>=halfsize-cpt)
	    image[i+j*size]=255;
	  else
	    image[i+j*size]=0;
	}
	
	if(j>halfsize){
	  if(i<=halfsize+cpt && i>=halfsize-cpt)
	    image[i+j*size]=255;
	  else
	    image[i+j*size]=0; 
	  }
      }
      if(j<halfsize)
	cpt++;
      else
	cpt--;
    }
    break;

  case 2:
    
    for(int j=0; j<size; j++)
      for(int i=0; i<size; i++){
	norme = sqrt(i*i + j*j);
	if(norme < halfsize){
	  image[(i+halfsize)+((j+halfsize)*size)]=255;
	  image[(-i+halfsize)+((-j+halfsize)*size)]=255;
	  image[(-i+halfsize)+((j+halfsize)*size)]=255;
	  image[(i+halfsize)+((-j+halfsize)*size)]=255;
	}
      }
    break;
  } 

  pnm_set_channel(imd, image, PnmRed);
  pnm_set_channel(imd, image, PnmGreen);
  pnm_set_channel(imd, image, PnmBlue);

  free(image);
  return imd;
}

void
mm_process(int shape, int halfsize, pnm ims, pnm imd, void (*pf)(unsigned short, unsigned short*)){
  
  pnm filter = mm_se(shape, halfsize);
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  int size = 2*halfsize+1;

  unsigned short * image = (unsigned short *) malloc(width * height * sizeof(unsigned short));
  unsigned short * image2 = (unsigned short *) malloc(width * height * sizeof(unsigned short));
  unsigned short * image_filter = (unsigned short *) malloc(size * size * sizeof(unsigned short));
  image = pnm_get_channel(ims, image, PnmRed);
  image_filter = pnm_get_channel(filter, image_filter, PnmRed);

  unsigned short val;
  int i2, j2;

  for(int j=0; j<height; j++)
    for(int i=0; i<width; i++){
      
      val = image[i+j*width];
      for(int l=0; l<size; l++){
	for(int k=0; k<size; k++){
	  if(image_filter[k+l*size] == 255){
	    i2 = i-halfsize+k;
	    j2 = j-halfsize+l;
	    /* on s'assure que la valeur a modifier n'est pas en dehors de l'image */
	    if(i2 >= 0 && j2 >= 0 && i2 < width && j2 < height){
	      pf(image[i2+width*j2],&val);
	    }
	  }
	}
      }
      image2[i+j*width] = val;
    }
  
  pnm_set_channel(imd, image2, PnmRed);
  pnm_set_channel(imd, image2, PnmGreen);
  pnm_set_channel(imd, image2, PnmBlue);
  
  free(image);
  //free(image2);
  free(image_filter);

}

void
mm_greater(unsigned short val, unsigned short* max){
    if(*max < val)
      *max = val;
}

void
mm_lesser(unsigned short val, unsigned short* min){
  if(*min > val)
    *min = val;
}
