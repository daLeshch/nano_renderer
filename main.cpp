#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include "tgaimage.h"

constexpr int width = 800;
constexpr int height = 800;

constexpr TGAColor white = { 255, 255, 255, 255};
constexpr TGAColor green = { 0, 255, 0, 255};
constexpr TGAColor red = { 0, 0, 255, 255};
constexpr TGAColor blue = { 255, 128, 64, 255};
constexpr TGAColor yellow = { 0, 200, 255, 255};


std::vector<std::vector<std::vector<int>>> read_obj_file(const std::string filename){
    // Пример вводных данных:
    // f 675/695/675 655/672/655 652/668/652
    // v 0.1852 0.490353 -0.0985485

    std::vector<std::vector<std::vector<int>>> render_obj = {};
    std::vector<std::vector<int>> vertexes = {};
    std::vector<std::vector<int>> faces = {};
    std::string s;
    std::ifstream obj(filename);
    if (!obj.is_open()) {
        std::cerr << "can't open file" << filename << "\n";
        return render_obj;
    }

    while (std::getline(obj, s)) {
        // std::cout << s << "\n";
        if (s.substr(0, 2) == "v ") {
            std::vector<int> vtx_coord;
            std::stringstream vtx_data(s);
            std::string scoord;
            vtx_data >> scoord;



            while (vtx_data >> scoord) {
                float coord = std::stof(scoord);
                int icoord = (coord + 1.) * width/2;
                vtx_coord.push_back(icoord);
            }
            vertexes.push_back(vtx_coord);
        } else if (s.substr(0, 2) == "f ") {
            std::vector<int> face_coord;
            std::stringstream face_data(s);
            std::string scoord;
            face_data >> scoord;
            while (face_data >> scoord) {
                int indx = std::stoi(scoord);
                face_coord.push_back(indx);
            }
            faces.push_back(face_coord);
        }
    }
    
    for (const auto& face : faces){
        std::vector<std::vector<int>> new_face;
        for (const auto& vtx : face) {
            std::vector<int> vtx_coord = vertexes[vtx-1];
            new_face.push_back(vtx_coord);
        }
        render_obj.push_back(new_face);
    }
        
    obj.close();
    std::cout << "Reading finished!" << "\n";
    return render_obj;
}

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color){
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx) {
        std::swap(ax, ay);
        std::swap(bx, by);
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


int main(int argc, char** argv) {

    TGAImage framebuffer(width, height, TGAImage::RGB);

    std::string file = "D:\\coding_stuff\\tinyrenderer\\obj\\diablo3_pose\\diablo3_pose.obj";

    std::vector<std::vector<std::vector<int>>> obj = read_obj_file(file);

    for (const auto& face : obj){
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
        std::cout << "Face " << ax << " drawed\n";
    }
    
    framebuffer.write_tga_file("framebuffer.tga");
}


