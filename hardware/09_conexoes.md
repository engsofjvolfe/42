---
documento:    09_conexoes.md
versão:       0.3.3
status:       APROVADO
data:         2026-06-26
depende_de:
  - _PADRAO.md v0.1.0           [BLOQUEADOR]
  - 00_conceito.md v0.3.0       [BLOQUEADOR]
  - 01_arquitetura.md v0.4.0    [BLOQUEADOR]
  - 02_sensor_impacto.md v0.2.3 [BLOQUEADOR]
  - 03_saida_visual.md v0.1.7   [BLOQUEADOR]
  - 05_alimentacao.md v0.3.3    [BLOQUEADOR]
  - 08_bom.md v0.3.3            [BLOQUEADOR]
impacta:
  - 10_cablagem.md              [OBRIGATÓRIO]
  - 11_montagem.md              [OBRIGATÓRIO]
---

# 09_CONEXOES.md — Conexões e Mapeamento de Pinos

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | 09_conexoes.md |
| Versão | 0.3.3 |
| Status | APROVADO |
| Escopo | Conexões elétricas completas — alimentação, sensores, LEDs, shield |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Especificar todas as conexões elétricas do sistema em forma esquemática. Serve como fonte única de verdade para montagem física e cablagem. Os fios individuais são especificados em [VER: 10_cablagem.md#tabela-fios].

---

## 3. Visão Geral do Sistema <a id="visao-geral"></a>

```mermaid
flowchart TD
    subgraph ALIMENTACAO["Alimentação"]
        FONTE["Fonte 12V\nNBS30G-120250VB"]
        LM["Módulo LM2596\n12V→3.3V"]
        FONTE -->|soldado direto| LM
    end

    subgraph ESP32_SYSTEM["ESP32 DevKit + Shield"]
        SHIELD["Shield Expansão\n38 pinos"]
        ESP["ESP32-WROOM-32U"]
        RAIL33["Rail 3.3V"]
        LM -->|"3.3V → pino 3V3"| SHIELD
        SHIELD -->|"pino 3V3"| RAIL33
        RAIL33 --> ESP
    end

    subgraph PROTECAO["Perfboard — Proteção Piezo"]
        PROT1["Zona Laranja\nR1(1MΩ)+D1(Zener 3.3V)"]
        PROT2["Zona Azul\nR1(1MΩ)+D1(Zener 3.3V)"]
        PROT3["Zona Amarelo\nR1(1MΩ)+D1(Zener 3.3V)"]
        PROT4["Zona Roxo\nR1(1MΩ)+D1(Zener 3.3V)"]
    end

    RAIL33 -->|GPIO34| PROT1
    RAIL33 -->|GPIO35| PROT2
    RAIL33 -->|GPIO32| PROT3
    RAIL33 -->|GPIO33| PROT4

    PROT1 -->|"Piezo(+)"| PZ1["Piezo Laranja"]
    PROT2 -->|"Piezo(+)"| PZ2["Piezo Azul"]
    PROT3 -->|"Piezo(+)"| PZ3["Piezo Amarelo"]
    PROT4 -->|"Piezo(+)"| PZ4["Piezo Roxo"]

    RAIL33 -->|"GPIO5 → R300Ω"| LED0["WS2812B\nLED[0] Esquerdo"]
    LED0 -->|DOUT→DIN| LED1["WS2812B\nLED[1] Central"]
    LED1 -->|DOUT→DIN| LED2["WS2812B\nLED[2] Direito"]
```

**Nota sobre o AMS1117-3.3:** o regulador linear integrado ao DevKit (E01) NÃO aparece no diagrama porque está fora do caminho de potência de operação desde 05_alimentacao v0.3.0 — o rail 3.3V é alimentado direto pelo LM2596 no pino **3V3**. O AMS1117 permanece soldado na placa e atua somente quando o USB alimenta o DevKit durante gravação de firmware. [VER: 05_alimentacao.md#cadeia-alimentacao]

---

## 4. Cadeia de Alimentação — Detalhe <a id="cadeia-alimentacao-ascii"></a>

```
Fonte 12V ──(soldado)──► LM2596 OUT+ ──► Barramento 3.3V (+)
                          LM2596 OUT− ──► Barramento 3.3V (−)

Barramento 3.3V (+) ──► Shield borne 3V3 ──► ESP32 DevKitC V4 pino 3V3
                                          │            │
                                          │       Rail 3.3V
                                          │            │
                                          │    ┌───────┤
                                          │    │       │
                                          │  ESP32   3× WS2812B VDD
                                          │  chip
                                          │
                                          └──► Decoupling: 1000μF/≥10V + 100nF/50V

LM2596 Saída: 470μF/10V + 100nF/50V (soldados no módulo) — ajustar 3.30V ± 0.05V
              EM VAZIO, antes de conectar ao borne 3V3 (máx. absoluto ESP32: 3.6V)
Entrada 12V:  100μF/25V (soldado no módulo)
WS2812B VDD:  10μF/16V + 100nF/50V (por LED) — [VER: 05_alimentacao.md#decoupling]
USB:          somente gravação de firmware — nunca alimentação de operação
```

### 4.1 Verificação obrigatória de serigrafia do shield <a id="verificacao-serigrafia"></a>

A serigrafia do shield de expansão (E02) NÃO é confiável. Incidente registrado
(2026-07-02, causa raiz do boot loop CA-07-01): o borne rotulado "GND" entre
SD3 e 5V corresponde, no soquete, ao pino **CMD/GPIO11** do DevKitC — o chip
select da flash SPI. O barramento de terra do sistema parafusado nesse borne
tornou a flash ilegível.

Procedimento obrigatório ANTES de parafusar qualquer fio em qualquer borne:

1. Identificar o pino correspondente pela serigrafia do **DevKitC** (nunca
   pela do shield).
2. Multímetro em continuidade: borne ↔ solda do pino no DevKitC encaixado —
   deve haver continuidade com o pino ESPERADO e somente com ele.
3. Marcar fisicamente como PROIBIDOS os bornes que correspondem a CLK, CMD,
   SD0–SD3 (barramento da flash) e o borne falso-GND identificado.
4. Registrar a verificação no checklist de montagem [VER: 11_montagem.md#identificacao].

---

## 5. Mapeamento de Pinos do Shield <a id="mapeamento-shield"></a>

Todos os rótulos abaixo referem-se à serigrafia do **DevKitC** — a serigrafia
do shield deve ser verificada antes de cada conexão [VER: #verificacao-serigrafia].

| Pino Shield | Sinal | Destino | Referência |
|---|---|---|---|
| 3V3 (rótulo no DevKitC V4) | 3.3V entrada | Barramento LM2596 saída (3.30V) | [VER: 05_alimentacao.md#conexao-fisica] |
| GND | GND | Barramento GND | — |
| GPIO 34 | ADC1 — Laranja | Perfboard saída Zona 1 | [VER: 01_arquitetura.md#mapeamento-gpios] |
| GPIO 35 | ADC1 — Azul | Perfboard saída Zona 2 | [VER: 01_arquitetura.md#mapeamento-gpios] |
| GPIO 32 | ADC1 — Amarelo | Perfboard saída Zona 3 | [VER: 01_arquitetura.md#mapeamento-gpios] |
| GPIO 33 | ADC1 — Roxo | Perfboard saída Zona 4 | [VER: 01_arquitetura.md#mapeamento-gpios] |
| GPIO 5 | Dados LED | R300Ω → LED[0] DIN | [VER: 01_arquitetura.md#mapeamento-gpios] |

---

## 6. Circuito de Proteção Piezo — Perfboard <a id="circuito-protecao-perfboard"></a>

4 instâncias idênticas em perfboard 5×7cm. Derivado de [VER: 02_sensor_impacto.md#esquema-protecao].

```
Por zona (repetir ×4):

  Piezo(+) ──────── R1 (1MΩ) ──────┬──── Para shield GPIO_N
                                     │
                                   [K]─── D1 Zener 3.3V ───[A]
                                     │
  Piezo(−) ────────────────────────┴──── GND comum perfboard
```

GND comum da perfboard → Shield GND.

---

## 7. Cadeia de LEDs <a id="cadeia-leds"></a>

```
Shield GPIO5 ──► R300Ω ──► LED[0] DIN
                            LED[0] DOUT ──► LED[1] DIN
                                            LED[1] DOUT ──► LED[2] DIN

Por LED (próximo ao VDD):
  VDD ──┬── 10μF/16V ── GND
        └── 100nF/50V ── GND
```

R300Ω em série na linha de dados: protege GPIO5 e atenua reflexões no cabo.

---

## 8. Critérios de Aceitação <a id="criterios-aceitacao"></a>

| # | Teste | Condição de aprovação |
|---|---|---|
| CA-09-01 | Continuidade alimentação | Borne 3V3 do shield ↔ pino 3V3 do DevKitC: continuidade; com fonte ligada: 3.30V ± 0.05V no pino |
| CA-09-02 | Isolamento 12V→3.3V | Sem curto entre entrada e saída LM2596 |
| CA-09-03 | Tensão GPIO em repouso | Todos os GPIOs de piezo leem 0V em repouso |
| CA-09-04 | Proteção Zener | GPIO não ultrapassa 3.3V com impacto forte |
| CA-09-05 | Cadeia LED | GPIO5 → LED[0] → LED[1] → LED[2]: todos respondem ao comando |
| CA-09-06 | Isolamento sinais ADC | Resistência entre GPIO34, GPIO35, GPIO32, GPIO33: ∞ — sem contato metálico entre os nós de sinal |
| CA-09-07 | Verificação de serigrafia | Para cada borne utilizado: rótulo conferido contra a serigrafia do DevKitC + continuidade borne↔pino confirmada; bornes da flash (CLK/CMD/SD0–SD3) e falso-GND marcados como proibidos [VER: #verificacao-serigrafia] |

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-26 | — | Criação — reescrita do zero com âncoras, _PADRAO v0.1.0 | 10, 11 |
| 0.2.0 | 2026-06-30 | #visao-geral | Adiciona nota: AMS1117-3.3 é regulador onboard do DevKit (E01), não componente externo; atualiza depende_de (02 v0.1.1, 03 v0.1.1, 05 v0.2.0, 08 v0.2.0) | 10, 11 [PATCH depende_de] |
| 0.2.1 | 2026-07-01 | #visao-geral | Corrige sintaxe Mermaid: labels de aresta com parênteses envolvidos em aspas (`|"Piezo(+)"|`) | 10, 11 [PATCH depende_de] |
| 0.2.2 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referências: 01 v0.1.0→v0.2.0, 02 v0.1.1→v0.1.2, 03 v0.1.1→v0.1.2, 05 v0.2.0→v0.2.1, 08 v0.2.0→v0.2.1 (bump MINOR retroativo de 01) | 10, 11 |
| 0.2.3 | 2026-07-01 | #visao-geral, #cadeia-alimentacao-ascii, #mapeamento-shield, #criterios-aceitacao | Substitui rótulo "VIN" por "pino 5V" / "5V" — DevKitC V4 rotula o pino de entrada como "5V"; atualiza depende_de: 01 v0.2.1, 02 v0.1.3, 03 v0.1.3, 05 v0.2.2, 08 v0.2.2 | 10, 11 |
| 0.3.0 | 2026-07-03 | #visao-geral, #cadeia-alimentacao-ascii, #verificacao-serigrafia (nova), #mapeamento-shield, #criterios-aceitacao | Cascata 05 v0.3.0: cadeia 12V→LM2596(3.30V)→pino 3V3 direto; AMS1117 fora do caminho; USB somente gravação; seção 4.1 nova — verificação obrigatória de serigrafia do shield (incidente falso-GND=CMD/GPIO11, causa raiz CA-07-01); CA-09-01/02 revisados, CA-09-07 novo; atualiza depende_de 02 v0.2.0, 03 v0.1.4, 05 v0.3.0, 08 v0.3.0 | 10, 11 [OBRIGATÓRIO] |
| 0.3.1 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do conceito v0.2.0 (exportação CSV+PDF com pré-visualização, M2/M3 validados): atualiza referências — 00_conceito.md v0.1.0→v0.2.0, 01_arquitetura.md v0.2.1→v0.3.0, 02_sensor_impacto.md v0.2.0→v0.2.1, 03_saida_visual.md v0.1.4→v0.1.5, 05_alimentacao.md v0.3.0→v0.3.1, 08_bom.md v0.3.0→v0.3.1 | — |
| 0.3.2 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do registro do manual do pedagogo (12_manual_pedagogo.md no impacta de 00 e 07): atualiza referências — 00_conceito.md v0.2.0→v0.2.1, 01_arquitetura.md v0.3.0→v0.3.1, 02_sensor_impacto.md v0.2.1→v0.2.2, 03_saida_visual.md v0.1.5→v0.1.6, 05_alimentacao.md v0.3.1→v0.3.2, 08_bom.md v0.3.1→v0.3.2 | — |
| 0.3.3 | 2026-07-04 | depende_de, Rastreabilidade | Cascata mecânica do conceito v0.3.0 (melhoria M1 — encerramento antecipado de sessão, sem impacto em conexões): atualiza referências — 00_conceito.md v0.2.1→v0.3.0, 01_arquitetura.md v0.3.1→v0.4.0, 02_sensor_impacto.md v0.2.2→v0.2.3, 03_saida_visual.md v0.1.6→v0.1.7, 05_alimentacao.md v0.3.2→v0.3.3, 08_bom.md v0.3.2→v0.3.3 | 10, 11 |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 00_conceito.md | 0.3.0 | BLOQUEADOR | #componentes-fisicos |
| Pai | 01_arquitetura.md | 0.4.0 | BLOQUEADOR | #hardware, #mapeamento-gpios |
| Pai | 02_sensor_impacto.md | 0.2.3 | BLOQUEADOR | #esquema-protecao, #mapeamento-gpios-sensor |
| Pai | 03_saida_visual.md | 0.1.7 | BLOQUEADOR | #mapeamento-led, #decoupling-led |
| Pai | 05_alimentacao.md | 0.3.3 | BLOQUEADOR | #cadeia-alimentacao, #decoupling, #conexao-fisica |
| Pai | 08_bom.md | 0.3.3 | BLOQUEADOR | #eletronicos-ativos, #passivos |
| Filho | 10_cablagem.md | — | OBRIGATÓRIO | #cadeia-alimentacao-ascii, #mapeamento-shield, #circuito-protecao-perfboard, #cadeia-leds |
| Filho | 11_montagem.md | — | OBRIGATÓRIO | todo este documento |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
