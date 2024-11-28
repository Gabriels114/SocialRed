#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <cstring>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace std;

// ------------------------------ Estructuras ------------------------------

struct Mensaje {
    string contenido;
};

struct Usuario {
    string nombre;
    vector<Mensaje> publicaciones;
    unordered_set<string> amigos;
    vector<pair<string, Mensaje>> mensajesPrivados; // Mensajes privados recibidos
};

// -------------------------- Clase RedSocial --------------------------

class RedSocial {
private:
    unordered_map<string, Usuario> usuarios;
    string usuarioConSesionIniciada;
    deque<string> logs;

    void agregarLog(const string& mensaje) {
        logs.push_back(mensaje);
        if (logs.size() > 10) { logs.pop_front(); } // Mantener los últimos 10 logs
    }

public:
    void registrarUsuario(const char* nombre) {
        if (strlen(nombre) == 0 || strlen(nombre) > 1024) {
            agregarLog("El nombre de usuario no puede estar vacío o exceder 1024 caracteres.");
            return;
        }
        string nombreStr(nombre);
        if (usuarios.find(nombreStr) != usuarios.end()) {
            agregarLog("El usuario '" + nombreStr + "' ya está registrado.");
            return;
        }
        usuarios[nombreStr] = {nombreStr, {}, {}, {}};
        agregarLog("Usuario '" + nombreStr + "' registrado con éxito.");
    }

    bool iniciarSesion(const char* nombre) {
        if (strlen(nombre) == 0) {
            agregarLog("El nombre de usuario no puede estar vacío.");
            return false;
        }
        string nombreStr(nombre);
        if (usuarios.find(nombreStr) == usuarios.end()) {
            agregarLog("Usuario '" + nombreStr + "' no encontrado.");
            return false;
        }
        usuarioConSesionIniciada = nombreStr;
        agregarLog("Sesión iniciada como '" + usuarioConSesionIniciada + "'.");
        return true;
    }

    void cerrarSesion() {
        if (usuarioConSesionIniciada.empty()) {
            agregarLog("No hay una sesión activa.");
            return;
        }
        agregarLog("Cerrando sesión de '" + usuarioConSesionIniciada + "'.");
        usuarioConSesionIniciada.clear();
    }

    void publicarMensaje(const char* mensaje) {
        if (usuarioConSesionIniciada.empty()) {
            agregarLog("Debes iniciar sesión para publicar un mensaje.");
            return;
        }
        if (strlen(mensaje) == 0) {
            agregarLog("El mensaje no puede estar vacío.");
            return;
        }
        usuarios[usuarioConSesionIniciada].publicaciones.push_back({mensaje});
        agregarLog("Mensaje publicado por '" + usuarioConSesionIniciada + "'.");
    }

    // Otros métodos...

    const deque<string>& getLogs() const { return logs; }
    const string& getUsuarioConSesionIniciada() const { return usuarioConSesionIniciada; }
};

// ----------------------- Funciones Auxiliares -----------------------

void renderizarLogs(const deque<string>& logs) {
    ImGui::Text("Logs:");
    ImGui::BeginChild("LogsWindow", ImVec2(0, 100), true);
    for (auto it = logs.rbegin(); it != logs.rend(); ++it) {
        ImGui::Text("%s", it->c_str());
    }
    ImGui::EndChild();
}

void renderizarMenuPrincipal(RedSocial& red, char* usuarioActual, char* nuevoUsuario) {
    ImGui::Text("Menú Principal");
    ImGui::Separator();

    ImGui::InputText("Nuevo Usuario", nuevoUsuario, 1024);
    if (ImGui::Button("Registrar Usuario")) {
        red.registrarUsuario(nuevoUsuario);
        nuevoUsuario[0] = '\0';
    }

    ImGui::InputText("Usuario", usuarioActual, 1024);
    if (ImGui::Button("Iniciar Sesión")) {
        red.iniciarSesion(usuarioActual);
        usuarioActual[0] = '\0';
    }
}

void renderizarMenuUsuario(RedSocial& red, char* mensaje, char* amigo) {
    ImGui::Text("Usuario Activo: %s", red.getUsuarioConSesionIniciada().c_str());
    ImGui::Separator();

    // Publicar mensaje
    ImGui::InputText("Mensaje Público", mensaje, 1024);
    if (ImGui::Button("Publicar Mensaje")) {
        red.publicarMensaje(mensaje);
        mensaje[0] = '\0';
    }

    // Mostrar mensajes propios
    ImGui::Text("Mis Mensajes:");
    ImGui::BeginChild("MisMensajes", ImVec2(0, 100), true);
    // Aquí irían los mensajes personales...
    ImGui::EndChild();

    // Agregar amigos
    ImGui::InputText("Agregar Amigo", amigo, 1024);
    if (ImGui::Button("Agregar")) {
        red.registrarUsuario(amigo);
        amigo[0] = '\0';
    }

    if (ImGui::Button("Cerrar Sesión")) {
        red.cerrarSesion();
    }
}

// ---------------------- Función Principal ----------------------

int main() {
    if (!glfwInit()) {
        cerr << "Error al inicializar GLFW" << endl;
        return -1;
    }

    GLFWwindow* ventana = glfwCreateWindow(800, 600, "SocialRed", nullptr, nullptr);
    if (!ventana) {
        cerr << "Error al crear la ventana GLFW" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(ventana);
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(ventana, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    RedSocial red;
    char usuarioActual[1024] = "";
    char nuevoUsuario[1024] = "";
    char mensaje[1024] = "";
    char amigo[1024] = "";

    while (!glfwWindowShouldClose(ventana)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Red Social");
        if (red.getUsuarioConSesionIniciada().empty()) {
            renderizarMenuPrincipal(red, usuarioActual, nuevoUsuario);
        } else {
            renderizarMenuUsuario(red, mensaje, amigo);
        }
        renderizarLogs(red.getLogs());
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(ventana, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(ventana);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(ventana);
    glfwTerminate();

    return 0;
}
