#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace std;

// Estructuras y clases
struct Mensaje {
    string contenido;
};

struct Usuario {
    string nombre;
    vector<Mensaje> publicaciones;
    unordered_set<string> amigos;
};

class RedSocial {
private:
    unordered_map<string, Usuario> usuarios;
    string usuarioConSesionIniciada;
    vector<string> logs;
    vector<string> listaUsuarios;

    void agregarLog(const string& mensaje) {
        logs.push_back(mensaje);
        if (logs.size() > 10) {
            logs.erase(logs.begin());
        }
    }

public:
    void registrarUsuario(const string& nombre) {
        if (usuarios.find(nombre) != usuarios.end()) {
            agregarLog("El usuario '" + nombre + "' ya está registrado.");
            return;
        }
        usuarios[nombre] = {nombre, {}, {}};
        listaUsuarios.push_back(nombre);
        agregarLog("Usuario '" + nombre + "' registrado con éxito.");
    }

    bool iniciarSesion(const string& nombre) {
        if (usuarios.find(nombre) == usuarios.end()) {
            agregarLog("Usuario '" + nombre + "' no encontrado.");
            return false;
        }
        usuarioConSesionIniciada = nombre;
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

    void publicarMensaje(const string& mensaje) {
        if (usuarioConSesionIniciada.empty()) {
            agregarLog("Debes iniciar sesión para publicar un mensaje.");
            return;
        }
        usuarios[usuarioConSesionIniciada].publicaciones.push_back({mensaje});
        agregarLog("Mensaje publicado por '" + usuarioConSesionIniciada + "'.");
    }

    vector<Mensaje> obtenerMensajesDeAmigo(const string& amigo) const {
        if (usuarios.find(amigo) == usuarios.end()) return {};
        return usuarios.at(amigo).publicaciones;
    }

    vector<Mensaje> obtenerMensajes() const {
        if (usuarioConSesionIniciada.empty()) return {};
        return usuarios.at(usuarioConSesionIniciada).publicaciones;
    }

    void anadirAmigo(const string& amigo) {
        if (usuarioConSesionIniciada.empty()) {
            agregarLog("Debes iniciar sesión primero.");
            return;
        }
        if (usuarios.find(amigo) == usuarios.end()) {
            agregarLog("El usuario '" + amigo + "' no está registrado.");
            return;
        }
        if (usuarios[usuarioConSesionIniciada].amigos.find(amigo) != usuarios[usuarioConSesionIniciada].amigos.end()) {
            agregarLog("Ya son amigos.");
            return;
        }
        usuarios[usuarioConSesionIniciada].amigos.insert(amigo);
        usuarios[amigo].amigos.insert(usuarioConSesionIniciada);
        agregarLog("Amistad entre '" + usuarioConSesionIniciada + "' y '" + amigo + "' agregada con éxito.");
    }

    vector<string> obtenerAmigos() const {
        if (usuarioConSesionIniciada.empty()) return {};
        vector<string> amigos(usuarios.at(usuarioConSesionIniciada).amigos.begin(),
                              usuarios.at(usuarioConSesionIniciada).amigos.end());
        return amigos;
    }

    const vector<string>& getLogs() const {
        return logs;
    }

    const string& getUsuarioConSesionIniciada() const {
        return usuarioConSesionIniciada;
    }
};

// Función auxiliar para manejar entradas de texto
bool InputTextWithBuffer(const char* label, string& str) {
    char buffer[128];
    strncpy(buffer, str.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    if (ImGui::InputText(label, buffer, sizeof(buffer))) {
        str = buffer;
        return true;
    }
    return false;
}

// Renderización con ImGui
void renderizarLogs(const vector<string>& logs) {
    ImGui::Text("Logs:");
    ImGui::BeginChild("LogsWindow", ImVec2(0, 100), true);
    for (auto it = logs.rbegin(); it != logs.rend(); ++it) {
        ImGui::Text("%s", it->c_str());
    }
    ImGui::EndChild();
}

void renderizarMenuPrincipal(RedSocial& red, string& usuarioActual, string& nuevoUsuario) {
    if (ImGui::Button("Registrar Usuario")) {
        red.registrarUsuario(nuevoUsuario);
        nuevoUsuario.clear();
    }
    InputTextWithBuffer("Nuevo Usuario", nuevoUsuario);
    ImGui::Separator();

    if (ImGui::Button("Iniciar Sesión")) {
        red.iniciarSesion(usuarioActual);
    }
    InputTextWithBuffer("Usuario", usuarioActual);
}

void renderizarSesion(RedSocial& red, string& mensaje, string& amigo) {
    ImGui::Text("Sesión iniciada como: %s", red.getUsuarioConSesionIniciada().c_str());
    if (ImGui::Button("Cerrar Sesión")) {
        red.cerrarSesion();
        return;
    }

    ImGui::Separator();

    ImGui::Text("Publicar mensaje:");
    InputTextWithBuffer("Mensaje", mensaje);
    if (ImGui::Button("Publicar")) {
        red.publicarMensaje(mensaje);
        mensaje.clear();
    }

    ImGui::Separator();

    ImGui::Text("Tus mensajes:");
    for (const auto& msg : red.obtenerMensajes()) {
        ImGui::BulletText("%s", msg.contenido.c_str());
    }

    ImGui::Separator();

    ImGui::Text("Agregar amigo:");
    InputTextWithBuffer("Amigo", amigo);
    if (ImGui::Button("Agregar")) {
        red.anadirAmigo(amigo);
        amigo.clear();
    }

    ImGui::Separator();

    ImGui::Text("Mensajes de tus amigos:");
    for (const auto& amigo : red.obtenerAmigos()) {
        ImGui::Text("%s:", amigo.c_str());
        for (const auto& msg : red.obtenerMensajesDeAmigo(amigo)) {
            ImGui::BulletText("%s", msg.contenido.c_str());
        }
    }
}

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
    string usuarioActual, nuevoUsuario, mensaje, amigo;

    while (!glfwWindowShouldClose(ventana)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Red Social");
        if (red.getUsuarioConSesionIniciada().empty()) {
            renderizarMenuPrincipal(red, usuarioActual, nuevoUsuario);
        } else {
            renderizarSesion(red, mensaje, amigo);
        }

        ImGui::Separator();
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
