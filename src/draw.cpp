#include "draw.h"

void drawRaster(SDL_Renderer* renderer,vector<triangle> triRasterizados){


	SDL_SetRenderDrawColor(renderer,255,255,255,1);
	for(int i=0;i<triRasterizados.size();i++){
        float dp = triRasterizados[i].intensity;
        float r = triRasterizados[i].color[0];
        float g = triRasterizados[i].color[1];
        float b = triRasterizados[i].color[2];
        vector<SDL_Vertex> verts = {
        {SDL_FPoint{triRasterizados[i].vertexs[0].x,triRasterizados[i].vertexs[0].y},SDL_Color{r*dp,b*dp,g*dp},SDL_FPoint{0}},
        {SDL_FPoint{triRasterizados[i].vertexs[1].x,triRasterizados[i].vertexs[1].y},SDL_Color{r*dp,b*dp,g*dp},SDL_FPoint{0}},
        {SDL_FPoint{triRasterizados[i].vertexs[2].x,triRasterizados[i].vertexs[2].y},SDL_Color{r*dp,b*dp,g*dp},SDL_FPoint{0}},
        };

        SDL_RenderGeometry(renderer,nullptr,verts.data(),verts.size(),nullptr,0);
      //  SDL_RenderDrawLine(renderer,triRasterizados[i].vertexs[0].x,triRasterizados[i].vertexs[0].y,triRasterizados[i].vertexs[1].x,triRasterizados[i].vertexs[1].y);
        //SDL_RenderDrawLine(renderer,triRasterizados[i].vertexs[1].x,triRasterizados[i].vertexs[1].y,triRasterizados[i].vertexs[2].x,triRasterizados[i].vertexs[2].y);
        //SDL_RenderDrawLine(renderer,triRasterizados[i].vertexs[2].x,triRasterizados[i].vertexs[2].y,triRasterizados[i].vertexs[0].x,triRasterizados[i].vertexs[0].y);
	}
}

