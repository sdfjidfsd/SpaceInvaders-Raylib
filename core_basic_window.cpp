/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   Example complexity rating: [★☆☆☆] 1/4
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute 'raylib_compile_execute' script
*   Note that compiled executable is placed in the same folder as .c file
*
*   To test the examples on Web, press F6 and execute 'raylib_compile_execute_web' script
*   Web version of the program is generated in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2026 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"


//ESTRUCTURAS DE ENTIDADES (Jugador, Enemigo, Balas)

struct Bala {
    float x, y;
    float velocidad;
    float ancho, alto;
    bool activa;
    Color color;

    void Mover() {
        if (activa) {
            y -= velocidad;
            if (y < 0) activa = false;
        }
    }

    void Dibujar() {
        if (activa) DrawRectangle(x, y, ancho, alto, color);
    }
};

struct BalaEnemiga {
    float x, y;
    float velocidad;
    float ancho, alto;
    bool activa;
    Color color;

    void Mover() {
        if (activa) {
            y += velocidad;
            if (y > GetScreenHeight()) activa = false;
        }
    }

    void Dibujar() {
        if (activa) DrawRectangle(x, y, ancho, alto, color);
    }
};

struct Nave {
    float x, y;
    float velocidad;
    float ancho, alto;
    Color color;

    void Mover() {
        if (IsKeyDown(KEY_LEFT) && x > 0) x -= velocidad;
        if (IsKeyDown(KEY_RIGHT) && x < GetScreenWidth() - ancho) x += velocidad;
    }

    void Dibujar() {
        DrawRectangle(x, y, ancho, alto, color);
    }
};

struct Enemigo {
    float x, y;
    float ancho, alto;
    Color color;
    int timer = 0;
    int vida = 3;
    bool activo = true;

    void Dibujar() {
        if (activo) DrawRectangle(x, y, ancho, alto, color);
    }
};


//SISTEMAS DE AUDIO (Efectos de sonido)


struct EfectosSonoros {
    Sound disparoJugador;
    Sound disparoEnemigo;
    Sound impacto;
    Sound muerteEnemigo;
    Sound muerteJugador;

    void Cargar() {
        disparoJugador = LoadSound("sounds/playerbullet.wav");
        disparoEnemigo = LoadSound("sounds/enemybullet.wav");
        impacto        = LoadSound("sounds/impact.wav");
        muerteEnemigo  = LoadSound("sounds/deadenemy.wav");
        muerteJugador  = LoadSound("sounds/dead.wav");
    }

    void Descargar() {
        UnloadSound(disparoJugador);
        UnloadSound(disparoEnemigo);
        UnloadSound(impacto);
        UnloadSound(muerteEnemigo);
        UnloadSound(muerteJugador);
    }
};


//BUCLE PRINCIPAL (main)

int main() {
    InitWindow(800, 450, "Space Invaders - Prototype");
    InitAudioDevice();

    EfectosSonoros misSonidos;
    misSonidos.Cargar();

    // Declaración de entidades principales
    Nave miNave = { 400, 400, 5.0f, 50, 20, BLUE };
    Bala miBala = { 0, 0, 10.0f, 5, 10, false, YELLOW };
    Enemigo miNaveEnemiga = { 400, 50, 35, 15, RAYWHITE, 0, 3, true };
    BalaEnemiga miBalaEnemiga = { 0, 0, 11.8f, 5, 10, false, RED };

    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        // --- 1. ACTUALIZACIÓN DE LÓGICA ---
        miNave.Mover();

        // Disparo del jugador
        if (IsKeyDown(KEY_SPACE) && !miBala.activa) {
            PlaySound(misSonidos.disparoJugador);
            miBala.activa = true;
            miBala.x = miNave.x + (miNave.ancho / 2) - (miBala.ancho / 2);
            miBala.y = miNave.y;
        }

        // Lógica de disparo del enemigo
        if (miNaveEnemiga.activo) {
            miNaveEnemiga.timer++;
            if (miNaveEnemiga.timer >= 30) {
                if (!miBalaEnemiga.activa) {
                    miBalaEnemiga.activa = true;
                    miBalaEnemiga.x = miNaveEnemiga.x + (miNaveEnemiga.ancho / 2);
                    miBalaEnemiga.y = miNaveEnemiga.y + miNaveEnemiga.alto;
                    PlaySound(misSonidos.disparoEnemigo);
                }
                miNaveEnemiga.timer = 0;
            }
        }

        miBala.Mover();
        miBalaEnemiga.Mover();

        // Chequeo de colisiones (Bala vs Enemigo)
        if (miBala.activa && miNaveEnemiga.activo) {
            Rectangle rectBala = { miBala.x, miBala.y, miBala.ancho, miBala.alto };
            Rectangle rectEnemigo = { miNaveEnemiga.x, miNaveEnemiga.y, miNaveEnemiga.ancho, miNaveEnemiga.alto };

            if (CheckCollisionRecs(rectBala, rectEnemigo)) {
                miNaveEnemiga.vida--;
                miBala.activa = false;

                if (miNaveEnemiga.vida <= 0) {
                    miNaveEnemiga.activo = false;
                    PlaySound(misSonidos.muerteEnemigo);
                } else {
                    PlaySound(misSonidos.impacto);
                }
            }
        }

        //RENDERIZADO / DIBUJO ---
        BeginDrawing();
        ClearBackground(BLACK);

        miNave.Dibujar();
        miBala.Dibujar();
        miNaveEnemiga.Dibujar();
        miBalaEnemiga.Dibujar();

        EndDrawing();
    }

    //LIMPIEZA DE RECURSOS ---
    misSonidos.Descargar();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}