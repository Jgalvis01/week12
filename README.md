# Algoritmos Embarazosamente Paralelos con OpenMP

Este proyecto implementa varios ejercicios de algoritmos embarazosamente paralelos utilizando OpenMP en C++.

## 📋 Ejercicios Implementados

### Ejercicio 1: Aplicar filtro a múltiples imágenes
**Archivo:** `ejercicio1_filtro_imagenes.cpp`

Procesa un lote de imágenes aplicando un filtro blanco y negro mediante umbralización.
- Genera imágenes de prueba con gradientes
- Aplica filtro en paralelo a cada imagen
- Guarda resultados en formato PGM (Portable Gray Map)

**Paralelización:** Cada imagen se procesa independientemente.

### Ejercicio 3: Calcular la suma de elementos en múltiples vectores
**Archivo:** `ejercicio3_suma_vectores.cpp`

Suma los elementos de múltiples vectores de forma independiente.
- Genera vectores aleatorios de gran tamaño
- Cada vector se suma de forma paralela
- Utiliza `reduction` para optimizar la suma dentro de cada vector

**Paralelización:** Cada vector se procesa en un thread diferente.

### Ejercicio 4: Buscar un valor en múltiples vectores
**Archivo:** `ejercicio4_buscar_valor.cpp`

Busca un valor específico en varios vectores de forma paralela.
- Genera vectores aleatorios grandes
- Implementa búsqueda con early exit
- Reporta si el valor fue encontrado y en qué posición

**Paralelización:** Cada búsqueda se realiza independientemente en diferentes vectores.

### Ejercicio 5: Simulación de caída libre para múltiples objetos
**Archivo:** `ejercicio5_caida_libre.cpp`

Calcula el tiempo de caída libre para diferentes objetos desde distintas alturas.
- Genera objetos con diferentes propiedades (altura, masa)
- Calcula tiempo de caída usando la fórmula: `t = √(2h/g)`
- Calcula velocidad final de impacto

**Paralelización:** Cada objeto se simula independientemente.

## 🚀 Compilación y Ejecución

### En Docker con OpenMP

```bash
# Compilar todos los ejercicios
make all

# Compilar ejercicio específico
make ejercicio1_filtro_imagenes
make ejercicio3_suma_vectores
make ejercicio4_buscar_valor
make ejercicio5_caida_libre

# Ejecutar ejercicio específico
make run1  # Ejercicio 1
make run3  # Ejercicio 3
make run4  # Ejercicio 4
make run5  # Ejercicio 5

# Ejecutar todos los ejercicios
make run_all

# Limpiar archivos compilados
make clean
```

### Compilación manual

```bash
# Ejercicio 1
g++ -fopenmp -O2 -std=c++11 ejercicio1_filtro_imagenes.cpp -o ejercicio1_filtro_imagenes

# Ejercicio 3
g++ -fopenmp -O2 -std=c++11 ejercicio3_suma_vectores.cpp -o ejercicio3_suma_vectores

# Ejercicio 4
g++ -fopenmp -O2 -std=c++11 ejercicio4_buscar_valor.cpp -o ejercicio4_buscar_valor

# Ejercicio 5
g++ -fopenmp -O2 -std=c++11 ejercicio5_caida_libre.cpp -o ejercicio5_caida_libre
```

## 🐳 Uso con Docker

Si estás usando un contenedor Docker con OpenMP instalado:

```bash
# Entrar al contenedor
docker exec -it <nombre_contenedor> bash

# Navegar al directorio
cd /ruta/al/proyecto

# Compilar y ejecutar
make run_all
```

## ⚙️ Configuración de OpenMP

Para controlar el número de threads:

```bash
# Linux/macOS/Docker
export OMP_NUM_THREADS=4

# Windows PowerShell
$env:OMP_NUM_THREADS=4

# Luego ejecutar el programa
./ejercicio1_filtro_imagenes
```

## 📊 Características de los Ejercicios

### Algoritmos Embarazosamente Paralelos
Todos los ejercicios implementan problemas donde:
- Las tareas son independientes entre sí
- No hay dependencias de datos entre iteraciones
- No se requiere sincronización compleja
- Son ideales para paralelización

### Directivas OpenMP Utilizadas

- `#pragma omp parallel for`: Paraleliza bucles
- `#pragma omp critical`: Secciones críticas para I/O
- `reduction(+:variable)`: Reducción paralela para sumas
- `schedule(dynamic)`: Balanceo dinámico de carga

## 📁 Archivos Generados

### Ejercicio 1
Genera archivos de imagen en formato PGM:
- `imagen_filtrada_1.pgm`
- `imagen_filtrada_2.pgm`
- ... (una por cada imagen procesada)

Para visualizar archivos PGM, puedes usar GIMP, ImageMagick o convertirlos:
```bash
convert imagen_filtrada_1.pgm imagen_filtrada_1.png
```

## 🔧 Requisitos

- Compilador C++ con soporte para C++11 o superior
- OpenMP (incluido en GCC, Clang, MSVC)
- Make (opcional, para usar el Makefile)

## 📈 Rendimiento

Los ejercicios están diseñados para demostrar aceleración paralela:
- El tiempo de ejecución debería reducirse con más threads
- Los problemas son computacionalmente intensivos
- Se puede medir el speedup comparando tiempos con 1 vs N threads

## 💡 Notas

1. **Ejercicio 1**: Las imágenes se guardan en formato PGM (texto ASCII) para simplicidad
2. **Ejercicio 3**: Los vectores tienen 1 millón de elementos para visualizar beneficios de paralelización
3. **Ejercicio 4**: El valor buscado se inserta estratégicamente en algunos vectores
4. **Ejercicio 5**: Se simula la física de caída libre ignorando resistencia del aire

## 📝 Autor

Implementación de ejercicios de paralelización con OpenMP para el curso de Programación Paralela.
