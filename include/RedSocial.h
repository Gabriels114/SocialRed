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
private:
    std::unordered_map<std::string, std::vector<Mensaje>> publicaciones;
    std::unordered_map<std::string, std::unordered_set<std::string>> amigos;
    std::string usuarioConSesionIniciada;

    // Vector de logs
    std::vector<std::string> logs;

    // Método para agregar logs
    void agregarLog(const std::string& mensaje) {
        logs.push_back(mensaje); // Agrega el mensaje al registro
        if (logs.size() > 10) { // Limita el tamaño del registro a los últimos 10 eventos
            logs.erase(logs.begin()); // Elimina el mensaje más antiguo
        }
    }

public:
    void registrarUsuario(const std::string& nombre);
    bool iniciarSesion(const std::string& nombre);
    void cerrarSesion();
    void publicarMensaje(const std::string& mensaje);
    void agregarAmigo(const std::string& amigo);

    const std::string& getUsuarioConSesionIniciada() const;
    std::vector<std::string> obtenerMensajesUsuario();
    std::unordered_set<std::string> obtenerAmigos();

    const std::vector<std::string>& getLogs() const { return logs; }
};

#endif // REDSOCIAL_H
