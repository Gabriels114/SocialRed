#include "RedSocial.h"

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
