---
documento:    10_cablagem.md
versão:       0.1.2
status:       APROVADO
data:         2026-06-26
depende_de:
  - _PADRAO.md v0.1.0        [BLOQUEADOR]
  - 09_conexoes.md v0.2.1    [BLOQUEADOR]
impacta:
  - 11_montagem.md           [OBRIGATÓRIO]
---

# 10_CABLAGEM.md — Especificação de Fios

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | 10_cablagem.md |
| Versão | 0.1.2 |
| Status | APROVADO |
| Escopo | Todos os fios do sistema — bitola, cor, comprimento e destino |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Especificar cada fio do sistema com bitola, cor, comprimento preciso de uso e comprimento de compra. Serve como guia de corte e identificação na bancada. As conexões elétricas que cada fio implementa estão em [VER: 09_conexoes.md#visao-geral].

---

## 3. Convenção de Cores <a id="convencao-cores"></a>

| Cor do fio | Função |
|---|---|
| Vermelho | Positivo / alimentação + |
| Preto | Negativo / GND |
| Laranja | Sinal piezo zona Laranja |
| Azul | Sinal piezo zona Azul |
| Amarelo | Sinal piezo zona Amarelo |
| Roxo | Sinal piezo zona Roxo |
| Branco | Dados LED (GPIO5 → LEDs) |

---

## 4. Regras de Montagem <a id="regras-montagem"></a>

1. **Heat shrink** em todas as emendas e terminais de piezo
2. **Strain relief** obrigatório nos cabos de piezo (evitar tração nos terminais soldados)
3. **Folga de 20%** já incluída nos comprimentos de compra — não cortar curto
4. **Identificação:** marcar cada fio de piezo na ponta com fita da cor da zona

---

## 5. Restrições de Comprimento <a id="restricoes-comprimento"></a>

| Fio(s) | Comprimento máximo | Motivo |
|---|---|---|
| F13 (GPIO5 → R300Ω) | 15cm | Minimizar capacitância na linha de dados |
| F14–F16 (inter-LED) | 10cm cada | Qualidade do sinal entre LEDs encadeados |
| F17, F19, F21, F23 (piezo → perfboard) | 50cm | Capacitância parasita degrada ADC acima deste limite |

---

## 6. Totais de Compra por Material <a id="totais-compra"></a>

| Material | Total preciso | Total com margem (compra) |
|---|---|---|
| Fio AWG 22 vermelho | 35cm | 0.5m |
| Fio AWG 22 preto | 35cm | 0.5m |
| Fio AWG 24 vermelho | 45cm | 0.5m |
| Fio AWG 24 preto | 125cm | 2m |
| Fio AWG 24 laranja | 80cm | 1m |
| Fio AWG 24 azul | 80cm | 1m |
| Fio AWG 24 amarelo | 80cm | 1m |
| Fio AWG 24 roxo | 80cm | 1m |
| Fio AWG 24 branco | 36cm | 0.5m |

Quantidades de compra registradas em [VER: 08_bom.md#cabos-fios].

---

## 7. Tabela Completa de Fios <a id="tabela-fios"></a>

| ID | De | Para | Bitola | Cor | Comprimento | Restrição |
|---|---|---|---|---|---|---|
| F01 | Fonte 12V (+) | LM2596 IN+ | AWG 22 | Vermelho | soldado fixo | — |
| F02 | Fonte 12V (−) | LM2596 IN− | AWG 22 | Preto | soldado fixo | — |
| F03 | LM2596 OUT+ | Barramento 5V (+) | AWG 22 | Vermelho | 15cm | — |
| F04 | LM2596 OUT− | Barramento 5V (−) | AWG 22 | Preto | 15cm | — |
| F05 | Barramento 5V (+) | Shield VIN | AWG 22 | Vermelho | 20cm | — |
| F06 | Barramento 5V (−) | Shield GND | AWG 22 | Preto | 20cm | — |
| F07 | Shield 3V3 | Cap. decoupling VDD LED[0] | AWG 24 | Vermelho | 15cm | — |
| F08 | Shield 3V3 | Cap. decoupling VDD LED[1] | AWG 24 | Vermelho | 15cm | — |
| F09 | Shield 3V3 | Cap. decoupling VDD LED[2] | AWG 24 | Vermelho | 15cm | — |
| F10 | Shield GND | GND LED[0] | AWG 24 | Preto | 15cm | — |
| F11 | Shield GND | GND LED[1] | AWG 24 | Preto | 15cm | — |
| F12 | Shield GND | GND LED[2] | AWG 24 | Preto | 15cm | — |
| F13 | Shield GPIO5 | R300Ω (entrada) | AWG 24 | Branco | 10cm | ≤ 15cm |
| F14 | R300Ω (saída) | LED[0] DIN | AWG 24 | Branco | 10cm | ≤ 10cm |
| F15 | LED[0] DOUT | LED[1] DIN | AWG 24 | Branco | 8cm | ≤ 10cm |
| F16 | LED[1] DOUT | LED[2] DIN | AWG 24 | Branco | 8cm | ≤ 10cm |
| F17 | Piezo Laranja (+) | Perfboard entrada Zona 1 | AWG 24 | Laranja | 50cm máx | ≤ 50cm |
| F18 | Perfboard saída Zona 1 | Shield GPIO34 | AWG 24 | Laranja | 30cm | — |
| F19 | Piezo Azul (+) | Perfboard entrada Zona 2 | AWG 24 | Azul | 50cm máx | ≤ 50cm |
| F20 | Perfboard saída Zona 2 | Shield GPIO35 | AWG 24 | Azul | 30cm | — |
| F21 | Piezo Amarelo (+) | Perfboard entrada Zona 3 | AWG 24 | Amarelo | 50cm máx | ≤ 50cm |
| F22 | Perfboard saída Zona 3 | Shield GPIO32 | AWG 24 | Amarelo | 30cm | — |
| F23 | Piezo Roxo (+) | Perfboard entrada Zona 4 | AWG 24 | Roxo | 50cm máx | ≤ 50cm |
| F24 | Perfboard saída Zona 4 | Shield GPIO33 | AWG 24 | Roxo | 30cm | — |
| F25 | Piezo (−) todos | GND perfboard | AWG 24 | Preto | 50cm máx | — |
| F26 | GND perfboard | Shield GND | AWG 24 | Preto | 30cm | — |

---

## 8. Critérios de Aceitação <a id="criterios-aceitacao"></a>

| # | Teste | Condição de aprovação |
|---|---|---|
| CA-10-01 | Continuidade | Multímetro em todos os 26 fios: resistência < 1Ω |
| CA-10-02 | Identificação | Cada fio de piezo identificado com fita na cor da zona |
| CA-10-03 | Strain relief | Tração suave no cabo piezo não transmite força ao terminal soldado |
| CA-10-04 | Comprimentos | F13 ≤ 15cm; F14–F16 ≤ 10cm; F17, F19, F21, F23 ≤ 50cm |
| CA-10-05 | Heat shrink | Todas as emendas e terminais cobertos, sem cobre exposto |

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-26 | — | Criação — reescrita do zero com âncoras, _PADRAO v0.1.0, derivada de 09_conexoes v0.1.0 | 11_montagem.md |
| 0.1.1 | 2026-06-30 | depende_de | Atualiza referência 09_conexoes.md de v0.1.0 para v0.2.0 (nota AMS1117 onboard) | — |
| 0.1.2 | 2026-07-01 | depende_de | Atualiza referência 09_conexoes.md de v0.2.0 para v0.2.1 (correção Mermaid) | — |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 09_conexoes.md | 0.2.1 | BLOQUEADOR | #cadeia-alimentacao-ascii, #mapeamento-shield, #circuito-protecao-perfboard, #cadeia-leds |
| Filho | 11_montagem.md | — | OBRIGATÓRIO | #tabela-fios, #regras-montagem |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
