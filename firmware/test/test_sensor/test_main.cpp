// test_main.cpp — testes unitarios de MOD_SENSOR
// Derivado de: spec/sensor/sensor.json + _governance/TESTING_STANDARD.md v0.1.0
// [VER: 02_sensor_impacto.md#criterios-aceitacao]
//
// CAs cobertos em native:
//   CA-02-01 (parcial): deteccao de impacto acima do threshold
//   CA-02-02 (parcial): ausencia de falso positivo abaixo do threshold
//   CA-02-05: debounce — uma batida -> exatamente 1 evento
//
// CAs que requerem hardware (nao testados aqui):
//   CA-02-03: isolamento entre zonas — REQUER HARDWARE (medir ADC de zonas adjacentes)
//   CA-02-04: GPIO sobrevive 100 impactos fortes — REQUER HARDWARE (osciloscopio)
//   CA-02-06: latencia deteccao < 10ms — REQUER HARDWARE (osciloscopio + analisador)

#include <unity.h>
#include <string.h>
#include "sensor/sensor.h"
#include "sensor/sensor_config.h"

// ---------------------------------------------------------------------------
// Mock state — unico local de definicao
// [VER: TESTING_STANDARD.md#mock-estado]
// ---------------------------------------------------------------------------
static uint16_t g_mock_adc[40];  // indexado por numero de GPIO (0-39)
static uint32_t g_mock_millis;

// ---------------------------------------------------------------------------
// Mock implementations — declaradas em test/mock/Arduino.h
// [VER: TESTING_STANDARD.md#mock-declare-define]
// ---------------------------------------------------------------------------
uint16_t analogRead(uint8_t pin)       { return (pin < 40u) ? g_mock_adc[pin] : 0u; }
uint32_t millis()                      { return g_mock_millis; }
void     pinMode(uint8_t, uint8_t)     {}
void     analogSetAttenuation(int)     {}

// ---------------------------------------------------------------------------
// Constantes de teste — derivadas de sensor_config.h
// [VER: TESTING_STANDARD.md#nomenclatura-constantes-teste]
// ---------------------------------------------------------------------------

// --- DERIVADO: sensor_config.h#SENSOR_THRESHOLD_ADC ---
// Valor minimo que dispara callback (leitura estritamente acima do limite)
constexpr uint16_t T_SENSOR_ADC_VALIDO = SENSOR_THRESHOLD_ADC + 1u; // DELTA_FRONTEIRA

// --- DERIVADO: sensor_config.h#SENSOR_THRESHOLD_ADC ---
// Valor exatamente no limite — nao dispara (condicao e leitura > THRESHOLD, nao >=)
constexpr uint16_t T_SENSOR_ADC_LIMITE = SENSOR_THRESHOLD_ADC;      // DERIVADO

// --- HARDCODED_TESTE: ADC=0 representa repouso do piezo (ausencia de impacto fisico) ---
constexpr uint16_t T_SENSOR_ADC_REPOUSO = 0u;

// --- DERIVADO: sensor_config.h#SENSOR_DEBOUNCE_MS ---
// Intervalo dentro da janela de debounce — segundo evento deve ser bloqueado
constexpr uint32_t T_SENSOR_DEBOUNCE_DENTRO = SENSOR_DEBOUNCE_MS - 1u; // DELTA_FRONTEIRA

// --- DERIVADO: sensor_config.h#SENSOR_DEBOUNCE_MS ---
// Intervalo apos a janela de debounce — segundo evento deve ser permitido
constexpr uint32_t T_SENSOR_DEBOUNCE_APOS = SENSOR_DEBOUNCE_MS + 1u;   // DELTA_FRONTEIRA

// ---------------------------------------------------------------------------
// Estado de observacao dos testes
// ---------------------------------------------------------------------------
static uint8_t       s_cb_count;
static EventoImpacto s_ultimo_ev;

static void test_callback(EventoImpacto ev) {
    s_cb_count++;
    s_ultimo_ev = ev;
}

// ---------------------------------------------------------------------------
// setUp / tearDown
// [VER: TESTING_STANDARD.md#estrutura-test-main]
// ---------------------------------------------------------------------------
void setUp() {
    memset(g_mock_adc, 0, sizeof(g_mock_adc));
    g_mock_millis = 0;
    s_cb_count    = 0;
    s_ultimo_ev.zona        = EventoImpacto::Zona::LARANJA;
    s_ultimo_ev.timestamp_ms = 0;
    sensorInit();
    sensorOnImpacto(test_callback);
}

void tearDown() {
    sensorOnImpacto(nullptr);
}

// ---------------------------------------------------------------------------
// Testes
// [VER: TESTING_STANDARD.md#rastreabilidade-ca]
// ---------------------------------------------------------------------------

// [CA-02-02] Repouso: ADC=0 em todos os GPIOs nao gera nenhum evento
void test_sensor_adc_zero_nao_dispara() {
    // g_mock_adc ja e zero apos setUp (memset)
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(0u, s_cb_count);
}

// [CA-02-02] Threshold: ADC exatamente no limite nao dispara (condicao e >, nao >=)
void test_sensor_adc_no_limite_nao_dispara() {
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_LIMITE;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(0u, s_cb_count);
}

// [CA-02-01] Threshold: ADC acima do limite dispara exatamente 1 callback
void test_sensor_adc_valido_dispara_callback() {
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_VALIDO;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(1u, s_cb_count);
}

// [CA-02-01] Zona LARANJA e identificada corretamente no evento
void test_sensor_zona_laranja_identificada() {
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_VALIDO;
    sensorLoop();
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(EventoImpacto::Zona::LARANJA),
        static_cast<int>(s_ultimo_ev.zona));
}

// [CA-02-01] Zona AZUL e identificada corretamente no evento
void test_sensor_zona_azul_identificada() {
    g_mock_adc[SENSOR_GPIO_AZUL] = T_SENSOR_ADC_VALIDO;
    sensorLoop();
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(EventoImpacto::Zona::AZUL),
        static_cast<int>(s_ultimo_ev.zona));
}

// [CA-02-01] Zona AMARELO e identificada corretamente no evento
void test_sensor_zona_amarelo_identificada() {
    g_mock_adc[SENSOR_GPIO_AMARELO] = T_SENSOR_ADC_VALIDO;
    sensorLoop();
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(EventoImpacto::Zona::AMARELO),
        static_cast<int>(s_ultimo_ev.zona));
}

// [CA-02-01] Zona ROXO e identificada corretamente no evento
void test_sensor_zona_roxo_identificada() {
    g_mock_adc[SENSOR_GPIO_ROXO] = T_SENSOR_ADC_VALIDO;
    sensorLoop();
    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(EventoImpacto::Zona::ROXO),
        static_cast<int>(s_ultimo_ev.zona));
}

// [CA-02-01] Timestamp do evento corresponde ao valor de millis() no momento da deteccao
void test_sensor_timestamp_correto() {
    g_mock_millis = 12345u;
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_VALIDO;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT32(12345u, s_ultimo_ev.timestamp_ms);
}

// [CA-02-05] Debounce: segundo impacto dentro da janela e bloqueado
void test_sensor_debounce_bloqueia_dentro_da_janela() {
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_VALIDO;
    g_mock_millis = 0u;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(1u, s_cb_count);

    g_mock_millis = T_SENSOR_DEBOUNCE_DENTRO;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(1u, s_cb_count);  // ainda 1 — segundo evento bloqueado
}

// [CA-02-05] Debounce: segundo impacto apos a janela e permitido
void test_sensor_debounce_permite_apos_janela() {
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_VALIDO;
    g_mock_millis = 0u;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(1u, s_cb_count);

    g_mock_millis = T_SENSOR_DEBOUNCE_APOS;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(2u, s_cb_count);  // segundo evento permitido
}

// [CA-02-05] Debounce: timers sao independentes por zona
// Impacto em LARANJA nao bloqueia impacto imediato em AZUL
void test_sensor_debounce_zonas_independentes() {
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_VALIDO;
    g_mock_millis = 0u;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(1u, s_cb_count);

    // AZUL nunca foi acionado — seu timer e independente do LARANJA
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_REPOUSO;
    g_mock_adc[SENSOR_GPIO_AZUL]    = T_SENSOR_ADC_VALIDO;
    g_mock_millis = 1u;  // dentro do debounce de LARANJA, mas AZUL e independente
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(2u, s_cb_count);
}

// [CA-02-01] Multiplas zonas ativas no mesmo sensorLoop() geram eventos independentes
void test_sensor_multiplas_zonas_simultaneas() {
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_VALIDO;
    g_mock_adc[SENSOR_GPIO_AZUL]    = T_SENSOR_ADC_VALIDO;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(2u, s_cb_count);
}

// [CA-02-02] Callback nulo: impacto nao causa crash (verificacao de nullptr)
void test_sensor_callback_nulo_nao_crasha() {
    sensorOnImpacto(nullptr);
    g_mock_adc[SENSOR_GPIO_LARANJA] = T_SENSOR_ADC_VALIDO;
    sensorLoop();
    TEST_ASSERT_EQUAL_UINT8(0u, s_cb_count);  // sem crash, sem chamada de callback
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
int main() {
    UNITY_BEGIN();

    // CA-02-02: ausencia de falso positivo (threshold)
    RUN_TEST(test_sensor_adc_zero_nao_dispara);
    RUN_TEST(test_sensor_adc_no_limite_nao_dispara);
    RUN_TEST(test_sensor_callback_nulo_nao_crasha);

    // CA-02-01: deteccao de impacto
    RUN_TEST(test_sensor_adc_valido_dispara_callback);
    RUN_TEST(test_sensor_zona_laranja_identificada);
    RUN_TEST(test_sensor_zona_azul_identificada);
    RUN_TEST(test_sensor_zona_amarelo_identificada);
    RUN_TEST(test_sensor_zona_roxo_identificada);
    RUN_TEST(test_sensor_timestamp_correto);
    RUN_TEST(test_sensor_multiplas_zonas_simultaneas);

    // CA-02-05: debounce
    RUN_TEST(test_sensor_debounce_bloqueia_dentro_da_janela);
    RUN_TEST(test_sensor_debounce_permite_apos_janela);
    RUN_TEST(test_sensor_debounce_zonas_independentes);

    return UNITY_END();
}
