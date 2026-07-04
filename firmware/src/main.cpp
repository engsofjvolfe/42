// main.cpp — Ponto de entrada do firmware
// Derivado de: system/01_arquitetura.md#modulos-firmware
// [VER: 01_arquitetura.md#modulos-firmware]
// [VER: 01_arquitetura.md#interface-sensor-jogo]
// [VER: 01_arquitetura.md#interface-jogo-wifi]
// [VER: 01_arquitetura.md#interface-jogo-led]  — fiacao interna a game.cpp
// [VER: 01_arquitetura.md#diagrama-estados]    — BOOT deve concluir antes de AGUARDANDO_CONEXAO

#include <Arduino.h>
#include "sensor/sensor.h"
#include "visual/visual.h"
#include "visual/visual_config.h"
#include "game/game.h"
#include "interface/interface.h"

// ---------------------------------------------------------------------------
// Estado interno — [VER: 01_arquitetura.md#diagrama-estados]
// BOOT --> AGUARDANDO_CONEXAO só ocorre com "varredura LEDs concluída".
// interfaceInit() sobe o AP/servidor (o que torna AGUARDANDO_CONEXAO possível)
// e por isso só pode ser chamado após a animação de boot terminar — do
// contrário uma sessão iniciada durante o boot tem seu primeiro comando de
// LED sobrescrito pelo passo final da animação (todos_OFF).
// ---------------------------------------------------------------------------
static bool s_interface_iniciada = false;

// ---------------------------------------------------------------------------
// Setup — ordem derivada de [VER: 01_arquitetura.md#modularidade-ordem-init]
// ---------------------------------------------------------------------------
void setup() {
    sensorInit();
    visualInit();
    gameInit();

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

    // --- DERIVADO: visual_config.h#VISUAL_BOOT_DURACAO_MS ---
    if (!s_interface_iniciada && millis() >= static_cast<uint32_t>(VISUAL_BOOT_DURACAO_MS)) {
        interfaceInit(); // registra gameOnEvento internamente
        s_interface_iniciada = true;
    }

    if (s_interface_iniciada) {
        interfaceLoop();
    }
}
