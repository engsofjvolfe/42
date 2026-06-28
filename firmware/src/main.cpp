// main.cpp — Ponto de entrada do firmware
// Derivado de: system/01_arquitetura.md#modulos-firmware
// [VER: 01_arquitetura.md#modulos-firmware]

#include <Arduino.h>
#include "sensor/sensor.h"
#include "visual/visual.h"

// ---------------------------------------------------------------------------
// Callback de impacto (stub — MOD_JOGO ainda nao implementado)
// ---------------------------------------------------------------------------
static void onImpacto(EventoImpacto ev) {
    // [STUB] Encaminhar para MOD_JOGO quando implementado
    (void)ev;
}

// ---------------------------------------------------------------------------
// Setup / Loop — ordem derivada de CODING_STANDARD.md#modularidade-ordem-init
// [VER: 01_arquitetura.md#modulos-firmware]
// ---------------------------------------------------------------------------
void setup() {
    sensorInit();
    sensorOnImpacto(onImpacto);
    visualInit();
}

void loop() {
    sensorLoop();
    visualLoop();
}
