#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include "tgaimage.h"
#include "model.cpp"
#include "obj_reader.cpp"

constexpr int width = 800;
constexpr int height = 800;

constexpr TGAColor white = { 255, 255, 255, 255};
constexpr TGAColor green = { 0, 255, 0, 255};
constexpr TGAColor red = { 0, 0, 255, 255};
constexpr TGAColor blue = { 255, 128, 64, 255};
constexpr TGAColor yellow = { 0, 200, 255, 255};



int main(int argc, char** argv) {

    TGAImage framebuffer(width, height, TGAImage::RGB);

    std::string file = "D:\\coding_stuff\\tinyrenderer\\obj\\diablo3_pose\\diablo3_pose.obj";
    // std::string file = "D:\\coding_stuff\\nano_renderer\\doris.obj";
    
    std::vector<std::vector<std::vector<int>>> obj = read_obj_file(file);

    for (const auto& face : obj){
        int count = face.size();
        // std::cout << "Face vector size:  " << count << "\n";

        int ax = face[0][0];
        int ay = face[0][1];
        
        int bx = face[1][0];
        int by = face[1][1];
        
        int cx = face[2][0];
        int cy = face[2][1];
        
        line(ax, ay, bx, by, framebuffer, red);
        line(cx, cy, bx, by, framebuffer, red);
        line(cx, cy, ax, ay, framebuffer, red);       

        framebuffer.set(ax, ay, white);
        framebuffer.set(bx, by, white);
        framebuffer.set(cx, cy, white);
        // std::cout << "Face " << ax << " drawed\n";
    }
    
    framebuffer.write_tga_file("framebuffer.tga");
}


