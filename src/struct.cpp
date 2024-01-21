
#include "struct.h"

vec3d addVecs(vec3d& a,vec3d& b){
    return {a.x+b.x , a.y+b.y , a.z+b.z};
}
vec3d subVecs(vec3d& a,vec3d& b){
    return {a.x-b.x , a.y-b.y , a.z-b.z};
}
vec3d mulVecs(vec3d& a,float k){
    return {a.x*k,a.y*k,a.z*k};
}
vec3d divVecs(vec3d& a,float b){
    return { (a.x)/b, (a.y)/b, (a.z)/b};
}
vec3d crossVecs(vec3d& a,vec3d& b){
    vec3d v;
    v.x = a.y*b.z - a.z*b.y;
    v.y = a.z*b.x - a.x*b.z;
    v.z = a.x*b.y - a.y*b.x;
    return v;
}
float productVecs(vec3d&a,vec3d&b){
    return (a.x*b.x+a.y*b.y+a.z*b.z);
}
float lenghtVecs(vec3d&a,vec3d&b){
    return sqrt(productVecs(a,b));
}
vec3d normalizeVecs(vec3d&normal){
        float l = lenghtVecs(normal,normal);
		vec3d tmp = normal;
		tmp.x /=l;
		tmp.y /=l;
		tmp.z /=l;
        return tmp;
}



void matrizInverse(matrix&m,matrix&modificar){
    matrix tmp;
    tmp.m[0][0] = m.m[0][0];
    tmp.m[1][0] = m.m[0][1];
    tmp.m[2][0] = m.m[0][2];

    tmp.m[0][1] = m.m[1][0];
    tmp.m[1][1] = m.m[1][1];
    tmp.m[2][1] = m.m[1][2];

    tmp.m[0][2] = m.m[2][0];
    tmp.m[1][2] = m.m[2][1];
    tmp.m[2][2] = m.m[2][2];

    tmp.m[0][3] = 0;
    tmp.m[1][3] = 0;
    tmp.m[2][3] = 0;



    tmp.m[3][0] =-(m.m[3][0] *tmp.m[0][0] + m.m[3][1] *tmp.m[1][0]+m.m[3][2] *tmp.m[2][0]);
    tmp.m[3][1] =-(m.m[3][0] *tmp.m[0][1] + m.m[3][1] *tmp.m[1][1]+m.m[3][2] *tmp.m[2][1]);
    tmp.m[3][2] = -(m.m[3][0] *tmp.m[0][2] + m.m[3][1] *tmp.m[1][2]+m.m[3][2] *tmp.m[2][2]);
    tmp.m[3][3] = 1.0f;

    modificar = tmp;

}

void matrizIdentidade(matrix &m){
		m.m[0][0] = 1.0f;
		m.m[1][1] = 1.0f;
		m.m[2][2] = 1.0f;
		m.m[3][3] = 1.0f;
	}

matrix matrizMultiplicar(matrix &m1, matrix &m2){
		matrix matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] *
				m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		return matrix;}


matrix matrizSomar(matrix &m1,matrix &m2){
		matrix matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
                matrix.m[c][r] = m1.m[c][r] + m2.m[c][r];

        return matrix;
}
