#version 130

uniform sampler2D Tcur;
uniform sampler2D Tnext;
uniform sampler2D Tnodes;
uniform sampler2D TedgesList;

uniform int verticesTexturesSize;
uniform int neighboursTexturesSize;
uniform float Eltime;

in vec4 position_obj;
in vec4 position_view;
in vec2 texcoord;

out vec4 out_color;

float normVector3f(vec3 v){
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}


//Fab_t = -Kab * |Wab - Lab| * Wab/|Wab|
vec3 hookesLaw(vec3 a_t, vec3 b_t, vec3 L_ab){
    float K_ab = 3.0; //facteur pour determiner la force d'un ressort
    vec3 W_ab = a_t - b_t;

    vec3 W_ab_L_ab = W_ab - L_ab;

    float coef1 = 1/normVector3f(W_ab);
    W_ab = W_ab* coef1;

    float coef2 = -K_ab * normVector3f(W_ab_L_ab);

    return W_ab*coef2;
}



float repulsiveForce(vec3 a_t, vec3 b_t){
    float q = 3.; // facteur d'echelle
    float alpha = 1.5; // alpha doit etre compris entre 1 et 2
    vec3 W_ab = a_t - b_t;
    float Wab = normVector3f(W_ab);
    return q / pow(Wab, alpha);
}

vec2 getNodeCoordFromEdge(int i, int j, int width, int k, sampler2D T_nodes, sampler2D T_edgesList){

    float u1, v1;

    u1 = texture2D(T_nodes, vec2(i,j)).r;
    v1 = texture2D(T_nodes, vec2(i,j)).g;

    float fK = k/width;
     if (u1+ fK > 1){

         v1 += 1/width;
         u1 = (u1 + fK - 1/width);
     }
     else
         u1+=fK;

    vec4 res = texture2D(T_edgesList, vec2(u1, v1));

    return vec2(res.r, res.g);
}

vec3 totalInternForce(int i, int j, int width, sampler2D T_cur, sampler2D T_next, sampler2D T_nodes, sampler2D T_edgesList){
    vec3 res = vec3(0,0,0);
    vec3 L_ab, a_Node, b_Node, a_t_Node, b_t_Node;
    vec2 coordB;

    a_Node = vec3(texture2D(T_next, vec2(i,j)).r, texture2D(T_next, vec2(i,j)).g, texture2D(T_next, vec2(i,j)).b);
    b_Node = vec3(texture2D(T_cur, vec2(i,j)).r, texture2D(T_cur, vec2(i,j)).g, texture2D(T_cur, vec2(i,j)).b);

    int nbNeighbors = int(texture2D(T_nodes, vec2(i,j)).b);

            int k= 0;
    for(k=0; k<nbNeighbors; k++){
        coordB = getNodeCoordFromEdge(i,j,width,k,T_nodes,T_edgesList);
        b_Node = vec3(texture2D(T_next, vec2(i,j)).r, texture2D(T_next, vec2(i,j)).g, texture2D(T_next, vec2(i,j)).b);
        b_t_Node = vec3(texture2D(T_cur, vec2(i,j)).r, texture2D(T_cur, vec2(i,j)).g, texture2D(T_cur, vec2(i,j)).b);
        L_ab = a_Node - b_Node;

        res += hookesLaw(a_t_Node, b_t_Node, L_ab);
    }
    return res;
}


vec3 totalRepulsiveForce(int i, int j, int width, sampler2D T_cur, sampler2D T_next, sampler2D T_nodes, sampler2D T_edgesList){
    vec3 res = vec3(0,0,0);
    vec3 a_t, b_t;
    vec2 coordB;
    float tmp;

    int nbNeighbors = int(texture2D(T_nodes, vec2(i,j)).b);

    for(int k=0; i<nbNeighbors; i++){
        a_t = vec3(texture2D(T_cur, vec2(i,j)).r, texture2D(T_cur, vec2(i,j)).g, texture2D(T_cur, vec2(i,j)).b);
        coordB = getNodeCoordFromEdge(i,j,width,k,T_nodes,T_edgesList);
        b_t = vec3(texture2D(T_cur, coordB).r, texture2D(T_cur, coordB).g, texture2D(T_cur, coordB).b);
        tmp = repulsiveForce(a_t, b_t);
        res += vec3(tmp,tmp,tmp);

    }

    return res;
}




/*
#include <stdio.h>
#include <math.h>
#define dist 0.2
#define MAX 1.0
int N=1;
void euler(double x, double y[], double step);

double f(double x, double y[], int i);

main()
{
    double t, y[N];
    int j;

    y[0]=1.0;


    for (j=0; j*dist<=MAX ;j++)
    {
       t=j*dist;
       printf("j =  %d,t = %f y[0] = %f\n", j,t, y[0]);
       euler(t, y, dist);

    }

}

void euler(double x, double y[], double step)
{
    double  s[N];
    int i;
    {
    for (i=0;i<N;i++){
        s[i]=step*f(x, y, i);
    }
}

{
for (i=0;i<N;i++)
     y[i]+=s[i];
}
}
double  f(double x, double y[], int i)
{
      return(x-y[0]);
}

*/

vec3 euler(vec3 a, sampler2D T_cur, int i, int j, int step, int size){
    for(int k=0; k<size; k++){
        = step*();
    }
}

float f(vec3 a, sampler2D T_cur, int i, int j){
    return vec3(a.r - texture2D(T_cur, vec2(i,j)).r,   a.g - texture2D(T_cur, vec2(i,j)).g, a.b - texture2D(T_cur, vec2(i,j)).b);
}


vec3 differentialEquation(int i, int j, int width, sampler2D T_cur, sampler2D T_next, sampler2D T_nodes, sampler2D T_edgesList){
    float m = 2.;
    vec3 res = totalInternForce(i, j, width, T_cur, T_next, T_nodes, T_edgesList) + totalRepulsiveForce(i, j, width, T_cur, T_next, T_nodes, T_edgesList);

    return res;

}


void main()
{


   // calculate forces and motions

   vec3 a = vec3(0.0, 0.0,0.0);

  a +=  differentialEquation(0,0, verticesTexturesSize, Tcur, Tnext, Tnodes, TedgesList);

  out_color = vec4(a.x+Eltime, a.y+Eltime, a.z+Eltime, 1);
}



