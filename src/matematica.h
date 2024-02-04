
#include "struct.h"

//Matrix Transformations
void matrizRotationX(matrix &m,float);
void matrizRotationY(matrix&m,float);
void matrizRotationZ(matrix&m,float);
void matrizTranslation(matrix &m,float,float,float);
void matriz3Dto2D(matrix&m,float,float,float,float);

//Matrix functions
void matrizPointAt(vec3d&,vec3d&,vec3d&,matrix&);

//Triangle Manipulation
void convertTriangle(triangle &converte,matrix &m);
vector<triangle> convertMeshtoScreen(mesh obj,matrix& worldView ,matrix& cameraView, matrix &proj2D,camera& cam,int WIDTH,int HEIGHT,int);
void sortTriangles(vector<triangle>&);
//Movement and Rotation Stuff
void rotateX(triangle& modificar,float degrees);
void rotateZ(triangle& modificar,float degrees);
void rotateY(triangle& modificar,float degrees);
void translate3D(triangle& mover,float x,float y,float z);


//Ilumination
float ilumination(vec3d normal,vec3d light,triangle&);
int trianglesClipped(vec3d plane, vec3d normalP, triangle &inTri, triangle &outTri, triangle &outTri2);
vec3d intersectPlaneVec(vec3d &plane, vec3d &normalP, vec3d &lineS, vec3d &lineE);






void debugTriangle(triangle &t);
void debugMatrix(matrix &m);
