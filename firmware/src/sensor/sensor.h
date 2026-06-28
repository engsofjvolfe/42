#pragma once

// sensor.h — Interface publica de MOD_SENSOR
// Derivado de: spec/sensor/sensor.json + system/01_arquitetura.md#interfaces-modulos
// [VER: 01_arquitetura.md#interfaces-modulos]

#include <stdint.h>

// ---------------------------------------------------------------------------
// Tipos publicos
// ---------------------------------------------------------------------------

struct EventoImpacto {
    enum class Zona : uint8_t {
        LARANJA = 0,
        AZUL    = 1,
        AMARELO = 2,
        ROXO    = 3
    };

    Zona     zona;
    uint32_t timestamp_ms;
};

// ---------------------------------------------------------------------------
// API publica
// [VER: 01_arquitetura.md#interfaces-modulos]
// ---------------------------------------------------------------------------

// Inicializa GPIOs, ADC e timers internos de debounce.
// Deve ser chamado uma vez em setup().
void sensorInit();

// Registra callback invocado a cada deteccao de impacto valido.
// Passar nullptr desabilita o callback.
using SensorCallback = void(*)(EventoImpacto);
void sensorOnImpacto(SensorCallback cb);

// Varre todas as zonas, detecta impactos acima do threshold e aplica debounce.
// Deve ser chamado a cada iteracao de loop().
void sensorLoop();
