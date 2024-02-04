#include "draw.h"

void drawRaster(SDL_Renderer* renderer,vector<triangle> triRasterizados){


	SDL_SetRenderDrawColor(renderer,255,255,255,1);

	for(int i=0;i<triRasterizados.size();i++){

        float dp = triRasterizados[i].intensity;
        float r = triRasterizados[i].color[0];
        float g = triRasterizados[i].color[1];
        float b = triRasterizados[i].color[2];
        int renderMode = triRasterizados[i].renderMode;



        float x1 = triRasterizados[i].vertexs[0].x;
        float y1 = triRasterizados[i].vertexs[0].y;
        float x2 = triRasterizados[i].vertexs[1].x;
        float y2 = triRasterizados[i].vertexs[1].y;
        float x3 = triRasterizados[i].vertexs[2].x;
        float y3 = triRasterizados[i].vertexs[2].y;

        float x_min = min(x1,x2);
        x_min = min(x_min,x3);

        float y_min = min(y1,y2);
        y_min = min(y_min,y3);

        float x_max = max(x1,x2);
        x_max = max(x_max,x3);

        float y_max = max(y1,y2);
        y_max = max(y_max,y3);

      /*  if(x_min<0 || y_min<0 || x_max>800 || y_max>640){

        }
        else{
        for(int y=y_min;y<=y_max;y++){

            for(int x=x_min;x<=x_max;x++){

                //First side
                vec3d ab = {x1-x2,y1-y2,1,1};
                vec3d bp = {x-x2,y-y2,1,1};
                int w0 = ab.x*bp.y - ab.y*bp.x;

                ab = {x2-x3,y2-y3,1,1};
                bp = {x-x3,y-y3,1,1};
                int w1 = ab.x*bp.y - ab.y*bp.x;

                //First side
                ab = {x3-x1,y3-y1,1,1};
                bp = {x-x1,y-y1,1,1};
                int w2 = ab.x*bp.y - ab.y*bp.x;


                if(w1>=0 && w2>=0 && w0>=0){
                SDL_SetRenderDrawColor(renderer,r*dp,g*dp,b*dp,1);
                SDL_RenderDrawPoint(renderer,x,y);
                }
            }

        }
        }

*/


        vector<SDL_Vertex> verts = {
        {SDL_FPoint{triRasterizados[i].vertexs[0].x,triRasterizados[i].vertexs[0].y},SDL_Color{r*dp,b*dp,g*dp},SDL_FPoint{0}},
        {SDL_FPoint{triRasterizados[i].vertexs[1].x,triRasterizados[i].vertexs[1].y},SDL_Color{r*dp,b*dp,g*dp},SDL_FPoint{0}},
        {SDL_FPoint{triRasterizados[i].vertexs[2].x,triRasterizados[i].vertexs[2].y},SDL_Color{r*dp,b*dp,g*dp},SDL_FPoint{0}},
        };


        if(renderMode==1){
        SDL_RenderGeometry(renderer,nullptr,verts.data(),verts.size(),nullptr,0);

        }
        else if(renderMode==-1){
        SDL_RenderDrawLine(renderer,triRasterizados[i].vertexs[0].x,triRasterizados[i].vertexs[0].y,triRasterizados[i].vertexs[1].x,triRasterizados[i].vertexs[1].y);
        SDL_RenderDrawLine(renderer,triRasterizados[i].vertexs[1].x,triRasterizados[i].vertexs[1].y,triRasterizados[i].vertexs[2].x,triRasterizados[i].vertexs[2].y);
        SDL_RenderDrawLine(renderer,triRasterizados[i].vertexs[2].x,triRasterizados[i].vertexs[2].y,triRasterizados[i].vertexs[0].x,triRasterizados[i].vertexs[0].y);
	}
	}
}

