// visual.cpp — Implementacao de MOD_LED
// Derivado de: spec/visual/visual.json + 03_saida_visual.md#mapeamento-led
// [VER: 03_saida_visual.md#mapeamento-led]

#include "visual.h"
#include "visual_config.h"
#include <FastLED.h>
#include <Arduino.h>

// --- HARDCODED JUSTIFICADO: DM-03 ---
// GRB e enum da biblioteca FastLED. visual.json#componente.color_order registra
// 'GRB' como string; o simbolo C++ e fornecido pela API da biblioteca e nao pode
// ser parametrizado sem reflexao.
// Declarado apos includes de plataforma conforme visual_config.h#DM-03.
static constexpr EOrder VISUAL_COLOR_ORDER = GRB;

// ---------------------------------------------------------------------------
// Paleta de cores indexada — derivada de visual.json#cores_rgb
// Ordem: Laranja, Azul, Amarelo, Roxo (conforme boot_animation.passos)
// ---------------------------------------------------------------------------

// --- DERIVADO: visual_config.h#VISUAL_COR_*_R/G/B ---
static const CRGB s_paleta[] = {
    CRGB(VISUAL_COR_LARANJA_R, VISUAL_COR_LARANJA_G, VISUAL_COR_LARANJA_B),
    CRGB(VISUAL_COR_AZUL_R,    VISUAL_COR_AZUL_G,    VISUAL_COR_AZUL_B),
    CRGB(VISUAL_COR_AMARELO_R, VISUAL_COR_AMARELO_G, VISUAL_COR_AMARELO_B),
    CRGB(VISUAL_COR_ROXO_R,    VISUAL_COR_ROXO_G,    VISUAL_COR_ROXO_B),
};

// Tamanho da paleta derivado do array acima (nao hardcoded).
static constexpr uint8_t VISUAL_N_PALETA =
    static_cast<uint8_t>(sizeof(s_paleta) / sizeof(s_paleta[0]));

// ---------------------------------------------------------------------------
// Sequencia de passos da boot animation
// Derivado de visual.json#boot_animation.passos
// led_indice == -1 significa todos OFF (passo de pausa)
// ---------------------------------------------------------------------------

struct BootPasso {
    int8_t  led_indice;  // indice do LED a acender; -1 = todos OFF
    uint8_t cor_indice;  // indice em s_paleta
    uint16_t duracao_ms; // duracao deste passo em ms
};

// --- DERIVADO: visual.json#boot_animation.passos + visual_config.h#VISUAL_BOOT_PASSO_MS/PAUSA_MS ---
static constexpr BootPasso s_boot_passos[] = {
    { 0, 0, VISUAL_BOOT_PASSO_MS },  // LED[0] = Laranja
    { 1, 1, VISUAL_BOOT_PASSO_MS },  // LED[1] = Azul
    { 2, 2, VISUAL_BOOT_PASSO_MS },  // LED[2] = Amarelo
    { 0, 3, VISUAL_BOOT_PASSO_MS },  // LED[0] = Roxo
    {-1, 0, VISUAL_BOOT_PAUSA_MS },  // Todos OFF
};

static constexpr uint8_t VISUAL_N_BOOT_PASSOS =
    static_cast<uint8_t>(sizeof(s_boot_passos) / sizeof(s_boot_passos[0]));

// Duracao de cada passo da celebracao: duracao_total / (rotacoes * n_paleta)
// --- DERIVADO: visual_config.h#VISUAL_CELEBRACAO_DURACAO_MS e VISUAL_CELEBRACAO_ROTACOES ---
static constexpr uint16_t VISUAL_CELEBRACAO_PASSO_MS =
    static_cast<uint16_t>(
        static_cast<uint32_t>(VISUAL_CELEBRACAO_DURACAO_MS) /
        (static_cast<uint32_t>(VISUAL_CELEBRACAO_ROTACOES) *
         static_cast<uint32_t>(VISUAL_N_PALETA))
    );

// ---------------------------------------------------------------------------
// Estado interno
// ---------------------------------------------------------------------------

// --- DERIVADO: visual_config.h#VISUAL_N_LEDS ---
static CRGB s_leds[VISUAL_N_LEDS];

enum class AnimacaoAtiva : uint8_t { NENHUMA, BOOT, CELEBRACAO };

static AnimacaoAtiva s_animacao;
static uint32_t      s_animacao_inicio_ms;
static uint8_t       s_passo_atual;
static uint32_t      s_passo_inicio_ms;

// ---------------------------------------------------------------------------
// Funcoes internas
// ---------------------------------------------------------------------------

static void _todos_off() {
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        s_leds[i] = CRGB(0u, 0u, 0u);
    }
    FastLED.show();
}

static void _renderizar_boot_passo(uint8_t passo) {
    const BootPasso& p = s_boot_passos[passo];
    if (p.led_indice < 0) {
        _todos_off();
        return;
    }
    const uint8_t idx = static_cast<uint8_t>(p.led_indice);
    // Apaga todos antes de acender o passo atual (apenas um LED por vez no boot)
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        s_leds[i] = CRGB(0u, 0u, 0u);
    }
    s_leds[idx] = s_paleta[p.cor_indice];
    FastLED.show();
}

static void _renderizar_celebracao_passo(uint8_t passo_cor) {
    // Todos os LEDs simultaneamente com a cor do passo atual
    // --- DERIVADO: visual.json#animacao_celebracao.leds_simultaneos ---
    const CRGB cor = s_paleta[passo_cor % VISUAL_N_PALETA];
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        s_leds[i] = cor;
    }
    FastLED.show();
}

// ---------------------------------------------------------------------------
// API publica
// ---------------------------------------------------------------------------

void visualInit() {
    // --- DERIVADO: visual_config.h#VISUAL_GPIO_DADOS, VISUAL_N_LEDS ---
    FastLED.addLeds<WS2812B, VISUAL_GPIO_DADOS, GRB>(s_leds, VISUAL_N_LEDS);
    // --- DERIVADO: visual_config.h#VISUAL_BRIGHTNESS ---
    FastLED.setBrightness(VISUAL_BRIGHTNESS);

    s_animacao         = AnimacaoAtiva::BOOT;
    s_animacao_inicio_ms = millis();
    s_passo_atual      = 0u;
    s_passo_inicio_ms  = s_animacao_inicio_ms;

    _renderizar_boot_passo(0u);
}

void visualLoop() {
    const uint32_t agora = millis();

    if (s_animacao == AnimacaoAtiva::BOOT) {
        // --- DERIVADO: visual_config.h#VISUAL_BOOT_DURACAO_MS ---
        if (agora - s_animacao_inicio_ms >= static_cast<uint32_t>(VISUAL_BOOT_DURACAO_MS)) {
            _todos_off();
            s_animacao = AnimacaoAtiva::NENHUMA;
            return;
        }
        const uint16_t duracao = s_boot_passos[s_passo_atual].duracao_ms;
        if (agora - s_passo_inicio_ms >= static_cast<uint32_t>(duracao)) {
            s_passo_atual = (s_passo_atual + 1u) % VISUAL_N_BOOT_PASSOS;
            s_passo_inicio_ms = agora;
            _renderizar_boot_passo(s_passo_atual);
        }
        return;
    }

    if (s_animacao == AnimacaoAtiva::CELEBRACAO) {
        // --- DERIVADO: visual_config.h#VISUAL_CELEBRACAO_DURACAO_MS ---
        if (agora - s_animacao_inicio_ms >= static_cast<uint32_t>(VISUAL_CELEBRACAO_DURACAO_MS)) {
            _todos_off();
            s_animacao = AnimacaoAtiva::NENHUMA;
            return;
        }
        if (agora - s_passo_inicio_ms >= static_cast<uint32_t>(VISUAL_CELEBRACAO_PASSO_MS)) {
            s_passo_atual++;
            s_passo_inicio_ms = agora;
            _renderizar_celebracao_passo(s_passo_atual);
        }
    }
}

void visualSetLED(ComandoLED cmd) {
    CRGB cor;
    switch (cmd.cor) {
        case ComandoLED::Cor::LARANJA:  cor = s_paleta[0]; break;
        case ComandoLED::Cor::AZUL:     cor = s_paleta[1]; break;
        case ComandoLED::Cor::AMARELO:  cor = s_paleta[2]; break;
        case ComandoLED::Cor::ROXO:     cor = s_paleta[3]; break;
        case ComandoLED::Cor::OFF:
        default:                        cor = CRGB(0u, 0u, 0u); break;
    }

    switch (cmd.led) {
        case ComandoLED::LED::CENTRAL:
            // --- DERIVADO: visual_config.h#VISUAL_LED_CENTRAL ---
            s_leds[VISUAL_LED_CENTRAL] = cor;
            break;
        case ComandoLED::LED::ESQUERDO:
            // --- DERIVADO: visual_config.h#VISUAL_LED_ESQUERDO ---
            s_leds[VISUAL_LED_ESQUERDO] = cor;
            break;
        case ComandoLED::LED::DIREITO:
            // --- DERIVADO: visual_config.h#VISUAL_LED_DIREITO ---
            s_leds[VISUAL_LED_DIREITO] = cor;
            break;
        case ComandoLED::LED::TODOS:
            for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
                s_leds[i] = cor;
            }
            break;
    }
    FastLED.show();
}

void visualRunCelebracao() {
    s_animacao           = AnimacaoAtiva::CELEBRACAO;
    s_animacao_inicio_ms = millis();
    s_passo_atual        = 0u;
    s_passo_inicio_ms    = s_animacao_inicio_ms;
    _renderizar_celebracao_passo(0u);
}
