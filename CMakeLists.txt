cmake_minimum_required(VERSION 3.10)
project(SocialRed)

set(CMAKE_CXX_STANDARD 17)

# Archivos fuente
set(SOURCES
        main.cpp
        imgui.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
        imgui_impl_glfw.cpp
        imgui_impl_opengl3.cpp
)

# Directorios de encabezados
include_directories(.)

# Bibliotecas necesarias
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(SocialRed ${SOURCES})

target_link_libraries(SocialRed PRIVATE OpenGL::GL glfw)
add_executable(main main.cpp)
add_executable(RedSocial RedSocial.cpp)