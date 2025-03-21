#ifndef MODEL3D_H
#define MODEL3D_H

#include <vector>
#include "geometry.h"

class Model3D {
    private:
        std::vector<Coordinates3f> vertexes_;
        std::vector<Coordinates3f> world_vertexes_;
        std::vector<std::vector<int>> faces_;
    public:
        std::vector<std::vector<Coordinates3f>> render_obj;
        Model3D(const std::string &filename, const int &width, const int &height);
        ~Model3D();
        Coordinates3f world_coords(int idx);
};


#endif // MODEL3D_H