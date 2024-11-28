#include "RedSocial.h"
#include <iostream>
#include <queue>
#include <unordered_set>

void RedSocial::registrarUsuario(const std::string& nombre) {
    if (publicaciones.find(nombre) != publicaciones.end()) {
        std::cout << "El usuario ya está registrado.\n";
        return;
    }
    publicaciones[nombre] = {};
    amigos[nombre] = {};
    usuarioIndice[nombre] = usuarioIndice.size();
    matrizAdyacencia.resize(usuarioIndice.size(), std::vector<bool>(usuarioIndice.size(), false));
    agregarLog("Usuario '" + nombre + "' registrado con éxito.");
    std::cout << "Usuario registrado con éxito.\n";
}

bool RedSocial::iniciarSesion(const std::string& nombre) {
    if (publicaciones.find(nombre) == publicaciones.end()) {
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
        std::cout << "No hay una sesión activa.\n";
        return;
    }
    agregarLog("Sesión cerrada para '" + usuarioConSesionIniciada + "'.");
    std::cout << "Sesión cerrada para " << usuarioConSesionIniciada << ".\n";
    usuarioConSesionIniciada.clear();
}

void RedSocial::publicarMensaje(const std::string& mensaje) {
    if (usuarioConSesionIniciada.empty()) {
        std::cout << "Inicia sesión para publicar mensajes.\n";
        return;
    }
    publicaciones[usuarioConSesionIniciada].push_back({mensaje});
    agregarLog("Mensaje publicado por '" + usuarioConSesionIniciada + "'.");
    std::cout << "Mensaje publicado con éxito.\n";
}

void RedSocial::agregarAmigo(const std::string& amigo) {
    if (usuarioConSesionIniciada.empty()) {
        std::cout << "Inicia sesión para agregar amigos.\n";
        return;
    }
    if (amigos[usuarioConSesionIniciada].find(amigo) != amigos[usuarioConSesionIniciada].end()) {
        std::cout << "Ya son amigos.\n";
        return;
    }
    amigos[usuarioConSesionIniciada].insert(amigo);
    amigos[amigo].insert(usuarioConSesionIniciada);
    actualizarMatrizAdyacencia(usuarioConSesionIniciada, amigo, true);
    agregarLog("Amigo '" + amigo + "' agregado con éxito.");
    std::cout << "Amigo agregado con éxito.\n";

    std::vector<std::string> amigosDeAmigos = obtenerAmigosDeAmigos();
    std::cout << "Amigos de tus amigos:\n";
    for (const auto& amigoDeAmigo : amigosDeAmigos) {
        std::cout << amigoDeAmigo << "\n";
    }
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

std::vector<std::string> RedSocial::obtenerMensajesDeAmigos() {
    std::vector<std::string> mensajes;
    if (usuarioConSesionIniciada.empty()) {
        return mensajes;
    }
    for (const auto& amigo : amigos[usuarioConSesionIniciada]) {
        for (const auto& mensaje : publicaciones[amigo]) {
            mensajes.push_back(amigo + ": " + mensaje.contenido);
        }
    }
    return mensajes;
}

void RedSocial::agregarLog(const std::string& mensaje) {
    logs.push_back(mensaje);
    if (logs.size() > 10) {
        logs.erase(logs.begin());
    }
}

std::vector<std::string> RedSocial::getLogs() const {
    return logs;
}

std::vector<std::string> RedSocial::obtenerAmigosDeAmigos() {
    std::vector<std::string> amigosDeAmigos;
    std::unordered_set<std::string> visitados;

    if (usuarioConSesionIniciada.empty()) {
        return amigosDeAmigos;
    }

    for (const auto& amigo : amigos[usuarioConSesionIniciada]) {
        visitados.insert(amigo);
    }

    for (const auto& amigo : amigos[usuarioConSesionIniciada]) {
        for (const auto& amigoDeAmigo : amigos[amigo]) {
            if (visitados.find(amigoDeAmigo) == visitados.end()) {
                amigosDeAmigos.push_back(amigoDeAmigo);
                visitados.insert(amigoDeAmigo);
            }
        }
    }

    return amigosDeAmigos;
}

std::vector<std::string> RedSocial::obtenerAmigosDeAmigosDeAmigos() {
    std::vector<std::string> amigosDeAmigosDeAmigos;
    std::queue<std::string> cola;
    std::unordered_set<std::string> visitados;

    if (usuarioConSesionIniciada.empty()) {
        return amigosDeAmigosDeAmigos;
    }

    for (const auto& amigo : amigos[usuarioConSesionIniciada]) {
        cola.push(amigo);
        visitados.insert(amigo);
    }

    while (!cola.empty()) {
        std::string amigoActual = cola.front();
        cola.pop();

        for (const auto& amigoDeAmigo : amigos[amigoActual]) {
            if (visitados.find(amigoDeAmigo) == visitados.end()) {
                amigosDeAmigosDeAmigos.push_back(amigoDeAmigo);
                visitados.insert(amigoDeAmigo);
                cola.push(amigoDeAmigo);

                for (const auto& amigoDeAmigoDeAmigo : amigos[amigoDeAmigo]) {
                    if (visitados.find(amigoDeAmigoDeAmigo) == visitados.end()) {
                        amigosDeAmigosDeAmigos.push_back(amigoDeAmigoDeAmigo);
                        visitados.insert(amigoDeAmigoDeAmigo);
                        cola.push(amigoDeAmigoDeAmigo);
                    }
                }
            }
        }
    }

    return amigosDeAmigosDeAmigos;
}

void RedSocial::actualizarMatrizAdyacencia(const std::string& usuario1, const std::string& usuario2, bool estado) {
    int indice1 = usuarioIndice[usuario1];
    int indice2 = usuarioIndice[usuario2];
    matrizAdyacencia[indice1][indice2] = estado;
    matrizAdyacencia[indice2][indice1] = estado;
}