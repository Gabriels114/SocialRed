cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)

project(SocialRed)

set(CMAKE_CXX_STANDARD 17)

# Directorios
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

# Archivos fuente principales
set(SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/RedSocial.cpp
        ${SRC_DIR}/MatrizAdyacencia.cpp
        ${SRC_DIR}/Menu.cpp
        ${THIRD_PARTY_DIR}/imgui.cpp
        ${THIRD_PARTY_DIR}/imgui_draw.cpp
        ${THIRD_PARTY_DIR}/imgui_tables.cpp
        ${THIRD_PARTY_DIR}/imgui_widgets.cpp
        ${THIRD_PARTY_DIR}/imgui_impl_glfw.cpp
        ${THIRD_PARTY_DIR}/imgui_impl_opengl3.cpp
)


# Incluir encabezados
include_directories(${INCLUDE_DIR} ${THIRD_PARTY_DIR})

# Dependencias
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Ejecutable
add_executable(SocialRed ${SOURCES})

# Linkeo de librerías
target_link_libraries(SocialRed PRIVATE OpenGL::GL glfw)
