// RedSocial.cpp
#include "RedSocial.h"

void RedSocial::agregarLog(const string& mensaje) {
    logs.push_back(mensaje);
    if (logs.size() > 10) { logs.pop_front(); }
}

void RedSocial::cargarUsuariosPredefinidos() {
    registrarUsuario("Luis");
    registrarUsuario("Roger");
    registrarUsuario("Nico");
    amigos["Luis"].insert("Roger");
    amigos["Roger"].insert("Luis");
}

void RedSocial::registrarUsuario(const string& nombre) {
    if (publicaciones.find(nombre) == publicaciones.end()) {
        publicaciones[nombre] = {};
        amigos[nombre] = {};
    }
}

bool RedSocial::iniciarSesion(const string& nombre) {
    if (publicaciones.find(nombre) != publicaciones.end()) {
        usuarioActivo = nombre;
        return true;
    }
    return false;
}

void RedSocial::cerrarSesion() {
    usuarioActivo = "";
}

void RedSocial::publicarMensaje(const string& mensaje) {
    if (!usuarioActivo.empty()) {
        publicaciones[usuarioActivo].push_back({mensaje});
    }
}

vector<string> RedSocial::obtenerListaUsuarios() {
    vector<string> usuarios;
    for (const auto& par : publicaciones) {
        usuarios.push_back(par.first);
    }
    return usuarios;
}

vector<string> RedSocial::obtenerAmigos(const string& usuario) {
    vector<string> listaAmigos(amigos[usuario].begin(), amigos[usuario].end());
    return listaAmigos;
}

vector<string> RedSocial::obtenerPublicaciones(const string& usuario) {
    vector<string> listaPublicaciones;
    for (const auto& publicacion : publicaciones[usuario]) {
        listaPublicaciones.push_back(publicacion.contenido);
    }
    return listaPublicaciones;
}

string RedSocial::obtenerUsuarioActivo() {
    return usuarioActivo;
}

vector<Mensaje> RedSocial::obtenerMensajesDeAmigos() {
    vector<Mensaje> mensajes;
    if (usuarioActivo.empty()) return mensajes;

    for (const auto& amigo : amigos[usuarioActivo]) {
        for (const auto& mensaje : publicaciones[amigo]) {
            mensajes.push_back(mensaje);
        }
    }
    return mensajes;
}

vector<pair<string, string>> RedSocial::obtenerAmigosDeAmigos() {
    vector<pair<string, string>> resultado;
    if (usuarioActivo.empty()) return resultado;

    unordered_set<string> visitados;
    for (const auto& amigo : amigos[usuarioActivo]) {
        for (const auto& amigoDeAmigo : amigos[amigo]) {
            if (amigoDeAmigo != usuarioActivo &&
                amigos[usuarioActivo].find(amigoDeAmigo) == amigos[usuarioActivo].end() &&
                visitados.find(amigoDeAmigo) == visitados.end()) {
                resultado.emplace_back(amigo, amigoDeAmigo);
                visitados.insert(amigoDeAmigo);
            }
        }
    }
    return resultado;
}

void RedSocial::enviarMensajePrivado(const string& amigo, const string& mensaje) {
    if (usuarioActivo.empty()) {
        agregarLog("Debes iniciar sesión para enviar un mensaje privado.");
        return;
    }
    if (mensaje.empty() || amigo.empty()) {
        agregarLog("El mensaje y el nombre del amigo no pueden estar vacíos.");
        return;
    }
    if (publicaciones.find(amigo) == publicaciones.end()) {
        agregarLog("El usuario '" + amigo + "' no existe.");
        return;
    }
    amigos[amigo].insert({usuarioActivo, {mensaje}});
    agregarLog("Mensaje privado enviado a '" + amigo + "'.");
}

const vector<Mensaje>& RedSocial::obtenerMisMensajes() {
    return publicaciones[usuarioActivo];
}

const deque<string>& RedSocial::getLogs() const {
    return logs;
}