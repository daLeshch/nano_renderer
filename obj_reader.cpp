#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <random>
#include "tgaimage.h"

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
