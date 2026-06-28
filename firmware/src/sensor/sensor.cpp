// sensor.cpp — Implementacao de MOD_SENSOR
// Derivado de: spec/sensor/sensor.json + 02_sensor_impacto.md#algoritmo-deteccao
// [VER: 02_sensor_impacto.md#algoritmo-deteccao]

#include "sensor.h"
#include "sensor_config.h"
#include <Arduino.h>

// --- HARDCODED JUSTIFICADO: DM-02 ---
// Tipo da camada Arduino (Arduino ESP32 3.x / IDF5) para range 0-3.3V no ADC1.
// adc_attenuation_t substitui adc_atten_t (ESP-IDF direto) a partir do Arduino ESP32 3.x.
// Nao e parametro de dominio; e configuracao de silicio.
// Declarada apos includes de plataforma conforme sensor_config.h#DM-02.
static constexpr adc_attenuation_t SENSOR_ADC_ATENUACAO = ADC_11db;

// ---------------------------------------------------------------------------
// Tabelas de mapeamento (indexadas por zona 0-3)
// Derivado de: spec/sensor/sensor.json#mapeamento_gpios
// ---------------------------------------------------------------------------

// --- DERIVADO: sensor_config.h#SENSOR_GPIO_* ---
static constexpr uint8_t s_gpios[SENSOR_N_ZONAS] = {
    SENSOR_GPIO_LARANJA,
    SENSOR_GPIO_AZUL,
    SENSOR_GPIO_AMARELO,
    SENSOR_GPIO_ROXO
};

// --- DERIVADO: sensor.h#EventoImpacto::Zona ---
static constexpr EventoImpacto::Zona s_zonas[SENSOR_N_ZONAS] = {
    EventoImpacto::Zona::LARANJA,
    EventoImpacto::Zona::AZUL,
    EventoImpacto::Zona::AMARELO,
    EventoImpacto::Zona::ROXO
};

// ---------------------------------------------------------------------------
// Estado interno
// ---------------------------------------------------------------------------

static SensorCallback s_callback;
static uint32_t       s_debounceTs[SENSOR_N_ZONAS];

// ---------------------------------------------------------------------------
// Implementacao
// ---------------------------------------------------------------------------

void sensorInit() {
    analogSetAttenuation(SENSOR_ADC_ATENUACAO);

    for (uint8_t i = 0u; i < SENSOR_N_ZONAS; i++) {
        pinMode(s_gpios[i], INPUT);
        // Inicializar fora da janela de debounce para que o primeiro
        // impacto seja detectado imediatamente apos boot.
        s_debounceTs[i] = millis() - (static_cast<uint32_t>(SENSOR_DEBOUNCE_MS) + 1u);
    }

    s_callback = nullptr;
}

void sensorOnImpacto(SensorCallback cb) {
    s_callback = cb;
}

void sensorLoop() {
    const uint32_t agora = millis();

    for (uint8_t i = 0u; i < SENSOR_N_ZONAS; i++) {
        const uint16_t leitura = analogRead(s_gpios[i]);

        if (leitura <= SENSOR_THRESHOLD_ADC) {
            continue;
        }

        if ((agora - s_debounceTs[i]) <= static_cast<uint32_t>(SENSOR_DEBOUNCE_MS)) {
            continue;
        }

        s_debounceTs[i] = agora;

        if (s_callback != nullptr) {
            EventoImpacto ev;
            ev.zona         = s_zonas[i];
            ev.timestamp_ms = agora;
            s_callback(ev);
        }
    }
}
