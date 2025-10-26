#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "tgaimage.h"
#include "math_core.h"
#include "SDL3/SDL.h"
#include "model.h"

class Zbuffer
{
public:
    Zbuffer(int widhth, int height);
    ~Zbuffer();
    void clear();

    void set(int x, int y, double z);

    double get(int x, int y);

private:
    int width, height;
    std::vector<double> depth_map;
};

struct Camera
{
    Camera() {};
    Camera(const vec3f &eye, const vec3f &target, const vec3f &up);
    vec3f xax;
    vec3f yax;
    vec3f zax;
    float f = 3;
    float near_clip = 0.1;
    float far_clip = 10.;

    int w = 800;
    int h = 800;
    float aspect = w / h;

    mat4 view_matrix;
    mat4 persp_matrix;
    mat4 screen_matrix;
    mat4 ortho_matrix;

    vec3f view_persp(const vec3f &point);
    std::tuple<int, int, int> screen(const vec3f &point);

    ~Camera() {};
};

class Renderer
{
public:
    Renderer();
    Renderer(int w, int h) : width(w), height(h) {};
    ~Renderer();

    int width;
    int height;

    void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &image, TGAColor color, Zbuffer &zbuffer, int width, int height);
    void line(int ax, int ay, int bx, int by, TGAImage &image, TGAColor color);
    static auto barycentric(int ax, int ay, int bx, int by, int cx, int cy, int px, int py) -> vec3d;
    std::tuple<int, int, int> project(vec3f vert, int width = 800, int height = 800);
    float light(vec3f v0, vec3f v1, vec3f v2);

    void render_model(const Model3D &model, Camera &camera, Zbuffer &buffer, TGAImage &image);
    void clear();

private:
    static double square(int ax, int ay, int bx, int by, int cx, int cy);
};

#endif // RENDER_H
