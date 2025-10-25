#include <cstring>
#include <random>
#include <filesystem>
#include "math_core.h"
#include "model.h"
#include "render.h"

constexpr int width = 800;
constexpr int height = 800;

TGAImage image(width, height, TGAImage::RGB);
Zbuffer buffer(width, height);

struct Button
{
    SDL_FRect rect;
    std::string label;
};

namespace fs = std::filesystem;

auto getRoot(fs::path path)
{
    std::cout << "current path is: " << path << "\n";
    if (path.filename() == "nano_renderer")
        return path;
    path = path.parent_path();
    std::cout << "new path is: " << path << "\n";
    return getRoot(path);
}

std::string getAssetPath(const std::string &relative)
{
    fs::path exe = fs::current_path();
    fs::path root = getRoot(exe);
    fs::path fullPath = root / "assets" / relative;
    return fullPath.string();
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("viewport", 800, 800, SDL_WINDOW_MAXIMIZED);
    SDL_Renderer *sdl_renderer = SDL_CreateRenderer(window, nullptr);
    SDL_Texture *texture = SDL_CreateTexture(
        sdl_renderer,
        (image.get_bpp() == 3 ? SDL_PIXELFORMAT_RGB24 : SDL_PIXELFORMAT_RGBA32),
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height);

    std::vector<Button> buttons = {
        {{10.0f, 10.0f, 150.0f, 40.0f}, "diablo3_pose.obj"},
        {{10.0f, 60.0f, 150.0f, 40.0f}, "spere_zero.obj"},
        {{10.0f, 110.0f, 150.0f, 40.0f}, "matilda.obj"},
        {{10.0f, 160.0f, 150.0f, 40.0f}, "RoninFinalS.obj"}};

    Model3D model(getAssetPath(buttons[0].label), width, height);
    SDL_Color buttonColorNormal = {100, 100, 200, 255};
    SDL_Color buttonColorHover = {200, 50, 50, 255};

    Camera camera;
    Renderer renderer(width, height);

    bool running = true;
    bool rotating = false;

    float phi = 1.5f;    // право-лево, 1.5 == камера фронт
    float theta = 1.5f;  // верх-низ, 0 == камера топ, 1.5 фронт
    float radius = 1.0f; // зум

    int lastX = 0, lastY = 0;

    const vec3f &target = {0, 0, 0};
    const vec3f &up = {0, 1, 0};

    while (running)
    {
        uint32_t frameStart = SDL_GetTicks();
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    int mx = e.button.x;
                    int my = e.button.y;
                    for (auto &btn : buttons)
                    {
                        if (mx >= btn.rect.x && mx <= btn.rect.x + btn.rect.w &&
                            my >= btn.rect.y && my <= btn.rect.y + btn.rect.h)
                        {
                            std::cout << "Selected model: " << btn.label << "\n";
                            model = Model3D(getAssetPath(btn.label), width, height);
                        }
                    }
                    rotating = true;
                    lastX = e.button.x;
                    lastY = e.button.y;
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (e.button.button == SDL_BUTTON_LEFT)
                    rotating = false;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                if (rotating)
                {
                    int dx = e.motion.x - lastX;
                    int dy = e.motion.y - lastY;
                    const float sensitivity = 0.005f;
                    phi += dx * sensitivity;
                    theta -= dy * sensitivity;
                    const float epsilon = 0.01f;
                    if (theta < epsilon)
                        theta = epsilon;
                    if (theta > M_PI - epsilon)
                        theta = M_PI - epsilon;
                    lastX = e.motion.x;
                    lastY = e.motion.y;
                }
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                radius -= e.wheel.y * 0.1f;
                if (radius < 0.3f)
                    radius = 0.3f;
                if (radius > 50.0f)
                    radius = 50.0f;
                break;
            }
        }

        vec3f cameraPos = {
            target.x + radius * std::cos(phi) * std::sin(theta),
            target.y + radius * std::cos(theta),
            target.z + radius * std::sin(phi) * std::sin(theta)};

        camera = Camera(cameraPos, target, up);

        image.clear();
        buffer.clear();
        renderer.render_model(model, camera, buffer, image);
        image.flip_vertically();

        SDL_UpdateTexture(texture, nullptr, image.framebuffer_ptr(), width * image.get_bpp());
        SDL_RenderClear(sdl_renderer);
        SDL_RenderTexture(sdl_renderer, texture, nullptr, nullptr);

        for (auto &btn : buttons)
        {
            float mx, my;
            SDL_GetMouseState(&mx, &my);

            bool hover = mx >= btn.rect.x && mx <= btn.rect.x + btn.rect.w &&
                         my >= btn.rect.y && my <= btn.rect.y + btn.rect.h;

            SDL_SetRenderDrawColor(sdl_renderer,
                                   hover ? buttonColorHover.r : buttonColorNormal.r,
                                   hover ? buttonColorHover.g : buttonColorNormal.g,
                                   hover ? buttonColorHover.b : buttonColorNormal.b,
                                   255);
            SDL_RenderFillRect(sdl_renderer, &btn.rect);
        }
        SDL_RenderPresent(sdl_renderer);
        uint32_t frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16)
            SDL_Delay(16 - frameTime);
    }

    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    image.flip_vertically();
    image.write_tga_file("nano_render_result.tga");

    std::cout << "Render finished!\n";
    system("pause");
}