# Nano Renderer

> *"What I cannot create, I do not understand." --- Richard Feynman*

## Overview

**Nano Renderer** is a cross‑platform, minimalist software renderer
written in C++.\
Its primary goal is to explore how modern graphics pipelines work by
manually implementing the fundamental components of a typical 3D engine.

>To keep the focus on 3D rendering itself, two non‑graphics components
>use existing third‑party solutions: 
>- SDL3 for displaying the viewport window
>- A ready‑made TGA writer for image export

------------------------------------------------------------------------

## 🚀 Features

-   Loading `.obj` models
-   Rasterizing triangulated meshes
-   Interactive viewport preview
-   Exporting rendered images to `.tga`

### Planned additions

-   Texture support
-   Adjustable light sources
-   Ambient occlusion
-   Additional model formats
-   Additional image export formats

------------------------------------------------------------------------

## ⚙️ Building

### Requirements

-   **C++20**
-   **CMake ≥ 3.21**
-   **Internet connection** (required for automatic SDL3 download)

### Build steps (Windows / Linux / macOS)

``` bash
mkdir build
cd build
cmake ..
cmake --build . --config release
```

After building, a `Release` folder will appear inside `bin/`, containing
the executable and required libraries.

------------------------------------------------------------------------

## 🖥️ Demo

### Interface
![main_window](https://github.com/user-attachments/assets/b832df3b-ab10-4f5f-a9c1-474cf450d9a9)

The UI allows switching between included demo models, each with a
different vertex count for scalability testing.

### Early version
![legacy_demo](https://github.com/user-attachments/assets/8608b26b-86f0-4e35-85ea-2d3dc1fb851d)

The first prototype could only render from a fixed camera.
Its output was a single `.tga` image based on the model placed next to
the program source.

### Current version
![demo](https://github.com/user-attachments/assets/6c514798-5728-4873-9be9-5abe3c36135e)

The application now features: - A realtime viewport
- Camera controls
- Selection of several `.obj` samples
- Automatic `.tga` export based on the last viewed model and camera
state

### Examples of application output at different stages
**Line rendering:**

![lines_draw](https://github.com/user-attachments/assets/5ed07703-ab4f-43cd-baea-2a024715eae2)

### Filled triangles, simplified orthographic projection, no z-buffer

![lagacy](https://github.com/user-attachments/assets/ce01597d-1de6-465e-928d-eddb8c62779a)

### Added z-buffer

![zbuffer](https://github.com/user-attachments/assets/77d6afa6-9421-4ba5-bf80-08c1fc8c27e0)

### Camera with perspective projection

![persp](https://github.com/user-attachments/assets/da3a6281-e51a-4a8b-b746-0208e02f3b18)
------------------------------------------------------------------------

## 📁 Project Structure

-   **src** --- core source code
-   **assets** --- demo models and example outputs
-   **bin** --- compiled builds
-   **lib** --- third‑party libraries (e.g., tgaimage)
-   [Documentation](https://daleshch.github.io/nano_renderer/)

### Dependencies

-   **SDL3** --- viewport window
-   **tgaimage** --- `.tga` export
  >All third‑party libraries are downloaded automatically via CMake during the first build.

------------------------------------------------------------------------

## 📚 Helpful Resources

- [Detailed article series on 3D rendering fundamentals](https://haqr.eu/)
- [Comprehensive OpenGL theory book](https://learnopengl.com/)
- [SDL tutorials](https://lazyfoo.net/tutorials/SDL/index.php)
- [Linear algebra essentials by 3Blue1Brown](https://youtube.com/playlist?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab&si=Kx5wlbKsYD5zDGR0)
