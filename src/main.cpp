#include <cstring>
#include <string>
#include <random>
#include <filesystem>
#include "../lib/tgaimage.h"
#include "templates.h"
#include "model.cpp"
#include "render.h"

constexpr int width = 800;
constexpr int height = 800;

TGAImage image(width, height, TGAImage::RGB);
Zbuffer buffer(width, height);
Model3D *model = NULL;

namespace fs = std::filesystem;

std::string getAssetPath(const std::string& relative){
    fs::path sourceFile = __FILE__;
    fs::path baseDir = sourceFile.parent_path();
    fs::path fullPath = baseDir / ".." / "assets" / relative;
    return fullPath.string();
}

int main(int argc, char **argv)
{
    std::string file = getAssetPath("diablo3_pose.obj");

    // std::string file = ("diablo3_pose.obj");
    // std::string file = ("D:\\coding_stuff\\nano_renderer\\assets\\diablo3_pose.obj");
    if (4 == argc)
    {
        int w;
        int h;
        w = std::stoi(argv[2]);
        h = std::stoi(argv[3]);
        model = new Model3D(argv[1], w, h);
        std::cout << "Opened custom file with custom picture size settings!" << "\n";
    }
    else if (2 == argc)
    {
        model = new Model3D(argv[1], width, height);
        std::cout << "Opened custom file with default picture size settings!" << "\n";
    }
    else
    {
        model = new Model3D(file, width, height);
        std::cout << "Opened default file with default picture size settings!" << "\n";
    }

    Renderer renderer;

    for (const auto &face : model->render_obj)
    {   
        auto [ax, ay, az] = renderer.project(renderer.persp(face[0]));

        auto [bx, by, bz] = renderer.project(renderer.persp(face[1]));
        
        auto [cx, cy, cz] = renderer.project(renderer.persp(face[2]));

        float intensity = renderer.light(face[0], face[1], face[2]);;

        if (intensity > 0)
        {
            TGAColor actual_color = {intensity * 255, intensity * 255, intensity * 255, 255};
            renderer.triangle(ax, ay, az, bx, by, bz, cx, cy, cz, image, actual_color, buffer);
        }
    }

    image.write_tga_file("nano_render_result.tga");
    std::cout << "Render finished!\n";
    system("pause");
}
