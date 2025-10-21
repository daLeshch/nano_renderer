#include <cstring>
#include <string>
#include <random>
#include <filesystem>
#include "templates.h"
#include "model.cpp"
#include "render.h"

constexpr int width = 800;
constexpr int height = 800;

TGAImage image(width, height, TGAImage::RGB);
Zbuffer buffer(width, height);
Model3D *model = NULL;

// namespace fs = std::filesystem;

// std::string getAssetPath(const std::string& relative){
//     fs::path sourceFile = __FILE__;
//     fs::path baseDir = sourceFile.parent_path();
//     fs::path fullPath = baseDir / ".." / "assets" / relative;
//     return fullPath.string();
// }

namespace fs = std::filesystem;

std::string getAssetPath(const std::string& relative) {
    // где реально лежит исполняемый файл
    fs::path exe = fs::current_path();
    fs::path fullPath = exe / ".." / "assets" / relative;
    return fullPath.string();
}

int main(int argc, char **argv)
{
    std::string file = getAssetPath("diablo3_pose.obj");
    // std::string file = getAssetPath("spere_non_zero.obj");

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

    const vec3f& eye = {0, 0, 3};
    const vec3f& target = {0, 0, 0};
    const vec3f& up = {0, 1, 0};

    Camera camera(eye, target, up);
    
    
    for (const auto &face : model->render_obj)
    {   

        auto nf0 = camera.view_point(face[0]);
        auto nf1 = camera.view_point(face[1]);
        auto nf2 = camera.view_point(face[2]);

        float intensity = renderer.light(nf0, nf1, nf2);

        auto [ax, ay, az] = renderer.project(nf0);
        auto [bx, by, bz] = renderer.project(nf1);
        auto [cx, cy, cz] = renderer.project(nf2);

        // // корректим точки чтобы не выходили за экран
        // ax = std::clamp(ax, 0, width - 1);
        // ay = std::clamp(ay, 0, height - 1);
        // bx = std::clamp(bx, 0, width - 1);
        // by = std::clamp(by, 0, height - 1);
        // cx = std::clamp(cx, 0, width - 1);
        // cy = std::clamp(cy, 0, height - 1);


        if (intensity > 0)
        {
            TGAColor actual_color = {intensity * 255, intensity * 255, intensity * 255, 255};
            renderer.triangle(ax, ay, az, bx, by, bz, cx, cy, cz, image, actual_color, buffer, width, height);
        }
    }
    std::cout << "max coords: " << model->max_coord[0] << model->max_coord[1] << model->max_coord[2] << "\n";
    image.write_tga_file("nano_render_result.tga");

    Viewport view;

    view.showImage(image, width, height);

    std::cout << "Render finished!\n";
    system("pause");
}
