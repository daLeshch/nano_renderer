cmake_minimum_required(VERSION 3.10)
project(nanorenderer)

# Основной исполняемый файл
add_executable(nanorenderer
    lib/tgaimage.cpp
    lib/tgaimage.h
    src/main.cpp
    src/model.cpp
    src/render.h
    src/render.cpp
    src/templates.h
)

set_target_properties(nanorenderer PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# # Тесты
# add_executable(tests
#     src/tests.cpp
#     src/templates.h
#     # lib/tgaimage.cpp
#     # lib/tgaimage.h
#     # src/model.cpp   # если тесты используют модель
#     # src/render.cpp  # если тесты используют рендер
#     # src/render.h
# )

# set_target_properties(tests PROPERTIES
#     CXX_STANDARD 20
#     CXX_STANDARD_REQUIRED ON
# )
