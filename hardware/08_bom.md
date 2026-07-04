---
documento:    08_bom.md
versão:       0.3.4
status:       APROVADO
data:         2026-07-04
depende_de:
  - _PADRAO.md v0.1.0        [BLOQUEADOR]
  - 00_conceito.md v0.3.0    [BLOQUEADOR]
  - 01_arquitetura.md v0.4.0 [BLOQUEADOR]
  - 02_sensor_impacto.md v0.2.3 [BLOQUEADOR]
  - 03_saida_visual.md v0.1.8   [BLOQUEADOR]
  - 05_alimentacao.md v0.3.3    [BLOQUEADOR]
impacta:
  - 09_conexoes.md           [OBRIGATÓRIO]
  - 11_montagem.md           [OBRIGATÓRIO]
---

# 08_BOM.md — Lista de Materiais (Bill of Materials)

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | 08_bom.md |
| Versão | 0.3.4 |
| Status | APROVADO |
| Escopo | Todos os componentes necessários para uma unidade do projeto |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Listar todos os componentes eletrônicos, cabos e materiais estruturais necessários para produzir uma unidade. As quantidades precisas de cabos estão em [VER: 10_cablagem.md#tabela-fios] — este documento lista as quantidades de compra com margem.

---

## 3. Eletrônicos — Ativos e Semicondutores <a id="eletronicos-ativos"></a>

| ID | Componente | Especificação | Qtd | Referência |
|---|---|---|---|---|
| E01 | ESP32 DevKitC V4 | Módulo ESP32-WROOM-32U, 38 pinos — pino de entrada 5V rotulado **"5V"** (não "VIN"); inclui AMS1117-3.3 onboard (integrado à placa; FORA do caminho de potência de operação desde 05_alimentacao v0.3.0 — atua somente na gravação via USB) | 1 | [VER: 01_arquitetura.md#hardware] |
| E02 | Shield de expansão | Compatível com ESP32 38 pinos. **ATENÇÃO — serigrafia não confiável:** na unidade em uso, o borne rotulado "GND" entre SD3 e 5V corresponde ao pino CMD/GPIO11 (chip select da flash — causa raiz do boot loop CA-07-01). Verificar TODO rótulo contra a serigrafia do DevKitC antes de conectar: [VER: 09_conexoes.md#verificacao-serigrafia] | 1 | [VER: 01_arquitetura.md#hardware] |
| E03 | LED WS2812B | RGB endereçável individual (não fita), 3.3V | 3 | [VER: 03_saida_visual.md#componente-led] |
| E04 | Disco piezoelétrico | 27mm, sem PCB, 2 fios soldados | 4 | [VER: 02_sensor_impacto.md#componente-piezo] |
| E05 | Zener 3.3V | BZX55C3V3 ou 1N5226B | 4 | [VER: 02_sensor_impacto.md#componentes-protecao] |
| E06 | Módulo LM2596 | DC-DC Buck ajustável, módulo pré-montado | 1 | [VER: 05_alimentacao.md#modulo-lm2596] |

---

## 4. Passivos <a id="passivos"></a>

| ID | Componente | Especificação | Qtd | Referência |
|---|---|---|---|---|
| P01 | Resistor 1MΩ | 1/4W, ±5% | 4 | [VER: 02_sensor_impacto.md#componentes-protecao] |
| P02 | Resistor 300Ω | 1/4W, ±5% | 1 | [VER: 03_saida_visual.md#mapeamento-led] — série na linha de dados LED |
| P03 | Cap. eletrolítico 100μF/25V | Entrada LM2596 | 1 | [VER: 05_alimentacao.md#decoupling] |
| P04 | Cap. eletrolítico 470μF/10V | Saída LM2596 | 1 | [VER: 05_alimentacao.md#decoupling] |
| P05 | Cap. eletrolítico 10μF/16V | VDD por LED | 3 | [VER: 05_alimentacao.md#decoupling] — 3 LEDs |
| P06 | Cap. cerâmico 100nF/50V | Saída LM2596 + pino 3V3 + LEDs | 5 | [VER: 05_alimentacao.md#decoupling] — 1+1+3 |
| P07 | Cap. eletrolítico 1000μF/≥10V | Pino 3V3 DevKitC V4 | 1 | [VER: 05_alimentacao.md#decoupling] — bulk local p/ transitório da ligada do rádio |

---

## 5. Alimentação <a id="alimentacao"></a>

| ID | Componente | Especificação | Qtd | Nota |
|---|---|---|---|---|
| A01 | Fonte AC/DC | NBS30G-120250VB — 12V 2.5A 30W | 1 | Já disponível — não adquirir |

---

## 6. Cabos e Fios <a id="cabos-fios"></a>

Quantidades de **compra** com margem. Quantidades precisas de uso: [VER: 10_cablagem.md#tabela-fios].

| ID | Material | Bitola | Cor | Qtd compra |
|---|---|---|---|---|
| M01 | Fio flexível | AWG 22 | Vermelho | 0.5m |
| M02 | Fio flexível | AWG 22 | Preto | 0.5m |
| M03 | Fio flexível | AWG 24 | Vermelho | 0.5m |
| M04 | Fio flexível | AWG 24 | Preto | 2m |
| M05 | Fio flexível | AWG 24 | Laranja | 1m |
| M06 | Fio flexível | AWG 24 | Azul | 1m |
| M07 | Fio flexível | AWG 24 | Amarelo | 1m |
| M08 | Fio flexível | AWG 24 | Roxo | 1m |
| M09 | Fio flexível | AWG 24 | Branco | 0.5m |

---

## 7. Materiais de Montagem <a id="materiais-montagem"></a>

| ID | Material | Especificação | Qtd |
|---|---|---|---|
| M10 | Perfboard | 5×7cm mínimo | 1 — circuito de proteção 4× piezo |
| M11 | Heat shrink | Sortido 2mm e 4mm | 1 kit |
| M12 | Epóxi bicomponente | Cura 24h | 1 seringa |
| M13 | Borracha/cortiça | Espessura ≥ 5mm | Para gap entre zonas |

---

## 8. Materiais Estruturais <a id="materiais-estruturais"></a>

| ID | Material | Especificação | Qtd |
|---|---|---|---|
| S01 | MDF | 6mm — zonas de impacto e base | A definir em 11_montagem |
| S02 | Madeira maciça | Cabos dos martelos | 2 peças |
| S03 | Tinta látex | Laranja, Azul, Amarelo, Roxo | 1 frasco cada |
| S04 | Verniz PU | Proteção superficial | 1 frasco |

---

## 9. Custo Estimado <a id="custo-estimado"></a>

| Categoria | Estimativa BRL |
|---|---|
| Eletrônicos (E01–E06) | R$ 100–150 |
| Passivos (P01–P06) | R$ 15–25 |
| Cabos e montagem (M01–M13) | R$ 30–50 |
| Estrutural (S01–S04) | R$ 30–50 |
| **Total estimado** | **R$ 175–275** |

Fonte (A01) não contabilizada — já disponível.

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-26 | — | Criação — reescrita do zero com âncoras, _PADRAO v0.1.0 | 09, 10, 11 |
| 0.2.0 | 2026-06-30 | #eletronicos-ativos | Adiciona nota em E01: AMS1117-3.3 integrado ao DevKit, não componente externo; atualiza depende_de (02 v0.1.1, 03 v0.1.1, 05 v0.2.0) | 09, 11 [PATCH depende_de] |
| 0.2.1 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referências: 01_arquitetura.md v0.1.0→v0.2.0, 02 v0.1.1→v0.1.2, 03 v0.1.1→v0.1.2, 05 v0.2.0→v0.2.1 (bump MINOR retroativo de 01) | 09, 11 |
| 0.2.2 | 2026-07-01 | #eletronicos-ativos, #passivos | Especifica E01 como DevKitC V4 e esclarece rótulo "5V" do pino de entrada; atualiza P05/P06: "VIN" → "pino 5V"; atualiza depende_de: 01 v0.2.1, 02 v0.1.3, 03 v0.1.3, 05 v0.2.2 | 09, 11 |
| 0.3.0 | 2026-07-03 | #eletronicos-ativos, #passivos, depende_de | Cascata 05 v0.3.0 (arquitetura 3.3V direta): E01 nota AMS1117 fora do caminho de potência; E02 alerta de serigrafia não confiável (falso-GND = CMD/GPIO11, causa raiz CA-07-01); P05 reduzido a 3 (LEDs); P06 pino 3V3; P07 novo (1000μF pino 3V3); atualiza depende_de 02 v0.2.0, 03 v0.1.4, 05 v0.3.0 | 09, 11 [OBRIGATÓRIO] |
| 0.3.1 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do conceito v0.2.0 (exportação CSV+PDF com pré-visualização, M2/M3 validados): atualiza referências — 00_conceito.md v0.1.0→v0.2.0, 01_arquitetura.md v0.2.1→v0.3.0, 02_sensor_impacto.md v0.2.0→v0.2.1, 03_saida_visual.md v0.1.4→v0.1.5, 05_alimentacao.md v0.3.0→v0.3.1 | — |
| 0.3.2 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do registro do manual do pedagogo (12_manual_pedagogo.md no impacta de 00 e 07): atualiza referências — 00_conceito.md v0.2.0→v0.2.1, 01_arquitetura.md v0.3.0→v0.3.1, 02_sensor_impacto.md v0.2.1→v0.2.2, 03_saida_visual.md v0.1.5→v0.1.6, 05_alimentacao.md v0.3.1→v0.3.2 | — |
| 0.3.3 | 2026-07-04 | depende_de, Rastreabilidade | Cascata mecânica do conceito v0.3.0 (melhoria M1 — encerramento antecipado de sessão, sem impacto em lista de materiais): atualiza referências — 00_conceito.md v0.2.1→v0.3.0, 01_arquitetura.md v0.3.1→v0.4.0, 02_sensor_impacto.md v0.2.2→v0.2.3, 03_saida_visual.md v0.1.6→v0.1.7, 05_alimentacao.md v0.3.2→v0.3.3 | 09, 11 |
| 0.3.4 | 2026-07-04 | depende_de, Rastreabilidade | Cascata mecânica de 03_saida_visual.md v0.1.8 (correção de RGB Laranja/Roxo, CA-03-02/03, sem impacto em lista de materiais): atualiza referência 03_saida_visual.md v0.1.7→v0.1.8 | 09, 11 |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 00_conceito.md | 0.3.0 | BLOQUEADOR | #componentes-fisicos |
| Pai | 01_arquitetura.md | 0.4.0 | BLOQUEADOR | #hardware, #mapeamento-gpios |
| Pai | 02_sensor_impacto.md | 0.2.3 | BLOQUEADOR | #componente-piezo, #componentes-protecao |
| Pai | 03_saida_visual.md | 0.1.8 | BLOQUEADOR | #componente-led, #decoupling-led |
| Pai | 05_alimentacao.md | 0.3.3 | BLOQUEADOR | #decoupling, #alimentacao |
| Filho | 09_conexoes.md | — | OBRIGATÓRIO | #eletronicos-ativos, #passivos |
| Filho | 11_montagem.md | — | OBRIGATÓRIO | todo este documento |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
