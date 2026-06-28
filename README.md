# Instrumento Lúdico-Pedagógico — ESP32

[![Verificação de Documentação](https://github.com/eflov/42/actions/workflows/verify-docs.yml/badge.svg)](https://github.com/eflov/42/actions/workflows/verify-docs.yml)

Instrumento de estímulo e avaliação de coordenação motora e discriminação visual por cor, destinado a crianças de 5 anos em ambiente pedagógico supervisionado. Derivado de brinquedos de reação por cor (Simon Says), com adição de interface de controle para o pedagogo, registro de sessão, exportação de dados e modo bimanual para público estendido.

> Fonte única de verdade: [`concept/00_conceito.md`](concept/00_conceito.md)

---

## Status do V-Model

| Fase | Artefato | Status |
|---|---|---|
| Conceito | [`concept/00_conceito.md`](concept/00_conceito.md) | APROVADO v0.1.0 |
| Arquitetura | [`system/01_arquitetura.md`](system/01_arquitetura.md) | APROVADO v0.1.0 |
| Spec módulos | [`modules/`](modules/) | APROVADO v0.1.0 |
| Spec hardware | [`hardware/`](hardware/) | APROVADO v0.1.0 |
| JSON Spec + Schema | [`spec/`](spec/) | Em andamento (1/5) |
| Firmware | `firmware/` | Pendente |
| Testes | `firmware/test/` | Pendente |
| Validação | — | Pendente |

---

## Hardware

| Componente | Especificação |
|---|---|
| MCU | ESP32-WROOM-32U, 38 pinos |
| LEDs | 3× WS2812B individuais, GPIO 5 |
| Sensores | 4× disco piezoelétrico 27mm, GPIOs 34/35/32/33 (ADC1) |
| Alimentação | 12V AC/DC → LM2596 buck → 5V → AMS1117 → 3.3V |

Mapeamento completo de GPIOs: [`system/01_arquitetura.md#mapeamento-gpios`](system/01_arquitetura.md#mapeamento-gpios)

---

## Módulos de Firmware

| Módulo | Responsabilidade | Spec |
|---|---|---|
| MOD_SENSOR | Leitura ADC, detecção de impacto, debounce | [`spec/sensor/`](spec/sensor/) |
| MOD_LED | Controle WS2812B, animações | [`spec/visual/`](spec/visual/) |
| MOD_JOGO | Máquina de estados, lógica, score, aleatoriedade | [`spec/game/`](spec/game/) |
| MOD_WIFI | Access Point, HTTP, WebSocket | [`spec/interface/`](spec/interface/) |

Interfaces entre módulos: [`system/01_arquitetura.md#interfaces-modulos`](system/01_arquitetura.md#interfaces-modulos)

---

## Documentação

| Documento | Conteúdo |
|---|---|
| [`concept/00_conceito.md`](concept/00_conceito.md) | Fonte única de verdade — glossário, modos, regras, timings |
| [`system/01_arquitetura.md`](system/01_arquitetura.md) | Stack, módulos, GPIOs, RNFs, critérios de aceitação de sistema |
| [`modules/sensor/02_sensor_impacto.md`](modules/sensor/02_sensor_impacto.md) | Piezo, circuito de proteção, threshold, debounce |
| [`modules/visual/03_saida_visual.md`](modules/visual/03_saida_visual.md) | WS2812B, cores, animações de boot e fim de sessão |
| [`modules/game/04_logica_jogo.md`](modules/game/04_logica_jogo.md) | Algoritmos de aleatoriedade, máquina de estados, score |
| [`modules/power/05_alimentacao.md`](modules/power/05_alimentacao.md) | Cadeia de alimentação, orçamento de corrente, decoupling |
| [`compliance/06_privacidade_lgpd.md`](compliance/06_privacidade_lgpd.md) | Dados de crianças — LGPD Lei 13.709/2018 |
| [`modules/interface/07_interface_pedagogo.md`](modules/interface/07_interface_pedagogo.md) | Interface web, localStorage, exportação CSV |
| [`hardware/08_bom.md`](hardware/08_bom.md) | BOM completa com part numbers |
| [`hardware/09_conexoes.md`](hardware/09_conexoes.md) | Esquemático, mapeamento de pinos, shield |
| [`hardware/10_cablagem.md`](hardware/10_cablagem.md) | Fios, bitolas, comprimentos |
| [`hardware/11_montagem.md`](hardware/11_montagem.md) | Ordem de montagem, fixação, testes por etapa |

---

## Desenvolvimento

### Pré-requisitos

- Python 3.11+
- PlatformIO CLI
- MinGW-w64 (testes nativos no Windows)

### Verificar integridade da documentação

```bash
python scripts/run_all.py
```

Verifica: links `[VER: file#anchor]`, versões em `depende_de`, simetria `impacta ↔ Rastreabilidade`.

### Compilar firmware (após ETAPA 7)

```bash
cd firmware && pio run
```

### Executar testes unitários (após ETAPA 7)

```bash
cd firmware && pio test -e native
```

---

## Protocolo de Trabalho

Este repositório segue o V-Model de engenharia com rastreabilidade bidirecional entre todos os artefatos.

- Guia de sessão: [`.claude/CLAUDE.md`](.claude/CLAUDE.md)
- Protocolo V-Model: [`v_model_protocol/arquivo2-protocolo-ia.md`](v_model_protocol/arquivo2-protocolo-ia.md)
- Padrão de documentos: [`_governance/_PADRAO.md`](_governance/_PADRAO.md)
- Histórico: [`CHANGELOG.md`](CHANGELOG.md)

Todo artefato (documento, spec JSON, firmware) deriva exclusivamente dos documentos declarados em `depende_de`. Nada é inventado ou inferido.

---

## Licença

GPL-3.0 — consulte [`LICENSE`](LICENSE).
