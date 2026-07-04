---
documento:    02_sensor_impacto.md
versão:       0.2.3
status:       APROVADO
data:         2026-06-26
depende_de:
  - _PADRAO.md v0.1.0        [BLOQUEADOR]
  - 00_conceito.md v0.3.0    [BLOQUEADOR]
  - 01_arquitetura.md v0.4.0 [BLOQUEADOR]
  - 05_alimentacao.md v0.3.3 [BLOQUEADOR]
impacta:
  - 08_bom.md                [OBRIGATÓRIO]
  - 09_conexoes.md           [OBRIGATÓRIO]
---

# 02_SENSOR_IMPACTO.md — Sensor de Impacto Piezoelétrico

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | 02_sensor_impacto.md |
| Versão | 0.2.3 |
| Status | APROVADO |
| Módulo firmware | MOD_SENSOR — [VER: 01_arquitetura.md#mod-sensor] |
| GPIOs | 34, 35, 32, 33 — [VER: 01_arquitetura.md#mapeamento-gpios] |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Especificar o componente sensor, o circuito de proteção obrigatório, o algoritmo de detecção e debounce, e os critérios de aceitação do MOD_SENSOR. Qualquer engenheiro deve poder implementar e validar este módulo exclusivamente a partir deste documento e de seus pais declarados.

---

## 3. Componente <a id="componente-piezo"></a>

| Campo | Valor |
|---|---|
| Tipo | Disco piezoelétrico passivo |
| Diâmetro | 27mm |
| Terminais | 2 fios: positivo (eletrodo superior) e negativo (eletrodo inferior/GND) |
| Encapsulamento | Sem PCB — disco cerâmico com fios soldados diretamente |
| Tensão gerada no impacto | 5V a 50V de pico (dependente da força e montagem) |
| Corrente gerada | Desprezível — sensor passivo |

**Por que disco simples:** leitura analógica via ADC permite threshold configurável por software — [VER: 01_arquitetura.md#mod-sensor]. Módulos com PCB adicionam comparador com saída digital fixa, impedindo calibração.

---

## 4. Circuito de Proteção <a id="circuito-protecao"></a>

O piezo pode gerar 5V–50V. O GPIO ADC1 do ESP32 suporta no máximo **3.3V** — [VER: 05_alimentacao.md#restricao-sensor]. Sem proteção, o primeiro impacto forte destrói o pino.

### 4.1 Esquema (repetir para as 4 zonas) <a id="esquema-protecao"></a>

```
Piezo(+) ──── R1 (1MΩ) ──┬──── GPIO_ADC1
                           │
                         [K] D1 (Zener 3.3V) [A]
                           │
Piezo(−) ─────────────────┴──── GND

K = cátodo → lado GPIO    A = ânodo → lado GND
Zener em polarização reversa: clipa tensão acima de 3.3V para GND
```

### 4.2 Componentes <a id="componentes-protecao"></a>

| Componente | Valor | Função |
|---|---|---|
| R1 | 1MΩ, 1/4W | Limita corrente + forma divisor com impedância de entrada do ADC |
| D1 | Zener **3.3V** — BZX55C3V3 ou 1N5226B | Grampeador de tensão — clipa picos acima de 3.3V |

**Atenção:** usar exclusivamente Zener de **3.3V**. Não usar 1N4728A (3.9V — não protege adequadamente).

### 4.3 Funcionamento <a id="funcionamento-protecao"></a>

```
Impacto leve:   piezo ~2V  → R1 limita → ADC lê ~1.5V → Zener não conduz
Impacto forte:  piezo ~30V → R1 limita → Zener conduz → ADC vê 3.3V máx
Repouso:        piezo 0V   → ADC lê 0V → sem evento
```

---

## 5. Mapeamento de GPIOs por Zona <a id="mapeamento-gpios-sensor"></a>

Derivado de [VER: 01_arquitetura.md#mapeamento-gpios] — reproduzido para referência do firmware.

| Zona | Cor | GPIO | Canal ADC |
|---|---|---|---|
| Zona 1 | Laranja | GPIO 34 | ADC1_CH6 |
| Zona 2 | Azul | GPIO 35 | ADC1_CH7 |
| Zona 3 | Amarelo | GPIO 32 | ADC1_CH4 |
| Zona 4 | Roxo | GPIO 33 | ADC1_CH5 |

**ADC2 proibido** — incompatível com WiFi ativo. [VER: 01_arquitetura.md#requisitos-nao-funcionais]

---

## 6. Algoritmo de Detecção e Debounce <a id="algoritmo-deteccao"></a>

### 6.1 Lógica <a id="logica-deteccao"></a>

```cpp
// A cada ciclo de loop(), para cada GPIO:
uint16_t leitura = analogRead(GPIO);
if (leitura > THRESHOLD) {
  uint32_t agora = millis();
  if ((agora - ultimo_impacto[zona]) > DEBOUNCE_MS) {
    ultimo_impacto[zona] = agora;
    onImpacto({ zona, agora });   // interface: [VER: 01_arquitetura.md#interface-sensor-jogo]
  }
}
```

### 6.2 Parâmetros <a id="parametros-deteccao"></a>

| Parâmetro | Valor padrão | Tipo | Descrição |
|---|---|---|---|
| `THRESHOLD` | 500 | int (0–4095) | ADC mínimo para impacto válido — 500/4095 × 3.3V ≈ 0.40V. [CALIBRAR] |
| `DEBOUNCE_MS` | 200 | int (ms) | Janela de bloqueio após detecção — cobre ringing mecânico do disco |

**Sobre THRESHOLD:** verificar empiricamente após montagem — batida leve deve ultrapassar 500; vibração de zona adjacente não deve ultrapassar 500. A calibração DEVE ser feita com a topologia de alimentação final energizada: o rail 3.3V é saída direta do LM2596 (sem o PSRR do AMS1117), e o piso de ruído do ADC muda com o ripple residual — [VER: 05_alimentacao.md#restricao-sensor].

**Sobre DEBOUNCE_MS:** 200ms é conservador para uso infantil — cobre ringing mecânico e impede dupla contagem. Cada GPIO tem timer independente: Modo 2 não é afetado (zonas distintas).

---

## 7. Montagem Física <a id="montagem-fisica"></a>

### 7.1 Fixação do disco <a id="fixacao-disco"></a>
- Epóxi bicomponente na face inferior da zona de madeira, centro geométrico
- Pressão leve durante cura (não deformar o disco cerâmico)
- Aguardar 24h de cura completa antes de testar

### 7.2 Isolamento entre zonas <a id="isolamento-zonas"></a>
- Gap mínimo entre zonas: **5mm** de ar ou borracha/cortiça
- Derivado de [VER: 00_conceito.md#zonas-impacto]
- Validar: bater em zona X → ADC das zonas adjacentes permanece abaixo de THRESHOLD

### 7.3 Fios <a id="fios-sensor"></a>
- Heat shrink na saída de cada fio do disco
- Strain relief no ponto de entrada no cabo
- Comprimento máximo piezo → perfboard: **50cm** — acima disso, capacitância parasita degrada sinal ADC
- Especificação completa: [VER: 10_cablagem.md#tabela-fios]

---

## 8. Critérios de Aceitação <a id="criterios-aceitacao"></a>

| # | Teste | Condição de aprovação | Rastreável a |
|---|---|---|---|
| CA-02-01 | Detecção de impacto | 50/50 batidas detectadas corretamente | [VER: 01_arquitetura.md#criterios-aceitacao] |
| CA-02-02 | Ausência de falso positivo | 0 eventos em 5 min sem toque | [VER: 01_arquitetura.md#requisitos-nao-funcionais] RNF-03 |
| CA-02-03 | Isolamento entre zonas | Bater em zona X não gera evento em zona Y | [VER: 00_conceito.md#zonas-impacto] |
| CA-02-04 | Proteção do GPIO | GPIO sobrevive 100 impactos fortes sem dano | [VER: 05_alimentacao.md#restricao-sensor] |
| CA-02-05 | Debounce | Uma batida → exatamente 1 evento | [VER: #parametros-deteccao] |
| CA-02-06 | Latência detecção → evento | < 10ms (budget parcial de RNF-01) | [VER: 01_arquitetura.md#requisitos-nao-funcionais] RNF-01 |

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-26 | — | Criação — reescrita do zero com âncoras, _PADRAO v0.1.0, derivada de 00_conceito v0.1.0, 01_arquitetura v0.1.0, 05_alimentacao v0.1.0 | 04, 08, 09 |
| 0.1.1 | 2026-06-30 | depende_de | Atualiza referência 05_alimentacao.md de v0.1.0 para v0.2.0 (nota AMS1117 onboard) | — |
| 0.1.2 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referências: 01_arquitetura.md v0.1.0→v0.2.0 (bump MINOR retroativo), 05_alimentacao.md v0.2.0→v0.2.1 | — |
| 0.1.3 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referências: 01_arquitetura.md v0.2.0→v0.2.1 (especifica DevKitC V4), 05_alimentacao.md v0.2.1→v0.2.2 (VIN→pino 5V) | — |
| 0.2.0 | 2026-07-03 | #parametros (nota THRESHOLD), depende_de, Rastreabilidade | Clarifica condição de calibração do THRESHOLD: obrigatória com topologia final energizada (rail 3.3V direto do LM2596, sem PSRR do AMS1117 — 05_alimentacao v0.3.0); atualiza depende_de 05 v0.2.2→v0.3.0 | 08, 09 (somente depende_de) |
| 0.2.1 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do conceito v0.2.0 (exportação CSV+PDF com pré-visualização, M2/M3 validados): atualiza referências — 00_conceito.md v0.1.0→v0.2.0, 01_arquitetura.md v0.2.1→v0.3.0, 05_alimentacao.md v0.3.0→v0.3.1 | — |
| 0.2.2 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do registro do manual do pedagogo (12_manual_pedagogo.md no impacta de 00 e 07): atualiza referências — 00_conceito.md v0.2.0→v0.2.1, 01_arquitetura.md v0.3.0→v0.3.1, 05_alimentacao.md v0.3.1→v0.3.2 | — |
| 0.2.3 | 2026-07-04 | depende_de, Rastreabilidade | Cascata mecânica do conceito v0.3.0 (melhoria M1 — encerramento antecipado de sessão, sem impacto em sensor): atualiza referências — 00_conceito.md v0.2.1→v0.3.0, 01_arquitetura.md v0.3.1→v0.4.0, 05_alimentacao.md v0.3.2→v0.3.3 | 08, 09 |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 00_conceito.md | 0.3.0 | BLOQUEADOR | #zonas-impacto |
| Pai | 01_arquitetura.md | 0.4.0 | BLOQUEADOR | #mod-sensor, #mapeamento-gpios, #interface-sensor-jogo, #requisitos-nao-funcionais |
| Pai | 05_alimentacao.md | 0.3.3 | BLOQUEADOR | #restricao-sensor |
| Filho | 08_bom.md | — | OBRIGATÓRIO | #componente-piezo, #componentes-protecao |
| Filho | 09_conexoes.md | — | OBRIGATÓRIO | #esquema-protecao, #mapeamento-gpios-sensor, #fios-sensor |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
