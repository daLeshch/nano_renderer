#include "render.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

auto Renderer::barycentric(int ax, int ay, int bx, int by, int cx, int cy, int px, int py) -> vec3d
{
    double triangle_sq = square(ax, ay, bx, by, cx, cy);
    if (std::abs(triangle_sq) < 1e-6)
        return vec3d(-1, -1, -1);

    double alpha = square(px, py, bx, by, cx, cy) / triangle_sq;
    double beta = square(ax, ay, px, py, cx, cy) / triangle_sq;
    double gamma = square(ax, ay, bx, by, px, py) / triangle_sq;

    return vec3d(alpha, beta, gamma);
}

std::tuple<int, int, int> Renderer::project(vec3f vert, int width, int height)
{
    return {((vert.x + 1.) * width / 2),
            (vert.y + 1.) * height / 2,
            (vert.z + 1.) * 255 / 2};
}

float Renderer::light(vec3f v0, vec3f v1, vec3f v2)
{
    vec3f light_dir(0, 0, -1);
    light_dir.normalize();
    vec3f normal = (v2 - v0) ^ (v1 - v0);
    normal.normalize();
    return normal * light_dir;
}

void Renderer::render_model(const Model3D &model, Camera &camera, Zbuffer &buffer, TGAImage &image)
{
    for (const auto &face : model.render_obj)
    {

        auto nf0 = camera.view_persp(face[0]);
        auto nf1 = camera.view_persp(face[1]);
        auto nf2 = camera.view_persp(face[2]);

        float intensity = light(nf0, nf1, nf2);

        auto [ax, ay, az] = camera.screen(nf0);
        auto [bx, by, bz] = camera.screen(nf1);
        auto [cx, cy, cz] = camera.screen(nf2);

        ax = std::clamp(ax, 0, width - 1);
        ay = std::clamp(ay, 0, height - 1);
        bx = std::clamp(bx, 0, width - 1);
        by = std::clamp(by, 0, height - 1);
        cx = std::clamp(cx, 0, width - 1);
        cy = std::clamp(cy, 0, height - 1);

        if (intensity > 0)
        {
            TGAColor actual_color = {intensity * 255, intensity * 255, intensity * 255, 255};
            triangle(ax, ay, az, bx, by, bz, cx, cy, cz, image, actual_color, buffer, width, height);
        }
    }
}

void Renderer::clear()
{
}

void Renderer::triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &image, TGAColor color, Zbuffer &zbuffer, int width, int height)
{
    int bb_min_x = std::min(std::min(ax, bx), cx);
    int bb_min_y = std::min(std::min(ay, by), cy);
    int bb_max_x = std::max(std::max(ax, bx), cx);
    int bb_max_y = std::max(std::max(ay, by), cy);
    double triangle_sq = square(ax, ay, bx, by, cx, cy);

    if (triangle_sq < 1)
        return;

    if (bb_max_x < 0 || bb_max_y < 0 || bb_min_x >= width || bb_min_y >= height)
        return;

#pragma omp parallel for
    for (int x = bb_min_x; x <= bb_max_x; x++)
    {
        for (int y = bb_min_y; y <= bb_max_y; y++)
        {
            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                auto bary = barycentric(ax, ay, bx, by, cx, cy, x, y);

                double z = static_cast<double>(bary[0] * az + bary[1] * bz + bary[2] * cz);

                if (bary[0] < 0 || bary[1] < 0 || bary[2] < 0)
                    continue;

                double prev_z = zbuffer.get(x, y);

                if (prev_z < z)
                {
                    zbuffer.set(x, y, z);
                    image.set(x, y, color);
                }
            }
        }
    }
}

void Renderer::line(int ax, int ay, int bx, int by, TGAImage &image, TGAColor color)
{
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep)
    {
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx)
    {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    int y = ay;
    for (int x = ax; x <= bx; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        y += (by - ay) / static_cast<float>(bx - ax);
    }
}

double Renderer::square(int ax, int ay, int bx, int by, int cx, int cy)
{
    return ((bx - ax) * (cy - ay) - (cx - ax) * (by - ay)) * 0.5;
}

Zbuffer::Zbuffer(int swidth, int sheight) : width(swidth), height(sheight), depth_map((width * height) + width, -std::numeric_limits<double>::infinity())
{
}

Zbuffer::~Zbuffer()
{
}

void Zbuffer::clear()
{
    std::fill(depth_map.begin(), depth_map.end(), -std::numeric_limits<float>::infinity());
}

void Zbuffer::set(int x, int y, double z)
{
    depth_map[y * width + x] = z;
}

double Zbuffer::get(int x, int y)
{
    return depth_map[y * width + x];
}

Camera::Camera(const vec3f &eye, const vec3f &target, const vec3f &up)
{
    zax = (eye - target).normalize();
    xax = (up ^ zax).normalize();
    yax = (zax ^ xax).normalize();

    view_matrix = {
        {xax.x, xax.y, xax.z, -(xax * eye)},
        {yax.x, yax.y, yax.z, -(yax * eye)},
        {zax.x, zax.y, zax.z, -(zax * eye)},
        {0, 0, 0, 1}};

    screen_matrix = {
        {w / 2.f, 0, 0, w / 2.f},
        {0, h / 2.f, 0, h / 2.f},
        {0, 0, 255 / 2.f, 255 / 2.f},
        {0, 0, 0, 1}};

    persp_matrix = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, -1 / f, 1}};
}

vec3f Camera::view_persp(const vec3f &point)
{
    vec4f p = {point.x, point.y, point.z, 1};
    p = view_matrix * p;
    p = persp_matrix * p;
    p = p / p.w;
    return {p.x, p.y, p.z};
}

std::tuple<int, int, int> Camera::screen(const vec3f &point)
{
    vec4f p = {point.x, point.y, point.z, 1};
    p = screen_matrix * p;
    return {p.x, p.y, p.z};
}
