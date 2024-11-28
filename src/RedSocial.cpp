#include "RedSocial.h"
#include <iostream>

void RedSocial::registrarUsuario(const std::string& nombre) {
    if (publicaciones.find(nombre) != publicaciones.end()) {
        agregarLog("El usuario '" + nombre + "' ya está registrado.");
        std::cout << "El usuario ya está registrado.\n";
        return;
    }
    publicaciones[nombre] = {};
    amigos[nombre] = {};
    agregarLog("Usuario '" + nombre + "' registrado con éxito.");
    std::cout << "Usuario registrado con éxito.\n";
}

bool RedSocial::iniciarSesion(const std::string& nombre) {
    if (publicaciones.find(nombre) == publicaciones.end()) {
        agregarLog("Usuario '" + nombre + "' no encontrado.");
        std::cout << "Usuario no encontrado.\n";
        return false;
    }
    usuarioConSesionIniciada = nombre;
    agregarLog("Sesión iniciada como '" + nombre + "'.");
    std::cout << "Sesión iniciada como " << nombre << ".\n";
    return true;
}

void RedSocial::cerrarSesion() {
    if (usuarioConSesionIniciada.empty()) {
        agregarLog("No hay una sesión activa.");
        std::cout << "No hay una sesión activa.\n";
        return;
    }
    agregarLog("Sesión cerrada para '" + usuarioConSesionIniciada + "'.");
    std::cout << "Sesión cerrada para " << usuarioConSesionIniciada << ".\n";
    usuarioConSesionIniciada.clear();
}

void RedSocial::publicarMensaje(const std::string& mensaje) {
    if (usuarioConSesionIniciada.empty()) {
        agregarLog("Inicia sesión para publicar mensajes.");
        std::cout << "Inicia sesión para publicar mensajes.\n";
        return;
    }
    publicaciones[usuarioConSesionIniciada].push_back({mensaje});
    agregarLog("Mensaje publicado por '" + usuarioConSesionIniciada + "'.");
    std::cout << "Mensaje publicado con éxito.\n";
}

void RedSocial::agregarAmigo(const std::string& amigo) {
    if (usuarioConSesionIniciada.empty()) {
        agregarLog("Inicia sesión para agregar amigos.");
        std::cout << "Inicia sesión para agregar amigos.\n";
        return;
    }
    if (amigos[usuarioConSesionIniciada].find(amigo) != amigos[usuarioConSesionIniciada].end()) {
        agregarLog("Ya son amigos.");
        std::cout << "Ya son amigos.\n";
        return;
    }
    amigos[usuarioConSesionIniciada].insert(amigo);
    amigos[amigo].insert(usuarioConSesionIniciada);
    agregarLog("Amigo '" + amigo + "' agregado con éxito.");
    std::cout << "Amigo agregado con éxito.\n";
}

const std::string& RedSocial::getUsuarioConSesionIniciada() const {
    return usuarioConSesionIniciada;
}

std::vector<std::string> RedSocial::obtenerMensajesUsuario() {
    std::vector<std::string> mensajes;
    if (usuarioConSesionIniciada.empty()) {
        std::cout << "No hay sesión activa.\n";
        return mensajes;
    }
    for (const auto& mensaje : publicaciones[usuarioConSesionIniciada]) {
        mensajes.push_back(mensaje.contenido);
    }
    return mensajes;
}

std::unordered_set<std::string> RedSocial::obtenerAmigos() {
    if (usuarioConSesionIniciada.empty()) {
        return {};
    }
    return amigos[usuarioConSesionIniciada];
}
