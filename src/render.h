#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "../lib/tgaimage.h"
#include "templates.h"


class Zbuffer
{   
    public:
        Zbuffer(int widhth, int height);
        ~Zbuffer();

    
    void set(int x, int y, double z);
    
    double get(int x, int y);
    
    private:
        int width, height;
        std::vector<double> depth_map;
};

class Camera //TODO: реализовать базовый функционал и отнаследовать на разные варианты камер?
{

};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    
    void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &image, TGAColor color, Zbuffer &zbuffer);
    void line(int ax, int ay, int bx, int by, TGAImage &image, TGAColor color);
    static auto barycentric(int ax, int ay, int bx, int by, int cx, int cy, int px, int py) -> vec3d;
    

private:
    static double square(int ax, int ay, int bx, int by, int cx, int cy);
};


#endif // RENDER_H
