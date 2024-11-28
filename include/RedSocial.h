#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Estructura para representar mensajes
struct Mensaje {
    std::string contenido;
};

// Clase RedSocial
class RedSocial {
private:
    std::unordered_map<std::string, std::vector<Mensaje>> publicaciones;
    std::unordered_map<std::string, std::unordered_set<std::string>> amigos;
    std::string usuarioConSesionIniciada;
    std::vector<std::string> logs; // Vector para almacenar los logs de eventos

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

    void agregarLog(const std::string& mensaje); // Función para agregar log
    std::vector<std::string> getLogs() const; // Función para obtener logs
};

#endif // REDSOCIAL_H
