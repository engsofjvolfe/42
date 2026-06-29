// main.cpp — Ponto de entrada do firmware
// Derivado de: system/01_arquitetura.md#modulos-firmware
// [VER: 01_arquitetura.md#modulos-firmware]
// [VER: 01_arquitetura.md#interface-sensor-jogo]
// [VER: 01_arquitetura.md#interface-jogo-wifi]
// [VER: 01_arquitetura.md#interface-jogo-led]  — fiacao interna a game.cpp

#include <Arduino.h>
#include "sensor/sensor.h"
#include "visual/visual.h"
#include "game/game.h"
#include "interface/interface.h"

// ---------------------------------------------------------------------------
// Setup — ordem derivada de [VER: 01_arquitetura.md#modularidade-ordem-init]
// ---------------------------------------------------------------------------
void setup() {
    sensorInit();
    visualInit();
    gameInit();
    interfaceInit(); // registra gameOnEvento internamente

    // Fiacao sensor → jogo — [VER: 01_arquitetura.md#interface-sensor-jogo]
    sensorOnImpacto(gameOnImpacto);
}

// ---------------------------------------------------------------------------
// Loop — [VER: 01_arquitetura.md#modulos-firmware]
// ---------------------------------------------------------------------------
void loop() {
    sensorLoop();
    visualLoop();
    gameLoop();
    interfaceLoop();
}
