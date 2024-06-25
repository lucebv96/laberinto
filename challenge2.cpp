#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

// Función que imprime el laberinto
void imprimirLaberinto(int **matriz, int filas, int columnas, int jugadorX, int jugadorY, int startX, int startY, int endX, int endY) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (i == startY && j == startX) {
                cout << "E"; // Entrada
            } else if (i == endY && j == endX) {
                cout << "S"; // Salida
            } else if (i == jugadorY && j == jugadorX) {
                cout << "0"; // Jugador
            } else if (matriz[i][j] == 1) {
                cout << "\u2588"; // Muro (carácter █)
            } else if (matriz[i][j] == 2) {
                cout << "."; // Camino
            } else {
                cout << " ";
            }
        }
        cout << "\n";
    }
}

// Función que genera el laberinto aleatoriamente
void generarLaberinto(int filas, int columnas, float densidad, int**& matriz, int& startX, int& startY, int& endX, int& endY) {
    matriz = new int*[filas];

    int FParedes = densidad * 8;
    densidad = filas * columnas * densidad / 4;

    for (int i = 0; i < filas; i++) {
        matriz[i] = new int[columnas];
    }

    // Crando muros del laberinto
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (i == 0 || j == 0 || i == filas - 1 || j == columnas - 1) {
                matriz[i][j] = 1;
            } else {
                matriz[i][j] = 0;
            }
        }
    }

    // Generación del laberinto
    srand(time(NULL));
    for (int i = 0; i < densidad; i++) {
        int x = rand() % (columnas - 4) + 2;
        x = (x / 2) * 2;

        int y = rand() % (filas - 4) + 2;
        y = (y / 2) * 2;

        matriz[y][x] = 1;

        for (int j = 0; j < FParedes; j++) {
            int mx[4] = {x, x, x + 2, x - 2};
            int my[4] = {y + 2, y - 2, y, y};
            int r = rand() % 4;
            if (matriz[my[r]][mx[r]] == 0) {
                matriz[my[r]][mx[r]] = 1;
                matriz[my[r] + (y - my[r]) / 2][mx[r] + (x - mx[r]) / 2] = 1;
            }
        }
    }

    // Colocar el inicio y el fin
    startX = 1; startY = 1;
    endX = columnas - 2; endY = filas - 2;
    matriz[startY][startX] = 0;
    matriz[endY][endX] = 0;

    imprimirLaberinto(matriz, filas, columnas, startX, startY, startX, startY, endX, endY);
}

// Función de backtracking
bool backtracking(int** matriz, int filas, int columnas, int x, int y, int endX, int endY) {
    if (x == endX && y == endY) {
        return true;
    }

    if (x < 0 || x >= columnas || y < 0 || y >= filas || matriz[y][x] != 0) {
        return false;
    }

    matriz[y][x] = 2; // Marcar como parte del camino

    // Movimiento en las 4 direcciones
    if (backtracking(matriz, filas, columnas, x + 1, y, endX, endY) ||
        backtracking(matriz, filas, columnas, x - 1, y, endX, endY) ||
        backtracking(matriz, filas, columnas, x, y + 1, endX, endY) ||
        backtracking(matriz, filas, columnas, x, y - 1, endX, endY)) {
        return true;
    }

    matriz[y][x] = 0; // Desmarcar si no va a la salida
    return false;
}

int main() {
   int filas, columnas;
    float densidad = 2;
    int startX, startY, endX, endY;


    // Solicitar al usuario que ingrese la cantidad de filas y columnas
    cout << "Ingrese la cantidad de filas del laberinto: ";
    cin >> filas;
    cout << "Ingrese la cantidad de columnas del laberinto: ";
    cin >> columnas;

    int** matriz;

    // Generar laberinto y obtener posiciones de inicio y fin
    generarLaberinto(filas, columnas, densidad, matriz, startX, startY, endX, endY);

    // Ejecutar el algoritmo de Backtracking
    bool encontrado = backtracking(matriz, filas, columnas, startX, startY, endX, endY);

    if (encontrado) {
        imprimirLaberinto(matriz, filas, columnas, startX, startY, startX, startY, endX, endY);
    } else {
        cout << "No se encontró una ruta.\n";
    }

    // Liberar la memoria de la matriz
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;

    //system("pause"); // No es necesario en sistemas UNIX
    return 0;
}
