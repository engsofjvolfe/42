// game.cpp — Implementacao de MOD_JOGO
// Derivado de: spec/game/game.json + modules/game/04_logica_jogo.md
// [VER: 04_logica_jogo.md#estados-jogo]

#include "game.h"
#include "game_config.h"
#include "visual.h"
#include <Arduino.h>
#include <stdlib.h>

// ---------------------------------------------------------------------------
// Estado interno — Mecanismo A
// [VER: 04_logica_jogo.md#mecanismo-a]
// ---------------------------------------------------------------------------
static Cor     s_bloco_A[GAME_MECA_A_BLOCO];
static uint8_t s_idx_A;

// ---------------------------------------------------------------------------
// Estado interno — Mecanismo B
// [VER: 04_logica_jogo.md#mecanismo-b]
// ---------------------------------------------------------------------------
static uint8_t s_peso_B[GAME_N_CORES];

// ---------------------------------------------------------------------------
// Estado interno — Sessao e Maquina de Estados
// [VER: 04_logica_jogo.md#estados-jogo]
// ---------------------------------------------------------------------------
enum class Estado : uint8_t {
    OCIOSO,
    ESTIMULO,
    AVALIANDO,
    INTERVALO,
    FIM_SESSAO,
    PAUSADO
};

static Estado       s_estado;
static Estado       s_estado_anterior; // salvo por gamePausarSessao
static ConfigSessao s_cfg;
static uint8_t      s_acertos;
static uint32_t     s_timestamp_inicio_ms;
static uint32_t     s_timestamp_intervalo_ms;
static GameCallback s_callback;

// Estado do estimulo atual
static Cor      s_cor_atual;
static ParCores s_par_atual;

// Estado interno para Modo 2
// [VER: 04_logica_jogo.md#logica-modo-2]
static bool                s_primeiro_impacto;
static EventoImpacto::Zona s_zona_correta1;
static uint32_t            s_t0_ms;

// ---------------------------------------------------------------------------
// Auxiliares internos
// ---------------------------------------------------------------------------

static uint32_t random_uint32(uint32_t n) {
    // HARDCODED JUSTIFICADO: rand() e a unica fonte de aleatoriedade disponivel
    // em native sem dependencias de plataforma. Em esp32dev, substituir por random().
    // [VER: 04_logica_jogo.md#mecanismo-a]
    return static_cast<uint32_t>(rand()) % n;
}

static void shuffle_bloco_A() {
    // Fisher-Yates — [VER: 04_logica_jogo.md#mecanismo-a]
    for (int i = static_cast<int>(GAME_MECA_A_BLOCO) - 1; i > 0; i--) {
        int j = static_cast<int>(random_uint32(static_cast<uint32_t>(i + 1)));
        Cor tmp      = s_bloco_A[i];
        s_bloco_A[i] = s_bloco_A[j];
        s_bloco_A[j] = tmp;
    }
}

static void reset_meca_A() {
    s_bloco_A[0] = Cor::LARANJA;
    s_bloco_A[1] = Cor::AZUL;
    s_bloco_A[2] = Cor::AMARELO;
    s_bloco_A[3] = Cor::ROXO;
    s_idx_A = GAME_MECA_A_INDICE_INICIAL; // forca shuffle na primeira chamada
}

static void reset_meca_B() {
    for (uint8_t i = 0u; i < GAME_N_CORES; i++) {
        s_peso_B[i] = GAME_MECA_B_PESO_INICIAL;
    }
}

// Converte Cor (game) em ComandoLED::Cor (visual).
// Mapeamento 1:1 derivado de game.json#mapeamento_zona_cor
// (nomes identicos em ambos os enums; OFF nao existe em Cor).
// [VER: 01_arquitetura.md#interface-jogo-led]
static ComandoLED::Cor cor_para_led(Cor cor) {
    switch (cor) {
        case Cor::LARANJA: return ComandoLED::Cor::LARANJA;
        case Cor::AZUL:    return ComandoLED::Cor::AZUL;
        case Cor::AMARELO: return ComandoLED::Cor::AMARELO;
        case Cor::ROXO:    return ComandoLED::Cor::ROXO;
        default:           return ComandoLED::Cor::OFF;
    }
}

static void emitir(ResultadoJogo resultado) {
    if (s_callback != nullptr) {
        EventoJogo ev;
        ev.resultado    = resultado;
        ev.acertos      = s_acertos;
        ev.n_configurado = s_cfg.n_configurado;
        ev.duracao_ms   = millis() - s_timestamp_inicio_ms;
        s_callback(ev);
    }
}

static void iniciar_estimulo() {
    if (s_cfg.modo == GAME_MODO_UM) {
        if (s_cfg.mecanismo == 'A') {
            s_cor_atual = gameProximaCorA();
        } else {
            s_cor_atual = gameProximaCorB();
        }
    } else {
        if (s_cfg.mecanismo == 'A') {
            s_par_atual = gameProximasDuasA();
        } else {
            s_par_atual = gameProximasDuasB();
        }
    }
    s_primeiro_impacto = false;
    s_estado = Estado::ESTIMULO;
    // Acende LED(s) do estimulo — [VER: 04_logica_jogo.md#logica-modo-1]
    // [VER: 04_logica_jogo.md#logica-modo-2]
    // [VER: 01_arquitetura.md#interface-jogo-led]
    if (s_cfg.modo == GAME_MODO_UM) {
        visualSetLED({ ComandoLED::LED::CENTRAL, cor_para_led(s_cor_atual) });
    } else {
        visualSetLED({ ComandoLED::LED::ESQUERDO, cor_para_led(s_par_atual.cor1) });
        visualSetLED({ ComandoLED::LED::DIREITO,  cor_para_led(s_par_atual.cor2) });
    }
    emitir(ResultadoJogo::ESTIMULO);
}

// ---------------------------------------------------------------------------
// API publica
// ---------------------------------------------------------------------------

void gameInit() {
    s_estado    = Estado::OCIOSO;
    s_acertos   = 0u;
    s_callback  = nullptr;
    s_primeiro_impacto = false;
    reset_meca_A();
    reset_meca_B();
}

void gameOnEvento(GameCallback cb) {
    s_callback = cb;
}

void gameIniciarSessao(ConfigSessao cfg) {
    s_cfg                  = cfg;
    s_acertos              = 0u;
    s_timestamp_inicio_ms  = millis();
    s_primeiro_impacto     = false;
    reset_meca_A();
    reset_meca_B();
    iniciar_estimulo();
}

void gameOnImpacto(EventoImpacto evt) {
    if (s_estado != Estado::ESTIMULO && s_estado != Estado::AVALIANDO) {
        return;
    }

    if (s_cfg.modo == GAME_MODO_UM) {
        // [VER: 04_logica_jogo.md#logica-modo-1]
        if (evt.zona == gameZonaParaCor(s_cor_atual)) {
            s_acertos += GAME_SCORE_DELTA_ACERTO;
            s_estado   = Estado::INTERVALO;
            s_timestamp_intervalo_ms = millis();
            // Apaga LEDs imediatamente apos acerto — [VER: spec/game/game.json#acerto.leds_acao]
            visualSetLED({ ComandoLED::LED::TODOS, ComandoLED::Cor::OFF });
            emitir(ResultadoJogo::ACERTO);
        } else {
            s_estado = Estado::AVALIANDO;
            emitir(ResultadoJogo::ERRO);
        }
    } else {
        // [VER: 04_logica_jogo.md#logica-modo-2]
        if (!s_primeiro_impacto) {
            if (evt.zona == gameZonaParaCor(s_par_atual.cor1) ||
                evt.zona == gameZonaParaCor(s_par_atual.cor2)) {
                s_primeiro_impacto = true;
                s_zona_correta1    = evt.zona;
                s_t0_ms            = evt.timestamp_ms;
                s_estado           = Estado::AVALIANDO;
            } else {
                s_estado = Estado::AVALIANDO;
                emitir(ResultadoJogo::ERRO);
                s_primeiro_impacto = false;
            }
        } else {
            EventoImpacto::Zona zona_esperada =
                (s_zona_correta1 == gameZonaParaCor(s_par_atual.cor1))
                    ? gameZonaParaCor(s_par_atual.cor2)
                    : gameZonaParaCor(s_par_atual.cor1);

            if (evt.zona == zona_esperada &&
                (evt.timestamp_ms - s_t0_ms) <= s_cfg.janela_ms) {
                s_acertos += GAME_SCORE_DELTA_ACERTO;
                s_estado   = Estado::INTERVALO;
                s_timestamp_intervalo_ms = millis();
                // Apaga LEDs imediatamente apos acerto — [VER: spec/game/game.json#acerto.leds_acao]
                visualSetLED({ ComandoLED::LED::TODOS, ComandoLED::Cor::OFF });
                emitir(ResultadoJogo::ACERTO);
            } else {
                s_primeiro_impacto = false;
                s_estado           = Estado::ESTIMULO;
                emitir(ResultadoJogo::ERRO);
            }
        }
    }
}

void gameLoop() {
    if (s_estado != Estado::INTERVALO) {
        return;
    }
    if ((millis() - s_timestamp_intervalo_ms) >= GAME_INTERVALO_MS) {
        if (s_acertos >= s_cfg.n_configurado) {
            s_estado = Estado::FIM_SESSAO;
            // Inicia celebracao de fim de sessao — [VER: 04_logica_jogo.md#transicoes]
            visualRunCelebracao();
            emitir(ResultadoJogo::FIM_SESSAO);
        } else {
            iniciar_estimulo();
        }
    }
}

void gamePausarSessao() {
    // Derivado de: [VER: 01_arquitetura.md#interface-jogo-wifi]
    if (s_estado == Estado::OCIOSO || s_estado == Estado::FIM_SESSAO ||
        s_estado == Estado::PAUSADO) {
        return;
    }
    s_estado_anterior = s_estado;
    s_estado          = Estado::PAUSADO;
}

void gameRetomarSessao() {
    // Derivado de: [VER: 01_arquitetura.md#interface-jogo-wifi]
    if (s_estado != Estado::PAUSADO) {
        return;
    }
    s_estado = s_estado_anterior;
    // Reacende LEDs se sessao estava em ESTIMULO ou AVALIANDO ao pausar
    // [VER: spec/game/game.json#criterios_aceitacao CA-04-09]
    if (s_estado == Estado::ESTIMULO || s_estado == Estado::AVALIANDO) {
        if (s_cfg.modo == GAME_MODO_UM) {
            visualSetLED({ ComandoLED::LED::CENTRAL, cor_para_led(s_cor_atual) });
        } else {
            visualSetLED({ ComandoLED::LED::ESQUERDO, cor_para_led(s_par_atual.cor1) });
            visualSetLED({ ComandoLED::LED::DIREITO,  cor_para_led(s_par_atual.cor2) });
        }
    }
}

// ---------------------------------------------------------------------------
// API de teste
// ---------------------------------------------------------------------------

EventoImpacto::Zona gameZonaParaCor(Cor cor) {
    // Mapeamento 1:1 — [VER: 04_logica_jogo.md#mapeamento-zona-cor]
    switch (cor) {
        case Cor::LARANJA:  return EventoImpacto::Zona::LARANJA;
        case Cor::AZUL:     return EventoImpacto::Zona::AZUL;
        case Cor::AMARELO:  return EventoImpacto::Zona::AMARELO;
        case Cor::ROXO:     return EventoImpacto::Zona::ROXO;
        default:            return EventoImpacto::Zona::LARANJA;
    }
}

Cor gameProximaCorA() {
    if (s_idx_A >= GAME_MECA_A_BLOCO) {
        shuffle_bloco_A();
        s_idx_A = 0u;
    }
    return s_bloco_A[s_idx_A++];
}

Cor gameProximaCorB() {
    uint8_t total = 0u;
    for (uint8_t i = 0u; i < GAME_N_CORES; i++) {
        total += s_peso_B[i];
    }
    if (total == 0u) {
        reset_meca_B();
        total = static_cast<uint8_t>(GAME_N_CORES) * GAME_MECA_B_PESO_INICIAL;
    }
    uint8_t r    = static_cast<uint8_t>(random_uint32(total));
    uint8_t acum = 0u;
    for (uint8_t i = 0u; i < GAME_N_CORES; i++) {
        acum += s_peso_B[i];
        if (r < acum) {
            if (s_peso_B[i] > GAME_MECA_B_PESO_MIN) {
                s_peso_B[i]--;
            }
            return static_cast<Cor>(i);
        }
    }
    return Cor::LARANJA; // fallback nunca atingido
}

ParCores gameProximasDuasA() {
    Cor c1 = gameProximaCorA();
    Cor c2 = gameProximaCorA();
    return {c1, c2};
}

ParCores gameProximasDuasB() {
    Cor     c1          = gameProximaCorB();
    uint8_t peso_salvo  = s_peso_B[static_cast<uint8_t>(c1)];
    s_peso_B[static_cast<uint8_t>(c1)] = 0u;
    Cor c2 = gameProximaCorB();
    s_peso_B[static_cast<uint8_t>(c1)] = peso_salvo;
    return {c1, c2};
}

Cor gameGetCorAtual() {
    return s_cor_atual;
}

ParCores gameGetParAtual() {
    return s_par_atual;
}
