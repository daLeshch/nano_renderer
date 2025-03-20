#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <random>
#include "tgaimage.h"
#include "geometry.h"
// #include "model.cpp"
// #include "render.cpp"


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
        char trash;
        if (s.substr(0, 2) == "v ") {
            std::vector<int> vtx_coord;
            std::stringstream vtx_data(s);
            vtx_data >> trash;
            float x, y, z;
            while (vtx_data >> x >> y >> z) {
                int m_x = (x + 1.) * width/2;
                int m_y = (y + 1.) * height/2;
                vtx_coord.push_back(m_x);
                vtx_coord.push_back(m_y);
                // vtx_coord.push_back(z);
            }
            vertexes.push_back(vtx_coord);
        } 
        else if (s.substr(0, 2) == "f ") {
            std::vector<int> face_coord;
            std::stringstream face_data(s);
            std::string scoord;
            face_data >> trash;
            while (face_data >> scoord) {
                int n = std::stoi(scoord);
                face_coord.push_back(n);
            }
            faces.push_back(face_coord);
        }
    }
    
    for (const auto& face : faces){
        std::vector<std::vector<int>> new_face;
        for (const auto& vtx : face) {
            std::vector<int> vtx_coord = vertexes[vtx-1];
            new_face.push_back(vtx_coord);
            // std::cout << vtx_coord[0] << "   " << vtx_coord[1] << "   " << vtx_coord[2] << "\n";
        }
        render_obj.push_back(new_face);
    }
        
    obj.close();
    std::cout << "Reading finished!" << "\n";
    return render_obj;
}

double square(int ax, int ay, int bx, int by, int cx, int cy) {
    double sq = std::abs((bx - ax) * (cy - ay) - (cx - ax) * (by - ay)) * 0.5;
    return sq;
}

void triangle( int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) {
    int bb_min_x = std::min(std::min(ax, bx), cx);
    int bb_min_y = std::min(std::min(ay, by), cy);
    int bb_max_x = std::max(std::max(ax, bx), cx);
    int bb_max_y = std::max(std::max(ay, by), cy);
    double triangle_sq = square(ax, ay, bx, by, cx, cy);
    if (triangle_sq < 1) return;

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
        
        std::vector<TGAColor> rand = {white, red, green, blue, yellow};
        std::random_device random_dev;
        std::mt19937 engine{random_dev()};
        std::uniform_int_distribution<int> dist(0, rand.size() - 1);

        TGAColor random_color = rand[dist(engine)];

        triangle(ax, ay, bx, by, cx, cy, framebuffer, random_color);
        line(ax, ay, bx, by, framebuffer, red);
        line(ax, ay, cx, cy, framebuffer, red);
        line(bx, by, cx, cy, framebuffer, red);
        
        framebuffer.set(ax, ay, white);
        framebuffer.set(bx, by, white);
        framebuffer.set(cx, cy, white);
        // std::cout << "Face " << ax << " drawed\n";
    }
    
    framebuffer.write_tga_file("framebuffer.tga");
}


