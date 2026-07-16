---
documento:    03_saida_visual.md
versão:       0.1.10
status:       APROVADO
data:         2026-07-16
depende_de:
  - _PADRAO.md v0.1.0        [BLOQUEADOR]
  - 00_conceito.md v0.5.0    [BLOQUEADOR]
  - 01_arquitetura.md v0.4.2 [BLOQUEADOR]
  - 05_alimentacao.md v0.3.5 [BLOQUEADOR]
impacta:
  - 08_bom.md                [OBRIGATÓRIO]
  - 09_conexoes.md           [OBRIGATÓRIO]
---

# 03_SAIDA_VISUAL.md — Saída Visual — LEDs WS2812B

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | 03_saida_visual.md |
| Versão | 0.1.8 |
| Status | APROVADO |
| Módulo firmware | MOD_LED — [VER: 01_arquitetura.md#mod-led] |
| GPIO de dados | 5 — [VER: 01_arquitetura.md#mapeamento-gpios] |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Especificar o componente LED, alimentação, mapeamento LED↔posição, cores RGB, animações de boot e celebração, e critérios de aceitação do MOD_LED. Derivado de [VER: 00_conceito.md#indicadores-led] e [VER: 00_conceito.md#modos-operacao].

---

## 3. Componente <a id="componente-led"></a>

| Campo | Valor |
|---|---|
| Tipo | WS2812B — LED RGB endereçável individual (não fita) |
| Quantidade | 3 unidades |
| Protocolo | 1-wire, 800kHz |
| Biblioteca | FastLED ≥ 3.6 — [VER: 01_arquitetura.md#stack-tecnologico] |
| COLOR_ORDER | GRB (padrão WS2812B) |

---

## 4. Alimentação e Compatibilidade de Nível Lógico <a id="alimentacao-led"></a>

Decisão derivada de [VER: 05_alimentacao.md#cadeia-alimentacao]:

| Parâmetro | Valor | Justificativa |
|---|---|---|
| Tensão VDD | **3.3V** (rail 3.3V — saída direta do LM2596) | WS2812B a 3.3V aceita dado ≥ 2.31V; GPIO5 emite 3.3V → compatível sem level shifter |
| Tensão dado GPIO5 | 3.3V | GPIO do ESP32 — [VER: 01_arquitetura.md#mapeamento-gpios] |
| Level shifter | Não necessário | Eliminado pela escolha de alimentar o LED em 3.3V |
| Corrente típica (3 LEDs) | 30mA | setBrightness(150), uma cor por LED |
| Corrente pico (3 LEDs) | 120mA | Branco full, brilho 255 — dentro do budget — [VER: 05_alimentacao.md#orcamento-corrente] |

---

## 5. Mapeamento LED → Posição → Modo <a id="mapeamento-led"></a>

Derivado de [VER: 00_conceito.md#indicadores-led] e [VER: 00_conceito.md#modos-operacao].

| Índice FastLED | Posição física | Modo ativo | Função |
|---|---|---|---|
| LED[0] | Esquerdo | Modo 2 exclusivamente | Exibe cor-alvo esquerda |
| LED[1] | Central | Modo 1 exclusivamente | Exibe cor-alvo única |
| LED[2] | Direito | Modo 2 exclusivamente | Exibe cor-alvo direita |

GPIO5 → R_série (300Ω) → LED[0] DIN → LED[0] DOUT → LED[1] DIN → LED[1] DOUT → LED[2] DIN

---

## 6. Cores RGB <a id="cores-rgb"></a>

Paleta derivada de [VER: 00_conceito.md#zonas-impacto] — Wong (2011) para acessibilidade em daltonismo.

| Cor | R | G | B | Hex |
|---|---|---|---|---|
| Laranja | 191 | 60 | 0 | #BF3C00 |
| Azul | 0 | 0 | 255 | #0000FF |
| Amarelo | 255 | 180 | 0 | #FFB400 |
| Roxo | 30 | 0 | 42 | #1E002A |
| OFF | 0 | 0 | 0 | #000000 |

**Brilho:** `FastLED.setBrightness(150)` — fixo, aplicado globalmente. [CALIBRAR] pós-montagem se percepção em ambiente pedagógico for insuficiente.

**Ajuste de Laranja e Roxo (2026-07-04):** valores originais (`#FF5000`, `#9400D3`) validados em bancada como não suficientemente distinguíveis (CA-03-02: Laranja/Amarelo confundidos por adulto; CA-03-03: Azul/Roxo). Ambas as cores escurecidas por escala proporcional em direção ao preto (mesmo matiz, luminância reduzida): Laranja a 75% do valor original (`191,60,0`); Roxo a 20% do valor original (`30,0,42`), após duas iterações intermediárias (80%, 35%) ainda avaliadas como claras demais. Azul e Amarelo mantidos no valor original — validados como suficientes após o ajuste dos outros dois. Validação: teste visual direto com adulto, boot animation e LED estático em sessão; recomenda-se reconfirmar com criança do público-alvo antes do fechamento definitivo do gate v1.0.0 (ambos observadores relevantes por RNF-05).

---

## 7. Animação de Boot <a id="boot-animation"></a>

Executada ao ligar, antes de qualquer conexão. Derivada de [VER: 00_conceito.md#estado-boot].

| Passo | LED | Cor | Duração |
|---|---|---|---|
| 1 | LED[0] | Laranja | 150ms |
| 2 | LED[1] | Azul | 150ms |
| 3 | LED[2] | Amarelo | 150ms |
| 4 | LED[0] | Roxo | 150ms |
| 5 | Todos | OFF | 500ms |
| 6–25 | Repetir passos 1–5 percorrendo cores | — | ~1500ms adicionais |

Sequência total ~3s. Após conclusão: todos OFF, sistema aguarda conexão do pedagogo.

---

## 8. Animação de Celebração (Fim de Sessão) <a id="animacao-celebracao"></a>

Executada ao atingir N interações. Derivada de [VER: 00_conceito.md#feedback-fim-sessao].

- 5 rotações completas nas 4 cores pelos 3 LEDs simultaneamente
- Duração total: ~3s
- Sempre executada, independente do score (não expõe resultado à criança)
- Após conclusão: todos OFF, sistema retorna à tela de resultados do pedagogo

---

## 9. Decoupling por LED <a id="decoupling-led"></a>

Derivado de [VER: 05_alimentacao.md#decoupling]. Por cada WS2812B:
- 10μF / 16V eletrolítico entre VDD e GND, próximo ao LED
- 100nF / 50V cerâmico entre VDD e GND, próximo ao LED

Especificação no BOM: [VER: 08_bom.md#passivos]

---

## 10. Critérios de Aceitação <a id="criterios-aceitacao"></a>

| # | Teste | Condição de aprovação |
|---|---|---|
| CA-03-01 | Boot animation | 3 LEDs percorrem 4 cores sem falha; duração ~3s |
| CA-03-02 | Cor Laranja | Comparar visualmente com amostra de cor — não confundir com Amarelo |
| CA-03-03 | Cor Azul | Distinguível de Roxo em condição de luz ambiente pedagógica |
| CA-03-04 | LED correto por modo | Modo 1: apenas LED[1] acende; Modo 2: apenas LED[0] e LED[2] |
| CA-03-05 | Latência LED | Ligação e apagamento < 10ms após comando (parte do RNF-01) |
| CA-03-06 | Celebração fim sessão | 5 rotações completas ~3s, todos apagam ao final |
| CA-03-07 | Temperatura LEDs | Sem aquecimento anormal após 60 min de operação contínua |

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-26 | — | Criação — reescrita do zero com âncoras, _PADRAO v0.1.0, derivada de 00_conceito v0.1.0, 01_arquitetura v0.1.0, 05_alimentacao v0.1.0 | 04, 08, 09 |
| 0.1.1 | 2026-06-30 | depende_de | Atualiza referência 05_alimentacao.md de v0.1.0 para v0.2.0 (nota AMS1117 onboard) | — |
| 0.1.2 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referências: 01_arquitetura.md v0.1.0→v0.2.0 (bump MINOR retroativo), 05_alimentacao.md v0.2.0→v0.2.1 | — |
| 0.1.3 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referências: 01_arquitetura.md v0.2.0→v0.2.1 (especifica DevKitC V4), 05_alimentacao.md v0.2.1→v0.2.2 (VIN→pino 5V) | — |
| 0.1.4 | 2026-07-03 | #alimentacao-led, depende_de, Rastreabilidade | Corrige origem do rail 3.3V: saída direta do LM2596, não mais AMS1117 (05_alimentacao v0.3.0 — AMS1117 fora do caminho de potência); VDD e nível de dado inalterados | 08, 09 (somente depende_de) |
| 0.1.5 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do conceito v0.2.0 (exportação CSV+PDF com pré-visualização, M2/M3 validados): atualiza referências — 00_conceito.md v0.1.0→v0.2.0, 01_arquitetura.md v0.2.1→v0.3.0, 05_alimentacao.md v0.3.0→v0.3.1 | — |
| 0.1.6 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do registro do manual do pedagogo (12_manual_pedagogo.md no impacta de 00 e 07): atualiza referências — 00_conceito.md v0.2.0→v0.2.1, 01_arquitetura.md v0.3.0→v0.3.1, 05_alimentacao.md v0.3.1→v0.3.2 | — |
| 0.1.7 | 2026-07-04 | depende_de, Rastreabilidade | Cascata mecânica do conceito v0.3.0 (melhoria M1 — encerramento antecipado de sessão, sem impacto em saída visual): atualiza referências — 00_conceito.md v0.2.1→v0.3.0, 01_arquitetura.md v0.3.1→v0.4.0, 05_alimentacao.md v0.3.2→v0.3.3 | 08, 09 |
| 0.1.8 | 2026-07-04 | #cores-rgb | Corrige valores de Laranja e Roxo após validação física de bancada apontar confusão perceptual (CA-03-02, CA-03-03, antes PENDENTE): Laranja `#FF5000`→`#BF3C00` (191,60,0 — 75% do valor original), Roxo `#9400D3`→`#1E002A` (30,0,42 — 20% do valor original), ambos por escala proporcional em direção ao preto, mesmo matiz. Azul e Amarelo mantidos inalterados. Testado direto no firmware (`visual_config.h`) antes desta cascata, por decisão do usuário | 08, 09 |
| 0.1.9 | 2026-07-13 | depende_de, Rastreabilidade | Cascata mecânica do conceito v0.4.0 (correção de honestidade das referências científicas — Wong 2011 mantida como referência da paleta, sem impacto em saída visual): atualiza referências — 00_conceito.md v0.3.0→v0.4.0, 01_arquitetura.md v0.4.0→v0.4.1, 05_alimentacao.md v0.3.3→v0.3.4 | 08, 09 |
| 0.1.10 | 2026-07-16 | depende_de, Rastreabilidade | Cascata mecânica do conceito v0.5.0 (§6.3 martelos — bater com a mão diretamente na zona também é detectado, martelo é opcional — sem impacto em saída visual: qual LED acende independe do objeto usado para bater): atualiza referências — 00_conceito.md v0.4.0→v0.5.0, 01_arquitetura.md v0.4.1→v0.4.2, 05_alimentacao.md v0.3.4→v0.3.5 | 08, 09 |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 00_conceito.md | 0.5.0 | BLOQUEADOR | #indicadores-led, #modos-operacao, #zonas-impacto, #feedback, #estado-boot |
| Pai | 01_arquitetura.md | 0.4.2 | BLOQUEADOR | #mod-led, #mapeamento-gpios, #interface-jogo-led, #stack-tecnologico, #requisitos-nao-funcionais |
| Pai | 05_alimentacao.md | 0.3.5 | BLOQUEADOR | #restricao-led, #decoupling |
| Filho | 08_bom.md | — | OBRIGATÓRIO | #componente-led, #decoupling-led |
| Filho | 09_conexoes.md | — | OBRIGATÓRIO | #mapeamento-led, #decoupling-led |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
