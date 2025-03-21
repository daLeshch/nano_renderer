#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "model.h"


Model3D::Model3D(const std::string &filename, const int &width, const int &height): vertexes_(), faces_() {
    std::string s;
    std::ifstream obj(filename);
    if (!obj.is_open()) {
        std::cerr << "can't open file" << filename << "\n";
        return;
    }
    std::cout << "File opened" << "\n";
    while (std::getline(obj, s)) {
        // std::cout << s << "\n";
        char trash;
        if (s.substr(0, 2) == "v ") {
            std::stringstream vtx_data(s);
            vtx_data >> trash;
            float x, y, z;
            while (vtx_data >> x >> y >> z) {
                // int m_x = (x + 1.) * width/2;
                // int m_y = (y + 1.) * height/2;
                Coordinates3f vtx_coord(x, y, z);
                // Coordinates3f wrld_coord(x, y, z);
                vertexes_.push_back(vtx_coord);
                // world_vertexes_.push_back(wrld_coord);
            }
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
        std::vector<Coordinates3f> new_face;
        for (const auto& vtx : face) {
            new_face.push_back(vertexes_[vtx-1]);
            // std::cout << vtx_coord[0] << "   " << vtx_coord[1] << "   " << vtx_coord[2] << "\n";
        }
        render_obj.push_back(new_face);
    }
        
    obj.close();
    std::cout << "Reading finished!" << "\n";
}

Model3D::~Model3D()
{
}

Coordinates3f Model3D::world_coords(int idx)
{
    return world_vertexes_[idx];
}
