#pragma once

// visual_config.h
// Fonte: spec/visual/visual.json (via spec/firmware_constants.json)
// Nenhum valor neste arquivo e inventado.
// Toda alteracao exige atualizacao de firmware_constants.json e regeneracao.

#include <stdint.h>

// --- DERIVADO: spec/visual/visual.json#alimentacao.gpio_dados ---
constexpr uint8_t VISUAL_GPIO_DADOS = 5;

// --- DERIVADO: spec/visual/visual.json#componente.quantidade ---
constexpr uint8_t VISUAL_N_LEDS = 3;

// --- DERIVADO: spec/visual/visual.json#alimentacao.brightness_fixo ---
constexpr uint8_t VISUAL_BRIGHTNESS = 150;

// --- DERIVADO: spec/visual/visual.json#mapeamento_led[0].indice ---
constexpr uint8_t VISUAL_LED_ESQUERDO = 0;

// --- DERIVADO: spec/visual/visual.json#mapeamento_led[1].indice ---
constexpr uint8_t VISUAL_LED_CENTRAL = 1;

// --- DERIVADO: spec/visual/visual.json#mapeamento_led[2].indice ---
constexpr uint8_t VISUAL_LED_DIREITO = 2;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Laranja.r ---
constexpr uint8_t VISUAL_COR_LARANJA_R = 191;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Laranja.g ---
constexpr uint8_t VISUAL_COR_LARANJA_G = 60;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Laranja.b ---
constexpr uint8_t VISUAL_COR_LARANJA_B = 0;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Azul.r ---
constexpr uint8_t VISUAL_COR_AZUL_R = 0;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Azul.g ---
constexpr uint8_t VISUAL_COR_AZUL_G = 0;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Azul.b ---
constexpr uint8_t VISUAL_COR_AZUL_B = 255;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Amarelo.r ---
constexpr uint8_t VISUAL_COR_AMARELO_R = 255;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Amarelo.g ---
constexpr uint8_t VISUAL_COR_AMARELO_G = 180;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Amarelo.b ---
constexpr uint8_t VISUAL_COR_AMARELO_B = 0;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Roxo.r ---
constexpr uint8_t VISUAL_COR_ROXO_R = 30;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Roxo.g ---
constexpr uint8_t VISUAL_COR_ROXO_G = 0;

// --- DERIVADO: spec/visual/visual.json#cores_rgb.Roxo.b ---
constexpr uint8_t VISUAL_COR_ROXO_B = 42;

// --- DERIVADO: spec/visual/visual.json#boot_animation.passos[0].duracao_ms ---
constexpr uint16_t VISUAL_BOOT_PASSO_MS = 150;

// --- DERIVADO: spec/visual/visual.json#boot_animation.passos[4].duracao_ms ---
constexpr uint16_t VISUAL_BOOT_PAUSA_MS = 500;

// --- DERIVADO: spec/visual/visual.json#boot_animation.duracao_total_ms ---
constexpr uint16_t VISUAL_BOOT_DURACAO_MS = 3000;

// --- DERIVADO: spec/visual/visual.json#animacao_celebracao.rotacoes_completas ---
constexpr uint8_t VISUAL_CELEBRACAO_ROTACOES = 5;

// --- DERIVADO: spec/visual/visual.json#animacao_celebracao.duracao_total_ms ---
constexpr uint16_t VISUAL_CELEBRACAO_DURACAO_MS = 3000;

// --- HARDCODED JUSTIFICADO: DM-03 ---
// GRB e enum da biblioteca FastLED. visual.json#componente.color_order registra 'GRB' como string; o simbolo C++ e fornecido pela API da biblioteca e nao pode ser parametrizado sem reflexao.
// [PLATAFORMA] constexpr EOrder VISUAL_COLOR_ORDER = GRB;
// Declarar em visual.cpp apos includes de plataforma.
