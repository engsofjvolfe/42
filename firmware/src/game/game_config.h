#pragma once

// game_config.h
// Fonte: spec/game/game.json (via spec/firmware_constants.json)
// Nenhum valor neste arquivo e inventado.
// Toda alteracao exige atualizacao de firmware_constants.json e regeneracao.

#include <stdint.h>

// --- DERIVADO: spec/game/game.json#modos[0].id ---
constexpr uint8_t GAME_MODO_UM = 1;

// --- DERIVADO: spec/game/game.json#modos[1].id ---
constexpr uint8_t GAME_MODO_DOIS = 2;

// --- DERIVADO: spec/game/game.json#mecanismos_aleatoriedade[0].n_cores ---
constexpr uint8_t GAME_N_CORES = 4;

// --- DERIVADO: spec/game/game.json#timings.intervalo_ms.valor_padrao ---
constexpr uint16_t GAME_INTERVALO_MS = 2000;  // [CALIBRAR] — confirmar apos prototipagem

// --- DERIVADO: spec/game/game.json#timings.janela_ms.valor_padrao ---
constexpr uint16_t GAME_JANELA_MS_PADRAO = 800;  // [CALIBRAR] — confirmar apos prototipagem

// --- DERIVADO: spec/game/game.json#mecanismos_aleatoriedade[0].tamanho_bloco ---
constexpr uint8_t GAME_MECA_A_BLOCO = 4;

// --- DERIVADO: spec/game/game.json#mecanismos_aleatoriedade[0].indice_inicial ---
constexpr uint8_t GAME_MECA_A_INDICE_INICIAL = 4;

// --- DERIVADO: spec/game/game.json#mecanismos_aleatoriedade[1].peso_inicial_por_cor ---
constexpr uint8_t GAME_MECA_B_PESO_INICIAL = 4;

// --- DERIVADO: spec/game/game.json#mecanismos_aleatoriedade[1].peso_minimo ---
constexpr uint8_t GAME_MECA_B_PESO_MIN = 0;

// --- DERIVADO: spec/game/game.json#acerto.score_delta ---
constexpr uint8_t GAME_SCORE_DELTA_ACERTO = 1;

// --- DERIVADO: spec/game/game.json#erro.score_delta ---
constexpr uint8_t GAME_SCORE_DELTA_ERRO = 0;
