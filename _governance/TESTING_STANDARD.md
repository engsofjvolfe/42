---
documento:    TESTING_STANDARD.md
versão:       0.1.3
status:       APROVADO
data:         2026-06-28
depende_de:
  - _PADRAO.md v0.1.0           [BLOQUEADOR]
  - CODING_STANDARD.md v0.2.1   [BLOQUEADOR]
  - 01_arquitetura.md v0.2.1    [BLOQUEADOR]
impacta: firmware/test/ (test_main.cpp de cada modulo — nao sao .md)
---

# TESTING_STANDARD.md — Padrão de Testes de Firmware

**Escopo:** Todo arquivo em `firmware/test/`, sem exceção.

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | TESTING_STANDARD.md |
| Versão | 0.1.2 |
| Status | APROVADO |
| Escopo | Todos os arquivos de teste em `firmware/test/` |
| Pais | _PADRAO.md v0.1.0, CODING_STANDARD.md v0.2.0, 01_arquitetura.md v0.2.0 |

---

## 2. Objetivo <a id="objetivo"></a>

Definir de forma determinística o padrão que governa todos os testes de firmware deste projeto. Este documento é o complemento de [VER: CODING_STANDARD.md#identificacao] para o lado de testes.

**Princípio central:** toda constante que aparece em um teste é derivada de `_config.h` ou é um delta de fronteira explicitamente justificado. Nenhum valor de domínio aparece literal em testes.

---

## 3. Módulos Testáveis em Native <a id="modulos-nativo"></a>

Derivado de [VER: 01_arquitetura.md#modulos-firmware]. MOD_WIFI não é testável em native
porque depende de ESPAsyncWebServer + stack WiFi — requer hardware real.

| Módulo | Testável em native | Diretório de teste |
|---|---|---|
| MOD_SENSOR | Sim | `firmware/test/test_sensor/` |
| MOD_LED | Sim | `firmware/test/test_visual/` |
| MOD_JOGO | Sim | `firmware/test/test_game/` |
| MOD_WIFI | Não | — |

---

## 4. Princípio: Zero Magic Numbers em Testes <a id="zero-magic-numbers"></a>

Derivado de [VER: CODING_STANDARD.md#constantes-zero-magic-numbers].

O mesmo princípio que governa o código de produção governa os testes: nenhum número
ou string literal de domínio aparece diretamente em chamadas de função, atribuições de
mock ou asserções.

### 4.1 Classificação obrigatória para constantes de teste <a id="classificacao-constantes-teste"></a>

| Classe | Definição | Ação |
|---|---|---|
| `DERIVADO` | Valor copiado diretamente de `_config.h` | Usar a constante de `_config.h` diretamente |
| `DELTA_FRONTEIRA` | Offset `+1` ou `-1` aplicado a uma constante de `_config.h` para cruzar um limite | Declarar como `T_` com comentário obrigatório |
| `HARDCODED_TESTE` | Valor sem equivalente em spec que representa estado físico óbvio (ex: ADC=0 = repouso) | Declarar como `T_` com comentário obrigatório |
| `MAGIC NUMBER` | Número literal sem nome | **Proibido. Sem exceção.** |

### 4.2 Convenção de nomeação das constantes de teste <a id="nomenclatura-constantes-teste"></a>

Formato: `T_<MODULO>_<CAMPO>_<CONDICAO>`

O prefixo `T_` distingue constantes de teste das constantes de produção (`SENSOR_`, `VISUAL_`, etc.).

Exemplos derivados de `sensor_config.h`:

```cpp
// --- DERIVADO: sensor_config.h#SENSOR_THRESHOLD_ADC ---
// Valor mínimo que dispara callback (acima do limite, estritamente)
constexpr uint16_t T_SENSOR_ADC_VALIDO = SENSOR_THRESHOLD_ADC + 1; // DELTA_FRONTEIRA

// --- DERIVADO: sensor_config.h#SENSOR_THRESHOLD_ADC ---
// Valor exatamente no limite — nao dispara (condicao e >, nao >=)
constexpr uint16_t T_SENSOR_ADC_LIMITE = SENSOR_THRESHOLD_ADC;     // DERIVADO

// --- HARDCODED_TESTE: ADC=0 representa repouso do piezo (ausencia de impacto) ---
constexpr uint16_t T_SENSOR_ADC_REPOUSO = 0;

// --- DERIVADO: sensor_config.h#SENSOR_DEBOUNCE_MS ---
// Intervalo dentro da janela de debounce — segundo evento deve ser bloqueado
constexpr uint32_t T_SENSOR_DEBOUNCE_DENTRO = SENSOR_DEBOUNCE_MS - 1; // DELTA_FRONTEIRA

// --- DERIVADO: sensor_config.h#SENSOR_DEBOUNCE_MS ---
// Intervalo apos a janela de debounce — segundo evento deve ser permitido
constexpr uint32_t T_SENSOR_DEBOUNCE_APOS = SENSOR_DEBOUNCE_MS + 1;   // DELTA_FRONTEIRA
```

**Regra para DELTA_FRONTEIRA:** `+1` e `-1` são permitidos **somente** na definição de
constantes `T_` com o comentário `// DELTA_FRONTEIRA`. Em nenhum outro lugar.

---

## 5. Estrutura de Arquivos por Módulo <a id="estrutura-arquivos"></a>

Derivada de [VER: CODING_STANDARD.md#estrutura-diretorios].

```
firmware/test/
├── mock/
│   └── Arduino.h          ← mocks de Arduino para compilação nativa
└── test_<modulo>/
    └── test_main.cpp      ← testes Unity (constantes T_ + setUp/tearDown + RUN_TEST)
```

Um único arquivo `test_main.cpp` por módulo. Sem subdivisão interna. Sem arquivos
separados de mock por módulo — o mock vive em `test/mock/`.

---

## 6. Padrão de Mock <a id="padrao-mock"></a>

### 6.1 Separação declare → define <a id="mock-declare-define"></a>

| Arquivo | Papel |
|---|---|
| `firmware/test/mock/Arduino.h` | Declara as funções (sem implementação) e tipos de plataforma |
| `firmware/test/test_<modulo>/test_main.cpp` | **Define** as implementações das funções mockadas |

Esta separação garante que `sensor.cpp` (que inclui `<Arduino.h>`) compile no ambiente
nativo sem duplicação de definição.

### 6.2 Variáveis de estado do mock <a id="mock-estado"></a>

```cpp
// Definidas em test_main.cpp — static (escopo de arquivo)
static uint16_t g_mock_adc[40];    // indexado por número de GPIO
static uint32_t g_mock_millis;
```

As implementações mockadas leem essas variáveis. Os testes as escrevem diretamente
para simular o hardware. **Proibido expor essas variáveis em um header**.

### 6.3 Conteúdo obrigatório de `test/mock/Arduino.h` <a id="mock-arduino-h"></a>

```cpp
#pragma once
// Mock mínimo de Arduino.h para compilação nativa em testes Unity.
// Não usar fora de test/.

#include <stdint.h>

typedef int adc_atten_t;
#define ADC_ATTEN_DB_11 3   // ADC_ATTEN_DB_11: range 0-3.3V — [VER: CODING_STANDARD.md#decisoes-manuais]
#define INPUT  0
#define OUTPUT 1

uint16_t analogRead(uint8_t pin);
uint32_t millis();
void     pinMode(uint8_t pin, uint8_t mode);
void     analogSetAttenuation(adc_atten_t attenuation);
```

Cada tipo e macro aqui corresponde a um uso real em algum módulo de firmware. Nenhum
símbolo é adicionado por conveniência; cada um tem rastreabilidade para o módulo que
o usa.

---

## 7. Estrutura Obrigatória de `test_main.cpp` <a id="estrutura-test-main"></a>

```cpp
// test_main.cpp — testes unitarios de MOD_<MODULO>
// Derivado de: spec/<modulo>/<modulo>.json + _governance/TESTING_STANDARD.md v0.1.0
// CAs cobertos: CA-XX-01, CA-XX-02, ... (indicar os CAs nativamente testáveis)

#include <unity.h>
#include <string.h>
#include "<modulo>/<modulo>.h"
#include "<modulo>/<modulo>_config.h"

// ---------------------------------------------------------------------------
// Mock state (único local de definição)
// ---------------------------------------------------------------------------
static <tipo_adc> g_mock_adc[40];
static uint32_t   g_mock_millis;

// ---------------------------------------------------------------------------
// Mock implementations (declaradas em test/mock/Arduino.h)
// ---------------------------------------------------------------------------
<tipo_adc> analogRead(uint8_t pin)  { return (pin < 40) ? g_mock_adc[pin] : 0; }
uint32_t   millis()                 { return g_mock_millis; }
void       pinMode(uint8_t, uint8_t) {}
void       analogSetAttenuation(adc_atten_t) {}

// ---------------------------------------------------------------------------
// Constantes de teste — todas derivadas de _config.h (ver secao 4 do TESTING_STANDARD)
// ---------------------------------------------------------------------------
// --- DERIVADO: <modulo>_config.h#<CONSTANTE> ---
constexpr <tipo> T_<MODULO>_<CAMPO>_<CONDICAO> = <CONSTANTE> ± 1; // DELTA_FRONTEIRA

// ---------------------------------------------------------------------------
// Estado de observação dos testes
// ---------------------------------------------------------------------------
static uint8_t         s_cb_count;
static <EventoTipo>    s_ultimo_ev;

static void test_callback(<EventoTipo> ev) {
    s_cb_count++;
    s_ultimo_ev = ev;
}

// ---------------------------------------------------------------------------
// setUp / tearDown
// ---------------------------------------------------------------------------
void setUp() {
    memset(g_mock_adc, 0, sizeof(g_mock_adc));
    g_mock_millis = 0;
    s_cb_count    = 0;
    <moduloInit>();
    <moduloOnEvento>(test_callback);
}

void tearDown() {
    <moduloOnEvento>(nullptr);
}

// ---------------------------------------------------------------------------
// Testes — cada função cobre um cenário rastreável a um CA
// ---------------------------------------------------------------------------
// [CA-XX-YY] <Descrição curta do cenário>
void test_<modulo>_<cenario>_<resultado>() { ... }

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
int main() {
    UNITY_BEGIN();
    RUN_TEST(test_<modulo>_...);
    ...
    return UNITY_END();
}
```

---

## 8. Rastreabilidade de CAs nos Testes <a id="rastreabilidade-ca"></a>

Cada função de teste deve ter, imediatamente antes da assinatura, um comentário
indicando qual CA ela cobre:

```cpp
// [CA-02-05] Uma batida em uma zona gera exatamente 1 evento (debounce)
void test_sensor_debounce_bloqueia_evento_dentro_da_janela() { ... }
```

Critérios de Aceitação que **não** são testáveis em native (requerem hardware) devem
ser documentados no próprio `test_main.cpp` como comentários:

```cpp
// CA-02-04: GPIO sobrevive 100 impactos fortes — REQUER HARDWARE (osciloscopio)
// CA-02-06: Latencia deteccao < 10ms — REQUER HARDWARE (osciloscopio + logica)
```

---

## 9. Configuração PlatformIO para Testes Nativos <a id="config-platformio"></a>

### 9.1 Ambiente `[env:native]` em `platformio.ini` <a id="env-native"></a>

```ini
[env:native]
platform         = native
test_framework   = unity
build_flags      =
    -std=gnu++17
    -I $PROJECT_DIR/test/mock
build_src_filter = +<sensor/>   ; atualizar conforme modulos sao adicionados
lib_extra_dirs   = src
```

O `-I $PROJECT_DIR/test/mock` faz o compilador nativo encontrar `test/mock/Arduino.h`
quando `sensor.cpp` executa `#include <Arduino.h>`.

O `build_src_filter` limita quais arquivos de `src/` são compilados no build principal
(`pio run`). Em PlatformIO 6.x, esta opção **não afeta** o build de testes (`pio test`).

O `lib_extra_dirs = src` instrui o LDF (Library Dependency Finder) a tratar os
subdiretórios de `src/` como bibliotecas. Quando `test_main.cpp` inclui
`"sensor/sensor.h"`, o LDF detecta a dependência e compila `src/sensor/sensor.cpp`
automaticamente no build de testes. Sem esta opção, `sensor.cpp` não é linkado e
os símbolos ficam indefinidos.

| Estado | build_src_filter |
|---|---|
| Apenas MOD_SENSOR implementado | `+<sensor/>` |
| MOD_SENSOR + MOD_LED | `+<sensor/> +<visual/>` |
| MOD_SENSOR + MOD_LED + MOD_JOGO | `+<sensor/> +<visual/> +<game/>` |

`lib_extra_dirs = src` permanece fixo independente do número de módulos ativos.

### 9.2 Execução <a id="execucao"></a>

```bash
pio test -e native
```

Resultado esperado: todos os testes passam, zero falhas.

---

## 10. Checklist antes de commitar testes <a id="checklist-testes"></a>

```
□ Todas as constantes de teste têm prefixo T_ e comentário de origem
□ Nenhum número literal de domínio no corpo dos testes
□ Cada função de teste tem comentário [CA-XX-YY]
□ CAs não testáveis em native estão documentados como comentário no arquivo
□ setUp() reseta todo o estado mock e chama <moduloInit>()
□ tearDown() limpa o callback (<moduloOnEvento>(nullptr))
□ pio test -e native → zero falhas antes de qualquer commit
□ CODING_STANDARD.md#zero-magic-numbers não foi violado
```

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-28 | — | Criação: padrão genérico de testes derivados — zero magic numbers, mock declare→define, constantes T_, rastreabilidade de CAs | firmware/test/ |
| 0.1.1 | 2026-06-28 | 9.1 | Corrige configuração `[env:native]`: adiciona `lib_extra_dirs = src` obrigatório para PlatformIO 6.x; `build_src_filter` sozinho não inclui `src/` em test builds | firmware/platformio.ini |
| 0.1.2 | 2026-07-01 | depende_de, Rastreabilidade, #identificacao | Atualiza referências: CODING_STANDARD.md v0.1.1→v0.2.0, 01_arquitetura.md v0.1.0→v0.2.0 (bump MINOR retroativo de 01) | — |
| 0.1.3 | 2026-07-01 | depende_de | Atualiza referências: CODING_STANDARD.md v0.2.0→v0.2.1, 01_arquitetura.md v0.2.0→v0.2.1 (especifica DevKitC V4) | — |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | CODING_STANDARD.md | 0.2.0 | BLOQUEADOR | #constantes-zero-magic-numbers, #estrutura-arquivo, #nomenclatura |
| Pai | 01_arquitetura.md | 0.2.0 | BLOQUEADOR | #modulos-firmware |
| Governa | firmware/test/ | — | OBRIGATÓRIO | — |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
