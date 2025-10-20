#include <iostream>
#include <vector>
#include <omp.h>
#include <cmath>
#include <iomanip>
#include <random>
#include <algorithm>

using namespace std;

// Estructura para representar un objeto en caída libre
struct Objeto {
    int id;
    string nombre;
    double altura;        // metros
    double masa;          // kg (no afecta tiempo de caída, pero es realista)
    double tiempo_caida;  // segundos (resultado)
    
    Objeto(int i, string n, double h, double m) 
        : id(i), nombre(n), altura(h), masa(m), tiempo_caida(0.0) {}
};

// Función para calcular el tiempo de caída libre
// Fórmula: t = sqrt(2*h/g)
// donde h = altura, g = aceleración de la gravedad (9.81 m/s²)
double calcularTiempoCaida(double altura) {
    const double GRAVEDAD = 9.81; // m/s²
    
    if (altura <= 0.0) {
        return 0.0;
    }
    
    double tiempo = sqrt(2.0 * altura / GRAVEDAD);
    
    // Simulación de cálculo más complejo (para visualizar paralelización)
    double suma = 0.0;
    for (int i = 0; i < 10000; i++) {
        suma += sin(i * 0.001) * cos(i * 0.001);
    }
    
    return tiempo;
}

// Función para calcular velocidad final: v = sqrt(2*g*h)
double calcularVelocidadFinal(double altura) {
    const double GRAVEDAD = 9.81;
    return sqrt(2.0 * GRAVEDAD * altura);
}

// Función para generar objetos aleatorios
vector<Objeto> generarObjetos(int cantidad) {
    vector<Objeto> objetos;
    
    vector<string> nombres = {
        "Pelota de tenis", "Roca", "Pluma", "Martillo", 
        "Balón de fútbol", "Libro", "Llave", "Teléfono",
        "Botella", "Caja", "Piedra", "Moneda", 
        "Pelota de golf", "Ladrillo", "Juguete"
    };
    
    mt19937 gen(42);
    uniform_real_distribution<> dis_altura(1.0, 500.0);
    uniform_real_distribution<> dis_masa(0.05, 50.0);
    
    for (int i = 0; i < cantidad; i++) {
        string nombre = nombres[i % nombres.size()];
        if (i >= nombres.size()) {
            nombre += " " + to_string(i / nombres.size() + 1);
        }
        
        double altura = dis_altura(gen);
        double masa = dis_masa(gen);
        
        objetos.push_back(Objeto(i + 1, nombre, altura, masa));
    }
    
    return objetos;
}

int main() {
    const int NUM_OBJETOS = 20;
    
    cout << "=== Ejercicio 5: Simulación de caída libre para múltiples objetos ===" << endl;
    cout << "Número de objetos: " << NUM_OBJETOS << endl;
    cout << "Gravedad: 9.81 m/s²" << endl;
    
    // Generar objetos
    vector<Objeto> objetos = generarObjetos(NUM_OBJETOS);
    
    cout << "\n--- Objetos generados ---" << endl;
    cout << setw(4) << "ID" << " | " 
         << setw(20) << left << "Nombre" 
         << " | " << setw(10) << right << "Altura(m)" 
         << " | " << setw(10) << "Masa(kg)" << endl;
    cout << string(60, '-') << endl;
    
    for (const auto& obj : objetos) {
        cout << setw(4) << obj.id << " | " 
             << setw(20) << left << obj.nombre 
             << " | " << setw(10) << right << fixed << setprecision(2) << obj.altura 
             << " | " << setw(10) << obj.masa << endl;
    }
    
    double inicio = omp_get_wtime();
    
    // Calcular tiempo de caída para cada objeto en paralelo
    // (embarazosamente paralelo)
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < NUM_OBJETOS; i++) {
        int thread_id = omp_get_thread_num();
        
        #pragma omp critical
        {
            cout << "Thread " << thread_id << ": Calculando caída del objeto " 
                 << objetos[i].id << " (" << objetos[i].nombre << ")" << endl;
        }
        
        // Calcular tiempo de caída
        objetos[i].tiempo_caida = calcularTiempoCaida(objetos[i].altura);
    }
    
    double fin = omp_get_wtime();
    
    // Mostrar resultados
    cout << "\n--- Resultados de la simulación ---" << endl;
    cout << setw(4) << "ID" << " | " 
         << setw(20) << left << "Nombre" 
         << " | " << setw(10) << right << "Altura(m)" 
         << " | " << setw(10) << "Tiempo(s)"
         << " | " << setw(12) << "Velocidad(m/s)" << endl;
    cout << string(75, '-') << endl;
    
    for (const auto& obj : objetos) {
        double velocidad = calcularVelocidadFinal(obj.altura);
        cout << setw(4) << obj.id << " | " 
             << setw(20) << left << obj.nombre 
             << " | " << setw(10) << right << fixed << setprecision(2) << obj.altura 
             << " | " << setw(10) << setprecision(4) << obj.tiempo_caida
             << " | " << setw(12) << setprecision(2) << velocidad << endl;
    }
    
    cout << "Simulación completada en " << (fin - inicio) << " segundos" << endl;
    
    // Encontrar el objeto con mayor tiempo de caída
    auto max_obj = *max_element(objetos.begin(), objetos.end(), 
        [](const Objeto& a, const Objeto& b) { 
            return a.tiempo_caida < b.tiempo_caida; 
        });
    
    cout << "Objeto con mayor tiempo de caída: " << max_obj.nombre 
         << " (" << max_obj.tiempo_caida << " segundos desde " 
         << max_obj.altura << " metros)" << endl;
    
    return 0;
}
