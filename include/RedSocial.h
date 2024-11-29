#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct Mensaje {
    std::string contenido;
};

class RedSocial {
public:
    void registrarUsuario(const std::string& nombre);
    bool iniciarSesion(const std::string& nombre);
    void cerrarSesion();
    void publicarMensaje(const std::string& mensaje);
    void agregarAmigo(const std::string& amigo);
    const std::string& getUsuarioConSesionIniciada() const;
    std::vector<std::string> obtenerMensajesUsuario();
    std::unordered_set<std::string> obtenerAmigos();
    std::vector<std::string> obtenerMensajesDeAmigos();
    void agregarLog(const std::string& mensaje);
    std::vector<std::string> getLogs() const;
    std::vector<std::string> obtenerAmigosDeAmigos() const;
    std::string obtenerUsuarioPorIndice(int indice) const;
    std::vector<std::string> obtenerAmigosDeAmigosDeAmigos();
    void actualizarMatrizAdyacencia(const std::string& usuario1, const std::string& usuario2, bool estado);
    // Method to get the adjacency matrix
    const std::vector<std::vector<bool>>& obtenerMatrizAdyacencia() const {
        return matrizAdyacencia;
    }
    // Método para obtener los nombres de los usuarios
    std::vector<std::string> obtenerUsuarios() const {
        std::vector<std::string> usuarios;
        for (const auto& par : usuarioIndice) {
            usuarios.push_back(par.first);
        }
        return usuarios;
    }

private:
    std::vector<std::vector<bool>> matrizAdyacencia;
    std::unordered_map<std::string, std::vector<Mensaje>> publicaciones;
    std::unordered_map<std::string, std::unordered_set<std::string>> amigos;
    std::vector<std::string> logs;
    std::string usuarioConSesionIniciada;
    std::unordered_map<std::string, int> usuarioIndice;
};
#endif // REDSOCIAL_H