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

    double alpha = square(px, py, bx, by, cx, cy ) / triangle_sq;
    double beta = square(ax, ay, px, py, cx, cy ) / triangle_sq;
    double gamma = square(ax, ay, bx, by, px, py )  / triangle_sq;

    return vec3d(alpha, beta, gamma);
}


std::tuple<int,int,int> Renderer::project(vec3f vert, int width, int height)
{   return    {((vert.x + 1.) * width / 2),
        (vert.y + 1.) * height / 2,
        (vert.z + 1.) * 255 / 2};
}

float Renderer::light(vec3f v0, vec3f v1, vec3f v2)
{   vec3f light_dir(0, 0, -1);
    light_dir.normalize();
    vec3f normal = (v2 - v0) ^ (v1 - v0);
    normal.normalize();
    return normal * light_dir;
}

vec3f Renderer::normilize(const vec3f &vertex, const vec3f& max_coord)
{   
    float max_val = std::max({std::abs(max_coord.x), std::abs(max_coord.y), std::abs(max_coord.z)});
    vec3f nv = vertex;
    nv.x /= max_val;
    nv.y /= max_val;
    nv.z /= max_val;
    return nv;
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

    if (bb_max_x < 0 || bb_max_y < 0 || bb_min_x >= width || bb_min_y >= height) return;

    // // корректим границы бокса чтобы не выходили за экран
    // bb_min_x = std::max(0, bb_min_x);
    // bb_min_y = std::max(0, bb_min_y);
    // bb_max_x = std::min(width-1, bb_max_x);
    // bb_max_y = std::min(height-1, bb_max_y);

#pragma omp parallel for
    for (int x = bb_min_x; x <= bb_max_x; x++)
    {
        // std::cout << "its x: " << x << "\n";
        for (int y = bb_min_y; y <= bb_max_y; y++)
        {   
            if (x >= 0 && x < width && y >= 0 && y < height){ // на всякий случай проверяем точно ли все в границах
                auto bary = barycentric(ax, ay, bx, by, cx, cy, x, y);
                
                unsigned char z = static_cast<unsigned char>(bary[0] * az + bary[1] * bz + bary[2] * cz);
                
                if (bary[0] < 0 || bary[1] < 0 || bary[2] < 0)
                continue;
                
                // std::cout << "x: " << x << " y: " << y << " depth: " << z << "\n";
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

Zbuffer::Zbuffer(int swidth, int sheight) : width(swidth), height(sheight), depth_map((width * height)+width, -std::numeric_limits<double>::infinity())
{
    std::cout << "Zbuffer size is: " << depth_map.size() << "\n";
}

Zbuffer::~Zbuffer()
{
}

void Zbuffer::set(int x, int y, double z)
{   
    // std::cout << "Trying to set: : " << y * width + x << "with: " << z << "\n";
    depth_map[y * width + x] = z;
}

double Zbuffer::get(int x, int y)
{
    // std::cout << "Trying to get: : " << y * width + x << "\n";
    return depth_map[y * width + x];
}

bool Viewport::init(int width, int height)
{    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial",  width, height, SDL_WINDOW_RESIZABLE);
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

bool Viewport::load_media(const char *media)
{
     //Loading success flag
    bool success = true;

    //Load splash image
    gHelloWorld = SDL_LoadBMP( media );
    if( gHelloWorld == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", media, SDL_GetError() );
        success = false;
    }

    return success;
}

void Viewport::close()
{
    //Deallocate surface
    SDL_DestroySurface( gHelloWorld );
    gHelloWorld = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void Viewport::create(const char *media, int width, int height)
{
        //Start up SDL and create window
    if( !init(width, height) )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !load_media(media) )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Apply the image
            SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
        }
    }
}

void Viewport::showImage(TGAImage &image, int width, int height)
{
    // Создаем окно
    if (!init(width, height)) return;

    // Получаем указатель на пиксели окна
    SDL_LockSurface(gScreenSurface); // блокируем surface для записи
    uint32_t* screenPixels = (uint32_t*)gScreenSurface->pixels;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            TGAColor c = image.get(x, height - 1 - y); // берём снизу-вверх
            screenPixels[y * width + x] =
                (c.bgra[3] << 24) | (c.bgra[2] << 16) | (c.bgra[1] << 8) | c.bgra[0];
        }
    }

    SDL_UnlockSurface(gScreenSurface);

    SDL_UpdateWindowSurface(gWindow); // обновляем окно

    // Простейший цикл событий, чтобы окно не закрывалось сразу
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = true;
        }
        SDL_Delay(16);
    }

    close();
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
        {0,     0,     0,      1}
    };
    persp_matrix = {
        {(f/aspect), 0, 0, 0},
        {0, f, 0, 0},
        {0, 0, (far_clip+near_clip)/(near_clip-far_clip), (2*far_clip*near_clip)/(near_clip-far_clip)},
        {0, 0, -1, 0}
    };    
    screen_matrix = {
        {w/2.f, 0, 0, w/2.f},
        {0, h/2.f, 0, h/2.f},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}


vec3f Camera::view_point(const vec3f &point)
{
    vec4f p = {point.x, point.y, point.z, 1};
    p = view_matrix * p;

    if (p.w != 0) {
        p.x /= p.w;
        p.y /= p.w;
        p.z /= p.w;
    }

    vec3f np = {p.x, p.y, p.z};
    return np;
}

vec3f Camera::persp(vec3f vert)
{   
    // float scale = 1.;
    // constexpr double c = 5.; // магическая чиселка показывает как далеко находится плоскость проекции, потом заменится
    // return face * scale /(1-face.z/c);
    return vert;
}
