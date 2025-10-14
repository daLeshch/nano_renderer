cmake_minimum_required(VERSION 3.10)
project(nanorenderer)

add_executable(nanorenderer lib/tgaimage.cpp lib/tgaimage.h src/main.cpp src/model.cpp src/render.h src/render.cpp src/templates.h)
set_target_properties(nanorenderer PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)