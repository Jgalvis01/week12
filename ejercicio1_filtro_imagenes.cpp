#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>
#include <cstring>

using namespace std;

// Estructura simple para representar una imagen en escala de grises
struct Imagen {
    int ancho;
    int alto;
    vector<unsigned char> pixeles;
    
    Imagen(int w, int h) : ancho(w), alto(h), pixeles(w * h, 128) {}
};

// Función para aplicar filtro blanco y negro (simplificado - umbralización)
void aplicarFiltroBlancoNegro(Imagen& img) {
    int umbral = 128;
    
    #pragma omp parallel for
    for (int i = 0; i < img.pixeles.size(); i++) {
        if (img.pixeles[i] >= umbral) {
            img.pixeles[i] = 255; // Blanco
        } else {
            img.pixeles[i] = 0;   // Negro
        }
    }
}

// Función para guardar imagen en formato PGM (formato simple de escala de grises)
void guardarImagen(const Imagen& img, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo) {
        cerr << "Error al crear archivo: " << nombreArchivo << endl;
        return;
    }
    
    // Escribir encabezado PGM
    archivo << "P2\n";
    archivo << img.ancho << " " << img.alto << "\n";
    archivo << "255\n";
    
    // Escribir pixeles
    for (int i = 0; i < img.pixeles.size(); i++) {
        archivo << (int)img.pixeles[i] << " ";
        if ((i + 1) % img.ancho == 0) archivo << "\n";
    }
    
    archivo.close();
}

// Función para crear una imagen de prueba con gradiente
Imagen crearImagenPrueba(int id, int ancho, int alto) {
    Imagen img(ancho, alto);
    
    // Crear un patrón simple basado en el ID
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            int idx = y * ancho + x;
            // Gradiente basado en posición
            img.pixeles[idx] = (x + y + id * 30) % 256;
        }
    }
    
    return img;
}

int main() {
    const int NUM_IMAGENES = 10;
    const int ANCHO = 200;
    const int ALTO = 200;
    
    cout << "=== Ejercicio 1: Filtro a múltiples imágenes ===" << endl;
    cout << "Procesando " << NUM_IMAGENES << " imágenes..." << endl;
    
    // Crear vector de imágenes
    vector<Imagen> imagenes;
    for (int i = 0; i < NUM_IMAGENES; i++) {
        imagenes.push_back(crearImagenPrueba(i, ANCHO, ALTO));
    }
    
    double inicio = omp_get_wtime();
    
    // Procesar imágenes en paralelo (embarazosamente paralelo)
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < NUM_IMAGENES; i++) {
        int thread_id = omp_get_thread_num();
        
        #pragma omp critical
        {
            cout << "Thread " << thread_id << ": Procesando imagen " << (i+1) << endl;
        }
        
        // Aplicar filtro blanco y negro
        aplicarFiltroBlancoNegro(imagenes[i]);
        
        // Guardar imagen
        string nombreArchivo = "imagen_filtrada_" + to_string(i+1) + ".pgm";
        guardarImagen(imagenes[i], nombreArchivo);
    }
    
    double fin = omp_get_wtime();
    cout << "Procesamiento completado en " << (fin - inicio) << " segundos" << endl;

    
    return 0;
}
