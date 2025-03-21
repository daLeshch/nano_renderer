cmake_minimum_required(VERSION 3.10)

project(nanorenderer)

add_executable(nanorenderer tgaimage.cpp tgaimage.h main.cpp model.h model.cpp render.h render.cpp geometry.h)
set_target_properties(nanorenderer PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)