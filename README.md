# Instrumento Lúdico-Pedagógico — ESP32

[![Verificação de Documentação](https://github.com/eflov/42/actions/workflows/verify-docs.yml/badge.svg)](https://github.com/eflov/42/actions/workflows/verify-docs.yml)

Instrumento de estímulo e avaliação de coordenação motora e discriminação visual por cor para crianças de 5 anos em ambiente pedagógico supervisionado. Hardware: ESP32-WROOM-32U, 4 piezos cerâmicos 27mm e 3 LEDs WS2812B individuais, alimentado por fonte 12V AC/DC. Interface de controle servida pelo próprio ESP32 via Access Point — qualquer browser móvel, sem instalação, sem rede externa. Dois modos de operação (unimanual e bimanual) e dois mecanismos de aleatoriedade selecionáveis pelo pedagogo.

> Fonte única de verdade: [`concept/00_conceito.md`](concept/00_conceito.md)

---

## Status do V-Model

| Fase | Artefato | Status |
|---|---|---|
| Conceito | [`concept/00_conceito.md`](concept/00_conceito.md) | APROVADO v0.1.0 |
| Arquitetura | [`system/01_arquitetura.md`](system/01_arquitetura.md) | APROVADO v0.1.0 |
| Spec módulos | [`modules/`](modules/) | APROVADO v0.1.0 |
| Spec hardware | [`hardware/`](hardware/) | APROVADO v0.1.0 |
| JSON Spec + Schema | [`spec/`](spec/) | CONCLUÍDO v0.2.0 |
| Firmware | [`firmware/src/`](firmware/src/) | CONCLUÍDO v0.3.0 — 0 errors, 0 warnings |
| Testes unitários | [`firmware/test/`](firmware/test/) | CONCLUÍDO v0.3.0 — 38/38 PASSED (native) |
| Validação com hardware | — | Pendente (requer hardware físico) |

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

### Compilar firmware

```bash
cd firmware && pio run
```

### Executar testes unitários (sem hardware)

```bash
cd firmware && pio test -e native
```

---

## Documentação

| Documento | Conteúdo |
|---|---|
| [`concept/00_conceito.md`](concept/00_conceito.md) | Glossário, modos, regras, timings, fundamentos pedagógicos |
| [`system/01_arquitetura.md`](system/01_arquitetura.md) | Stack, módulos, GPIOs, RNFs, critérios de aceitação |
| [`modules/sensor/02_sensor_impacto.md`](modules/sensor/02_sensor_impacto.md) | Piezo, circuito de proteção, threshold, debounce |
| [`modules/visual/03_saida_visual.md`](modules/visual/03_saida_visual.md) | WS2812B, cores, animações |
| [`modules/game/04_logica_jogo.md`](modules/game/04_logica_jogo.md) | Aleatoriedade, máquina de estados, score |
| [`modules/power/05_alimentacao.md`](modules/power/05_alimentacao.md) | Cadeia de alimentação, orçamento de corrente, decoupling |
| [`compliance/06_privacidade_lgpd.md`](compliance/06_privacidade_lgpd.md) | LGPD Lei 13.709/2018 — dados de crianças |
| [`modules/interface/07_interface_pedagogo.md`](modules/interface/07_interface_pedagogo.md) | Interface web, WebSocket, localStorage, exportação CSV |
| [`hardware/08_bom.md`](hardware/08_bom.md) | BOM completa com part numbers |
| [`hardware/09_conexoes.md`](hardware/09_conexoes.md) | Esquemático, mapeamento de pinos, shield |
| [`hardware/10_cablagem.md`](hardware/10_cablagem.md) | Fios, bitolas, comprimentos |
| [`hardware/11_montagem.md`](hardware/11_montagem.md) | Ordem de montagem, fixação, testes por etapa |
| [`_governance/_PADRAO.md`](_governance/_PADRAO.md) | Padrão de documentos |
| [`_governance/CODING_STANDARD.md`](_governance/CODING_STANDARD.md) | Padrão de firmware C++ |
| [`_governance/TESTING_STANDARD.md`](_governance/TESTING_STANDARD.md) | Padrão de testes Unity |
| [`_governance/WEB_STANDARD.md`](_governance/WEB_STANDARD.md) | Padrão de HTML/CSS/JS embutido |

---

## Licença

GPL-3.0 — consulte [`LICENSE`](LICENSE).
