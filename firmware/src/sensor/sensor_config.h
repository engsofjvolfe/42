#pragma once

// sensor_config.h
// Fonte: spec/sensor/sensor.json (via spec/firmware_constants.json)
// Nenhum valor neste arquivo e inventado.
// Toda alteracao exige atualizacao de firmware_constants.json e regeneracao.

#include <stdint.h>

// --- DERIVADO: spec/sensor/sensor.json#mapeamento_gpios[0].gpio ---
constexpr uint8_t SENSOR_GPIO_LARANJA = 34;

// --- DERIVADO: spec/sensor/sensor.json#mapeamento_gpios[1].gpio ---
constexpr uint8_t SENSOR_GPIO_AZUL = 35;

// --- DERIVADO: spec/sensor/sensor.json#mapeamento_gpios[2].gpio ---
constexpr uint8_t SENSOR_GPIO_AMARELO = 32;

// --- DERIVADO: spec/sensor/sensor.json#mapeamento_gpios[3].gpio ---
constexpr uint8_t SENSOR_GPIO_ROXO = 33;

// --- DERIVADO: spec/sensor/sensor.json#deteccao.threshold ---
constexpr uint16_t SENSOR_THRESHOLD_ADC = 500;  // [CALIBRAR] — confirmar apos prototipagem

// --- DERIVADO: spec/sensor/sensor.json#deteccao.debounce_ms ---
constexpr uint16_t SENSOR_DEBOUNCE_MS = 200;

// --- DERIVADO: spec/sensor/sensor.json#adc.contagens_maximas ---
constexpr uint16_t SENSOR_ADC_MAX = 4095;

// --- DERIVADO: spec/sensor/sensor.json#componente.quantidade ---
constexpr uint8_t SENSOR_N_ZONAS = 4;

// --- HARDCODED JUSTIFICADO: DM-01 ---
// Timeout do WDT nao e parametro de dominio. Calculado como duracao_max_loop_ms multiplicado por fator de seguranca. Determinar apos medir o loop mais longo na implementacao. Comentario obrigatorio com o calculo explicito.
// [DETERMINAR] constexpr uint32_t SENSOR_WDT_TIMEOUT_MS = ???;

// --- HARDCODED JUSTIFICADO: DM-02 ---
// Tipo da camada Arduino (Arduino ESP32 3.x / IDF5) para range 0-3.3V no ADC1. adc_attenuation_t substitui adc_atten_t (ESP-IDF direto) a partir do Arduino ESP32 3.x. Nao e parametro de dominio; e configuracao de silicio. Nao existe campo correspondente em nenhum spec JSON.
// [PLATAFORMA] constexpr adc_attenuation_t SENSOR_ADC_ATENUACAO = ADC_11db;
// Declarar em sensor.cpp apos includes de plataforma.
