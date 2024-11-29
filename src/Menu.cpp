#include "Menu.h"
#include "RedSocial.h"
#include "imgui.h"


void renderizarMenuPrincipal(RedSocial& red, bool& salirPrograma) {
    static char nuevoUsuario[128] = "";
    static char iniciarUsuario[128] = "";

    ImGui::Begin("Red social Troyana");

    ImGui::Text("Bienvenido a la red social Troyana");
    ImGui::Separator();
    ImGui::Text("\n");
    ImGui::Text("Registra un nuevo usuario o inicia sesión con uno existente");
    ImGui::Separator();
    ImGui::Text("\n");

    ImGui::Text("Registrate");
    ImGui::InputText(" ", nuevoUsuario, IM_ARRAYSIZE(nuevoUsuario));
    if (ImGui::Button("Registrar Usuario")) {
        red.registrarUsuario(nuevoUsuario);
        nuevoUsuario[0] = '\0';
    }

    ImGui::Separator();

    ImGui::Text("\nIniciar Sesión con tu Usuario");
    ImGui::InputText("  ", iniciarUsuario, IM_ARRAYSIZE(iniciarUsuario));
    if (ImGui::Button("Iniciar Sesión")) {
        red.iniciarSesion(iniciarUsuario);
        iniciarUsuario[0] = '\0';
    }

    ImGui::Separator();
    renderizarLogs(red.getLogs()); // Mostrar los logs en la interfaz

    ImGui::Separator();

    if (ImGui::Button("Salir")) {
        salirPrograma = true;
    }

    ImGui::End();
}

// Menu.cpp
#include "Menu.h"
#include "RedSocial.h"
#include "imgui.h"

// Renderiza la ventana de logs
void renderizarLogs(const std::vector<std::string>& logs) {
    ImGui::Text("Logs:");
    ImGui::BeginChild("LogsWindow", ImVec2(0, 100), true); // Ventana con desplazamiento
    for (auto it = logs.rbegin(); it != logs.rend(); ++it) { // Muestra los logs en orden inverso
        ImGui::Text("%s", it->c_str());
    }
    ImGui::EndChild();
}

void renderizarMenuUsuario(RedSocial& red) {
    static char mensaje[256] = "";
    static char amigo[128] = "";
    static bool mostrarAmigosDeAmigos = false;
    static bool mostrarMatrizAdyacencia = false;

    ImGui::Begin("Menú de Usuario");
    ImGui::Text("Usuario Activo: %s", red.getUsuarioConSesionIniciada().c_str());
    ImGui::Separator();

    // Publicar Mensaje
    ImGui::InputText("Escribir Mensaje Público", mensaje, IM_ARRAYSIZE(mensaje));
    if (ImGui::Button("Publicar Mensaje")) {
        red.publicarMensaje(mensaje);
        mensaje[0] = '\0';
    }

    ImGui::Separator();

    // Agregar amigo
    ImGui::InputText("Nombre del Amigo", amigo, IM_ARRAYSIZE(amigo));
    if (ImGui::Button("Agregar Amigo")) {
        red.agregarAmigo(amigo);
        amigo[0] = '\0';
    }

    ImGui::Separator();

    // Mostrar amigos de amigos
    if (ImGui::Button("Ver Amigos de Mis Amigos")) {
        mostrarAmigosDeAmigos = !mostrarAmigosDeAmigos;
    }

    if (mostrarAmigosDeAmigos) {
        ImGui::Text("Amigos de tus amigos:");
        std::vector<std::string> amigosDeAmigos = red.obtenerAmigosDeAmigos();
        for (const auto& amigoDeAmigo : amigosDeAmigos) {
            ImGui::BulletText("%s es amigo de %s", amigoDeAmigo.c_str(), red.getUsuarioConSesionIniciada().c_str());
        }
    }

    ImGui::Separator();

    // Mostrar mensajes del usuario
    ImGui::Text("Mis Mensajes:");
    for (const auto& publicacion : red.obtenerMensajesUsuario()) {
        ImGui::BulletText("%s", publicacion.c_str());
    }

    ImGui::Separator();

    // Mostrar mensajes de amigos
    ImGui::Text("Mensajes de Mis Amigos:");
    for (const auto& mensajeDeAmigo : red.obtenerMensajesDeAmigos()) {
        ImGui::BulletText("%s", mensajeDeAmigo.c_str());
    }

    ImGui::Separator();
    ImGui::Separator();
    renderizarLogs(red.getLogs()); // Mostrar los logs en la interfaz principal

    ImGui::Separator();

    // Botón para mostrar la matriz de adyacencia
    if (ImGui::Button("Mostrar Matriz de Adyacencia")) {
        mostrarMatrizAdyacencia = !mostrarMatrizAdyacencia;
    }

    if (mostrarMatrizAdyacencia) {
        ImGui::Text("Matriz de Adyacencia:");
        const auto& matriz = red.obtenerMatrizAdyacencia();
        const auto& usuarios = red.obtenerUsuarios(); // Asumiendo que tienes un método para obtener los nombres de los usuarios
        if (ImGui::BeginTable("MatrizAdyacencia", usuarios.size() + 1)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(" ");
            for (const auto& usuario : usuarios) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", usuario.c_str());
            }
            for (size_t i = 0; i < usuarios.size(); ++i) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%s", usuarios[i].c_str());
                for (size_t j = 0; j < usuarios.size(); ++j) {
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", matriz[i][j]);
                }
            }
            ImGui::EndTable();
        }
    }

    ImGui::Separator();

    // Botón de cerrar sesión
    if (ImGui::Button("Cerrar Sesión")) {
        red.cerrarSesion();
    }

    ImGui::End();
}