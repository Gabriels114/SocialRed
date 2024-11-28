#ifndef MATRIZ_ADYACENCIA_H
#define MATRIZ_ADYACENCIA_H

#include <vector>

// Declaración de funciones para manejar la matriz de adyacencia
void inicializarMatriz(int tamanoInicial);
void redimensionarMatriz(int nuevoTamano);
void anadirConexion(int usuario1, int usuario2);
void imprimirMatriz();

// Función auxiliar para verificar si dos usuarios están conectados
bool estanConectados(int usuario1, int usuario2);

#endif // MATRIZ_ADYACENCIA_H
