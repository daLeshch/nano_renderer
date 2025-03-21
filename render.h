#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "tgaimage.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer,  TGAColor color);

    void line(int ax, int ay, int bx, int by, TGAImage &framebuffer,  TGAColor color);

private:
    double square(int ax, int ay, int bx, int by, int cx, int cy);
};

#endif // RENDER_H
