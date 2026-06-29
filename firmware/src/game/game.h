#pragma once

// game.h — Interface publica de MOD_JOGO
// Derivado de: spec/game/game.json + modules/game/04_logica_jogo.md
// [VER: 04_logica_jogo.md#estados-jogo]
// [VER: 01_arquitetura.md#interfaces-modulos]

#include <stdint.h>
#include "sensor.h"

// ---------------------------------------------------------------------------
// Tipos publicos
// [VER: 04_logica_jogo.md#aleatoriedade]
// ---------------------------------------------------------------------------

// Derivado de game.json#mapeamento_zona_cor (indices 0-3)
enum class Cor : uint8_t {
    LARANJA = 0,
    AZUL    = 1,
    AMARELO = 2,
    ROXO    = 3
};

// Par de cores para Modo 2
// [VER: 04_logica_jogo.md#logica-modo-2]
struct ParCores {
    Cor cor1;
    Cor cor2;
};

// Resultado de uma interacao ou fim de sessao
// [VER: 04_logica_jogo.md#tratamento-resultados]
enum class ResultadoJogo : uint8_t {
    ESTIMULO,
    ACERTO,
    ERRO,
    FIM_SESSAO
};

// Evento emitido pelo callback de MOD_JOGO
struct EventoJogo {
    ResultadoJogo resultado;
    uint8_t       acertos;
    uint8_t       n_configurado;
    uint32_t      duracao_ms;
};

// Configuracao de sessao recebida de MOD_WIFI
// [VER: 04_logica_jogo.md#timings]
struct ConfigSessao {
    uint8_t  modo;           // GAME_MODO_UM ou GAME_MODO_DOIS
    char     mecanismo;      // 'A' ou 'B'
    uint8_t  n_configurado;
    uint16_t janela_ms;      // relevante apenas para modo == GAME_MODO_DOIS
};

// ---------------------------------------------------------------------------
// Callback
// ---------------------------------------------------------------------------
using GameCallback = void(*)(EventoJogo);

// ---------------------------------------------------------------------------
// API publica
// ---------------------------------------------------------------------------

// Inicializa estado interno. Deve ser chamado uma vez em setup().
void gameInit();

// Registra callback invocado a cada resultado ou fim de sessao.
void gameOnEvento(GameCallback cb);

// Inicia uma sessao com a configuracao recebida.
// [VER: 04_logica_jogo.md#estados-jogo]
void gameIniciarSessao(ConfigSessao cfg);

// Processa um impacto detectado por MOD_SENSOR.
// [VER: 04_logica_jogo.md#logica-modo-1]
// [VER: 04_logica_jogo.md#logica-modo-2]
void gameOnImpacto(EventoImpacto evt);

// Avanca timers internos (intervalo entre interacoes).
// Deve ser chamado a cada iteracao de loop().
void gameLoop();

// Pausa a sessao ativa preservando o estado atual.
// Chamado por MOD_WIFI ao detectar desconexao do browser.
// Derivado de: [VER: 01_arquitetura.md#interface-jogo-wifi]
void gamePausarSessao();

// Retoma a sessao do ponto de pausa.
// Chamado por MOD_WIFI ao detectar reconexao do browser.
// Derivado de: [VER: 01_arquitetura.md#interface-jogo-wifi]
void gameRetomarSessao();

// ---------------------------------------------------------------------------
// API de teste — expoe estado interno para test_main.cpp
// Nao chamar em producao.
// ---------------------------------------------------------------------------

// Retorna a zona fisica correspondente a uma cor.
// [VER: 04_logica_jogo.md#mapeamento-zona-cor]
EventoImpacto::Zona gameZonaParaCor(Cor cor);

// Sorteia a proxima cor pelo Mecanismo A.
// [VER: 04_logica_jogo.md#mecanismo-a]
Cor gameProximaCorA();

// Sorteia a proxima cor pelo Mecanismo B.
// [VER: 04_logica_jogo.md#mecanismo-b]
Cor gameProximaCorB();

// Sorteia o proximo par de cores distintas pelo Mecanismo A.
ParCores gameProximasDuasA();

// Sorteia o proximo par de cores distintas pelo Mecanismo B.
ParCores gameProximasDuasB();

// Retorna a cor do estimulo atual (Modo 1).
Cor gameGetCorAtual();

// Retorna o par de cores do estimulo atual (Modo 2).
ParCores gameGetParAtual();
