---
documento:    CODING_STANDARD.md
versão:       0.2.3
status:       APROVADO
data:         2026-06-28
depende_de:
  - _PADRAO.md v0.1.0           [BLOQUEADOR]
  - 01_arquitetura.md v0.3.1    [BLOQUEADOR]
impacta:
  - TESTING_STANDARD.md         [CONDICIONAL: #constantes-zero-magic-numbers, #estrutura-arquivo, #nomenclatura]
  - firmware/src/ (arquivos .h e .cpp — nao sao .md)
---

# CODING_STANDARD.md — Padrão de Código Firmware

**Escopo:** Todo código C++ em `firmware/src/`, sem exceção.

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | CODING_STANDARD.md |
| Versão | 0.2.3 |
| Status | APROVADO |
| Escopo | Firmware C++/Arduino no PlatformIO — todos os módulos |
| Pais | _PADRAO.md v0.1.0, 01_arquitetura.md v0.3.0 |
| Fonte de derivação | spec/firmware_constants.json |

---

## 2. Objetivo <a id="objetivo"></a>

Definir de forma determinística as regras que governam todo código em `firmware/src/`. Cada regra deriva de um documento aprovado. Nenhuma regra é inventada.

**Princípio central:** nenhum número ou string literal aparece em `.cpp`. Todo valor com significado de domínio é uma constante nomeada declarada em `_config.h`, com origem rastreável a um campo de spec JSON via `spec/firmware_constants.json`.

**Geração automática:** as seções 3, 6.2 e 7, e os arquivos `firmware/src/<modulo>/<modulo>_config.h`, são gerados automaticamente por `scripts/generate_coding_standard.py` a partir de `spec/firmware_constants.json`. Não editar manualmente. Para regenerar:

```
python scripts/generate_coding_standard.py
```

Para verificar sync (executado por `run_all.py`):

```
python scripts/generate_coding_standard.py --check
```

---

## 3. Estrutura de Diretórios <a id="estrutura-diretorios"></a>

Derivada de [VER: 01_arquitetura.md#modulos-firmware] e de `spec/firmware_constants.json#modulos`.

<!-- BEGIN GENERATED:arvore_diretorios -->
```
firmware/
├── platformio.ini
├── src/
│   ├── main.cpp
│   ├── sensor/
│   │   ├── sensor_config.h
│   │   ├── sensor.h
│   │   └── sensor.cpp
│   ├── visual/
│   │   ├── visual_config.h
│   │   ├── visual.h
│   │   └── visual.cpp
│   ├── game/
│   │   ├── game_config.h
│   │   ├── game.h
│   │   └── game.cpp
│   └── interface/
│       ├── interface_config.h
│       ├── interface.h
│       └── interface.cpp
└── test/
    ├── test_sensor/
    ├── test_visual/
    └── test_game/
```
<!-- END GENERATED:arvore_diretorios -->

---

## 4. Estrutura Obrigatória por Arquivo <a id="estrutura-arquivo"></a>

### 4.1 `_config.h` — constantes derivadas do JSON <a id="estrutura-config-h"></a>

```cpp
#pragma once

// sensor_config.h
// Fonte: spec/sensor/sensor.json (via spec/firmware_constants.json)
// Nenhum valor neste arquivo e inventado.
// Toda alteracao exige atualizacao de firmware_constants.json e regeneracao.

#include <stdint.h>

// --- DERIVADO: spec/sensor/sensor.json#mapeamento_gpios[0].gpio ---
constexpr uint8_t SENSOR_GPIO_LARANJA = 34;
```

Regras:
- `#pragma once` obrigatório. Nunca usar `#ifndef` guard.
- Primeiro include: `<stdint.h>`.
- Cada constante precedida de comentário citando o campo JSON exato.
- Nenhum comentário narrativo — apenas a referência ao campo.
- Formato: `// --- DERIVADO: <spec_arquivo>#<spec_campo> ---`
- Constante `[CALIBRAR]`: sufixo `// [CALIBRAR] — confirmar apos prototipagem`.
- Constante `HARDCODED JUSTIFICADO`: comentário com justificativa técnica completa.
- O conteúdo de cada `_config.h` é gerado pelo script. Não editar manualmente.

### 4.2 `.h` — interface pública do módulo <a id="estrutura-h"></a>

```cpp
#pragma once

// sensor.h — interface publica de MOD_SENSOR
// [VER: 01_arquitetura.md#mod-sensor]

#include <stdint.h>

struct EventoImpacto {
    enum class Zona { LARANJA, AZUL, AMARELO, ROXO };
    Zona zona;
    uint32_t timestamp_ms;
};

void sensorInit();
void sensorLoop();
void sensorOnImpacto(void (*callback)(EventoImpacto));
```

Regras:
- `#pragma once` obrigatório.
- Apenas declarações públicas. Nenhuma implementação.
- Nenhuma variável global exposta. Sem `extern`.
- Tipos de structs e enums definidos canonicamente no `.h` do módulo produtor.
- Cabeçalho com referência ao documento pai.

### 4.3 `.cpp` — implementação <a id="estrutura-cpp"></a>

```cpp
// sensor.cpp — implementacao de MOD_SENSOR

#include "sensor.h"
#include "sensor_config.h"
#include <Arduino.h>

static uint32_t s_debounceTs[4] = {0, 0, 0, 0};
static void (*s_callback)(EventoImpacto) = nullptr;

void sensorInit() {
    pinMode(SENSOR_GPIO_LARANJA, INPUT);
}
```

Regras:
- Ordem de includes: `"<modulo>.h"`, `"<modulo>_config.h"`, `<Arduino.h>`.
- Variáveis internas: sempre `static`. Nunca `extern` de outro módulo.
- Nenhum número ou string literal no corpo de funções.
- Sem lógica de domínio de outro módulo. Derivado de [VER: 01_arquitetura.md#interfaces-modulos].

---

## 5. Convenção de Nomenclatura <a id="nomenclatura"></a>

### 5.1 Constantes <a id="nomenclatura-constantes"></a>

Formato: `MODULO_CONTEXTO_CAMPO`

| Módulo | Prefixo |
|---|---|
| MOD_SENSOR | `SENSOR_` |
| MOD_LED | `VISUAL_` |
| MOD_JOGO | `GAME_` |
| MOD_WIFI | `INTERFACE_` |

Prefixos definidos em `spec/firmware_constants.json#modulos[].prefixo`. A unidade faz parte do nome quando relevante: `_MS`, `_ADC`, `_PCT`.

### 5.2 Tipos (structs e enums) <a id="nomenclatura-tipos"></a>

PascalCase. Tipos canonicamente definidos em [VER: 01_arquitetura.md#interfaces-modulos]:

```
EventoImpacto   EventoImpacto::Zona
ComandoLED      ComandoLED::LED    ComandoLED::Cor
ConfigSessao
EventoResultado EventoResultado::Tipo
```

Nenhum novo tipo de troca entre módulos sem atualizar `01_arquitetura.md`.

### 5.3 Funções <a id="nomenclatura-funcoes"></a>

camelCase com prefixo do módulo:

```
sensorInit()     sensorLoop()     sensorOnImpacto()
visualInit()     visualLoop()     visualSetLED()
gameInit()       gameLoop()       gameIniciarSessao()
interfaceInit()  interfaceLoop()
```

### 5.4 Variáveis internas <a id="nomenclatura-variaveis"></a>

camelCase com prefixo `s_` (static) para variáveis de módulo. camelCase sem prefixo para variáveis locais de função.

---

## 6. Regra de Constantes: Zero Magic Numbers <a id="constantes-zero-magic-numbers"></a>

### 6.1 Classificação obrigatória <a id="classificacao-constantes"></a>

| Classe | Definição | Ação |
|---|---|---|
| `DERIVADO` | Existe como campo em spec JSON mapeado em `firmware_constants.json` | Declarar em `_config.h` gerado automaticamente |
| `HARDCODED JUSTIFICADO` | Não existe no JSON; é detalhe de plataforma ou API | Declarar em `_config.h` com comentário técnico obrigatório |
| `MAGIC NUMBER` | Número sem nome no corpo do código | **Proibido. Sem exceção.** |

### 6.2 Tabelas de constantes por módulo <a id="tabela-constantes"></a>

Geradas automaticamente de `spec/firmware_constants.json`. Não editar manualmente.

#### sensor_config.h <a id="sensor-config"></a>

<!-- BEGIN GENERATED:tabela_sensor -->
| Constante | Valor | Tipo C | Classe | Origem JSON |
|---|---|---|---|---|
| `SENSOR_GPIO_LARANJA` | 34 | `uint8_t` | DERIVADO | `spec/sensor/sensor.json#mapeamento_gpios[0].gpio` |
| `SENSOR_GPIO_AZUL` | 35 | `uint8_t` | DERIVADO | `spec/sensor/sensor.json#mapeamento_gpios[1].gpio` |
| `SENSOR_GPIO_AMARELO` | 32 | `uint8_t` | DERIVADO | `spec/sensor/sensor.json#mapeamento_gpios[2].gpio` |
| `SENSOR_GPIO_ROXO` | 33 | `uint8_t` | DERIVADO | `spec/sensor/sensor.json#mapeamento_gpios[3].gpio` |
| `SENSOR_THRESHOLD_ADC` | 500 | `uint16_t` | DERIVADO [CALIBRAR] | `spec/sensor/sensor.json#deteccao.threshold` |
| `SENSOR_DEBOUNCE_MS` | 200 | `uint16_t` | DERIVADO | `spec/sensor/sensor.json#deteccao.debounce_ms` |
| `SENSOR_ADC_MAX` | 4095 | `uint16_t` | DERIVADO | `spec/sensor/sensor.json#adc.contagens_maximas` |
| `SENSOR_N_ZONAS` | 4 | `uint8_t` | DERIVADO | `spec/sensor/sensor.json#componente.quantidade` |
<!-- END GENERATED:tabela_sensor -->

#### visual_config.h <a id="visual-config"></a>

<!-- BEGIN GENERATED:tabela_visual -->
| Constante | Valor | Tipo C | Classe | Origem JSON |
|---|---|---|---|---|
| `VISUAL_GPIO_DADOS` | 5 | `uint8_t` | DERIVADO | `spec/visual/visual.json#alimentacao.gpio_dados` |
| `VISUAL_N_LEDS` | 3 | `uint8_t` | DERIVADO | `spec/visual/visual.json#componente.quantidade` |
| `VISUAL_BRIGHTNESS` | 150 | `uint8_t` | DERIVADO | `spec/visual/visual.json#alimentacao.brightness_fixo` |
| `VISUAL_LED_ESQUERDO` | 0 | `uint8_t` | DERIVADO | `spec/visual/visual.json#mapeamento_led[0].indice` |
| `VISUAL_LED_CENTRAL` | 1 | `uint8_t` | DERIVADO | `spec/visual/visual.json#mapeamento_led[1].indice` |
| `VISUAL_LED_DIREITO` | 2 | `uint8_t` | DERIVADO | `spec/visual/visual.json#mapeamento_led[2].indice` |
| `VISUAL_COR_LARANJA_R` | 255 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Laranja.r` |
| `VISUAL_COR_LARANJA_G` | 80 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Laranja.g` |
| `VISUAL_COR_LARANJA_B` | 0 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Laranja.b` |
| `VISUAL_COR_AZUL_R` | 0 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Azul.r` |
| `VISUAL_COR_AZUL_G` | 0 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Azul.g` |
| `VISUAL_COR_AZUL_B` | 255 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Azul.b` |
| `VISUAL_COR_AMARELO_R` | 255 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Amarelo.r` |
| `VISUAL_COR_AMARELO_G` | 180 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Amarelo.g` |
| `VISUAL_COR_AMARELO_B` | 0 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Amarelo.b` |
| `VISUAL_COR_ROXO_R` | 148 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Roxo.r` |
| `VISUAL_COR_ROXO_G` | 0 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Roxo.g` |
| `VISUAL_COR_ROXO_B` | 211 | `uint8_t` | DERIVADO | `spec/visual/visual.json#cores_rgb.Roxo.b` |
| `VISUAL_BOOT_PASSO_MS` | 150 | `uint16_t` | DERIVADO | `spec/visual/visual.json#boot_animation.passos[0].duracao_ms` |
| `VISUAL_BOOT_PAUSA_MS` | 500 | `uint16_t` | DERIVADO | `spec/visual/visual.json#boot_animation.passos[4].duracao_ms` |
| `VISUAL_BOOT_DURACAO_MS` | 3000 | `uint16_t` | DERIVADO | `spec/visual/visual.json#boot_animation.duracao_total_ms` |
| `VISUAL_CELEBRACAO_ROTACOES` | 5 | `uint8_t` | DERIVADO | `spec/visual/visual.json#animacao_celebracao.rotacoes_completas` |
| `VISUAL_CELEBRACAO_DURACAO_MS` | 3000 | `uint16_t` | DERIVADO | `spec/visual/visual.json#animacao_celebracao.duracao_total_ms` |
<!-- END GENERATED:tabela_visual -->

#### game_config.h <a id="game-config"></a>

<!-- BEGIN GENERATED:tabela_game -->
| Constante | Valor | Tipo C | Classe | Origem JSON |
|---|---|---|---|---|
| `GAME_MODO_UM` | 1 | `uint8_t` | DERIVADO | `spec/game/game.json#modos[0].id` |
| `GAME_MODO_DOIS` | 2 | `uint8_t` | DERIVADO | `spec/game/game.json#modos[1].id` |
| `GAME_N_CORES` | 4 | `uint8_t` | DERIVADO | `spec/game/game.json#mecanismos_aleatoriedade[0].n_cores` |
| `GAME_INTERVALO_MS` | 2000 | `uint16_t` | DERIVADO [CALIBRAR] | `spec/game/game.json#timings.intervalo_ms.valor_padrao` |
| `GAME_JANELA_MS_PADRAO` | 800 | `uint16_t` | DERIVADO [CALIBRAR] | `spec/game/game.json#timings.janela_ms.valor_padrao` |
| `GAME_MECA_A_BLOCO` | 4 | `uint8_t` | DERIVADO | `spec/game/game.json#mecanismos_aleatoriedade[0].tamanho_bloco` |
| `GAME_MECA_A_INDICE_INICIAL` | 4 | `uint8_t` | DERIVADO | `spec/game/game.json#mecanismos_aleatoriedade[0].indice_inicial` |
| `GAME_MECA_B_PESO_INICIAL` | 4 | `uint8_t` | DERIVADO | `spec/game/game.json#mecanismos_aleatoriedade[1].peso_inicial_por_cor` |
| `GAME_MECA_B_PESO_MIN` | 0 | `uint8_t` | DERIVADO | `spec/game/game.json#mecanismos_aleatoriedade[1].peso_minimo` |
| `GAME_SCORE_DELTA_ACERTO` | 1 | `uint8_t` | DERIVADO | `spec/game/game.json#acerto.score_delta` |
| `GAME_SCORE_DELTA_ERRO` | 0 | `uint8_t` | DERIVADO | `spec/game/game.json#erro.score_delta` |
<!-- END GENERATED:tabela_game -->

#### interface_config.h <a id="interface-config"></a>

<!-- BEGIN GENERATED:tabela_interface -->
| Constante | Valor | Tipo C | Classe | Origem JSON |
|---|---|---|---|---|
| `INTERFACE_AP_SSID` | `"BMI"` | `const char*` | DERIVADO | `spec/interface/interface.json#access_point.ssid` |
| `INTERFACE_AP_IP` | `"192.168.4.1"` | `const char*` | DERIVADO | `spec/interface/interface.json#access_point.ip` |
| `INTERFACE_AP_CANAL` | 1 | `uint8_t` | DERIVADO | `spec/interface/interface.json#access_point.canal` |
| `INTERFACE_FEEDBACK_ACERTO_MS` | 1500 | `uint16_t` | DERIVADO | `spec/interface/interface.json#estados_interface.duracao_feedback_acerto_ms` |
| `INTERFACE_WS_PATH` | `"/ws"` | `const char*` | DERIVADO | `spec/interface/interface.json#servidor_http.rotas[1].rota` |
<!-- END GENERATED:tabela_interface -->

---

## 7. Decisões Manuais Inerentes <a id="decisoes-manuais"></a>

Valores que não existem em nenhum spec JSON. São detalhes de plataforma, API ou timing que dependem da implementação concreta. Declarados em `_config.h` como `HARDCODED JUSTIFICADO` com comentário técnico obrigatório.

<!-- BEGIN GENERATED:decisoes_manuais -->
| # | Constante | Modulo | Tipo C | Justificativa |
|---|---|---|---|---|
| DM-01 | `SENSOR_WDT_TIMEOUT_MS` | sensor | `uint32_t` | Timeout do WDT nao e parametro de dominio. Calculado como duracao_max_loop_ms multiplicado por fator de seguranca. Determinar apos medir o loop mais longo na implementacao. Comentario obrigatorio com o calculo explicito. |
| DM-02 | `SENSOR_ADC_ATENUACAO` | sensor | `adc_attenuation_t` | Tipo da camada Arduino (Arduino ESP32 3.x / IDF5) para range 0-3.3V no ADC1. adc_attenuation_t substitui adc_atten_t (ESP-IDF direto) a partir do Arduino ESP32 3.x. Nao e parametro de dominio; e configuracao de silicio. Nao existe campo correspondente em nenhum spec JSON. |
| DM-03 | `VISUAL_COLOR_ORDER` | visual | `EOrder` | GRB e enum da biblioteca FastLED. visual.json#componente.color_order registra 'GRB' como string; o simbolo C++ e fornecido pela API da biblioteca e nao pode ser parametrizado sem reflexao. |
<!-- END GENERATED:decisoes_manuais -->

**Regra para identificar decisão manual genuína:** se remover o valor do JSON não quebra nenhum critério de aceitação nem nenhuma especificação de módulo — ele não é de domínio.

**Quando um spec JSON é atualizado:** verificar se o campo alterado está mapeado em `firmware_constants.json`. Se sim: atualizar `firmware_constants.json` e rodar `generate_coding_standard.py`. O `run_all.py` detecta divergência automaticamente.

---

## 8. Modularidade: Isolamento entre Módulos <a id="modularidade"></a>

Derivado de [VER: 01_arquitetura.md#interfaces-modulos].

### 8.1 O que cada módulo pode fazer <a id="modularidade-pode"></a>

| Módulo | Pode incluir | Pode usar |
|---|---|---|
| MOD_SENSOR | `sensor.h`, `sensor_config.h`, `<Arduino.h>` | ADC1, `millis()`, callback registrado |
| MOD_LED | `visual.h`, `visual_config.h`, `<FastLED.h>` | GPIO 5, FastLED API |
| MOD_JOGO | `game.h`, `game_config.h`, `sensor.h`, `visual.h` | Callbacks de MOD_SENSOR; `visualSetLED()` |
| MOD_WIFI | `interface.h`, `interface_config.h`, `game.h`, `<ArduinoJson.h>` | ESPAsyncWebServer, WebSocket, ArduinoJson, callback de MOD_JOGO |

### 8.2 O que cada módulo não pode fazer <a id="modularidade-nao-pode"></a>

- Incluir `_config.h` de outro módulo.
- Declarar `extern` apontando para variável de outro módulo.
- Chamar `analogRead()` em GPIO que pertence a outro módulo.
- Chamar `FastLED.show()` fora de MOD_LED.
- Conhecer o estado interno de outro módulo.

### 8.3 Como módulos trocam dados <a id="modularidade-troca"></a>

Exclusivamente por callback registrado ou chamada direta de função pública com dado por parâmetro. Proibido: variável global de outro módulo, ponteiro para struct interna.

### 8.4 Ordem de inicialização em main.cpp <a id="modularidade-ordem-init"></a>

```cpp
void setup() {
    sensorInit();
    visualInit();
    gameInit();
    interfaceInit();
}

void loop() {
    sensorLoop();
    visualLoop();
    gameLoop();
    interfaceLoop();
}
```

Derivada das dependências em [VER: 01_arquitetura.md#modulos-firmware]. Alteração exige atualização desse documento.

---

## 9. Interfaces entre Módulos <a id="interfaces-modulos"></a>

Tipos canonicamente definidos em [VER: 01_arquitetura.md#interfaces-modulos].

### 9.1 MOD_SENSOR → MOD_JOGO <a id="interface-sensor-jogo"></a>

Derivado de [VER: 01_arquitetura.md#interface-sensor-jogo].

```cpp
struct EventoImpacto {
    enum class Zona { LARANJA, AZUL, AMARELO, ROXO };
    Zona zona;
    uint32_t timestamp_ms;
};
void sensorOnImpacto(void (*callback)(EventoImpacto));
```

### 9.2 MOD_JOGO → MOD_LED <a id="interface-jogo-led"></a>

Derivado de [VER: 01_arquitetura.md#interface-jogo-led].

```cpp
struct ComandoLED {
    enum class LED { CENTRAL, ESQUERDO, DIREITO, TODOS };
    enum class Cor  { LARANJA, AZUL, AMARELO, ROXO, OFF };
    LED led;
    Cor cor;
};
void visualSetLED(ComandoLED cmd);
```

### 9.3 MOD_WIFI → MOD_JOGO <a id="interface-wifi-jogo-config"></a>

Derivado de [VER: 01_arquitetura.md#interface-jogo-wifi].

```cpp
struct ConfigSessao {
    String nome_crianca;
    uint8_t n_interacoes;
    enum class Modo { UM_MARTELO, DOIS_MARTELOS } modo;
    enum class Mecanismo { A_SHUFFLE, B_PESO } mecanismo;
    uint16_t janela_ms;
};
void gameIniciarSessao(ConfigSessao cfg);
void gamePausarSessao();
void gameRetomarSessao();
```

### 9.4 MOD_JOGO → MOD_WIFI <a id="interface-jogo-wifi-resultado"></a>

Derivado de [VER: 01_arquitetura.md#interface-jogo-wifi].

```cpp
struct EventoResultado {
    enum class Tipo { ACERTO, ERRO, FIM_SESSAO };
    Tipo tipo;
    uint8_t acertos;
    uint8_t total;
    uint32_t duracao_ms;
};
void gameOnResultado(void (*callback)(EventoResultado));
```

---

## 10. Comentários <a id="comentarios"></a>

### 10.1 Obrigatórios <a id="comentarios-obrigatorios"></a>

| Onde | Conteúdo obrigatório |
|---|---|
| Cada constante em `_config.h` | `// --- DERIVADO: <spec_arquivo>#<spec_campo> ---` |
| Constante `HARDCODED JUSTIFICADO` | Justificativa técnica completa |
| Constante `[CALIBRAR]` | `// [CALIBRAR] — confirmar apos prototipagem` |
| Cabeçalho de cada `.h` | Nome do módulo e referência ao documento pai |
| Loop que excede 10ms | Por que é necessário e como o WDT é alimentado |

### 10.2 Proibidos <a id="comentarios-proibidos"></a>

- Comentários que descrevem o que o código obviamente faz.
- Código comentado. Remover completamente.
- `TODO` sem número de issue rastreável.
- Comentários de autoria.

---

## 11. Include Guards <a id="include-guards"></a>

`#pragma once` em todo `.h` e `_config.h`. Nunca `#ifndef`/`#define`/`#endif`.

---

## 12. Tipagem <a id="tipagem"></a>

Usar tipos de largura fixa de `<stdint.h>`: `uint8_t`, `uint16_t`, `uint32_t`, `int16_t`, `int32_t`. Nunca `int`, `long`, `byte`, `unsigned int`.

- `const char*` para constantes de configuração estáticas.
- `String` para dados de usuário com tamanho variável. Nunca em loop crítico.
- `enum class` (scoped). Nunca `enum` simples.

---

## 13. Tratamento de Erros <a id="tratamento-erros"></a>

Derivado de [VER: 01_arquitetura.md#requisitos-nao-funcionais].

- Valor ADC fora de `[0, SENSOR_ADC_MAX]`: descartado (threshold não atinge).
- Callback nulo: verificar `if (callback != nullptr)` antes de chamar.
- Desconexão WiFi: `gamePausarSessao()` chamado automaticamente. Derivado de [VER: 01_arquitetura.md#mod-wifi].
- WDT disparado: reset do ESP32. Timeout configurado acima do loop mais longo.

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-28 | — | Criação: padrão de código firmware. Seções 3, 6.2 e 7 geradas automaticamente por generate_coding_standard.py a partir de spec/firmware_constants.json | firmware/src/ |
| 0.1.0 | 2026-06-28 | 2, 4.1 | generate_coding_standard.py estendido para gerar _config.h. Seção 2 corrigida para listar _config.h como artefato gerado. Constantes com tipo de plataforma (DM-02, DM-03) emitidas como stub [PLATAFORMA] — compilaveis em native. | firmware/src/ |
| 0.1.1 | 2026-06-28 | frontmatter, Rastreabilidade | Adicionado TESTING_STANDARD.md em impacta (CONDICIONAL: secoes de constantes, estrutura e nomenclatura). Bump PATCH. | TESTING_STANDARD.md |
| 0.2.0 | 2026-06-28 | 8.1 | Adiciona ArduinoJson a includes e usos permitidos de MOD_WIFI — conforme 01_arquitetura.md#stack-tecnologico. Bump MINOR retroativo. | firmware/src/interface/ |
| 0.2.0 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referência 01_arquitetura.md de v0.1.0 para v0.2.0 (bump MINOR retroativo) | — |
| 0.2.1 | 2026-07-01 | depende_de | Atualiza referência 01_arquitetura.md v0.2.0→v0.2.1 (especifica DevKitC V4) | TESTING_STANDARD.md |
| 0.2.2 | 2026-07-03 | depende_de, Rastreabilidade, #identificacao | Cascata do conceito v0.2.0 (exportação CSV+PDF com pré-visualização, M2/M3 validados): atualiza referências — 01_arquitetura.md v0.2.1→v0.3.0; corrige versões desatualizadas na Identificação (0.2.0 e 01 v0.1.0) e na Rastreabilidade (01 v0.2.0) | — |
| 0.2.3 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do registro do manual do pedagogo (12_manual_pedagogo.md no impacta de 00 e 07): atualiza referências — 01_arquitetura.md v0.3.0→v0.3.1 | — |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 01_arquitetura.md | 0.3.1 | BLOQUEADOR | #modulos-firmware, #interfaces-modulos, #stack-tecnologico, #mapeamento-gpios, #requisitos-nao-funcionais |
| Fonte | spec/firmware_constants.json | — | DERIVADO | modulos, constantes, decisoes_manuais_inerentes |
| Filho | TESTING_STANDARD.md | — | CONDICIONAL: #constantes-zero-magic-numbers, #estrutura-arquivo, #nomenclatura | — |
| Governa | firmware/src/ | — | OBRIGATÓRIO | todos os .h e .cpp |

---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
