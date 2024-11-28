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

// Estructura que representa un mensaje con su contenido
struct Mensaje {
    string contenido;
};

// Estructura que representa un usuario en la red social
struct Usuario {
    string nombre;                       // Nombre del usuario
    vector<Mensaje> publicaciones;      // Mensajes publicados por el usuario
    unordered_set<string> amigos;       // Lista de amigos del usuario
};

// Clase principal que implementa la lógica de la red social
class RedSocial {
private:
    unordered_map<string, Usuario> usuarios; // Almacena los usuarios registrados
    string usuarioConSesionIniciada;         // Usuario actual con sesión activa
    vector<string> logs;                     // Registro de eventos y actividades
    vector<string> listaUsuarios;            // Lista de todos los usuarios

    // Método interno para agregar un mensaje al registro de logs
    void agregarLog(const string& mensaje) {
        logs.push_back(mensaje); // Agrega el mensaje al registro
        if (logs.size() > 10) { // Limita el tamaño del registro a los últimos 10 eventos
            logs.erase(logs.begin());
        }
    }

public:
    // Registra un nuevo usuario
    void registrarUsuario(const string& nombre) {
        if (usuarios.find(nombre) != usuarios.end()) {
            agregarLog("El usuario '" + nombre + "' ya está registrado.");
            return;
        }
        usuarios[nombre] = {nombre, {}, {}}; // Crea un nuevo usuario
        listaUsuarios.push_back(nombre);    // Agrega el usuario a la lista
        agregarLog("Usuario '" + nombre + "' registrado con éxito.");
    }

    // Inicia sesión con un usuario existente
    bool iniciarSesion(const string& nombre) {
        if (usuarios.find(nombre) == usuarios.end()) {
            agregarLog("Usuario '" + nombre + "' no encontrado.");
            return false;
        }
        usuarioConSesionIniciada = nombre; // Establece la sesión activa
        agregarLog("Sesión iniciada como '" + usuarioConSesionIniciada + "'.");
        return true;
    }

    // Cierra la sesión activa
    void cerrarSesion() {
        if (usuarioConSesionIniciada.empty()) {
            agregarLog("No hay una sesión activa.");
            return;
        }
        agregarLog("Cerrando sesión de '" + usuarioConSesionIniciada + "'.");
        usuarioConSesionIniciada.clear(); // Limpia la sesión activa
    }

    // Publica un mensaje desde el usuario con sesión activa
    void publicarMensaje(const string& mensaje) {
        if (usuarioConSesionIniciada.empty()) {
            agregarLog("Debes iniciar sesión para publicar un mensaje.");
            return;
        }
        usuarios[usuarioConSesionIniciada].publicaciones.push_back({mensaje});
        agregarLog("Mensaje publicado por '" + usuarioConSesionIniciada + "'.");
    }

    // Obtiene los mensajes de un amigo específico
    vector<Mensaje> obtenerMensajesDeAmigo(const string& amigo) const {
        if (usuarios.find(amigo) == usuarios.end()) return {};
        return usuarios.at(amigo).publicaciones;
    }

    // Obtiene los mensajes del usuario con sesión activa
    vector<Mensaje> obtenerMensajes() const {
        if (usuarioConSesionIniciada.empty()) return {};
        return usuarios.at(usuarioConSesionIniciada).publicaciones;
    }

    // Añade un amigo al usuario con sesión activa
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

    // Obtiene la lista de amigos del usuario con sesión activa
    vector<string> obtenerAmigos() const {
        if (usuarioConSesionIniciada.empty()) return {};
        vector<string> amigos(usuarios.at(usuarioConSesionIniciada).amigos.begin(),
                              usuarios.at(usuarioConSesionIniciada).amigos.end());
        return amigos;
    }

    // Devuelve el registro de logs
    const vector<string>& getLogs() const {
        return logs;
    }

    // Devuelve el nombre del usuario con sesión activa
    const string& getUsuarioConSesionIniciada() const {
        return usuarioConSesionIniciada;
    }
};

// Función auxiliar para manejar entradas de texto
bool InputTextWithBuffer(const char* label, string& str) {
    char buffer[128]; // Buffer de tamaño fijo
    strncpy(buffer, str.c_str(), sizeof(buffer)); // Copia el contenido de la cadena al buffer
    buffer[sizeof(buffer) - 1] = '\0'; // Asegura que el buffer esté terminado en nulo
    if (ImGui::InputText(label, buffer, sizeof(buffer))) {
        str = buffer; // Actualiza la cadena si cambia
        return true;
    }
    return false;
}

// Renderiza la ventana de logs
void renderizarLogs(const vector<string>& logs) {
    ImGui::Text("Logs:");
    ImGui::BeginChild("LogsWindow", ImVec2(0, 100), true);
    for (auto it = logs.rbegin(); it != logs.rend(); ++it) { // Muestra los logs en orden inverso
        ImGui::Text("%s", it->c_str());
    }
    ImGui::EndChild();
}

// Renderiza el menú principal
void renderizarMenuPrincipal(RedSocial& red, string& usuarioActual, string& nuevoUsuario) {
    if (ImGui::Button("Registrar Usuario")) {
        red.registrarUsuario(nuevoUsuario);
        nuevoUsuario.clear(); // Limpia la entrada
    }
    InputTextWithBuffer("Nuevo Usuario", nuevoUsuario);
    ImGui::Separator();

    if (ImGui::Button("Iniciar Sesión")) {
        red.iniciarSesion(usuarioActual);
    }
    InputTextWithBuffer("Usuario", usuarioActual);
}

// Renderiza la sesión activa
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

// Función principal
int main() {
    // Inicialización de GLFW
    if (!glfwInit()) {
        cerr << "Error al inicializar GLFW" << endl;
        return -1;
    }

    // Creación de la ventana
    GLFWwindow* ventana = glfwCreateWindow(800, 600, "SocialRed", nullptr, nullptr);
    if (!ventana) {
        cerr << "Error al crear la ventana GLFW" << endl;
        glfwTerminate();
        return -1;
    }

    // Configuración de OpenGL y Dear ImGui
    glfwMakeContextCurrent(ventana);
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(ventana, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Variables del programa
    RedSocial red;
    string usuarioActual, nuevoUsuario, mensaje, amigo;

    // Bucle principal
    while (!glfwWindowShouldClose(ventana)) {
        // Actualización de eventos e inicio de frame
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Interfaz principal
        ImGui::Begin("Red Social");
        if (red.getUsuarioConSesionIniciada().empty()) {
            renderizarMenuPrincipal(red, usuarioActual, nuevoUsuario);
        } else {
            renderizarSesion(red, mensaje, amigo);
        }

        ImGui::Separator();
        renderizarLogs(red.getLogs()); // Renderiza los logs
        ImGui::End();

        // Renderizado
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(ventana, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Intercambio de buffers
        glfwSwapBuffers(ventana);
    }

    // Finalización
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(ventana);
    glfwTerminate();

    return 0;
}
