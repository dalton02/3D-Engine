#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <strstream>
#include <algorithm>
using namespace std;

typedef struct vec3d{
    float x=0;
    float y=0;
    float z=0;
    float w=1;
}vec3d;
typedef struct triangle{
    vec3d vertexs[3];
    float intensity;
    float color[3];
}triangle;
typedef struct mesh{
    vector<triangle> tris;
    int id;
    vec3d rot;
    vec3d pos;
}mesh;

typedef struct matrix{
    float m[4][4] = {0};
}matrix;

typedef struct camera{
   vec3d vCamera;
   vec3d vLook;
   vec3d vUp;
   vec3d vTarget;
}camera;


//This file contain a serie of util functions to deal with my vec3d and matrix functions


//Vectors functions
vec3d addVecs(vec3d&,vec3d&);
vec3d subVecs(vec3d&,vec3d&);
vec3d mulVecs(vec3d&,float);
vec3d divVecs(vec3d&,float);
vec3d crossVecs(vec3d&,vec3d&);
float productVecs(vec3d&,vec3d&);
float lenghtVecs(vec3d&,vec3d&);
vec3d normalizeVecs(vec3d&);
vec3d multiplicar3D(vec3d &original,matrix &m);

//Matrix functions
void matrizInverse(matrix&,matrix&);
void matrizIdentidade(matrix &m);
matrix matrizSomar(matrix &m1,matrix &m2);
matrix matrizMultiplicar(matrix &m1, matrix &m2);

#endif
