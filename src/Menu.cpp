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

void renderizarMenuPrincipal(RedSocial& red, bool& salirPrograma) {
    static char nuevoUsuario[128] = "";
    static char iniciarUsuario[128] = "";

    ImGui::Begin("Menú Principal");

    // Aseguramos que cada campo tenga un ID único
    ImGui::InputText("Nuevo Usuario", nuevoUsuario, IM_ARRAYSIZE(nuevoUsuario));
    if (ImGui::Button("Registrar Usuario")) {
        red.registrarUsuario(nuevoUsuario);
        nuevoUsuario[0] = '\0';
    }

    ImGui::Separator();

    ImGui::InputText("Iniciar Sesión con Usuario", iniciarUsuario, IM_ARRAYSIZE(iniciarUsuario));
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

void renderizarMenuUsuario(RedSocial& red) {
    static char mensaje[256] = "";
    static char amigo[128] = "";

    ImGui::Begin("Menú de Usuario");
    ImGui::Text("Usuario Activo: %s", red.getUsuarioConSesionIniciada().c_str());
    ImGui::Separator();

    ImGui::InputText("Escribir Mensaje Público", mensaje, IM_ARRAYSIZE(mensaje));
    if (ImGui::Button("Publicar Mensaje")) {
        red.publicarMensaje(mensaje);
        mensaje[0] = '\0';
    }

    ImGui::Separator();

    ImGui::InputText("Nombre del Amigo", amigo, IM_ARRAYSIZE(amigo));
    if (ImGui::Button("Agregar Amigo")) {
        red.agregarAmigo(amigo); // Cambié 'anadirAmigo' a 'agregarAmigo'
        amigo[0] = '\0';
    }

    ImGui::Separator();

    ImGui::Text("Mis Mensajes:");
    for (const auto& publicacion : red.obtenerMensajesUsuario()) {
        ImGui::BulletText("%s", publicacion.c_str());
    }

    ImGui::Separator();

    if (ImGui::Button("Cerrar Sesión")) {
        red.cerrarSesion();
    }

    ImGui::End();
}