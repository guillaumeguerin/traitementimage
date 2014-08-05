#include <bcl.h>
#include <pnm.h>
#include <fft.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define FILTRE_ORDRE 2
#define PARAMETRES_NUMBER 3

void usage (char *s){
  fprintf(stderr, "Usage: %s  <ims> <imd>  <filtre> <d0>  <v0>  <u0>\n", s);
  exit(EXIT_FAILURE);
}

float low_pass(int d0, int ordre_n, float d_u_v) {
  return 1/(1 +  pow((d_u_v/d0), 2*ordre_n));
}

float high_pass(int d0, int ordre_n, float d_u_v) {
  return 1/(1 +  pow((d0/d_u_v), 2*ordre_n));
}

float bande_rejet(int d0, int ordre_n, float d_u_v, int band_width) {
  return 1/(1 + pow((d_u_v*band_width/(d_u_v*d_u_v - d0*d0 )), 2*ordre_n));
}

float pass_bande(int d0, int ordre_n, float d_u_v, int band_width){
  return 1 - (1/(1 + pow((d_u_v*band_width/(d_u_v*d_u_v - d0*d0 )), 2*ordre_n)));
}

float rejet_encoche(int d0, int ordre_n, float u, float v, int u0, int v0) {

    float d1 = hypot((u - u0), (v - v0));
    float d2 = hypot((u + u0), (v + v0));
    
  return 1/(1 + pow((d0*d0)/(d1*d2), 2*ordre_n ));
}

void  process(pnm ims,pnm imd, /*float(*filtre_ptr)(float,float)*/ int filtre_num, int filtre_ordre_n, int d0, int u0, int v0, int w,int h){

    int size_img = w*h;

    fftw_complex* img_freq_tmp = malloc(size_img*sizeof(fftw_complex));
    fftw_complex* img_freq_res = malloc(size_img*sizeof(fftw_complex));
    fftw_complex*  img_buffer_complex = malloc(size_img*sizeof(fftw_complex));
    
    //float* img_float_tmp = malloc(size_img*sizeof(float));
    unsigned char* img_float_tmp = malloc(size_img*sizeof( unsigned char));
    
    //float* img_float_res = malloc(size_img*sizeof(float));
    unsigned char* img_float_res = malloc(size_img*sizeof(unsigned char));
    float* img_buffer = malloc(size_img*sizeof(float));
    
    float filtre_res, d_u_v;
    
    float* spectr = malloc(size_img*sizeof(float));
    float* amplitude = malloc(size_img*sizeof(float));
    
    float* spectr_res = malloc(size_img*sizeof(float));
    float* amplitude_res = malloc(size_img*sizeof(float));
    float* amplitude_buf = malloc(size_img*sizeof(float));
    
    img_float_tmp = pnm_get_channel(ims, pnm_get_image(ims),0);
    img_freq_tmp = fft_forward(w,h, img_float_tmp);
    img_freq_res = fft_forward(w,h, img_float_tmp);
    

int halfx = w / 2;
int   halfy = h / 2;
int x, y, y1, x1;

fft_fr_to_spectra( w,  h, img_freq_tmp, amplitude, spectr);
fft_fr_to_spectra( w,  h, img_freq_tmp, amplitude_res, spectr);

/*for (int y = 0; y < h; y++){
        for(int x = 0; x< w; x++){
            
              d_u_v = hypot(x,y);        
            filtre_res = low_pass(d0, filtre_ordre_n, d_u_v);
            
           // printf("%f, ", filtre_res);
            
           // amplitude_res[y*w + x] = amplitude_res[y*w + x]/filtre_res;      
            spectr_res[y*w + x] = spectr_res[y*w + x]*filtre_res;         
        }
    }
    fft_spectra_to_fr( w,h, amplitude,  spectr_res, img_freq_res);
*/
   /* Loop over Y axis */
  
  /*float MinVal = 9999;
      for (y = 0; y < h; y++)
	 for (x = 0; x < w; x++)
	    if (MinVal > creal(img_freq_tmp[y*w + x]))
	       MinVal = creal(img_freq_tmp[y*w + x]);
      for (y = 0; y < h; y++)
	 for (x = 0; x < w; x++)
	   img_freq_res[y*w + x] = (float) log((double) (1.0 + creal(img_freq_tmp[y*w + x]) - MinVal)) + I*cimag(img_freq_tmp[y*w + x]);
	 */  
	   
   /*for (y = 0; y < h; y++)
   {
      if (y < halfy)
	 y1 = y;
      else
	 y1 = y - h;


      for (x = 0; x < w; x++)
      {
	 if (x < halfx)
	    x1 = x;
	 else
	    x1 = x - w;

        d_u_v = hypot(x1,y1);        
            filtre_res = low_pass(d0, filtre_ordre_n, d_u_v);
            img_freq_res[y*w + x] = creal(img_freq_tmp[y*w + x])/filtre_res + I*cimag(img_freq_tmp[y*w + x])/filtre_res;
      }
   }*/
 //for(int i = 0; i< 1; i++){
    for (int y = 0; y < h; y++){
        for(int x = 0; x< w; x++){
            
              d_u_v  = sqrt((float)(y-h/2)*(y-h/2)+(float)(x-w/2)*(x-w/2));       
            filtre_res = low_pass(d0, filtre_ordre_n, d_u_v);
            
            //img_freq_res[y*w + x] = creal( img_freq_tmp[y*w + x]) + creal(img_freq_tmp[y*w + x])*filtre_res + I*(cimag(img_freq_tmp[y*w + x])+ cimag(img_freq_tmp[y*w + x])*filtre_res);  
            
            amplitude_res[y*w + x] = amplitude_res[y*w + x]*filtre_res;             
        }
    //}
         
       /* memcpy(img_buffer_complex, img_freq_tmp, size_img*sizeof(complex));
        memcpy(img_freq_tmp, img_freq_res, size_img*sizeof(complex));
        memcpy(img_freq_res, img_buffer_complex, size_img*sizeof(complex));*/
        
        /* memcpy(amplitude_buf, amplitude, size_img*sizeof(complex));
        memcpy(amplitude, amplitude_res ,size_img*sizeof(complex));
        memcpy(amplitude_res, amplitude_buf, size_img*sizeof(complex));*/
  }
  
   fft_spectra_to_fr( w,h, amplitude_res,  spectr, img_freq_res);
   
       img_float_res = fft_backward(w, h, img_freq_res ); 
       
       unsigned short e;
       
     for(int y = 0; y<h; y++)
     for(int x = 0; x<w; x++) 
       {
		  e=(unsigned short)img_float_res[y*w*3+x*3]; 
       pnm_set_component(imd,y,x,PnmRed,(unsigned short)e);
        pnm_set_component(imd,y,x,PnmGreen,(unsigned short)e);
         pnm_set_component(imd,y,x,PnmBlue,(unsigned short)e);
	 }
    
     pnm_save(imd, PnmRawPpm,"low_pas.ppm");  
    
   /* free(img_float_tmp);
    free(img_float_res);
    free(img_buffer);*/
}

int main(int argc, char *argv[]){

if (argc != PARAMETRES_NUMBER+1) 
    printf(argv[0]);

     char* ims_name = argv[1];
       char* imd_name = argv[2];

  
  pnm ims = pnm_load(ims_name);
  
  int w = pnm_get_width(ims);
  int h = pnm_get_height(ims);    
  

  pnm imd = pnm_new(w,h, PnmRawPpm);
    
 /* float  (*function_ptr)(float, float);
  int num_iter = atoi(argv[1]);
  int lambda = atoi(argv[2]);
  
  if (!strcmp(func_name, "c1")){
    function_ptr = c1;
  }
  else  if (!strcmp(func_name, "c2")){
    function_ptr = c2;
  }
  else  if (!strcmp(func_name, "c3")){
    function_ptr = c3;
  }
  */
  

       int filtre_num   = atoi(argv[3]); 
   int d0_param = atoi(argv[4]);

  
  process( ims, imd, filtre_num, FILTRE_ORDRE, d0_param,  0,  0, w, h);
  
  //pnm_save(imd, PnmRawPpm, imd_name);    
  
  pnm_free(imd);
  pnm_free(ims);
 
  return EXIT_SUCCESS;
}

