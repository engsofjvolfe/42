---
documento:    11_montagem.md
versão:       0.3.3
status:       APROVADO
data:         2026-06-26
depende_de:
  - _PADRAO.md v0.1.0        [BLOQUEADOR]
  - 00_conceito.md v0.3.0    [BLOQUEADOR]
  - 08_bom.md v0.3.3         [BLOQUEADOR]
  - 09_conexoes.md v0.3.3    [BLOQUEADOR]
  - 10_cablagem.md v0.2.3    [BLOQUEADOR]
impacta: []
---

# 11_MONTAGEM.md — Instruções de Montagem e Teste

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | 11_montagem.md |
| Versão | 0.3.3 |
| Status | APROVADO |
| Escopo | Montagem física completa — uma unidade do instrumento |
| Posição na cadeia | Documento final — não tem filhos |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Especificar a sequência completa de montagem: dimensões de corte (define S01 do BOM), ferramentas necessárias, ordem das fases, procedimentos passo a passo e testes de aceitação por fase. Este documento não repete especificações dos pais — referencia-as. Conteúdo único aqui: **sequência**, **método**, **dimensões**, **ferramentas** e **calibração**.

---

## 3. Dimensões e Cortes MDF — Define S01 <a id="dimensoes-mdf"></a>

Define o item S01 de [VER: 08_bom.md#materiais-estruturais]. Madeira: MDF 6mm.

### 3.1 Peças das zonas de impacto <a id="pecas-zonas"></a>

4 peças idênticas — uma por zona (Laranja, Azul, Amarelo, Roxo):

| Peça | Dimensão | Qtd | Obs |
|---|---|---|---|
| Painel de zona | 12×12cm | 4 | Superfície de impacto — tamanho mínimo para uso infantil |

### 3.2 Estrutura de base <a id="estrutura-base"></a>

| Peça | Dimensão | Qtd | Obs |
|---|---|---|---|
| Base principal | 60×25cm | 1 | Suporte das 4 zonas + gap + margens |
| Divisórias de gap | 0.5×12cm | 3 | Separadores entre zonas — substituíveis por M13 |
| Painel LED | 25×8cm | 1 | Suporte dos 3 LEDs — posicionado à frente e acima das zonas |
| Fundo eletrônico | 20×15cm | 1 | Base do compartimento eletrônico |

### 3.3 Plano de corte <a id="plano-corte"></a>

```
Folha MDF 6mm — 61×61cm mínimo (1 folha suficiente):

┌─────────────────────────────────────────────────┐ 61cm
│  Zona×4 (12×12)    │  Base (60×25)             │
│  [12][12][12][12]  │                            │
│                    │                            │
│  Painel LED        │  Fundo eletr.              │
│  (25×8)            │  (20×15)                  │
│                    │                            │
│  Divisórias (3×0.5×12)                         │
└─────────────────────────────────────────────────┘
```

**Quantidade total S01:** 1 folha MDF 6mm 61×61cm ou equivalente.

---

## 4. Ferramentas Necessárias <a id="ferramentas"></a>

| Ferramenta | Uso |
|---|---|
| Ferro de solda (25–40W) + estanho | Soldagem de toda a eletrônica |
| Multímetro | Verificação de tensão, continuidade e isolamento |
| Serra ou estilete de corte MDF | Corte das peças estruturais |
| Lixa 120 e 220 | Acabamento superficial das zonas |
| Pincel fino + pincel largo | Pintura das zonas e verniz |
| Pistola de cola quente (opcional) | Fixação provisória durante montagem |
| Alicate de corte | Corte de fios e terminais |
| Decapador de fio | Desencapamento de fios |
| Régua e caneta marcadora | Marcação de cortes e posicionamento |
| Cronômetro | Verificação do intervalo de 2000ms |
| Computador com PlatformIO | Flash do firmware — [VER: 01_arquitetura.md#stack-tecnologico] |

---

## 5. Sequência de Fases <a id="sequencia-fases"></a>

A ordem abaixo é obrigatória. Duas razões principais:

1. **Epóxi** (Fase 3) precisa de 24h de cura — iniciar cedo
2. **Eletrônica deve ser testada antes de encapsular** na estrutura — Fases 1 e 2 completas e aprovadas antes da Fase 6

```
Fase 1 → Fase 2 → Fase 3 ──(24h)──► Fase 4 → Fase 5 → Fase 6 → Fase 7 → Fase 8
Estrutura    Eletrônica   Piezos              Cablagem   Firmware  Calibração  Aceitação
```

**Dia 1:** Fases 1, 2, 3 (iniciar epóxi ao final do dia)
**Dia 2:** Fases 4, 5, 6, 7, 8

---

## 6. Fase 1 — Estrutura e Acabamento <a id="fase-estrutura"></a>

**Objetivo:** peças MDF cortadas, pintadas e envernizadas antes de receber eletrônica.

### 6.1 Corte e lixamento <a id="corte-lixamento"></a>

1. Cortar todas as peças conforme [VER: #dimensoes-mdf]
2. Lixar com lixa 120 → depois lixa 220 em todas as superfícies
3. Remover pó com pano úmido — deixar secar completamente

### 6.2 Pintura das zonas <a id="pintura-zonas"></a>

Cores: [VER: 08_bom.md#materiais-estruturais] S03 — Laranja, Azul, Amarelo, Roxo.

1. Identificar cada painel de zona (marcar no verso antes de pintar)
2. Aplicar 2 demãos de tinta látex na cor correspondente
3. Aguardar 1h entre demãos
4. Verificar cobertura uniforme — zona deve ser facilmente distinguível
5. Após última demão seca: aplicar 1 demão de verniz PU (S04) — proteção ao impacto infantil
6. Aguardar cura completa do verniz antes de prosseguir (≥ 2h)

### 6.3 Preparação dos gaps <a id="preparacao-gaps"></a>

Cortar tiras de M13 (borracha/cortiça ≥ 5mm) com dimensão 12×1cm — 3 tiras.
Verificar espessura ≥ 5mm conforme [VER: 02_sensor_impacto.md#isolamento-zonas].

**Critério de aprovação desta fase:**
- Todas as 4 zonas pintadas e identificadas no verso
- Cores distinguíveis entre si em condição de luz ambiente
- Verniz seco sem marcas de pressão

---

## 7. Fase 2 — Eletrônica de Alimentação <a id="fase-alimentacao"></a>

**Objetivo:** LM2596 ajustado e testado a 5V antes de conectar qualquer outro componente.

### 7.1 Montagem do LM2596 <a id="montagem-lm2596"></a>

1. **Não conectar ESP32 ainda**
2. Conferir capacitores conforme [VER: 09_conexoes.md#cadeia-alimentacao-ascii]:
   - P03 (100μF/25V) na entrada 12V e P04 (470μF/10V) na saída — já soldados no
     módulo pré-montado; apenas conferir presença
   - P06 (100nF/50V) na saída 3.3V — soldar se ausente
3. Soldar F01 e F02 (soldado fixo) da fonte ao LM2596 IN+ e IN−
4. Conectar fonte 12V

### 7.2 Ajuste de tensão <a id="ajuste-tensao"></a>

1. Medir tensão na saída do LM2596 com multímetro — **saída desconectada de
   qualquer borne** (em vazio)
2. Girar potenciômetro do módulo até medir **3.30V ± 0.05V**
3. Travar/marcar o trimpot (esmalte) após o ajuste
4. Desconectar fonte antes de continuar

**PROIBIDO conectar a saída ao pino 3V3 acima de 3.6V** — máximo absoluto do
ESP32; excedê-lo destrói o chip sem aviso.

**Critério CA-05-01** — [VER: 05_alimentacao.md#criterios-aceitacao]:
Saída LM2596 em vazio: 3.30V ± 0.05V. **Não avançar sem aprovação.**

---

## 8. Fase 3 — Perfboard de Proteção Piezo <a id="fase-perfboard"></a>

**Objetivo:** 4 circuitos de proteção montados e testados antes de conectar GPIOs.

### 8.1 Montagem dos circuitos <a id="montagem-circuitos"></a>

Soldar 4 instâncias idênticas no perfboard 5×7cm conforme [VER: 09_conexoes.md#circuito-protecao-perfboard]:

```
Por zona — repetir ×4:
  Piezo(+) ── R1 1MΩ ──┬── Pino saída (→ GPIO)
                        │
                      Zener 3.3V (K→saída, A→GND)
  Piezo(−) ───────────── GND comum
```

Componentes: P01 (4× R1 1MΩ) e E05 (4× Zener BZX55C3V3 / 1N5226B) de [VER: 08_bom.md#eletronicos-ativos].

Atenção à polaridade do Zener: **cátodo (K) no nó de sinal, ânodo (A) no GND.**

### 8.2 Teste da proteção Zener <a id="teste-zener"></a>

Com multímetro em modo DC:
1. Aplicar 5V no nó Piezo(+): verificar que a saída não ultrapassa 3.3V
2. Verificar isolamento entre os 4 nós de sinal: resistência ∞ entre GPIO34, GPIO35, GPIO32, GPIO33

**Critérios CA-09-04 e CA-09-06** — [VER: 09_conexoes.md#criterios-aceitacao]. **Não avançar sem aprovação.**

---

## 9. Fase 4 — Sensores Piezoelétricos <a id="fase-piezos"></a>

**Objetivo:** piezos fixados com epóxi e 24h de cura. Iniciar no final do Dia 1.

### 9.1 Fixação dos discos <a id="fixacao-piezos"></a>

Conforme [VER: 02_sensor_impacto.md#fixacao-disco]:

1. Identificar centro geométrico da face inferior de cada painel de zona
2. Misturar epóxi bicomponente (M12) conforme instruções do fabricante
3. Aplicar fina camada de epóxi no centro da face inferior do painel
4. Posicionar disco piezo (E04) sobre o epóxi, face eletrodo voltada para baixo (fios saindo pela lateral)
5. Aplicar pressão leve e uniforme — **não deformar o disco cerâmico**
6. Manter pressão por 5 min ou até o epóxi não escorregar mais
7. Deixar curar **24h sem perturbação**

### 9.2 Soldagem dos fios nos piezos <a id="soldagem-piezos"></a>

Após cura completa, conforme [VER: 10_cablagem.md#tabela-fios] e [VER: 10_cablagem.md#regras-montagem]:

1. Cortar fios F17, F19, F21, F23 e F25 nos comprimentos especificados (máx 50cm cada)
2. Encaixar heat shrink (M11 2mm) nos fios **antes de soldar**
3. Soldar fio colorido (cor da zona) no terminal (+) do piezo
4. Soldar fio preto no terminal (−) do piezo → todos os negativos formam F25 (GND comum)
5. Deslizar heat shrink sobre a junta e contrair com calor
6. Fixar strain relief no ponto onde o cabo sai do painel (cola quente ou presilha)
7. Marcar cada fio com fita adesiva colorida na ponta livre

**Critério CA-10-02** — [VER: 10_cablagem.md#criterios-aceitacao]: identificação por cor confirmada.

---

## 10. Fase 5 — Conjunto ESP32 + Shield <a id="fase-esp32"></a>

**Objetivo:** ESP32 DevKitC V4 instalado no shield, capacitores do pino 3V3 soldados, pronto para conectar periféricos.

### 10.1 Instalação do DevKit no shield <a id="instalacao-devkit"></a>

> **Nota:** o rail 3.3V é alimentado DIRETO pelo LM2596 no pino **3V3** —
> o AMS1117-3.3 onboard do DevKit está fora do caminho de potência de operação
> ([VER: 05_alimentacao.md#cadeia-alimentacao]) e atua somente na gravação via
> USB. [VER: 09_conexoes.md#cadeia-alimentacao-ascii]

1. **Executar a verificação de serigrafia do shield ANTES de qualquer conexão**
   — [VER: 09_conexoes.md#verificacao-serigrafia]: conferir cada rótulo contra
   a serigrafia do DevKitC, testar continuidade borne↔pino, marcar como
   PROIBIDOS os bornes da flash (CLK/CMD/SD0–SD3) e o falso-GND (CA-09-07)
2. Encaixar ESP32-WROOM-32U (E01) no shield de expansão (E02) — verificar alinhamento dos 38 pinos
3. Soldar capacitor P07 (1000μF/≥10V) entre o pino 3V3 e GND do shield
4. Soldar capacitor P06 (100nF/50V) entre o pino 3V3 e GND do shield, próximo ao P07

### 10.2 Conexão do barramento de alimentação <a id="conexao-barramento"></a>

Conforme [VER: 09_conexoes.md#mapeamento-shield] e fios [VER: 10_cablagem.md#tabela-fios]:

1. Conferir o ajuste do LM2596: 3.30V ± 0.05V em vazio ([VER: #ajuste-tensao]) — re-medir se o trimpot puder ter sido tocado
2. Conectar F03 (LM2596 OUT+ → barramento 3.3V+) e F04 (LM2596 OUT− → barramento 3.3V−)
3. Conectar F05 (barramento 3.3V+ → shield pino 3V3) e F06 (barramento 3.3V− → shield GND)
4. Conectar fonte 12V e medir 3.30V no pino 3V3 do shield

**Critério CA-09-01** — [VER: 09_conexoes.md#criterios-aceitacao]: 3.30V ± 0.05V no pino 3V3. **Não avançar sem aprovação.**

---

## 11. Fase 6 — LEDs WS2812B <a id="fase-leds"></a>

**Objetivo:** 3 LEDs montados em cadeia com decoupling, posicionados no painel LED.

### 11.1 Montagem dos LEDs <a id="montagem-leds"></a>

Conforme [VER: 09_conexoes.md#cadeia-leds] e [VER: 03_saida_visual.md#mapeamento-led]:

Posição: LED[0]=Esquerdo, LED[1]=Central, LED[2]=Direito — fixados ao painel LED (S01 25×8cm).

1. Para cada LED (×3), soldar próximo ao VDD:
   - P05 (10μF/16V) entre VDD e GND
   - P06 (100nF/50V) entre VDD e GND
2. Cortar e conectar fios de alimentação (F07–F12) conforme [VER: 10_cablagem.md#tabela-fios]
3. Soldar resistor P02 (300Ω) em série na linha de dados
4. Conectar cadeia de dados: R300Ω saída → F14 → LED[0] DIN → F15 → LED[1] DIN → F16 → LED[2] DIN
5. Conectar F13 (Shield GPIO5 → R300Ω entrada)

Respeitar restrições de comprimento: F13 ≤ 15cm; F14–F16 ≤ 10cm — [VER: 10_cablagem.md#restricoes-comprimento].

### 11.2 Teste dos LEDs <a id="teste-leds"></a>

Com firmware básico de teste (acender cada LED individualmente):
1. LED[0] Esquerdo: verificar posição física e cor programada
2. LED[1] Central: idem
3. LED[2] Direito: idem

**Critério CA-09-05** — [VER: 09_conexoes.md#criterios-aceitacao]: todos os 3 LEDs respondem ao comando.

---

## 12. Fase 7 — Cablagem dos Sensores <a id="fase-cablagem-sensores"></a>

**Objetivo:** piezos conectados ao perfboard; perfboard conectado ao shield.

### 12.1 Conexão piezo → perfboard <a id="conexao-piezo-perfboard"></a>

1. Conectar fio colorido de cada zona ao nó Piezo(+) da instância correspondente no perfboard:
   - F17 (Laranja) → Zona 1
   - F19 (Azul) → Zona 2
   - F21 (Amarelo) → Zona 3
   - F23 (Roxo) → Zona 4
2. Conectar F25 (todos os Piezo(−)) ao GND comum do perfboard

### 12.2 Conexão perfboard → shield <a id="conexao-perfboard-shield"></a>

Conforme [VER: 09_conexoes.md#mapeamento-shield]:

1. F18 (saída Zona 1) → Shield GPIO34
2. F20 (saída Zona 2) → Shield GPIO35
3. F22 (saída Zona 3) → Shield GPIO32
4. F24 (saída Zona 4) → Shield GPIO33
5. F26 (GND perfboard) → Shield GND

### 12.3 Verificação de comprimentos <a id="verificacao-comprimentos"></a>

**Critério CA-10-04** — [VER: 10_cablagem.md#criterios-aceitacao]:
F13 ≤ 15cm; F14–F16 ≤ 10cm; F17, F19, F21, F23 ≤ 50cm.

**Critério CA-10-01**: continuidade de todos os 26 fios (multímetro, resistência < 1Ω).

---

## 13. Fase 8 — Firmware <a id="fase-firmware"></a>

**Objetivo:** firmware compilado, gravado e boot animation confirmado.

### 13.1 Compilação e flash <a id="compilacao-flash"></a>

Ferramentas: PlatformIO conforme [VER: 01_arquitetura.md#stack-tecnologico].

1. Conectar ESP32 ao computador via USB
2. Compilar projeto: `pio run`
3. Gravar firmware: `pio run --target upload`
4. Abrir monitor serial: `pio device monitor`
5. Verificar mensagens de inicialização sem erro

### 13.2 Verificação de boot <a id="verificacao-boot"></a>

Ao inicializar sem USB conectado:
1. LEDs devem executar animação de boot conforme [VER: 03_saida_visual.md#boot-animation]:
   - Varredura nas 4 cores pelos 3 LEDs em sequência
   - Duração ~3s
   - Todos apagam ao final
2. Hotspot `BMI` deve aparecer na lista WiFi em < 5s

**Critério CA-01-01** — [VER: 01_arquitetura.md#criterios-aceitacao].

---

## 14. Fase 9 — Calibração <a id="fase-calibracao"></a>

**Objetivo:** ajustar THRESHOLD para o ambiente físico real de montagem.

### 14.1 Procedimento de calibração do THRESHOLD <a id="calibracao-threshold"></a>

O valor padrão `THRESHOLD = 500` é ponto de partida — [VER: 02_sensor_impacto.md#parametros-deteccao].

1. Conectar monitor serial (`pio device monitor`)
2. Bater com martelo em cada zona com força típica de uma criança de 5 anos
3. Registrar o valor ADC de pico em cada batida
4. Bater nas bordas e cantos (batida não centrada): registrar valor ADC
5. Deixar instrumento em repouso 2 min sem tocar: registrar valor máximo de ruído

**Critério de calibração:**

```
Valor mínimo de batida leve:   > THRESHOLD
Valor máximo de ruído/repouso: < THRESHOLD
Margem mínima:                   50 unidades ADC entre ruído e batida
```

Ajustar THRESHOLD até satisfazer os três critérios. Registrar valor final escolhido.

### 14.2 Calibração do isolamento entre zonas <a id="calibracao-isolamento"></a>

1. Bater com força total em zona X
2. Verificar no monitor serial: zona Y não gera evento
3. Repetir para todos os pares adjacentes
4. Se zona vizinha disparar: verificar gap físico ≥ 5mm — [VER: 02_sensor_impacto.md#isolamento-zonas]

---

## 15. Fase 10 — Montagem Estrutural Final <a id="fase-montagem-final"></a>

**Objetivo:** apenas após aprovação eletrônica completa, encapsular na estrutura.

### 15.1 Montagem da base <a id="montagem-base"></a>

1. Fixar 4 painéis de zona na base com cola ou parafusos
2. Inserir tiras de gap M13 (borracha/cortiça ≥ 5mm) entre cada zona
3. Fixar painel LED à frente e acima das zonas — separação espacial obrigatória — [VER: 00_conceito.md#indicadores-led]
4. Montar compartimento eletrônico no fundo da base

### 15.2 Organização dos cabos <a id="organizacao-cabos"></a>

1. Agrupar fios por função (alimentação, sinal, LED)
2. Prender com abraçadeiras — manter folga para não tracionar terminais
3. Verificar que nenhum fio cruza borda cortante de MDF

### 15.3 Martelos <a id="martelos"></a>

Conforme [VER: 00_conceito.md#martelos]:
1. Lixar superfícies e arestas — sem pontas, sem farpas
2. Arredondar todas as arestas (segurança infantil)
3. Pintura opcional com tinta não-tóxica
4. Verificar peso e tamanho: adequados para criança de 5 anos

---

## 16. Critérios de Aceitação Final <a id="criterios-aceitacao-final"></a>

Aprovação final exige todos os CAs abaixo verificados nesta ordem:

### 16.1 CAs de alimentação <a id="ca-alimentacao"></a>

| CA | Descrição | Fonte |
|---|---|---|
| CA-05-01 | Saída LM2596 em vazio: 3.30V ± 0.05V | [VER: 05_alimentacao.md#criterios-aceitacao] |
| CA-05-02 | Saída LM2596 sob carga: 3.3V ± 5% em 60min | [VER: 05_alimentacao.md#criterios-aceitacao] |
| CA-05-03 | Pino 3V3 sob carga: 3.3V ± 5% em 60min | [VER: 05_alimentacao.md#criterios-aceitacao] |
| CA-05-05 | Temperatura LM2596 < 70°C após 30min | [VER: 05_alimentacao.md#criterios-aceitacao] |
| CA-05-08 | 10 boots consecutivos com init WiFi sem brownout | [VER: 05_alimentacao.md#criterios-aceitacao] |

### 16.2 CAs de conexões e cablagem <a id="ca-conexoes"></a>

| CA | Descrição | Fonte |
|---|---|---|
| CA-09-01 | 3.30V ± 0.05V no pino 3V3 | [VER: 09_conexoes.md#criterios-aceitacao] |
| CA-09-04 | GPIO não ultrapassa 3.3V com impacto forte | [VER: 09_conexoes.md#criterios-aceitacao] |
| CA-09-06 | Resistência ∞ entre sinais ADC | [VER: 09_conexoes.md#criterios-aceitacao] |
| CA-09-07 | Serigrafia do shield verificada; bornes proibidos marcados | [VER: 09_conexoes.md#criterios-aceitacao] |
| CA-10-01 | Continuidade: todos os 26 fios < 1Ω | [VER: 10_cablagem.md#criterios-aceitacao] |
| CA-10-04 | Comprimentos respeitados | [VER: 10_cablagem.md#criterios-aceitacao] |
| CA-10-05 | Heat shrink: sem cobre exposto | [VER: 10_cablagem.md#criterios-aceitacao] |
| CA-10-06 | Junções da cadeia LED soldadas/travadas (sem clipe de pressão) | [VER: 10_cablagem.md#criterios-aceitacao] |

### 16.3 CAs de sensores <a id="ca-sensores"></a>

| CA | Descrição | Fonte |
|---|---|---|
| CA-02-01 | 50/50 batidas detectadas | [VER: 02_sensor_impacto.md#criterios-aceitacao] |
| CA-02-02 | 0 falsos positivos em 5min | [VER: 02_sensor_impacto.md#criterios-aceitacao] |
| CA-02-03 | Batida em X não gera evento em Y | [VER: 02_sensor_impacto.md#criterios-aceitacao] |

### 16.4 CAs visuais e de sistema <a id="ca-sistema"></a>

| CA | Descrição | Fonte |
|---|---|---|
| CA-03-01 | Boot animation: 3 LEDs × 4 cores ~3s | [VER: 03_saida_visual.md#criterios-aceitacao] |
| CA-03-04 | LED correto por modo | [VER: 03_saida_visual.md#criterios-aceitacao] |
| CA-01-01 | Boot completo em < 5s, hotspot visível | [VER: 01_arquitetura.md#criterios-aceitacao] |
| CA-01-02 | Interface pedagogo carrega em < 3s | [VER: 01_arquitetura.md#criterios-aceitacao] |
| CA-01-05 | Sessão completa Modo 1 (N=5) | [VER: 01_arquitetura.md#criterios-aceitacao] |
| CA-01-06 | Sessão completa Modo 2 (N=5) | [VER: 01_arquitetura.md#criterios-aceitacao] |
| CA-01-07 | Desconexão e retomada | [VER: 01_arquitetura.md#criterios-aceitacao] |
| CA-01-08 | Exportação CSV correta | [VER: 01_arquitetura.md#criterios-aceitacao] |

### 16.5 Inspeção de segurança infantil <a id="ca-seguranca"></a>

| Item | Condição de aprovação |
|---|---|
| Arestas dos painéis MDF | Todas arredondadas ou chanfradas — sem ponto de corte |
| Martelos | Sem farpas, sem pontas, sem partes soltas |
| Fios | Nenhum exposto fora do compartimento eletrônico |
| Tampa do compartimento eletrônico | Requer ferramenta para abrir — criança não acessa |
| Fonte AC/DC | Cabo fixado com strain relief — criança não alcança conexão |

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-26 | — | Criação — documento final da cadeia. Define S01 (MDF 61×61cm). Derivado de 00_conceito v0.1.0, 08_bom v0.1.0, 09_conexoes v0.1.0, 10_cablagem v0.1.0. | — |
| 0.2.0 | 2026-06-30 | #instalacao-devkit | Adiciona nota: AMS1117-3.3 integrado ao DevKit (E01), não há etapa de instalação para regulação 3.3V; atualiza depende_de (08 v0.2.0, 09 v0.2.0, 10 v0.1.1) | — |
| 0.2.1 | 2026-07-01 | depende_de | Atualiza referência 10_cablagem.md de v0.1.1 para v0.1.2 (cascata da correção Mermaid em 09_conexoes) | — |
| 0.2.2 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referências: 08_bom.md v0.2.0→v0.2.1, 09_conexoes.md v0.2.1→v0.2.2, 10_cablagem.md v0.1.2→v0.1.3 (cascata bump MINOR retroativo de 01_arquitetura) | — |
| 0.2.3 | 2026-07-01 | #instalacao-devkit, #energizacao-inicial | Substitui "VIN" por "pino 5V" / "DevKitC V4" em instruções de soldagem e conexão; atualiza depende_de: 08 v0.2.2, 09 v0.2.3, 10 v0.1.4 | — |
| 0.3.0 | 2026-07-03 | #montagem-lm2596, #ajuste-tensao, #fase-esp32, #criterios-aceitacao-final, depende_de | Cascata 05/08/09/10 (arquitetura 3.3V direta): ajuste do LM2596 para 3.30V ± 0.05V com trava de trimpot e proibição >3.6V; verificação de serigrafia como passo 1 da instalação; P07 (1000μF) no pino 3V3; barramento 3.3V; checklist final com CA-05-08, CA-09-07, CA-10-06; atualiza depende_de 08 v0.3.0, 09 v0.3.0, 10 v0.2.0 | — |
| 0.3.1 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do conceito v0.2.0 (exportação CSV+PDF com pré-visualização, M2/M3 validados): atualiza referências — 00_conceito.md v0.1.0→v0.2.0, 08_bom.md v0.3.0→v0.3.1, 09_conexoes.md v0.3.0→v0.3.1, 10_cablagem.md v0.2.0→v0.2.1 | — |
| 0.3.2 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do registro do manual do pedagogo (12_manual_pedagogo.md no impacta de 00 e 07): atualiza referências — 00_conceito.md v0.2.0→v0.2.1, 08_bom.md v0.3.1→v0.3.2, 09_conexoes.md v0.3.1→v0.3.2, 10_cablagem.md v0.2.1→v0.2.2 | — |
| 0.3.3 | 2026-07-04 | depende_de, Rastreabilidade | Cascata mecânica do conceito v0.3.0 (melhoria M1 — encerramento antecipado de sessão, sem impacto em montagem física): atualiza referências — 00_conceito.md v0.2.1→v0.3.0, 08_bom.md v0.3.2→v0.3.3, 09_conexoes.md v0.3.2→v0.3.3, 10_cablagem.md v0.2.2→v0.2.3 | — |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 00_conceito.md | 0.3.0 | BLOQUEADOR | #componentes-fisicos, #martelos, #indicadores-led |
| Pai | 08_bom.md | 0.3.3 | BLOQUEADOR | todo o documento |
| Pai | 09_conexoes.md | 0.3.3 | BLOQUEADOR | todo o documento |
| Pai | 10_cablagem.md | 0.2.3 | BLOQUEADOR | #tabela-fios, #regras-montagem, #restricoes-comprimento |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
