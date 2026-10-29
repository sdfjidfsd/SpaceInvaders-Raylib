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
    int vida = 3; 
    
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
    
    void Mover(float velocidadX) {
        if (activo) {
            x += velocidadX;
        }
    }

    void Dibujar() {
        if (activo) DrawRectangle(x, y, ancho, alto, color);
    }
};

//BUCLE PRINCIPAL (main)

int main() {
    InitWindow(800, 450, "Space Invaders - Prototype");
    InitAudioDevice();

    // Declaración de entidades principales
    Nave miNave = { 400, 400, 5.0f, 50, 20, BLUE };
    Bala miBala = { 0, 0, 10.0f, 5, 10, false, YELLOW };
    BalaEnemiga miBalaEnemiga = { 0, 0, 11.8f, 5, 10, false, RED };

    // Fila de enemigos (5 unidades espaciadas horizontalmente)
    const int MAX_ENEMIGOS = 5;
    Enemigo enemigos[MAX_ENEMIGOS];
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        enemigos[i] = { 150.0f + (i * 105.0f), 50.0f, 35, 15, RAYWHITE, 0, 3, true };
    }

    float direccionEnemigos = 2.0f; // Velocidad y sentido del movimiento lateral

    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        // --- 1. ACTUALIZACIÓN DE LÓGICA ---
        miNave.Mover();

        // Disparo del jugador
        if (IsKeyDown(KEY_SPACE) && !miBala.activa) {
            miBala.activa = true;
            miBala.x = miNave.x + (miNave.ancho / 2) - (miBala.ancho / 2);
            miBala.y = miNave.y;
        }

        // Movimiento de los enemigos y rebote en los bordes de la pantalla
        bool tocarBorde = false;
        for (int i = 0; i < MAX_ENEMIGOS; i++) {
            if (enemigos[i].activo) {
                enemigos[i].Mover(direccionEnemigos);
                if (enemigos[i].x <= 10 || enemigos[i].x >= GetScreenWidth() - enemigos[i].ancho - 10) {
                    tocarBorde = true;
                }
            }
        }

        // Si algún enemigo toca el borde, invierte la dirección de todos y bajan una línea
        if (tocarBorde) {
            direccionEnemigos *= -1.0f;
            for (int i = 0; i < MAX_ENEMIGOS; i++) {
                enemigos[i].y += 15.0f;
            }
        }

        // Lógica de disparo del enemigo (dispara el primero que esté activo que encuentre)
        for (int i = 0; i < MAX_ENEMIGOS; i++) {
            if (enemigos[i].activo) {
                enemigos[i].timer++;
                if (enemigos[i].timer >= 60) { // Intervalo de disparo ajustado
                    if (!miBalaEnemiga.activa) {
                        miBalaEnemiga.activa = true;
                        miBalaEnemiga.x = enemigos[i].x + (enemigos[i].ancho / 2);
                        miBalaEnemiga.y = enemigos[i].y + enemigos[i].alto;
                    }
                    enemigos[i].timer = 0;
                }
                break; // Solo un enemigo evalúa el disparo por frame para evitar spam
            }
        }

        miBala.Mover();
        miBalaEnemiga.Mover();

        // Chequeo de colisiones (Bala del jugador vs cada Enemigo)
        if (miBala.activa) {
            Rectangle rectBala = { miBala.x, miBala.y, miBala.ancho, miBala.alto };

            for (int i = 0; i < MAX_ENEMIGOS; i++) {
                if (enemigos[i].activo) {
                    Rectangle rectEnemigo = { enemigos[i].x, enemigos[i].y, enemigos[i].ancho, enemigos[i].alto };

                    if (CheckCollisionRecs(rectBala, rectEnemigo)) {
                        enemigos[i].vida--;
                        miBala.activa = false;

                        if (enemigos[i].vida <= 0) {
                            enemigos[i].activo = false;
                        }
                        break; // Salimos del bucle al impactar
                    }
                }
                
            // Chequeo de colisión: Bala enemiga vs Mi Nave
            if (miBalaEnemiga.activa) {
                Rectangle rectBalaEnemiga = { miBalaEnemiga.x, miBalaEnemiga.y, miBalaEnemiga.ancho, miBalaEnemiga.alto };
                Rectangle rectMiNave = { miNave.x, miNave.y, miNave.ancho, miNave.alto };
            if (CheckCollisionRecs(rectBalaEnemiga, rectMiNave)) {
                miNave.vida--;              // Resta vida a tu nave
                miBalaEnemiga.activa = false; // Desactiva la bala enemiga
        }
}
            }
        }

        // --- RENDERIZADO / DIBUJO ---
        BeginDrawing();
        ClearBackground(BLACK);

        // Dibujar entidades del juego
        miNave.Dibujar();
        miBala.Dibujar();

        for (int i = 0; i < MAX_ENEMIGOS; i++) {
            enemigos[i].Dibujar();
        }

        miBalaEnemiga.Dibujar();

        // --- INTERFAZ / HUD ---
        // Texto alineado a la izquierda (Vidas / Puntuación)
        DrawText(TextFormat("VIDAS: %d", miNave.vida), 20, 20, 20, GREEN);
        DrawText("PUNTOS: 0000", GetScreenWidth() - 160, 20, 20, RAYWHITE);

        // Línea divisoria inferior estilo arcade (opcional para estética)
        DrawLine(20, GetScreenHeight() - 40, GetScreenWidth() - 20, GetScreenHeight() - 40, DARKGRAY);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}