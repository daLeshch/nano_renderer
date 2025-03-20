#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include "tgaimage.h"
#include "geometry.h"


class Model3D {
    private:
        std::vector<Coordinates3f> vertexes_;
        std::vector<std::vector<int>> faces_;
        std::vector<std::vector<Coordinates3f>> render_obj = {};
    public:
        Model3D(const std::string &filename, const int &width, const int &height);
        ~Model3D();

};

Model3D::Model3D(const std::string &filename, const int &width, const int &height): vertexes_(), faces_() {
    std::string s;
    std::ifstream obj(filename);
    if (!obj.is_open()) {
        std::cerr << "can't open file" << filename << "\n";
        return false;
    }

    while (std::getline(obj, s)) {
        // std::cout << s << "\n";
        char trash;
        if (s.substr(0, 2) == "v ") {
            Coordinates3i vtx_coord;
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
            vertexes_.push_back(vtx_coord);
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
            faces_.push_back(face_coord);
        }
    }
    
    for (const auto& face : faces_){
        std::vector<std::vector<int>> new_face;
        for (const auto& vtx : face) {
            std::vector<int> vtx_coord = vertexes_[vtx-1];
            new_face.push_back(vtx_coord);
            // std::cout << vtx_coord[0] << "   " << vtx_coord[1] << "   " << vtx_coord[2] << "\n";
        }
        render_obj.push_back(new_face);
    }
        
    obj.close();
    std::cout << "Reading finished!" << "\n";
    return true;
}

Model3D::~Model3D()
{
}
