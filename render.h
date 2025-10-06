#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "lib/tgaimage.h"
#include "templates.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &image, TGAColor color);
    void line(int ax, int ay, int bx, int by, TGAImage &image, TGAColor color);
    double barycentric(int ax, int ay, int bx, int by, int cx, int cy, int px, int py);

private:
    double square(int ax, int ay, int bx, int by, int cx, int cy);
};

#endif // RENDER_H
