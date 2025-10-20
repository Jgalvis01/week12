#include <iostream>
#include <vector>
#include <omp.h>
#include <random>

using namespace std;

// Función para generar un vector aleatorio
vector<double> generarVectorAleatorio(int tamanio, int seed) {
    vector<double> vec(tamanio);
    mt19937 gen(seed);
    uniform_real_distribution<> dis(1.0, 100.0);
    
    for (int i = 0; i < tamanio; i++) {
        vec[i] = dis(gen);
    }
    
    return vec;
}

// Función para calcular la suma de un vector
double sumarVector(const vector<double>& vec) {
    double suma = 0.0;
    
    // Paralelizar la suma dentro de cada vector
    #pragma omp parallel for reduction(+:suma)
    for (int i = 0; i < vec.size(); i++) {
        suma += vec[i];
    }
    
    return suma;
}

int main() {
    const int NUM_VECTORES = 8;
    const int TAMANIO_VECTOR = 1000000;
    
    cout << "=== Ejercicio 3: Suma de elementos en múltiples vectores ===" << endl;
    cout << "Número de vectores: " << NUM_VECTORES << endl;
    cout << "Tamaño de cada vector: " << TAMANIO_VECTOR << endl;
    
    // Crear múltiples vectores
    vector<vector<double>> vectores;
    cout << "\nGenerando vectores..." << endl;
    for (int i = 0; i < NUM_VECTORES; i++) {
        vectores.push_back(generarVectorAleatorio(TAMANIO_VECTOR, i * 100));
    }
    
    // Vector para almacenar resultados
    vector<double> sumas(NUM_VECTORES);
    
    double inicio = omp_get_wtime();
    
    // Procesar cada vector en paralelo (embarazosamente paralelo)
    // Cada iteración calcula la suma de un vector diferente
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < NUM_VECTORES; i++) {
        int thread_id = omp_get_thread_num();
        
        #pragma omp critical
        {
            cout << "Thread " << thread_id << ": Sumando vector " << (i+1) << endl;
        }
        
        sumas[i] = sumarVector(vectores[i]);
    }
    
    double fin = omp_get_wtime();
    
    // Mostrar resultados
    cout << "\n--- Resultados ---" << endl;
    for (int i = 0; i < NUM_VECTORES; i++) {
        cout << "Vector " << (i+1) << ": Suma = " << sumas[i] << endl;
    }
    
    cout << "\n✓ Procesamiento completado en " << (fin - inicio) << " segundos" << endl;
    
    // Calcular suma total de todas las sumas
    double sumaTotal = 0.0;
    for (double s : sumas) {
        sumaTotal += s;
    }
    
    return 0;
}
