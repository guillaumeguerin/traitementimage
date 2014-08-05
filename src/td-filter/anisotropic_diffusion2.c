#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>
#include <math.h>

#define param 5

void usage (char *s){
  fprintf(stderr, "Usage: %s <min> <max> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

float c0(float i,float lambda){
  return 1.0;
}

float c1(float i, float lambda){
  return 1/ (1 + (i/lambda)*(i/lambda));
}

float c2(float i, float lambda){
  float tmp = i/lambda;
  return exp(-(tmp*tmp));
}

float valeurGradiant1(int i, int j, int w, float* tab){
  return (tab[i+1+j*w]-tab[i+j*w]);
}

float valeurGradiant2(int i, int j, int w, float* tab){
  return (tab[i+(j+1)*w]-tab[i+j*w]);
}

float normeDelta(float val1, float val2){
  return sqrt(val1*val1+val2*val2);
}

float dive(int i, int j, int w, float* p1, float *p2){
  float res=p1[i+j*w];
  if(i!=0){
    res-=p1[i-1+j*w];
  }
  else{
    res-=p1[i+1+j*w];
  }
  res+=p2[i+j*w];
  if(j!=0){
    res-=p2[i+(j-1)*w];
  }
  else{
    res-=p1[i+(j+1)*w];
  }
  return res;
}


void process(int n, int lambda, char * func_name, char* ims, char* imd){
  pnm imgs=pnm_load(ims);
  int w=pnm_get_width(imgs);
  int h=pnm_get_height(imgs);
  int taille=w*h;
  
  pnm imgd=pnm_new(w,h,PnmRawPpm);
  unsigned short *tab_img=malloc(sizeof(unsigned short)*taille);
  float *tab_imgd=malloc(sizeof(float)*taille);
  float *tab_tmp=malloc(sizeof(float)*taille);
  
  float *grad=malloc(sizeof(float)*2);
  float *tab_grad_c=malloc(sizeof(float)*taille);
  float *tab_gradiant_X=malloc(sizeof(float)*taille);
  float *tab_gradiant_Y=malloc(sizeof(float)*taille);
  float gradNorme;

  tab_img=pnm_get_channel(imgs,tab_img,0);
  for(int i=0;i<taille;i++){
    tab_imgd[i]=(float)tab_img[i];
  }

  float r=0.25;
  
  for(int i=0;i<n;i++){
    for(int j=0;j<taille;j++){
      tab_tmp[j]=tab_imgd[j];
      }
    for(int x=0;x<w;x++){
      for(int y=0;y<h;y++){
	if(x!=w-1){
	  grad[0]=tab_tmp[x+1+y*w]-tab_tmp[x+y*w];
	}
	else{
	  grad[0]=tab_tmp[x-1+y*w]-tab_tmp[x+y*w];
	}
	if(y!=h-1){
	  grad[1]=tab_tmp[x+(y+1)*w]-tab_tmp[x+y*w];
	}
	else{
	  grad[1]=tab_tmp[x+(y-1)*w]-tab_tmp[x+y*w];
	}
	gradNorme=normeDelta(grad[0],grad[1]);
	tab_gradiant_X[x+y*w]=grad[0];
	tab_gradiant_Y[x+y*w]=grad[1];
	tab_grad_c[x+y*w]=pf(gradNorme,lambda);
      }
    }
    for(int x=0;x<w;x++){
      for(int y=0;y<h;y++){
	tab_gradiant_X[x+y*w]*=tab_grad_c[x+y*w];
	tab_gradiant_Y[x+y*w]*=tab_grad_c[x+y*w];
	tab_imgd[x+y*w]=tab_tmp[x+y*w]+r*dive(x,y,w,tab_gradiant_X,tab_gradiant_Y);
      }
    }
  }
  for(int i=0;i<taille;i++){
    tab_img[i]=(unsigned short)tab_imgd[i];
  }
  pnm_set_channel(imgd,tab_img,0);
  pnm_set_channel(imgd,tab_img,1);
  pnm_set_channel(imgd,tab_img,2);

  pnm_save(imgd, PnmRawPpm, imd);


  free(grad);
  free(tab_grad_c);
  free(tab_gradiant_X);
  free(tab_gradiant_Y);
  free(tab_tmp);
  free(tab_img);
  free(tab_imgd);
  pnm_free(imgs);
  pnm_free(imgd);
}



void usage (char *s){
  fprintf(stderr, "Usage: %s <n> <lambda> <function> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 5
int main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);
  int n = atoi(argv[1]);
  int lambda = atoi(argv[2]);
  char *func_name = argv[3];
  char *ims_name = argv[4];
  char *imd_name = argv[5];
  process(n, lambda, func_name, ims_name, imd_name);
 
  return EXIT_SUCCESS;
}

/*
int main(int argc, char *argv[]){

 if (argc != param+1) 
    usage(argv[0]); 

  switch(atoi(argv[3])){
   case 0:
     process(atoi(argv[1]),atoi(argv[2]),c_zero,argv[4],argv[5]);
     break;
   case 1:
     process(atoi(argv[1]),atoi(argv[2]),c_un,argv[4],argv[5]);
     break;
   case 2:
     process(atoi(argv[1]),atoi(argv[2]),c_deux,argv[4],argv[5]);
     break;
   default:
     process(atoi(argv[1]),atoi(argv[2]),c_zero,argv[4],argv[5]);
     break;
   }

 }
*/
