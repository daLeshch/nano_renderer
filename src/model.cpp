#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "templates.h"

class Model3D
{
public:
    std::vector<std::vector<vec3f>> render_obj;
    Model3D(const std::string &filename, const int &width, const int &height) : vertexes_(), faces_()
    {
        std::string s;
        std::ifstream obj(filename);
        if (!obj.is_open())
        {
            std::cerr << "can't open file" << filename << "\n";
            return;
        }
        std::cout << "File opened" << "\n";
        while (std::getline(obj, s))
        {
            char trash;
            if (s.substr(0, 2) == "v ")
            {
                std::stringstream vtx_data(s);
                vtx_data >> trash;
                float x, y, z;
                while (vtx_data >> x >> y >> z)
                {
                    vec3f vtx_coord(x, y, z);
                    vertexes_.push_back(vtx_coord);
                }
            }
            else if (s.substr(0, 2) == "f ")
            {
                std::vector<int> face_coord;
                std::stringstream face_data(s);
                std::string scoord;
                face_data >> trash;
                while (face_data >> scoord)
                {
                    int n = std::stoi(scoord);
                    face_coord.push_back(n);
                }
                faces_.push_back(face_coord);
            }
        }

        for (const auto &face : faces_)
        {
            std::vector<vec3f> new_face;
            for (const auto &vtx : face)
            {
                new_face.push_back(vertexes_[vtx - 1]);
            }
            render_obj.push_back(new_face);
        }

        obj.close();
        std::cout << "Reading finished!" << "\n";
    };
    ~Model3D() {
    };

private:
    std::vector<vec3f> vertexes_;
    std::vector<vec3f> world_vertexes_;
    std::vector<std::vector<int>> faces_;
};
