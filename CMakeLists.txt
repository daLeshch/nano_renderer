cmake_minimum_required(VERSION 3.10)

project(nanorenderer)

add_executable(nanorenderer lib/tgaimage.cpp lib/tgaimage.h main.cpp model.h model.cpp render.h render.cpp templates.h)
set_target_properties(nanorenderer PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)