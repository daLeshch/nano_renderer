#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "tgaimage.h"
#include "templates.h"
#include "SDL3/SDL.h"


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

struct Viewport
{
    
    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    //The surface contained by the window
    SDL_Surface* gScreenSurface = NULL;

    //The image we will load and show on the screen
    SDL_Surface* gHelloWorld = NULL;

    bool init(int width, int height);
    bool load_media(const char *media);
    void close();
    void create(const char *media, int width, int height);
    void showImage (TGAImage &image, int width, int height);

};

struct Camera
{
    Camera(){};
    Camera(const vec3f& eye, const vec3f& target, const vec3f& up);
    // Camera(const vec3f& eye, const vec3f& target, const vec3f& up, int width = 800, int height = 800);
    
    vec3f xax;
    vec3f yax;
    vec3f zax;
    float f = 1/tan(40/2);// фокусное расстояние камеры, пока фиксированное будет
    float aspect;
    float near_clip = 1.;
    float far_clip = 1000.;
    
    int w;
    int h;

    mat4 view_matrix;
    mat4 persp_matrix;
    mat4 screen_matrix;


    ~Camera(){};

    vec3f view_point(const vec3f& point);
    vec3f persp(vec3f vert);
    vec2f screen();
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    
    void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &image, TGAColor color, Zbuffer &zbuffer, int width, int height);
    void line(int ax, int ay, int bx, int by, TGAImage &image, TGAColor color);
    static auto barycentric(int ax, int ay, int bx, int by, int cx, int cy, int px, int py) -> vec3d;
    auto project();
    auto rotate();
    std::tuple<int,int,int> project(vec3f vert, int width = 800, int height = 800 );
    float light(vec3f v0, vec3f v1, vec3f v2);
    vec3f normilize(const vec3f &vertex, const vec3f&);
    
    

private:
    static double square(int ax, int ay, int bx, int by, int cx, int cy);
};


#endif // RENDER_H
