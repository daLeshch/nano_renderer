#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <algorithm>
#include "tgaimage.h"


constexpr int width = 128;
constexpr int height = 128;

constexpr TGAColor white = { 255, 255, 255, 255};
constexpr TGAColor green = { 0, 255, 0, 255};
constexpr TGAColor red = { 0, 0, 255, 255};
constexpr TGAColor blue = { 255, 128, 64, 255};
constexpr TGAColor yellow = { 0, 200, 255, 255};

TGAImage framebuffer(width, height, TGAImage::RGB);


void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
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
    for (int x = ax; x <= bx; x ++){
        if (steep) {
            framebuffer.set(y, x, color);
        }    
        else {
            framebuffer.set(x, y, color);
        }    
        y += (by - ay) / static_cast<float>(bx - ax);
    }    
}

void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) {
    if (ay>by){ std::swap( ax, bx); std::swap(ay, by);}
    if (ay>cy){ std::swap( ax, cx); std::swap(ay, cy);}
    if (by>cy){ std::swap( bx, cx); std::swap(by, cy);}
    int total_height = cy-ay;

    if (ay != by) {
        int segment_height = by - ay;
        for (int y = ay; y <= by; y++) {
            int x1 = ax + ((cx - ax) * (y - ay)) / total_height;
            int x2 = ax + ((bx - ax) * (y - ay)) / segment_height;
            for (int x = std::min(x1, x2); x < std::max(x1, x2); x++) {
                framebuffer.set(x, y, color);
            }
        }
    
    if (by != cy) {
        int segment_height = cy - by;
        for (int y = by; y <= cy; y++) {
            int x1 = ax + ((cx - ax) * (y - ay)) / total_height;
            int x2 = bx + ((cx - bx) * (y - by)) / segment_height;
            for (int x = std::min(x1, x2); x < std::max(x1, x2); x++) {
                framebuffer.set(x, y, color);
            }
        }
    }
}
}

double square(int ax, int ay, int bx, int by, int cx, int cy) {
    double sq = std::abs((bx - ax) * (cy - ay) - (cx - ax) * (by - ay)) * 0.5;
    return sq;
}

void parallel_triangle( int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) {
    int bb_min_x = std::min(std::min(ax, bx), cx);
    int bb_min_y = std::min(std::min(ay, by), cy);
    int bb_max_x = std::max(std::max(ax, bx), cx);
    int bb_max_y = std::max(std::max(ay, by), cy);
    double triangle_sq = square(ax, ay, bx, by, cx, cy);

    #pragma omp parallel for
    for (int x = bb_min_x; x <= bb_max_x; x++) {
        for (int y = bb_min_y; y <= bb_max_y; y++) {
            double ABP_sq = square(ax, ay, bx, by, x, y);
            double ACP_sq = square(ax, ay, cx, cy, x, y);
            double BCP_sq = square(bx, by, cx, cy, x, y);
            double summ = ABP_sq + ACP_sq + BCP_sq;
            if (std::abs(summ - triangle_sq) < 1e-9) {
                framebuffer.set(x, y, color);
            }
        }
    }

}

int main(int argc, char** argv) {
    parallel_triangle(7, 45, 35, 100, 45,  60, framebuffer, red);
    parallel_triangle(120, 35, 90,   5, 45, 110, framebuffer, white);
    parallel_triangle(115, 83, 80,  90, 85, 120, framebuffer, green);
    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
