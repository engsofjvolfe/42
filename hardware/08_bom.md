---
documento:    08_bom.md
versão:       0.1.0
status:       APROVADO
data:         2026-06-26
depende_de:
  - _PADRAO.md v0.1.0        [BLOQUEADOR]
  - 00_conceito.md v0.1.0    [BLOQUEADOR]
  - 01_arquitetura.md v0.1.0 [BLOQUEADOR]
  - 02_sensor_impacto.md v0.1.0 [BLOQUEADOR]
  - 03_saida_visual.md v0.1.0   [BLOQUEADOR]
  - 05_alimentacao.md v0.1.0    [BLOQUEADOR]
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
| Versão | 0.1.0 |
| Status | APROVADO |
| Escopo | Todos os componentes necessários para uma unidade do projeto |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Listar todos os componentes eletrônicos, cabos e materiais estruturais necessários para produzir uma unidade. As quantidades precisas de cabos estão em [VER: 10_cablagem.md#tabela-fios] — este documento lista as quantidades de compra com margem.

---

## 3. Eletrônicos — Ativos e Semicondutores <a id="eletronicos-ativos"></a>

| ID | Componente | Especificação | Qtd | Referência |
|---|---|---|---|---|
| E01 | ESP32 DevKit | ESP32-WROOM-32U, 38 pinos | 1 | [VER: 01_arquitetura.md#hardware] |
| E02 | Shield de expansão | Compatível com ESP32 38 pinos | 1 | [VER: 01_arquitetura.md#hardware] |
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
| P05 | Cap. eletrolítico 10μF/16V | VIN DevKit + VDD por LED | 4 | [VER: 05_alimentacao.md#decoupling] — 1 VIN + 3 LEDs |
| P06 | Cap. cerâmico 100nF/50V | Saída LM2596 + VIN + LEDs | 5 | [VER: 05_alimentacao.md#decoupling] — 1+1+3 |

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

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 00_conceito.md | 0.1.0 | BLOQUEADOR | #componentes-fisicos |
| Pai | 01_arquitetura.md | 0.1.0 | BLOQUEADOR | #hardware, #mapeamento-gpios |
| Pai | 02_sensor_impacto.md | 0.1.0 | BLOQUEADOR | #componente-piezo, #componentes-protecao |
| Pai | 03_saida_visual.md | 0.1.0 | BLOQUEADOR | #componente-led, #decoupling-led |
| Pai | 05_alimentacao.md | 0.1.0 | BLOQUEADOR | #decoupling, #alimentacao |
| Filho | 09_conexoes.md | — | OBRIGATÓRIO | #eletronicos-ativos, #passivos |
| Filho | 11_montagem.md | — | OBRIGATÓRIO | todo este documento |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
