#include <iostream>
#include <vector>
#include "tgaimage.h"
#include "render.h"


Renderer::Renderer(){
}

Renderer::~Renderer(){
}
double Renderer::barycentric(int ax, int ay, int bx, int by, int cx, int cy, int px, int py) {
    double triangle_sq = square(ax, ay, bx, by, cx, cy);
    if (triangle_sq < 1) return 2;
    double ABP_sq = square(ax, ay, bx, by, px, py);
    double ACP_sq = square(ax, ay, cx, cy, px, py);
    double BCP_sq = square(bx, by, cx, cy, px, py);
    double summ = ABP_sq + ACP_sq + BCP_sq;
    return std::abs(summ - triangle_sq);
}
void Renderer::triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color)
{
    int bb_min_x = std::min(std::min(ax, bx), cx);
    int bb_min_y = std::min(std::min(ay, by), cy);
    int bb_max_x = std::max(std::max(ax, bx), cx);
    int bb_max_y = std::max(std::max(ay, by), cy);
    double triangle_sq = square(ax, ay, bx, by, cx, cy);
    if (triangle_sq < 1) return;

    #pragma omp parallel for
    for (int x = bb_min_x; x <= bb_max_x; x++) {
        for (int y = bb_min_y; y <= bb_max_y; y++) {
            double bary = barycentric(ax, ay, bx, by, cx, cy, x, y);
            if (bary < 1e-9) {
                framebuffer.set(x, y, color);
            }
        }
    }
}

void Renderer::line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color)
{
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    float y = ay;
    for (int x = ax; x <= bx; x++) {
        if (steep) {
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
        y += (by - ay) / static_cast<float>(bx - ax);
    }
}

double Renderer::square(int ax, int ay, int bx, int by, int cx, int cy)
{
    return std::abs((bx - ax) * (cy - ay) - (cx - ax) * (by - ay)) * 0.5;
}

