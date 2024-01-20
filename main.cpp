

#include "draw.h"
#include "loader.h"
#include "matematica.h"

int main(int argc,char**argv)
{
    int WIDTH = 800;int HEIGHT = 640;

    mesh c2 = loadMesh("mountains.obj");
    mesh c1 = loadMesh("nave.obj");
    c1.id = 1;
    c2.id = 2;

    vector<mesh> collectionMesh;
    collectionMesh.push_back(c1);
    collectionMesh.push_back(c2);

	// Projection Matrix
    float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio = (float)HEIGHT/(float)WIDTH;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
    float fTheta =1.0f;


	SDL_Init(SDL_INIT_VIDEO);SDL_Window* window = SDL_CreateWindow("FUCKING DOPE 3D CUBE MOTHERFUCKER",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
	WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,0);
	SDL_RenderClear(renderer);
    SDL_Event event;


    bool game = true;


    matrix proj2D;
    matriz3Dto2D(proj2D,fAspectRatio,fFovRad,fFar,fNear);

    //Matrixs for rotation Z X and translation World
    matrix rZ,rX,rY,rT,worldV;

	vec3d walkDirection;
	camera cam;
    cam.vCamera = {0,0,0,1};
    cam.vLook = {0,0,1,1};
    cam.vUp = {0,1,0,1};
    cam.vTarget = {0,0,1,1};
    walkDirection= {4,0,0,0};

	matrizIdentidade(worldV);

	//Speed Front and Back
	vec3d tmpF = mulVecs(cam.vLook,1);
	vec3d tmpB = mulVecs(cam.vLook,-1);

	vec3d spaceStar = {0,0,0};

	float angleSpeed=0.0f;

	while(game){
	SDL_SetRenderDrawColor(renderer,0,0,0,1);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer,255,0,0,1);



    //Start the frame timer
    int x = SDL_GetTicks();
        //Events
    while( SDL_PollEvent( &event ) ){
        if( event.type == SDL_QUIT )
           {
              game=false;
           }
        if(event.type == SDL_KEYDOWN){

            vec3d w = {-2*sinf(angleSpeed),0,2*cosf(angleSpeed)};
            vec3d s = {2*sinf(angleSpeed),0,-2*cosf(angleSpeed)};
            vec3d newFoward;
            switch (event.key.keysym.sym)
            {
                case SDLK_a:
                    angleSpeed +=0.1;
                    if(angleSpeed >2*3.14159f){
                        angleSpeed =0;
                    }
                    cout << angleSpeed <<endl;
                break;

                case SDLK_d:
                      angleSpeed-=0.1;
                    if(angleSpeed <0){
                        angleSpeed =2*3.14159f;
                    }
                    cout << angleSpeed <<endl;
                    break;

                case SDLK_w: spaceStar = addVecs(spaceStar,w); break;
                case SDLK_s: spaceStar = addVecs(spaceStar,s); break;
            }
        }
    }


    fTheta+=0.1;
    vector<triangle> triRasterizados;


	matrix camRotationY;

	matrizRotationY(camRotationY,angleSpeed);

    cam.vTarget = {0,0,1};

    vec3d newPos = subVecs(spaceStar,cam.vCamera);
    cam.vCamera = addVecs(cam.vCamera,newPos);

	cam.vLook = multiplicar3D(cam.vTarget,camRotationY);
    cam.vTarget = addVecs(cam.vCamera,cam.vLook);

    matrix matCamera,matView;
    matrizPointAt(cam.vCamera,cam.vTarget,cam.vUp,matCamera);
    matrizInverse(matCamera,matView);

	for(int j=0;j<collectionMesh.size();j++){
        mesh a = collectionMesh[j];
        //Per mesh

        matrizRotationX(rX,0);matrizRotationY(rY,0);matrizRotationZ(rZ,0);

        if(a.id==1){
            float distCam = 10;
            float newXpos = 4+cam.vCamera.x + distCam*cam.vLook.x;
            float newZpos = 5+cam.vCamera.z + distCam*cam.vLook.z;

            //Movement ship
            matrizTranslation(rT,newXpos,0,newZpos);

            matrizRotationY(rY,angleSpeed);
            worldV = matrizMultiplicar(rZ,rY);

        }
        else if(a.id==2){
           matrizTranslation(rT,3,40,40);
           matrizRotationZ(rZ,3.147);
           matrizRotationZ(rY,0);
           worldV = matrizMultiplicar(rZ,rY);

        }
        worldV = matrizMultiplicar(worldV,rT);

        vector <triangle> tmp = convertMeshtoScreen(a,worldV,matView,proj2D,cam,WIDTH,HEIGHT);
        for(int x=0;x<tmp.size();x++)
            triRasterizados.push_back(tmp[x]);

	} //END loop mesh

        sortTriangles(triRasterizados);
        drawRaster(renderer,triRasterizados);

        SDL_RenderPresent(renderer);
        while( (SDL_GetTicks() - x) < 1000 / 30){}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
    return 0;
}
