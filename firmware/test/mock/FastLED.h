#pragma once

// FastLED.h — Mock minimo de FastLED para compilacao nativa em testes Unity.
// [VER: TESTING_STANDARD.md#mock-arduino-h]
// Nao usar fora de firmware/test/.
// Cada simbolo tem rastreabilidade para visual.cpp que o usa.

#include <stdint.h>

// --- HARDCODED_JUSTIFICADO: DM-03 ---
// EOrder e o tipo de enum da biblioteca FastLED para ordem de cores.
// GRB e o valor concreto do enum; em native o valor inteiro e irrelevante
// (e apenas tag de template — nenhum protocolo de hardware e executado).
// [VER: CODING_STANDARD.md#decisoes-manuais]
typedef int EOrder;
#define GRB 0

// --- HARDCODED_JUSTIFICADO: WS2812B ---
// Tag de tipo de CI usado como parametro de template em addLeds<WS2812B, ...>.
// Em native nao executa nenhum protocolo; necessario apenas para compilacao.
struct WS2812B {};

// --- CRGB: estrutura de cor RGB usada por FastLED ---
// Campos r, g, b derivados de visual.json#cores_rgb (formato RGB uint8).
struct CRGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    constexpr CRGB() : r(0u), g(0u), b(0u) {}
    constexpr CRGB(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}

    bool operator==(const CRGB& o) const { return r == o.r && g == o.g && b == o.b; }
    bool operator!=(const CRGB& o) const { return !(*this == o); }
};

// --- Estado observavel do mock ---
// Tamanho de g_mock_led_buf definido em test_main.cpp como VISUAL_N_LEDS.
// Padrao declare->define: [VER: TESTING_STANDARD.md#mock-declare-define]
extern CRGB g_mock_led_buf[];
extern bool g_mock_show_called;

// --- CFastLED: classe controladora mockada ---
class CFastLED {
    CRGB*   m_leds  = nullptr;
    uint8_t m_count = 0u;

public:
    // addLeds<IC, DATA_PIN, ORDER>(leds, count)
    // Registra ponteiro e contagem do array de LEDs para uso em show().
    template<typename IC, uint8_t DATA_PIN, EOrder ORDER>
    CFastLED& addLeds(CRGB* leds, uint8_t count) {
        m_leds  = leds;
        m_count = count;
        return *this;
    }

    void setBrightness(uint8_t) {}

    // show(): copia estado de m_leds para g_mock_led_buf (observavel nos testes).
    // Limite de copia: m_count (derivado de addLeds — sem hardcode de tamanho).
    void show() {
        g_mock_show_called = true;
        for (uint8_t i = 0u; i < m_count; i++) {
            g_mock_led_buf[i] = m_leds[i];
        }
    }
};

// Instancia global — definida em test_main.cpp.
extern CFastLED FastLED;
