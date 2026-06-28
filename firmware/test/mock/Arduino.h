#pragma once

// Mock minimo de Arduino.h para compilacao nativa em testes Unity.
// [VER: TESTING_STANDARD.md#mock-arduino-h]
// Nao usar fora de firmware/test/.
// Cada simbolo aqui tem rastreabilidade para o modulo de firmware que o usa.

#include <stdint.h>

// --- HARDCODED_TESTE: tipos de plataforma ESP-IDF/Arduino para ADC ---
// Corresponde a DM-02 em CODING_STANDARD.md#decisoes-manuais
typedef int adc_atten_t;
#define ADC_ATTEN_DB_11 3

#define INPUT  0
#define OUTPUT 1

// Declaracoes (definicoes em test_main.cpp de cada modulo — padrao declare→define)
uint16_t analogRead(uint8_t pin);
uint32_t millis();
void     pinMode(uint8_t pin, uint8_t mode);
void     analogSetAttenuation(adc_atten_t attenuation);
