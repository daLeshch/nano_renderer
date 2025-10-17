#include <iostream>
#include <vector>
#include "../lib/tgaimage.h"
#include "render.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

auto Renderer::barycentric(int ax, int ay, int bx, int by, int cx, int cy, int px, int py) -> vec3d
{
    double triangle_sq = square(ax, ay, bx, by, cx, cy);
    if (std::abs(triangle_sq) < 1e-6)
        return vec3d(-1, -1, -1);

    double alpha = square(px, py, bx, by, cx, cy ) / triangle_sq;
    double beta = square(ax, ay, px, py, cx, cy ) / triangle_sq;
    double gamma = square(ax, ay, bx, by, px, py )  / triangle_sq;

    return vec3d(alpha, beta, gamma);
}

void Renderer::triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &image, TGAColor color, Zbuffer &zbuffer)
{
    int bb_min_x = std::min(std::min(ax, bx), cx);
    int bb_min_y = std::min(std::min(ay, by), cy);
    int bb_max_x = std::max(std::max(ax, bx), cx);
    int bb_max_y = std::max(std::max(ay, by), cy);
    double triangle_sq = square(ax, ay, bx, by, cx, cy);
    if (triangle_sq < 1)
        return;

#pragma omp parallel for
    for (int x = bb_min_x; x <= bb_max_x; x++)
    {
        for (int y = bb_min_y; y <= bb_max_y; y++)
        {
            auto bary = barycentric(ax, ay, bx, by, cx, cy, x, y);

            unsigned char z = static_cast<unsigned char>(bary[0] * az + bary[1] * bz + bary[2] * cz);

            if (bary[0] < 0 || bary[1] < 0 || bary[2] < 0)
                continue;

            // std::cout << "x: " << x << " y: " << y << " depth: " << z << "\n";
            double prev_z = zbuffer.get(x, y);

            if (prev_z < z)
            {
                zbuffer.set(x, y, z);
                image.set(x, y, color);
            }
        }
    }
}

void Renderer::line(int ax, int ay, int bx, int by, TGAImage &image, TGAColor color)
{
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep)
    {
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx)
    {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    int y = ay;
    for (int x = ax; x <= bx; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        y += (by - ay) / static_cast<float>(bx - ax);
    }
}

double Renderer::square(int ax, int ay, int bx, int by, int cx, int cy)
{
    return ((bx - ax) * (cy - ay) - (cx - ax) * (by - ay)) * 0.5;
}

Zbuffer::Zbuffer(int swidth, int sheight) : width(swidth), height(sheight), depth_map((width * height)+width, -std::numeric_limits<double>::infinity())
{
    std::cout << "Zbuffer size is: " << depth_map.size() << "\n";
}

Zbuffer::~Zbuffer()
{
}

void Zbuffer::set(int x, int y, double z)
{   
    // std::cout << "Trying to set: : " << y * width + x << "with: " << z << "\n";
    depth_map[y * width + x] = z;
}

double Zbuffer::get(int x, int y)
{
    // std::cout << "Trying to get: : " << y * width + x << "\n";
    return depth_map[y * width + x];
}