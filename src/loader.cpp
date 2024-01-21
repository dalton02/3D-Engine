
#include "loader.h"

mesh loadMesh(const char* file){
    mesh cubo = mesh();
    cubo.tris = {};
    cubo.tris = loadObj(file);
    return cubo;
}

vector<triangle> loadObj(const char* sfileName){

        FILE* f;
        f = fopen(sfileName,"r");

        if(f==nullptr){
            fclose(f);
            return {};
        }
        //Cache
        vector<vec3d> ve;
        vec3d v;
        vector<triangle> cache;
        char line[100];

       while (fgets(line, sizeof(line), f)) {
            strstream s;
            s  << line;
            char junk;
            if(line[0]=='v'){
                s >> junk >> v.x >> v.y >> v.z;
               ve.push_back(v);
            }
            else if(line[0]=='f'){
                int p1,p2,p3;
                s >> junk >> p1 >> p2 >> p3;
                triangle t;
                t.vertexs[0] = ve[p1-1];
                t.vertexs[1] = ve[p2-1];
                t.vertexs[2] = ve[p3-1];
                cache.push_back(t);
            }
        }
    fclose(f);
    return cache;
}
