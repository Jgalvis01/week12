#include <iostream>
#include <vector>
#include <omp.h>
#include <random>

using namespace std;

// Función para generar un vector aleatorio
vector<int> generarVectorAleatorio(int tamanio, int seed, int rango_max) {
    vector<int> vec(tamanio);
    mt19937 gen(seed);
    uniform_int_distribution<> dis(1, rango_max);
    
    for (int i = 0; i < tamanio; i++) {
        vec[i] = dis(gen);
    }
    
    return vec;
}

// Función para buscar un valor en un vector
bool buscarValor(const vector<int>& vec, int valor) {
    bool encontrado = false;
    
    #pragma omp parallel for shared(encontrado)
    for (int i = 0; i < vec.size(); i++) {
        if (encontrado) continue; // Si ya lo encontramos, saltar
        
        if (vec[i] == valor) {
            #pragma omp critical
            {
                encontrado = true;
            }
        }
    }
    
    return encontrado;
}

// Función alternativa con early exit
int buscarPosicion(const vector<int>& vec, int valor) {
    int posicion = -1;
    
    #pragma omp parallel for
    for (int i = 0; i < vec.size(); i++) {
        if (posicion != -1) continue; // Ya encontrado
        
        if (vec[i] == valor) {
            #pragma omp critical
            {
                if (posicion == -1) { // Double-check
                    posicion = i;
                }
            }
        }
    }
    
    return posicion;
}

int main() {
    const int NUM_VECTORES = 6;
    const int TAMANIO_VECTOR = 5000000;
    const int RANGO_MAX = 1000000;
    const int VALOR_BUSCAR = 12345;
    
    cout << "=== Ejercicio 4: Buscar un valor en múltiples vectores ===" << endl;
    cout << "Número de vectores: " << NUM_VECTORES << endl;
    cout << "Tamaño de cada vector: " << TAMANIO_VECTOR << endl;
    cout << "Valor a buscar: " << VALOR_BUSCAR << endl;
    
    // Crear múltiples vectores
    vector<vector<int>> vectores;
    cout << "\nGenerando vectores..." << endl;
    for (int i = 0; i < NUM_VECTORES; i++) {
        vectores.push_back(generarVectorAleatorio(TAMANIO_VECTOR, i * 777, RANGO_MAX));
    }
    
    // Insertar el valor buscado en algunos vectores aleatorios
    vectores[1][TAMANIO_VECTOR / 2] = VALOR_BUSCAR;
    vectores[3][TAMANIO_VECTOR / 4] = VALOR_BUSCAR;
    vectores[4][TAMANIO_VECTOR - 100] = VALOR_BUSCAR;
    
    cout << "Valor insertado estratégicamente en vectores 2, 4 y 5" << endl;
    
    // Vectores para almacenar resultados
    vector<bool> resultados(NUM_VECTORES);
    vector<int> posiciones(NUM_VECTORES);
    
    double inicio = omp_get_wtime();
    
    // Buscar en cada vector en paralelo (embarazosamente paralelo)
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < NUM_VECTORES; i++) {
        int thread_id = omp_get_thread_num();
        
        #pragma omp critical
        {
            cout << "Thread " << thread_id << ": Buscando en vector " << (i+1) << endl;
        }
        
        resultados[i] = buscarValor(vectores[i], VALOR_BUSCAR);
        
        if (resultados[i]) {
            posiciones[i] = buscarPosicion(vectores[i], VALOR_BUSCAR);
        } else {
            posiciones[i] = -1;
        }
    }
    
    double fin = omp_get_wtime();
    
    // Mostrar resultados
    cout << "\n--- Resultados de búsqueda ---" << endl;
    int vectoresConValor = 0;
    for (int i = 0; i < NUM_VECTORES; i++) {
        cout << "Vector " << (i+1) << ": ";
        if (resultados[i]) {
            cout << "✓ ENCONTRADO en posición " << posiciones[i] << endl;
            vectoresConValor++;
        } else {
            cout << "✗ NO encontrado" << endl;
        }
    }

    cout << "Búsqueda completada en " << (fin - inicio) << " segundos" << endl;
    cout << "Valor encontrado en " << vectoresConValor << " vectores" << endl;
    
    return 0;
}
