

#include "draw.h"
#include "loader.h"
#include "matematica.h"

int main(int argc,char**argv)
{
    int WIDTH = 800;int HEIGHT = 640;


    //Load your obj file here
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
	float fFov = 95.0f;
	float fAspectRatio = (float)HEIGHT/(float)WIDTH;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
    float fTheta =1.0f;


	SDL_Init(SDL_INIT_VIDEO);SDL_Window* window = SDL_CreateWindow("NAVIGATION 3D",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
	WIDTH,HEIGHT,SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
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

	float angleSpeedX=0.0f;
	float angleSpeedY=0.0f;
    float limitAngleX = 0;
    float limitAngleY = 0;
    int renderMode = 1;
	while(game){
	SDL_SetRenderDrawColor(renderer,10,10,10,1);
	SDL_RenderClear(renderer);



    //Start the frame timer
    int x = SDL_GetTicks();

    //Events

    while( SDL_PollEvent( &event ) ){
        if( event.type == SDL_QUIT ){game=false;}

        if(event.type == SDL_KEYDOWN){

            vec3d w = {-2*sinf(angleSpeedX),2*sinf(angleSpeedY),2*cosf(angleSpeedX)};
            vec3d s = {2*sinf(angleSpeedX),-2*sinf(angleSpeedY),-2*cosf(angleSpeedX)};

            vec3d newFoward;

            switch (event.key.keysym.sym)
            {
                case SDLK_LEFT:
                    angleSpeedX +=0.1;
                    limitAngleX += 0.1;
                    if(limitAngleX>=3.14)    limitAngleX=3.14;

                    if(angleSpeedX >2*3.14159f) angleSpeedX =0;


                break;

                case SDLK_RIGHT:
                    angleSpeedX-=0.1;
                    limitAngleX -= 0.1;
                    if(limitAngleX<=-3.14)    limitAngleX=-3.14;
                    if(angleSpeedX <0) angleSpeedX =2*3.14159f;

                    break;

                case SDLK_w: spaceStar = addVecs(spaceStar,w); break;
                case SDLK_s: spaceStar = addVecs(spaceStar,s); break;
                case SDLK_UP:
                    angleSpeedY +=0.1;
                    limitAngleY += 0.1;
                    if(limitAngleY>=3.14)    limitAngleY=3.14;


                    if(angleSpeedY >2*3.14159f) angleSpeedY =0;

                break;

                case SDLK_DOWN:
                    angleSpeedY -=0.1;
                    limitAngleY -= 0.1;
                    if(limitAngleY<0)    limitAngleY=3.14;


                    if(angleSpeedY <0) angleSpeedY =2*3.14159f;


                break;
                case SDLK_r: renderMode= renderMode*-1; break;
            }
        }
    }


    fTheta+=0.1;
    vector<triangle> triRasterizados;


	matrix camRotationY;
	matrix camRotationX;

	matrizRotationY(camRotationY,angleSpeedX);
	matrizRotationX(camRotationX,angleSpeedY*-1);

    matrix result = matrizMultiplicar(camRotationX,camRotationY);

    cam.vTarget = {0,0,1};

    vec3d newPos = subVecs(spaceStar,cam.vCamera);
    cam.vCamera = addVecs(cam.vCamera,newPos);

	cam.vLook = multiplicar3D(cam.vTarget,result);
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

            a.pos.x = 4+cam.vCamera.x + distCam*cam.vLook.x;
            a.pos.z = 5+cam.vCamera.z + distCam*cam.vLook.z;

            a.rot.y = a.rot.y + angleSpeedY;

            matrizRotationY(rY,fTheta);
            worldV = matrizMultiplicar(rZ,rY);

            matrizTranslation(rT,10,30,10);


            worldV = matrizMultiplicar(worldV,rT);



        }
        else if(a.id==2){
           matrizTranslation(rT,3,40,40);
           matrizRotationZ(rZ,3.147);
           matrizRotationY(rY,0);
           worldV = matrizMultiplicar(rZ,rY);
           worldV = matrizMultiplicar(worldV,rT);

        }
        vector <triangle> tmp = convertMeshtoScreen(a,worldV,matView,proj2D,cam,WIDTH,HEIGHT,1);
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
