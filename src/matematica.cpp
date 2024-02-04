#include "matematica.h"




void sortTriangles(vector<triangle>& triRasterizados){
	//Buffer Depth
	sort(triRasterizados.begin(),triRasterizados.end(),[](triangle &t1,triangle &t2){
    float z1 = (t1.vertexs[0].z+t1.vertexs[1].z+t1.vertexs[2].z)/3.0f;
    float z2 = (t2.vertexs[0].z+t2.vertexs[1].z+t2.vertexs[2].z)/3.0f;
	return z1>z2;
	});


}
vector<triangle> convertMeshtoScreen(mesh obj, matrix& worldView ,matrix& cameraView, matrix &proj2D,camera &cam,int WIDTH,int HEIGHT,int renderMode){

 vec3d normal, line1,line2,light;
 vector<triangle> triRasterizados;
 for (int i=0;i<obj.tris.size();i++){


        //Recebem copia do triangulo
        triangle triTransform = obj.tris[i];
        triangle triView = obj.tris[i];


        convertTriangle(triTransform,worldView);


		line1 = subVecs(triTransform.vertexs[1],triTransform.vertexs[0]);
		line2 = subVecs(triTransform.vertexs[2],triTransform.vertexs[0]);
		normal = crossVecs(line1,line2);
        normal = normalizeVecs(normal);

        vec3d cameraRay = subVecs(triTransform.vertexs[0],cam.vCamera);


        if( productVecs(normal,cameraRay) < 0.0f){

        light = {0,-1,0,1};

        //For every triangle lets cast a shadow by the normal of light
        if(light.y==normal.y){


        }



        float dp = ilumination(normal,light,triTransform);
        triTransform.intensity = dp;
        triTransform.color[0] = 255;
        triTransform.color[1] = 255;
        triTransform.color[2] = 255;
        triTransform.renderMode = renderMode;

        triView = triTransform;
        convertTriangle(triView,cameraView);
        // Clip Viewed Triangle against near plane, this could form two additional
		// additional triangles.
		int nClippedTriangles = 0;
		triangle clipped[2];
		nClippedTriangles =
		trianglesClipped({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triView, clipped[0], clipped[1]);

        for(int n=0;n<nClippedTriangles;n++){
        triView = clipped[n];
        convertTriangle(triView,proj2D);

        triView.vertexs[0] = divVecs(triView.vertexs[0],triView.vertexs[0].w);
        triView.vertexs[1] = divVecs(triView.vertexs[1],triView.vertexs[1].w);
        triView.vertexs[2] = divVecs(triView.vertexs[2],triView.vertexs[2].w);

		//Posicionamento
        vec3d offset = {1,1,0};
        triView.vertexs[0] = addVecs(triView.vertexs[0],offset);
        triView.vertexs[1] = addVecs(triView.vertexs[1],offset);
        triView.vertexs[2] = addVecs(triView.vertexs[2],offset);
        triView.vertexs[0].x*=0.5f*WIDTH; triView.vertexs[0].y*=0.5f*HEIGHT;
        triView.vertexs[1].x*=0.5f*WIDTH; triView.vertexs[1].y*=0.5f*HEIGHT;
        triView.vertexs[2].x*=0.5f*WIDTH; triView.vertexs[2].y*=0.5f*HEIGHT;
        triRasterizados.push_back(triView);
        }
        }
    }
    return triRasterizados;

}


vec3d multiplicar3D(vec3d &original,matrix &m){
    vec3d tmp;
    tmp.x = original.x * m.m[0][0] + original.y * m.m[1][0] + original.z * m.m[2][0] + original.w* m.m[3][0];
    tmp.y = original.x * m.m[0][1] + original.y * m.m[1][1] + original.z * m.m[2][1] + original.w* m.m[3][1];
    tmp.z = original.x * m.m[0][2] + original.y * m.m[1][2] + original.z * m.m[2][2] + original.w* m.m[3][2];
    tmp.w = original.x * m.m[0][3] + original.y * m.m[1][3] + original.z * m.m[2][3] + original.w* m.m[3][3];
    return tmp;
}

vec3d intersectPlaneVec(vec3d &plane, vec3d &normalP, vec3d &lineS, vec3d &lineE){
		normalP = normalizeVecs(normalP);
		float planed = -productVecs(normalP, plane);
		float ad = productVecs(lineS, normalP);
		float bd = productVecs(lineE, normalP);
		float t = (-planed - ad) / (bd - ad);
		vec3d lineStartToEnd = subVecs(lineE, lineS);
		vec3d lineToIntersect = mulVecs(lineStartToEnd, t);
		return addVecs(lineS, lineToIntersect);
}

int trianglesClipped(vec3d plane, vec3d normalP, triangle &inTri, triangle &outTri, triangle &outTri2){

    //Now here is my notes about this code and everyday i gonna read this until i understand what the hell is going on
    //Remember to take a notebook and draw the shit vectors

    //This make a normal vector wich points foward to her face direction you know

    normalP = normalizeVecs(normalP);

    // Return signed shortest distance from point to plane, plane normal must be normalised
	//This shit basically will take the short distance to the camera in the triangle so....

	//For each vec3d hes gonna see who is closes, okay so far....
    auto dist = [&](vec3d &p){
    vec3d n = normalizeVecs(p);
    return (normalP.x * p.x + normalP.y * p.y + normalP.z * p.z -  productVecs(normalP,plane));
    };


    // Create two temporary storage arrays to classify points either side of plane
	// If distance sign is positive, point lies on "inside" of plane
	vec3d* inside_points[3];  int nInsidePointCount = 0;
	vec3d* outside_points[3]; int nOutsidePointCount = 0;
    // Get signed distance of each point in triangle to plane
    //Calls that function you see early....
    float d0 = dist(inTri.vertexs[0]);
	float d1 = dist(inTri.vertexs[1]);
	float d2 = dist(inTri.vertexs[2]);

	//Easy shit, if the distance is less than zero is cause is behind the camera dumb ass
		if (d0 >= 0) { inside_points[nInsidePointCount++] = &inTri.vertexs[0]; }
		else { outside_points[nOutsidePointCount++] = &inTri.vertexs[0]; }
		if (d1 >= 0) { inside_points[nInsidePointCount++] = &inTri.vertexs[1]; }
		else { outside_points[nOutsidePointCount++] = &inTri.vertexs[1]; }
		if (d2 >= 0) { inside_points[nInsidePointCount++] = &inTri.vertexs[2]; }
		else { outside_points[nOutsidePointCount++] = &inTri.vertexs[2]; }
// Now classify triangle points, and break the input triangle into
		// smaller output triangles if required. There are four possible
		// outcomes...

		if (nInsidePointCount == 0)
		{
			// All points lie on the outside of plane, so clip whole triangle
			// It ceases to exist

			return 0; // No returned triangles are valid
		}

		if (nInsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the triangle to simply pass through
			outTri = inTri;

			return 1; // Just the one returned original triangle is valid
		}
		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
		{
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// Copy appearance info to new triangle
			outTri.color[0] =  inTri.color[0];
			outTri.color[1] =  inTri.color[1];
			outTri.color[2] =  inTri.color[2];
			outTri.intensity = inTri.intensity;

			// The inside point is valid, so keep that...
			//OKAY SO THAT SHIT, keeps the vertex that is in the scope and kinda save
			outTri.vertexs[0] = *inside_points[0];

			// but the two new points are at the locations where the
			// original sides of the triangle (lines) intersect with the plane
			outTri.vertexs[1] = intersectPlaneVec(plane, normalP, *inside_points[0], *outside_points[0]);
			outTri.vertexs[2] = intersectPlaneVec(plane, normalP, *inside_points[0], *outside_points[1]);

			return 1; // Return the newly formed single triangle
		}

		if (nInsidePointCount == 2 && nOutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			outTri.color[0] =  inTri.color[0];
			outTri.color[1] =  inTri.color[1];
            outTri.color[2] =  inTri.color[2];
			outTri.intensity = inTri.intensity;

			outTri2.color[0] =  inTri.color[0];
			outTri2.color[1] =  inTri.color[1];
            outTri2.color[2] =  inTri.color[2];
			outTri2.intensity = inTri.intensity;


			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			outTri.vertexs[0] = *inside_points[0];
			outTri.vertexs[1] = *inside_points[1];
			outTri.vertexs[2] = intersectPlaneVec(plane, normalP, *inside_points[0], *outside_points[0]);

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the
			// triangle and the plane, and the newly created point above
			outTri2.vertexs[0] = *inside_points[1];
			outTri2.vertexs[1] = outTri.vertexs[2];
			outTri2.vertexs[2] = intersectPlaneVec(plane, normalP, *inside_points[1], *outside_points[0]);

			return 2; // Return two newly formed triangles which form a quad
		}


}

// Project triangles from 3D --> 2D
void convertTriangle(triangle& converte,matrix&m){
		triangle tmp;
		tmp.vertexs[0] = multiplicar3D(converte.vertexs[0], m);
		tmp.vertexs[1] = multiplicar3D(converte.vertexs[1], m);
		tmp.vertexs[2] = multiplicar3D(converte.vertexs[2], m);
		converte = tmp;
}



void matrizTranslation(matrix &m,float x,float y,float z){
    m.m[0][0] = 1;
    m.m[1][1] = 1;
    m.m[2][2] = 1;
    m.m[3][3] = 1;
    m.m[3][0] = x;
    m.m[3][1] = y;
    m.m[3][2] = z;
}

void matrizRotationX(matrix &m,float fTheta){
    m.m[0][0] = 1;
	m.m[1][1] = cosf(fTheta);
	m.m[1][2] = sinf(fTheta);
	m.m[2][1] = -sinf(fTheta);
	m.m[2][2] = cosf(fTheta);
	m.m[3][3] = 1.0f;
}

void rotateX(triangle& modificar,float degrees){

    matrix m;
    matrizRotationX(m,degrees);
    triangle tmp;
    tmp.vertexs[0] = multiplicar3D(modificar.vertexs[0], m);
	tmp.vertexs[1] = multiplicar3D(modificar.vertexs[1], m);
    tmp.vertexs[2] = multiplicar3D(modificar.vertexs[2], m);
    modificar = tmp;
}

void rotateY(triangle& modificar,float degrees){


    matrix m;
    matrizIdentidade(m);
    matrizRotationY(m,degrees);
    triangle tmp;
    tmp.vertexs[0] = multiplicar3D(modificar.vertexs[0], m);
	tmp.vertexs[1] = multiplicar3D(modificar.vertexs[1], m);
    tmp.vertexs[2] = multiplicar3D(modificar.vertexs[2], m);
    modificar = tmp;
}

void matrizRotationZ(matrix&m,float fTheta){
   m.m[0][0] = cosf(fTheta);
   m.m[0][1] = sinf(fTheta);
   m.m[1][0] = -sinf(fTheta);
   m.m[1][1] = cosf(fTheta);
   m.m[2][2] = 1.0f;
   m.m[3][3] = 1.0f;
}

void rotateZ(triangle& modificar,float degrees){
    matrix m;
    matrizRotationZ(m,degrees);
    triangle tmp;
    tmp.vertexs[0] = multiplicar3D(modificar.vertexs[0], m);
	tmp.vertexs[1] = multiplicar3D(modificar.vertexs[1], m);
    tmp.vertexs[2] = multiplicar3D(modificar.vertexs[2], m);
    modificar = tmp;
}

void matrizRotationY(matrix&m,float fTheta){
   m.m[0][0] = cosf(fTheta);
   m.m[0][2] = sinf(fTheta);
   m.m[2][0] = -sinf(fTheta);
   m.m[1][1] = 1.0f;
   m.m[2][2] = cosf(fTheta);
   m.m[3][3] = 1.0f;
}

void translate3D(triangle& mover,float x,float y,float z){
    matrix m;
    matrizTranslation(m,x,y,z);
    triangle tmp;
    tmp.vertexs[0] = multiplicar3D(mover.vertexs[0], m);
    tmp.vertexs[1] = multiplicar3D(mover.vertexs[1], m);
    tmp.vertexs[2] = multiplicar3D(mover.vertexs[2], m);
    mover = tmp;
}


float ilumination(vec3d normal,vec3d light,triangle& tri){

    float lig = sqrt(light.x*light.x+light.y*light.y+light.z*light.z);
    light.x /=lig;
	light.y /=lig;
	light.z /=lig;


    float dp = normal.x*light.x + normal.y*light.y + normal.z*light.z;
    tri.cast = normal;

    return dp;
}

void matriz3Dto2D(matrix&m,float fAspectRatio,float fFovRad,float fFar,float fNear){
	m.m[0][0] = fAspectRatio * fFovRad;
	m.m[1][1] = fFovRad;
	m.m[2][2] = fFar / (fFar - fNear);
	m.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	m.m[2][3] = 1.0f;
	m.m[3][3] = 0.0f;
}

void matrizPointAt(vec3d &pos,vec3d &target,vec3d &up,matrix&m){

    vec3d foward = subVecs(target,pos);
    foward = normalizeVecs(foward);

    vec3d a = mulVecs(foward,(productVecs(up,foward)));
    vec3d newUp = subVecs(up,a);
    newUp = normalizeVecs(newUp);

    vec3d newRight = crossVecs(newUp,foward);

    m.m[0][0] = newRight.x;
    m.m[0][1] = newRight.y;
    m.m[0][2] = newRight.z;
    m.m[0][3] = 0;
    m.m[1][0] = newUp.x;
    m.m[1][1] = newUp.y;
    m.m[1][2] = newUp.z;
    m.m[1][3] = 0;
    m.m[2][0] = foward.x;
    m.m[2][1] = foward.y;
    m.m[2][2] = foward.z;
    m.m[2][3] = 0;
    m.m[3][0] = pos.x;
    m.m[3][1] = pos.y;
    m.m[3][2] = pos.z;
    m.m[3][3] = 1;


}

void debugTriangle(triangle &t){
    printf("\nTriangle Debug:\n");
    printf("\n\nVertexs 1: %.2f %2.f %.2f",t.vertexs[0].x,t.vertexs[0].y,t.vertexs[0].z);
    printf("\nVertexs 2: %.2f %2.f %.2f",t.vertexs[1].x,t.vertexs[1].y,t.vertexs[1].z);
    printf("\nVertexs 3: %.2f %2.f %.2f\n",t.vertexs[2].x,t.vertexs[2].y,t.vertexs[2].z);
}

void debugMatrix(matrix &m){
    printf("\nMatrix Debug:\n");
        for(int i=0;i<4;i++){

            for(int j=0;j<4;j++){

                printf(" %f ",m.m[i][j]);

            }
            printf("\n");
        }
}
