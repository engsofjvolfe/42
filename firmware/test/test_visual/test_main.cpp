// test_main.cpp — testes unitarios de MOD_LED
// Derivado de: spec/visual/visual.json + _governance/TESTING_STANDARD.md v0.1.1
// [VER: 03_saida_visual.md#criterios-aceitacao]
//
// CAs cobertos em native:
//   CA-03-01 (parcial): boot animation percorre passos na sequencia correta
//   CA-03-04: LED correto acende por modo (Modo 1: LED[1]; Modo 2: LED[0] e LED[2])
//   CA-03-06 (parcial): celebracao executa rotacoes e apaga ao final
//
// CAs que requerem hardware (nao testados aqui):
//   CA-03-02: cor Laranja distinta de Amarelo — REQUER HARDWARE (inspecao visual)
//   CA-03-03: cor Azul distinta de Roxo em luz ambiente — REQUER HARDWARE (inspecao visual)
//   CA-03-05: latencia LED < 10ms — REQUER HARDWARE (osciloscopio)
//   CA-03-07: temperatura leds apos 60min — REQUER HARDWARE (termometro)

#include <unity.h>
#include <string.h>
#include <FastLED.h>
#include "visual/visual.h"
#include "visual/visual_config.h"

// ---------------------------------------------------------------------------
// Mock state — unico local de definicao
// [VER: TESTING_STANDARD.md#mock-estado]
// ---------------------------------------------------------------------------
CRGB g_mock_led_buf[VISUAL_N_LEDS];
bool g_mock_show_called = false;
CFastLED FastLED;

static uint32_t g_mock_millis;

// ---------------------------------------------------------------------------
// Mock implementations — declaradas em test/mock/Arduino.h e FastLED.h
// [VER: TESTING_STANDARD.md#mock-declare-define]
// ---------------------------------------------------------------------------
uint32_t millis() { return g_mock_millis; }

// ---------------------------------------------------------------------------
// Constantes de teste — derivadas de visual_config.h
// [VER: TESTING_STANDARD.md#nomenclatura-constantes-teste]
// ---------------------------------------------------------------------------

// --- DERIVADO: visual_config.h#VISUAL_BOOT_PASSO_MS ---
// Instante exatamente no final do passo de boot (condicao e >=)
constexpr uint32_t T_VISUAL_BOOT_PASSO_EXATO = static_cast<uint32_t>(VISUAL_BOOT_PASSO_MS);

// --- DERIVADO: visual_config.h#VISUAL_BOOT_PASSO_MS ---
// Instante apos o fim do passo — avanca para o proximo
constexpr uint32_t T_VISUAL_BOOT_PASSO_APOS = static_cast<uint32_t>(VISUAL_BOOT_PASSO_MS) + 1u; // DELTA_FRONTEIRA

// --- DERIVADO: visual_config.h#VISUAL_BOOT_PAUSA_MS ---
// Instante apos o fim da pausa OFF — avanca para o proximo ciclo
constexpr uint32_t T_VISUAL_BOOT_PAUSA_APOS = static_cast<uint32_t>(VISUAL_BOOT_PAUSA_MS) + 1u; // DELTA_FRONTEIRA

// --- DERIVADO: visual_config.h#VISUAL_BOOT_DURACAO_MS ---
// Instante apos duracao total de boot — animacao deve terminar
constexpr uint32_t T_VISUAL_BOOT_FIM_APOS = static_cast<uint32_t>(VISUAL_BOOT_DURACAO_MS) + 1u; // DELTA_FRONTEIRA

// --- HARDCODED_TESTE: 4 = N_CORES_PALETA (Laranja, Azul, Amarelo, Roxo) ---
// Derivado de visual.json#cores_rgb: 4 entradas excluindo OFF.
// Nao existe VISUAL_N_PALETA em visual_config.h — a paleta nao e parametro de
// dominio independente; seu tamanho e inferido por sizeof(s_paleta) em visual.cpp.
constexpr uint8_t T_VISUAL_N_PALETA = 4u;

// --- DERIVADO: visual_config.h#VISUAL_CELEBRACAO_DURACAO_MS e VISUAL_CELEBRACAO_ROTACOES ---
// Duracao de cada passo de cor na animacao de celebracao.
// Formula: duracao_total / (rotacoes * n_paleta)
constexpr uint32_t T_CELEBRACAO_PASSO_MS =
    static_cast<uint32_t>(VISUAL_CELEBRACAO_DURACAO_MS) /
    (static_cast<uint32_t>(VISUAL_CELEBRACAO_ROTACOES) * static_cast<uint32_t>(T_VISUAL_N_PALETA));

// Cores derivadas de visual_config.h para comparacao nas asservacoes
// --- DERIVADO: visual_config.h#VISUAL_COR_LARANJA_* ---
constexpr CRGB T_COR_LARANJA = CRGB(VISUAL_COR_LARANJA_R, VISUAL_COR_LARANJA_G, VISUAL_COR_LARANJA_B);
// --- DERIVADO: visual_config.h#VISUAL_COR_AZUL_* ---
constexpr CRGB T_COR_AZUL    = CRGB(VISUAL_COR_AZUL_R,    VISUAL_COR_AZUL_G,    VISUAL_COR_AZUL_B);
// --- DERIVADO: visual_config.h#VISUAL_COR_AMARELO_* ---
constexpr CRGB T_COR_AMARELO = CRGB(VISUAL_COR_AMARELO_R, VISUAL_COR_AMARELO_G, VISUAL_COR_AMARELO_B);
// --- DERIVADO: visual_config.h#VISUAL_COR_ROXO_* ---
constexpr CRGB T_COR_ROXO    = CRGB(VISUAL_COR_ROXO_R,    VISUAL_COR_ROXO_G,    VISUAL_COR_ROXO_B);
// --- HARDCODED_TESTE: RGB(0,0,0) representa LED apagado (OFF) ---
constexpr CRGB T_COR_OFF     = CRGB(0u, 0u, 0u);

// ---------------------------------------------------------------------------
// setUp / tearDown
// [VER: TESTING_STANDARD.md#estrutura-test-main]
// ---------------------------------------------------------------------------
void setUp() {
    g_mock_millis      = 0u;
    g_mock_show_called = false;
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        g_mock_led_buf[i] = T_COR_OFF;
    }
    visualInit();
}

void tearDown() {}

// ---------------------------------------------------------------------------
// Testes — Boot Animation (CA-03-01)
// [VER: TESTING_STANDARD.md#rastreabilidade-ca]
// ---------------------------------------------------------------------------

// [CA-03-01] Passo 0: LED[0] = Laranja apos visualInit()
void test_visual_boot_passo0_led0_laranja() {
    // visualInit() renderiza passo 0 imediatamente
    TEST_ASSERT_EQUAL_UINT8(T_COR_LARANJA.r, g_mock_led_buf[VISUAL_LED_ESQUERDO].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_LARANJA.g, g_mock_led_buf[VISUAL_LED_ESQUERDO].g);
    TEST_ASSERT_EQUAL_UINT8(T_COR_LARANJA.b, g_mock_led_buf[VISUAL_LED_ESQUERDO].b);
}

// [CA-03-01] Passo 1: LED[1] = Azul apos VISUAL_BOOT_PASSO_MS
void test_visual_boot_passo1_led1_azul() {
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS;
    visualLoop();
    TEST_ASSERT_EQUAL_UINT8(T_COR_AZUL.r, g_mock_led_buf[VISUAL_LED_CENTRAL].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_AZUL.g, g_mock_led_buf[VISUAL_LED_CENTRAL].g);
    TEST_ASSERT_EQUAL_UINT8(T_COR_AZUL.b, g_mock_led_buf[VISUAL_LED_CENTRAL].b);
}

// [CA-03-01] Passo 2: LED[2] = Amarelo apos 2 x VISUAL_BOOT_PASSO_MS
void test_visual_boot_passo2_led2_amarelo() {
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS;
    visualLoop();
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS * 2u;
    visualLoop();
    TEST_ASSERT_EQUAL_UINT8(T_COR_AMARELO.r, g_mock_led_buf[VISUAL_LED_DIREITO].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_AMARELO.g, g_mock_led_buf[VISUAL_LED_DIREITO].g);
    TEST_ASSERT_EQUAL_UINT8(T_COR_AMARELO.b, g_mock_led_buf[VISUAL_LED_DIREITO].b);
}

// [CA-03-01] Passo 3: LED[0] = Roxo apos 3 x VISUAL_BOOT_PASSO_MS
void test_visual_boot_passo3_led0_roxo() {
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS;       visualLoop();
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS * 2u;  visualLoop();
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS * 3u;  visualLoop();
    TEST_ASSERT_EQUAL_UINT8(T_COR_ROXO.r, g_mock_led_buf[VISUAL_LED_ESQUERDO].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_ROXO.g, g_mock_led_buf[VISUAL_LED_ESQUERDO].g);
    TEST_ASSERT_EQUAL_UINT8(T_COR_ROXO.b, g_mock_led_buf[VISUAL_LED_ESQUERDO].b);
}

// [CA-03-01] Passo 4 (pausa): todos OFF apos 4 x VISUAL_BOOT_PASSO_MS
void test_visual_boot_passo4_todos_off() {
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS;       visualLoop();
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS * 2u;  visualLoop();
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS * 3u;  visualLoop();
    g_mock_millis = T_VISUAL_BOOT_PASSO_APOS * 4u;  visualLoop();
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[i].r);
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.g, g_mock_led_buf[i].g);
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.b, g_mock_led_buf[i].b);
    }
}

// [CA-03-01] Fim de boot: todos OFF apos VISUAL_BOOT_DURACAO_MS
void test_visual_boot_fim_todos_off() {
    g_mock_millis = T_VISUAL_BOOT_FIM_APOS;
    visualLoop();
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[i].r);
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.g, g_mock_led_buf[i].g);
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.b, g_mock_led_buf[i].b);
    }
}

// [CA-03-01] show() foi chamado durante init (LED renderizado)
void test_visual_init_chama_show() {
    TEST_ASSERT_TRUE(g_mock_show_called);
}

// ---------------------------------------------------------------------------
// Testes — visualSetLED (CA-03-04)
// ---------------------------------------------------------------------------

// [CA-03-04] Modo 1: visualSetLED com LED::CENTRAL acende apenas LED[1]
void test_visual_set_led_central_acende_led1() {
    // Encerrar boot primeiro
    g_mock_millis = T_VISUAL_BOOT_FIM_APOS;
    visualLoop();

    ComandoLED cmd;
    cmd.led = ComandoLED::LED::CENTRAL;
    cmd.cor = ComandoLED::Cor::LARANJA;
    visualSetLED(cmd);

    TEST_ASSERT_EQUAL_UINT8(T_COR_LARANJA.r, g_mock_led_buf[VISUAL_LED_CENTRAL].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_LARANJA.g, g_mock_led_buf[VISUAL_LED_CENTRAL].g);
    TEST_ASSERT_EQUAL_UINT8(T_COR_LARANJA.b, g_mock_led_buf[VISUAL_LED_CENTRAL].b);
    // LEDs 0 e 2 permanecem OFF (estado apos boot)
    TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[VISUAL_LED_ESQUERDO].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[VISUAL_LED_DIREITO].r);
}

// [CA-03-04] Modo 2: visualSetLED com LED::ESQUERDO acende apenas LED[0]
void test_visual_set_led_esquerdo_acende_led0() {
    g_mock_millis = T_VISUAL_BOOT_FIM_APOS;
    visualLoop();

    ComandoLED cmd;
    cmd.led = ComandoLED::LED::ESQUERDO;
    cmd.cor = ComandoLED::Cor::AZUL;
    visualSetLED(cmd);

    TEST_ASSERT_EQUAL_UINT8(T_COR_AZUL.r, g_mock_led_buf[VISUAL_LED_ESQUERDO].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_AZUL.g, g_mock_led_buf[VISUAL_LED_ESQUERDO].g);
    TEST_ASSERT_EQUAL_UINT8(T_COR_AZUL.b, g_mock_led_buf[VISUAL_LED_ESQUERDO].b);
    TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[VISUAL_LED_CENTRAL].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[VISUAL_LED_DIREITO].r);
}

// [CA-03-04] Modo 2: visualSetLED com LED::DIREITO acende apenas LED[2]
void test_visual_set_led_direito_acende_led2() {
    g_mock_millis = T_VISUAL_BOOT_FIM_APOS;
    visualLoop();

    ComandoLED cmd;
    cmd.led = ComandoLED::LED::DIREITO;
    cmd.cor = ComandoLED::Cor::ROXO;
    visualSetLED(cmd);

    TEST_ASSERT_EQUAL_UINT8(T_COR_ROXO.r, g_mock_led_buf[VISUAL_LED_DIREITO].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_ROXO.g, g_mock_led_buf[VISUAL_LED_DIREITO].g);
    TEST_ASSERT_EQUAL_UINT8(T_COR_ROXO.b, g_mock_led_buf[VISUAL_LED_DIREITO].b);
    TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[VISUAL_LED_CENTRAL].r);
    TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[VISUAL_LED_ESQUERDO].r);
}

// [CA-03-04] LED::TODOS + Cor::OFF apaga todos os LEDs
void test_visual_set_led_todos_off() {
    g_mock_millis = T_VISUAL_BOOT_FIM_APOS;
    visualLoop();

    // Acender algo primeiro
    ComandoLED cmd_on;
    cmd_on.led = ComandoLED::LED::CENTRAL;
    cmd_on.cor = ComandoLED::Cor::AMARELO;
    visualSetLED(cmd_on);

    ComandoLED cmd_off;
    cmd_off.led = ComandoLED::LED::TODOS;
    cmd_off.cor = ComandoLED::Cor::OFF;
    visualSetLED(cmd_off);

    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[i].r);
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.g, g_mock_led_buf[i].g);
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.b, g_mock_led_buf[i].b);
    }
}

// ---------------------------------------------------------------------------
// Testes — Celebracao (CA-03-06)
// ---------------------------------------------------------------------------

// [CA-03-06] visualRunCelebracao: todos os LEDs acesos (nao OFF) no primeiro passo
void test_visual_celebracao_leds_acesos_no_inicio() {
    g_mock_millis = T_VISUAL_BOOT_FIM_APOS;
    visualLoop();

    visualRunCelebracao();

    // Todos os LEDs devem estar com a mesma cor (nao OFF)
    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        const bool aceso = (g_mock_led_buf[i].r != 0u ||
                            g_mock_led_buf[i].g != 0u ||
                            g_mock_led_buf[i].b != 0u);
        TEST_ASSERT_TRUE(aceso);
    }
}

// [CA-03-06] visualRunCelebracao: todos LEDs OFF apos VISUAL_CELEBRACAO_DURACAO_MS
void test_visual_celebracao_estado_final_off() {
    g_mock_millis = T_VISUAL_BOOT_FIM_APOS;
    visualLoop();

    visualRunCelebracao();

    g_mock_millis = T_VISUAL_BOOT_FIM_APOS + static_cast<uint32_t>(VISUAL_CELEBRACAO_DURACAO_MS) + 1u;
    visualLoop();

    for (uint8_t i = 0u; i < VISUAL_N_LEDS; i++) {
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.r, g_mock_led_buf[i].r);
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.g, g_mock_led_buf[i].g);
        TEST_ASSERT_EQUAL_UINT8(T_COR_OFF.b, g_mock_led_buf[i].b);
    }
}

// [CA-03-06] visualRunCelebracao: cor muda a cada passo (nao fica estatica)
void test_visual_celebracao_cor_muda_entre_passos() {
    g_mock_millis = T_VISUAL_BOOT_FIM_APOS;
    visualLoop();

    visualRunCelebracao();
    const CRGB cor_passo0 = g_mock_led_buf[VISUAL_LED_CENTRAL];

    // Avanca para o proximo passo da celebracao usando T_CELEBRACAO_PASSO_MS (escopo de arquivo)
    g_mock_millis = T_VISUAL_BOOT_FIM_APOS + T_CELEBRACAO_PASSO_MS + 1u;
    visualLoop();

    const CRGB cor_passo1 = g_mock_led_buf[VISUAL_LED_CENTRAL];
    TEST_ASSERT_TRUE(cor_passo0 != cor_passo1);
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
int main() {
    UNITY_BEGIN();

    // CA-03-01: boot animation
    RUN_TEST(test_visual_init_chama_show);
    RUN_TEST(test_visual_boot_passo0_led0_laranja);
    RUN_TEST(test_visual_boot_passo1_led1_azul);
    RUN_TEST(test_visual_boot_passo2_led2_amarelo);
    RUN_TEST(test_visual_boot_passo3_led0_roxo);
    RUN_TEST(test_visual_boot_passo4_todos_off);
    RUN_TEST(test_visual_boot_fim_todos_off);

    // CA-03-04: LED correto por modo
    RUN_TEST(test_visual_set_led_central_acende_led1);
    RUN_TEST(test_visual_set_led_esquerdo_acende_led0);
    RUN_TEST(test_visual_set_led_direito_acende_led2);
    RUN_TEST(test_visual_set_led_todos_off);

    // CA-03-06: celebracao fim de sessao
    RUN_TEST(test_visual_celebracao_leds_acesos_no_inicio);
    RUN_TEST(test_visual_celebracao_estado_final_off);
    RUN_TEST(test_visual_celebracao_cor_muda_entre_passos);

    return UNITY_END();
}
