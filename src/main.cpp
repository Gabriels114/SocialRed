#include "RedSocial.h"
#include "Menu.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


int main() {
    // Inicialización de GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    // Crear una ventana con GLFW
    GLFWwindow* ventana = glfwCreateWindow(800, 600, "SocialRed", NULL, NULL);
    if (!ventana) {
        std::cerr << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(ventana);
    glfwSwapInterval(1); // Habilitar V-Sync

    // Inicialización de OpenGL y ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(ventana, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    RedSocial red;
    bool salirPrograma = false;

    // Bucle principal
    while (!glfwWindowShouldClose(ventana) && !salirPrograma) {
        glfwPollEvents(); // Procesar eventos de GLFW

        // Comienza un nuevo frame de ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Aquí va el código del menú
        if (red.getUsuarioConSesionIniciada().empty()) {
            renderizarMenuPrincipal(red, salirPrograma);
        } else {
            renderizarMenuUsuario(red);
        }

        // Renderizar el contenido
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(ventana, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar los datos de ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Intercambiar buffers
        glfwSwapBuffers(ventana);
    }

    // Limpieza de recursos
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(ventana);
    glfwTerminate();

    return 0;
}
