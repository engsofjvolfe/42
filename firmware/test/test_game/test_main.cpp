// test_main.cpp — testes unitarios de MOD_JOGO
// Derivado de: spec/game/game.json + _governance/TESTING_STANDARD.md v0.1.1
// [VER: 04_logica_jogo.md#criterios-aceitacao]
//
// CAs cobertos em native:
//   CA-04-01: Mecanismo A — distribuicao uniforme (40 interacoes, 10 por cor ± 0)
//   CA-04-02: Mecanismo B — sem repeticao > 3x consecutivas (40 interacoes)
//   CA-04-03: Modo 2 — pares sempre com cores distintas (20 pares)
//   CA-04-04: Acerto Modo 1 — LED apaga (buffer mock), score+1
//   CA-04-05: Erro Modo 1 — LED mantido (buffer mock), score inalterado
//   CA-04-06: Acerto Modo 2 dentro da janela
//   CA-04-07: Erro Modo 2 fora da janela
//   CA-04-08: Fim de sessao — FIM_SESSAO emitido com totais corretos
//   CA-04-10: Intervalo entre interacoes — 2000ms ± 100ms
//   CA-04-11: Encerramento antecipado — FIM_SESSAO com acertos parciais
//
// CAs que requerem hardware (nao testados aqui):
//   CA-04-09: Pausa/retomada — REQUER HARDWARE (desconexao WiFi real)

#include <unity.h>
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>
#include <FastLED.h>
#include "visual/visual.h"
#include "visual/visual_config.h"
#include "game/game.h"
#include "game/game_config.h"

// ---------------------------------------------------------------------------
// Mock state — unico local de definicao
// [VER: TESTING_STANDARD.md#mock-estado]
// ---------------------------------------------------------------------------

// FastLED mock (padrao declare->define) — [VER: TESTING_STANDARD.md#mock-declare-define]
// Necessario porque game.cpp inclui visual.h: LDF compila visual.cpp neste build.
CRGB     g_mock_led_buf[VISUAL_N_LEDS];
bool     g_mock_show_called = false;
CFastLED FastLED;

static uint32_t g_mock_millis;

// ---------------------------------------------------------------------------
// Mock implementations — declaradas em test/mock/Arduino.h
// [VER: TESTING_STANDARD.md#mock-declare-define]
// ---------------------------------------------------------------------------
uint32_t millis() { return g_mock_millis; }
void     pinMode(uint8_t, uint8_t) {}
void     analogSetAttenuation(adc_attenuation_t) {}
uint16_t analogRead(uint8_t) { return 0; }

// random(max) — unico consumidor de aleatoriedade de game.cpp em native.
// [VER: 04_logica_jogo.md#mecanismo-a] [VER: 04_logica_jogo.md#mecanismo-b]
// Em esp32dev esta funcao e fornecida pelo framework Arduino (esp_random());
// em native, delega a rand() da libc — determinismo aceitavel pois os testes
// verificam invariantes do algoritmo (distribuicao, nao-repeticao, distincao
// de pares), nao valores especificos sorteados.
long random(long max) { return static_cast<long>(rand()) % max; }

// ---------------------------------------------------------------------------
// Constantes de teste — derivadas de game_config.h
// [VER: TESTING_STANDARD.md#nomenclatura-constantes-teste]
// ---------------------------------------------------------------------------

// --- DERIVADO: game_config.h#GAME_INTERVALO_MS ---
// Instante apos o intervalo — proximo estimulo deve ser emitido
constexpr uint32_t T_GAME_INTERVALO_APOS = static_cast<uint32_t>(GAME_INTERVALO_MS) + 1u; // DELTA_FRONTEIRA

// --- DERIVADO: game_config.h#GAME_INTERVALO_MS ---
// Tolerancia superior do intervalo (CA-04-10)
constexpr uint32_t T_GAME_INTERVALO_MAX = static_cast<uint32_t>(GAME_INTERVALO_MS) + 100u; // DERIVADO de game.json#timings.intervalo_ms.tolerancia_ca_ms

// --- DERIVADO: game_config.h#GAME_JANELA_MS_PADRAO ---
// Instante dentro da janela de simultaneidade — deve resultar em ACERTO
constexpr uint32_t T_GAME_JANELA_DENTRO = static_cast<uint32_t>(GAME_JANELA_MS_PADRAO);    // DERIVADO (limite inclusivo)

// --- DERIVADO: game_config.h#GAME_JANELA_MS_PADRAO ---
// Instante apos a janela — deve resultar em ERRO
constexpr uint32_t T_GAME_JANELA_APOS   = static_cast<uint32_t>(GAME_JANELA_MS_PADRAO) + 1u; // DELTA_FRONTEIRA

// --- DERIVADO: game_config.h#GAME_N_CORES ---
// Numero de interacoes para distribuicao uniforme (CA-04-01)
// 40 = 10 * GAME_N_CORES, garantindo exatamente 10 por cor
constexpr uint8_t T_GAME_N_INTERACOES_DISTRIB = 10u * GAME_MECA_A_BLOCO; // DERIVADO

// --- DERIVADO: game_config.h#GAME_N_CORES ---
// Contagem esperada por cor em T_GAME_N_INTERACOES_DISTRIB interacoes (CA-04-01)
constexpr uint8_t T_GAME_CONTAGEM_ESPERADA = 10u; // DERIVADO de game.json#criterios_aceitacao[CA-04-01]

// --- HARDCODED_TESTE: 3 = max_consecutivas_mesma_cor (CA-04-02) ---
// Derivado de game.json#criterios_aceitacao[CA-04-02].max_consecutivas_mesma_cor
constexpr uint8_t T_GAME_MAX_CONSECUTIVAS = 3u;

// --- HARDCODED_TESTE: 20 = n_pares_sorteados (CA-04-03) ---
// Derivado de game.json#criterios_aceitacao[CA-04-03].n_pares_sorteados
constexpr uint8_t T_GAME_N_PARES = 20u;

// --- HARDCODED_TESTE: n_configurado = 5 para CA-04-08 ---
// Derivado de game.json#criterios_aceitacao[CA-04-08].n_configurado
constexpr uint8_t T_GAME_N_CONFIG_SESSAO = 5u;

// ---------------------------------------------------------------------------
// Estado de observacao dos testes
// ---------------------------------------------------------------------------
static uint8_t        s_cb_count;
static EventoJogo     s_ultimo_ev;

static void test_callback(EventoJogo ev) {
    s_cb_count++;
    s_ultimo_ev = ev;
}

// ---------------------------------------------------------------------------
// setUp / tearDown
// [VER: TESTING_STANDARD.md#estrutura-test-main]
// ---------------------------------------------------------------------------
void setUp() {
    g_mock_millis      = 0u;
    g_mock_show_called = false;
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        g_mock_led_buf[i] = CRGB(0u, 0u, 0u);
    }
    s_cb_count    = 0u;
    memset(&s_ultimo_ev, 0, sizeof(s_ultimo_ev));
    visualInit(); // inicializa estado de visual.cpp — [VER: 01_arquitetura.md#modularidade-ordem-init]
    gameInit();
    gameOnEvento(test_callback);
}

void tearDown() {
    gameOnEvento(nullptr);
}

// ---------------------------------------------------------------------------
// Testes — Mecanismo A (CA-04-01)
// ---------------------------------------------------------------------------

// [CA-04-01] Mecanismo A: cada cor aparece exatamente 10 vezes em 40 interacoes
void test_game_meca_a_distribuicao_uniforme() {
    uint8_t contagem[GAME_N_CORES] = {0u, 0u, 0u, 0u};
    for (uint8_t i = 0u; i < T_GAME_N_INTERACOES_DISTRIB; i++) {
        Cor c = gameProximaCorA();
        TEST_ASSERT_LESS_THAN(GAME_N_CORES, static_cast<uint8_t>(c));
        contagem[static_cast<uint8_t>(c)]++;
    }
    for (uint8_t i = 0u; i < GAME_N_CORES; i++) {
        TEST_ASSERT_EQUAL_UINT8(T_GAME_CONTAGEM_ESPERADA, contagem[i]);
    }
}

// ---------------------------------------------------------------------------
// Testes — Mecanismo B (CA-04-02)
// ---------------------------------------------------------------------------

// [CA-04-02] Mecanismo B: nenhuma cor repete mais de 3x consecutivas em 40 interacoes
void test_game_meca_b_sem_repeticao_excessiva() {
    Cor ultima    = static_cast<Cor>(GAME_N_CORES); // valor invalido como sentinela
    uint8_t streak = 0u;
    for (uint8_t i = 0u; i < T_GAME_N_INTERACOES_DISTRIB; i++) {
        Cor c = gameProximaCorB();
        if (c == ultima) {
            streak++;
        } else {
            streak = 1u;
            ultima = c;
        }
        TEST_ASSERT_LESS_OR_EQUAL(T_GAME_MAX_CONSECUTIVAS, streak);
    }
}

// ---------------------------------------------------------------------------
// Testes — Modo 2, cores distintas (CA-04-03)
// ---------------------------------------------------------------------------

// [CA-04-03] Modo 2 Mec A: 20 pares sorteados, nenhum par com corE == corD
void test_game_modo2_meca_a_cores_distintas() {
    for (uint8_t i = 0u; i < T_GAME_N_PARES; i++) {
        ParCores par = gameProximasDuasA();
        TEST_ASSERT_NOT_EQUAL(static_cast<uint8_t>(par.cor1), static_cast<uint8_t>(par.cor2));
    }
}

// [CA-04-03] Modo 2 Mec B: 20 pares sorteados, nenhum par com corE == corD
void test_game_modo2_meca_b_cores_distintas() {
    for (uint8_t i = 0u; i < T_GAME_N_PARES; i++) {
        ParCores par = gameProximasDuasB();
        TEST_ASSERT_NOT_EQUAL(static_cast<uint8_t>(par.cor1), static_cast<uint8_t>(par.cor2));
    }
}

// ---------------------------------------------------------------------------
// Testes — Logica Modo 1 (CA-04-04, CA-04-05)
// ---------------------------------------------------------------------------

// [CA-04-04] Acerto Modo 1: zona correta -> score+1, evento ACERTO
void test_game_modo1_acerto_zona_correta() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_UM;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO;
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg);
    s_cb_count = 0u; // resetar: ESTIMULO ja foi emitido por gameIniciarSessao

    Cor cor_estimulo = gameGetCorAtual();
    EventoImpacto imp;
    imp.zona         = gameZonaParaCor(cor_estimulo);
    imp.timestamp_ms = g_mock_millis;
    gameOnImpacto(imp);

    TEST_ASSERT_EQUAL_UINT8(1u, s_cb_count);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(ResultadoJogo::ACERTO),
                            static_cast<uint8_t>(s_ultimo_ev.resultado));
    TEST_ASSERT_EQUAL_UINT8(1u, s_ultimo_ev.acertos);
    // CA-04-04: LED apaga imediatamente apos acerto
    // [VER: spec/game/game.json#acerto.leds_acao]
    static const CRGB LED_APAGADO(0u, 0u, 0u);
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        TEST_ASSERT_EQUAL_MEMORY(&LED_APAGADO, &g_mock_led_buf[i], sizeof(CRGB));
    }
}

// [CA-04-05] Erro Modo 1: zona errada -> score inalterado, evento ERRO
void test_game_modo1_erro_zona_errada() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_UM;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO;
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg);
    s_cb_count = 0u; // resetar: ESTIMULO ja foi emitido por gameIniciarSessao

    Cor cor_estimulo = gameGetCorAtual();
    // Zona errada: usar a proxima zona (diferente da correta)
    uint8_t zona_errada = (static_cast<uint8_t>(gameZonaParaCor(cor_estimulo)) + 1u) % GAME_N_CORES;
    EventoImpacto imp;
    imp.zona         = static_cast<EventoImpacto::Zona>(zona_errada);
    imp.timestamp_ms = g_mock_millis;
    gameOnImpacto(imp);

    TEST_ASSERT_EQUAL_UINT8(1u, s_cb_count);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(ResultadoJogo::ERRO),
                            static_cast<uint8_t>(s_ultimo_ev.resultado));
    TEST_ASSERT_EQUAL_UINT8(0u, s_ultimo_ev.acertos);
    // CA-04-05: LED central mantido apos erro (estímulo nao apaga)
    // [VER: spec/game/game.json#erro.leds_acao]
    bool led_central_aceso = (g_mock_led_buf[VISUAL_LED_CENTRAL].r != 0u ||
                              g_mock_led_buf[VISUAL_LED_CENTRAL].g != 0u ||
                              g_mock_led_buf[VISUAL_LED_CENTRAL].b != 0u);
    TEST_ASSERT_TRUE(led_central_aceso);
}

// ---------------------------------------------------------------------------
// Testes — Logica Modo 2, janela (CA-04-06, CA-04-07)
// ---------------------------------------------------------------------------

// [CA-04-06] Acerto Modo 2: dois impactos corretos dentro da janela
void test_game_modo2_acerto_dentro_da_janela() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_DOIS;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO;
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg);

    ParCores par = gameGetParAtual();
    EventoImpacto imp1;
    imp1.zona         = gameZonaParaCor(par.cor1);
    imp1.timestamp_ms = g_mock_millis;
    gameOnImpacto(imp1);

    // Segundo impacto dentro da janela
    g_mock_millis = T_GAME_JANELA_DENTRO;
    EventoImpacto imp2;
    imp2.zona         = gameZonaParaCor(par.cor2);
    imp2.timestamp_ms = g_mock_millis;
    gameOnImpacto(imp2);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(ResultadoJogo::ACERTO),
                            static_cast<uint8_t>(s_ultimo_ev.resultado));
    TEST_ASSERT_EQUAL_UINT8(1u, s_ultimo_ev.acertos);
}

// [CA-04-07] Erro Modo 2: segundo impacto correto apos a janela
void test_game_modo2_erro_fora_da_janela() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_DOIS;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO;
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg);

    ParCores par = gameGetParAtual();
    EventoImpacto imp1;
    imp1.zona         = gameZonaParaCor(par.cor1);
    imp1.timestamp_ms = g_mock_millis;
    gameOnImpacto(imp1);

    // Segundo impacto apos a janela
    g_mock_millis = T_GAME_JANELA_APOS;
    EventoImpacto imp2;
    imp2.zona         = gameZonaParaCor(par.cor2);
    imp2.timestamp_ms = g_mock_millis;
    gameOnImpacto(imp2);

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(ResultadoJogo::ERRO),
                            static_cast<uint8_t>(s_ultimo_ev.resultado));
    TEST_ASSERT_EQUAL_UINT8(0u, s_ultimo_ev.acertos);
}

// ---------------------------------------------------------------------------
// Testes — Fim de sessao (CA-04-08)
// ---------------------------------------------------------------------------

// [CA-04-08] N=5 acertos: FIM_SESSAO emitido com acertos e n_configurado corretos
void test_game_fim_sessao_emitido_com_totais_corretos() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_UM;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO;
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg);

    for (uint8_t i = 0u; i < T_GAME_N_CONFIG_SESSAO; i++) {
        Cor cor = gameGetCorAtual();
        EventoImpacto imp;
        imp.zona         = gameZonaParaCor(cor);
        imp.timestamp_ms = g_mock_millis;
        gameOnImpacto(imp);
        // Avanca o intervalo para o proximo estimulo
        g_mock_millis += T_GAME_INTERVALO_APOS;
        gameLoop();
    }

    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(ResultadoJogo::FIM_SESSAO),
                            static_cast<uint8_t>(s_ultimo_ev.resultado));
    TEST_ASSERT_EQUAL_UINT8(T_GAME_N_CONFIG_SESSAO, s_ultimo_ev.acertos);
    TEST_ASSERT_EQUAL_UINT8(T_GAME_N_CONFIG_SESSAO, s_ultimo_ev.n_configurado);
}

// ---------------------------------------------------------------------------
// Testes — Intervalo entre interacoes (CA-04-10)
// ---------------------------------------------------------------------------

// [CA-04-10] Intervalo: proximo estimulo emitido apos GAME_INTERVALO_MS
void test_game_intervalo_aciona_proximo_estimulo() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_UM;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO;
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg);

    // Primeiro acerto
    Cor cor = gameGetCorAtual();
    EventoImpacto imp;
    imp.zona         = gameZonaParaCor(cor);
    imp.timestamp_ms = g_mock_millis;
    gameOnImpacto(imp);
    s_cb_count = 0u; // resetar contagem apos o acerto

    // Antes do intervalo: nao deve emitir novo estimulo
    g_mock_millis = static_cast<uint32_t>(GAME_INTERVALO_MS) - 1u; // DELTA_FRONTEIRA
    gameLoop();
    TEST_ASSERT_EQUAL_UINT8(0u, s_cb_count);

    // Apos o intervalo: deve emitir ESTIMULO
    g_mock_millis = T_GAME_INTERVALO_APOS;
    gameLoop();
    TEST_ASSERT_EQUAL_UINT8(1u, s_cb_count);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(ResultadoJogo::ESTIMULO),
                            static_cast<uint8_t>(s_ultimo_ev.resultado));
}

// [CA-04-10] Intervalo dentro da tolerancia: nao excede GAME_INTERVALO_MS + 100ms
void test_game_intervalo_dentro_da_tolerancia() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_UM;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO;
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg);

    Cor cor = gameGetCorAtual();
    EventoImpacto imp;
    imp.zona         = gameZonaParaCor(cor);
    imp.timestamp_ms = g_mock_millis;
    gameOnImpacto(imp);
    s_cb_count = 0u;

    // gameLoop chamado continuamente ate emitir o proximo estimulo
    uint32_t t_estimulo = 0u;
    for (uint32_t t = 1u; t <= T_GAME_INTERVALO_MAX + 1u; t++) {
        g_mock_millis = t;
        gameLoop();
        if (s_cb_count > 0u &&
            s_ultimo_ev.resultado == ResultadoJogo::ESTIMULO) {
            t_estimulo = t;
            break;
        }
    }
    TEST_ASSERT_GREATER_OR_EQUAL(static_cast<uint32_t>(GAME_INTERVALO_MS), t_estimulo);
    TEST_ASSERT_LESS_OR_EQUAL(T_GAME_INTERVALO_MAX, t_estimulo);
}

// ---------------------------------------------------------------------------
// Testes — Encerramento antecipado (CA-04-11)
// [VER: 04_logica_jogo.md#criterios-aceitacao]
// ---------------------------------------------------------------------------

// gameEncerrarSessao() no meio da sessao: emite FIM_SESSAO com acertos parciais
void test_game_encerrar_sessao_emite_fim_sessao_parcial() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_UM;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO; // 5
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg);

    // Um acerto antes de encerrar
    Cor cor = gameGetCorAtual();
    EventoImpacto imp;
    imp.zona         = gameZonaParaCor(cor);
    imp.timestamp_ms = g_mock_millis;
    gameOnImpacto(imp);
    g_mock_millis += T_GAME_INTERVALO_APOS;
    gameLoop(); // avanca INTERVALO -> ESTIMULO (2o estimulo)

    s_cb_count = 0u; // resetar: interessa so o evento do encerramento
    gameEncerrarSessao();

    TEST_ASSERT_EQUAL_UINT8(1u, s_cb_count);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(ResultadoJogo::FIM_SESSAO),
                            static_cast<uint8_t>(s_ultimo_ev.resultado));
    TEST_ASSERT_EQUAL_UINT8(1u, s_ultimo_ev.acertos); // parcial, nao T_GAME_N_CONFIG_SESSAO
    TEST_ASSERT_EQUAL_UINT8(T_GAME_N_CONFIG_SESSAO, s_ultimo_ev.n_configurado);
}

// gameEncerrarSessao() apaga os LEDs do estimulo em curso
void test_game_encerrar_sessao_apaga_leds() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_UM;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO;
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg); // ESTIMULO: LED central aceso

    gameEncerrarSessao();

    static const CRGB LED_APAGADO(0u, 0u, 0u);
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        TEST_ASSERT_EQUAL_MEMORY(&LED_APAGADO, &g_mock_led_buf[i], sizeof(CRGB));
    }
}

// gameEncerrarSessao() sem sessao iniciada (OCIOSO): no-op, nenhum evento
void test_game_encerrar_sessao_ociosa_no_op() {
    gameEncerrarSessao();
    TEST_ASSERT_EQUAL_UINT8(0u, s_cb_count);
}

// gameEncerrarSessao() apos FIM_SESSAO natural: no-op, sem segundo evento
void test_game_encerrar_sessao_apos_fim_sessao_no_op() {
    ConfigSessao cfg;
    cfg.modo      = GAME_MODO_UM;
    cfg.mecanismo = 'A';
    cfg.n_configurado = T_GAME_N_CONFIG_SESSAO;
    cfg.janela_ms = GAME_JANELA_MS_PADRAO;
    gameIniciarSessao(cfg);

    for (uint8_t i = 0u; i < T_GAME_N_CONFIG_SESSAO; i++) {
        Cor cor = gameGetCorAtual();
        EventoImpacto imp;
        imp.zona         = gameZonaParaCor(cor);
        imp.timestamp_ms = g_mock_millis;
        gameOnImpacto(imp);
        g_mock_millis += T_GAME_INTERVALO_APOS;
        gameLoop();
    }
    s_cb_count = 0u; // resetar: interessa so o que vem depois do FIM_SESSAO natural

    gameEncerrarSessao();

    TEST_ASSERT_EQUAL_UINT8(0u, s_cb_count);
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
int main() {
    UNITY_BEGIN();

    // CA-04-01: distribuicao uniforme Mecanismo A
    RUN_TEST(test_game_meca_a_distribuicao_uniforme);

    // CA-04-02: variacao Mecanismo B
    RUN_TEST(test_game_meca_b_sem_repeticao_excessiva);

    // CA-04-03: cores distintas Modo 2
    RUN_TEST(test_game_modo2_meca_a_cores_distintas);
    RUN_TEST(test_game_modo2_meca_b_cores_distintas);

    // CA-04-04 e CA-04-05: acerto/erro Modo 1
    RUN_TEST(test_game_modo1_acerto_zona_correta);
    RUN_TEST(test_game_modo1_erro_zona_errada);

    // CA-04-06 e CA-04-07: janela Modo 2
    RUN_TEST(test_game_modo2_acerto_dentro_da_janela);
    RUN_TEST(test_game_modo2_erro_fora_da_janela);

    // CA-04-08: fim de sessao
    RUN_TEST(test_game_fim_sessao_emitido_com_totais_corretos);

    // CA-04-10: intervalo entre interacoes
    RUN_TEST(test_game_intervalo_aciona_proximo_estimulo);
    RUN_TEST(test_game_intervalo_dentro_da_tolerancia);

    // CA-04-11: encerramento antecipado
    RUN_TEST(test_game_encerrar_sessao_emite_fim_sessao_parcial);
    RUN_TEST(test_game_encerrar_sessao_apaga_leds);
    RUN_TEST(test_game_encerrar_sessao_ociosa_no_op);
    RUN_TEST(test_game_encerrar_sessao_apos_fim_sessao_no_op);

    return UNITY_END();
}
