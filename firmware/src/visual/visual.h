#pragma once

// visual.h — Interface publica de MOD_LED
// Derivado de: spec/visual/visual.json + system/01_arquitetura.md#interface-jogo-led
// [VER: 01_arquitetura.md#mod-led]

#include <stdint.h>

// ---------------------------------------------------------------------------
// Tipos publicos
// [VER: 01_arquitetura.md#interface-jogo-led]
// ---------------------------------------------------------------------------

struct ComandoLED {
    enum class LED : uint8_t { CENTRAL, ESQUERDO, DIREITO, TODOS };
    enum class Cor  : uint8_t { LARANJA, AZUL, AMARELO, ROXO, OFF };
    LED led;
    Cor cor;
};

// ---------------------------------------------------------------------------
// API publica
// [VER: 01_arquitetura.md#interface-jogo-led]
// ---------------------------------------------------------------------------

// Inicializa FastLED e inicia animacao de boot (non-blocking).
// Deve ser chamado uma vez em setup().
void visualInit();

// Avanca animacoes em andamento (boot, celebracao).
// Deve ser chamado a cada iteracao de loop().
void visualLoop();

// Define a cor de um LED especifico (ou todos) e atualiza hardware.
// Chamado por MOD_JOGO para exibir estimulo ou apagar LEDs.
void visualSetLED(ComandoLED cmd);

// Inicia animacao de celebracao de fim de sessao (non-blocking).
// Chamado por MOD_JOGO ao atingir N interacoes.
// [VER: 01_arquitetura.md#mod-led]
void visualRunCelebracao();
