#include "MatrizAdyacencia.h"
#include <iostream>
using namespace std;

// Definimos la matriz de adyacencia como una variable global
static vector<vector<int>> matriz;

// Inicializa la matriz con un tamaño inicial
void inicializarMatriz(int tamanoInicial) {
    matriz.resize(tamanoInicial, vector<int>(tamanoInicial, 0));
}

// Redimensiona la matriz cuando se agregan nuevos usuarios
void redimensionarMatriz(int nuevoTamano) {
    for (auto& fila : matriz) {
        fila.resize(nuevoTamano, 0); // Expandir cada fila existente
    }
    matriz.resize(nuevoTamano, vector<int>(nuevoTamano, 0)); // Agregar nuevas filas
}

// Añade una conexión entre dos usuarios
void anadirConexion(int usuario1, int usuario2) {
    if (usuario1 < matriz.size() && usuario2 < matriz.size()) {
        matriz[usuario1][usuario2] = 1;
        matriz[usuario2][usuario1] = 1;
    } else {
        cout << "Error: Índices fuera de rango.\n";
    }
}

// Imprime la matriz de adyacencia en consola
void imprimirMatriz() {
    for (const auto& fila : matriz) {
        for (const auto& valor : fila) {
            cout << valor << " ";
        }
        cout << endl;
    }
}

// Verifica si dos usuarios están conectados
bool estanConectados(int usuario1, int usuario2) {
    if (usuario1 < matriz.size() && usuario2 < matriz.size()) {
        return matriz[usuario1][usuario2] == 1;
    }
    return false;
}
