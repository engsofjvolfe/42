#pragma once

// Mock minimo de Arduino.h para compilacao nativa em testes Unity.
// [VER: TESTING_STANDARD.md#mock-arduino-h]
// Nao usar fora de firmware/test/.
// Cada simbolo aqui tem rastreabilidade para o modulo de firmware que o usa.

#include <stdint.h>

// --- HARDCODED_TESTE: tipos de plataforma Arduino ESP32 3.x para ADC ---
// Corresponde a DM-02 em CODING_STANDARD.md#decisoes-manuais
// adc_attenuation_t (camada Arduino) substitui adc_atten_t (ESP-IDF) a partir do Arduino ESP32 3.x
typedef int adc_attenuation_t;
#define ADC_11db 3

#define INPUT  0
#define OUTPUT 1

// String — classe minima para dados de usuario com tamanho variavel.
// [VER: CODING_STANDARD.md#tipagem]
// Rastreabilidade: game.h/game.cpp (ConfigSessao.nome_crianca)
class String {
public:
    String() {}
    explicit String(const char*) {}
    String(const String&)            = default;
    String& operator=(const String&) = default;
    String& operator=(const char*)   { return *this; }
};

// Declaracoes (definicoes em test_main.cpp de cada modulo — padrao declare→define)
uint16_t analogRead(uint8_t pin);
uint32_t millis();
void     pinMode(uint8_t pin, uint8_t mode);
void     analogSetAttenuation(adc_attenuation_t attenuation);

// random(max) — retorna long no intervalo [0, max-1].
// Rastreabilidade: game.cpp (Mecanismo A Fisher-Yates, Mecanismo B peso).
// [VER: 04_logica_jogo.md#aleatoriedade]
long random(long max);
