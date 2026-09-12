# Space Invaders - Core Engine Prototype (C++ / Raylib)

Prototipo interactivo desarrollado en **C++** utilizando la librería gráfica **Raylib**. El proyecto tiene como objetivo demostrar la implementación manual de la lógica base para un videojuego 2D sin depender de motores gráficos comerciales (como Godot o Unity).

## 🚀 Características Implementadas
- **Game Loop Nativo:** Gestión manual de actualización de estados y renderizado por fotograma.
- **Detección de Colisiones AABB:** Cálculo de hitboxes en tiempo real para proyectiles y entidades.
- **Gestión de Entidades:** Control de movimiento con delimitación de bordes en pantalla y ciclo de vida de entidades (salud y eliminación de objetos).
- **Disparos Automáticos:** Lógica de temporizadores para el comportamiento de la entidad enemiga.

## 🛠️ Tecnologías Utilizadas
- **Lenguaje:** C++
- **Librería Gráfica:** Raylib
- **Compilador:** GCC / MinGW

## 💻 Compilación y Ejecución
Para compilar el proyecto en Windows desde la terminal con `g++`:

```bash
g++ core_basic_window.cpp -o SpaceInvaders.exe -lraylib -lopengl32 -lgdi32 -lwinmm
