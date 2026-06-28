// main.cpp — Ponto de entrada do firmware
// Derivado de: system/01_arquitetura.md#modulos-firmware
// [VER: 01_arquitetura.md#modulos-firmware]

#include <Arduino.h>
#include "sensor/sensor.h"

// ---------------------------------------------------------------------------
// Callback de impacto (stub — MOD_JOGO ainda nao implementado)
// ---------------------------------------------------------------------------
static void onImpacto(EventoImpacto ev) {
    // [STUB] Encaminhar para MOD_JOGO quando implementado
    (void)ev;
}

// ---------------------------------------------------------------------------
// Setup / Loop
// ---------------------------------------------------------------------------
void setup() {
    sensorInit();
    sensorOnImpacto(onImpacto);
}

void loop() {
    sensorLoop();
}
