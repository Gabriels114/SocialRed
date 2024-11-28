#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

struct Mensaje {
    string contenido;
};

class RedSocial {
private:
    unordered_map<string, vector<Mensaje>> publicaciones;
    unordered_map<string, unordered_set<string>> amigos;
    string usuarioActivo;

public:
    void cargarUsuariosPredefinidos();
    void registrarUsuario(const string& nombre);
    bool iniciarSesion(const string& nombre);
    void cerrarSesion();
    void publicarMensaje(const string& mensaje);
    vector<string> obtenerListaUsuarios();
    vector<string> obtenerAmigos(const string& usuario);
    vector<string> obtenerPublicaciones(const string& usuario);
    string obtenerUsuarioActivo();
};

#endif //REDSOCIAL_H
