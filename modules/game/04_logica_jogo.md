---
documento:    04_logica_jogo.md
versão:       0.1.4
status:       APROVADO
data:         2026-06-26
depende_de:
  - _PADRAO.md v0.1.0        [BLOQUEADOR]
  - 00_conceito.md v0.2.1    [BLOQUEADOR]
  - 01_arquitetura.md v0.3.1 [BLOQUEADOR]
impacta:
  - 07_interface_pedagogo.md [CONDICIONAL: #gestao-score]
---

# 04_LOGICA_JOGO.md — Lógica de Jogo (MOD_JOGO)

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | 04_logica_jogo.md |
| Versão | 0.1.1 |
| Status | APROVADO |
| Módulo firmware | MOD_JOGO — [VER: 01_arquitetura.md#mod-jogo] |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Especificar a implementação de MOD_JOGO: máquina de estados, algoritmos de aleatoriedade, lógica de acerto/erro por modo, gestão de score e timings. As interfaces com os outros módulos são definidas em [VER: 01_arquitetura.md#interfaces-modulos] e **não se repetem aqui** — este documento as referencia e descreve apenas o comportamento interno de MOD_JOGO ao usá-las.

---

## 3. Máquina de Estados — Implementação <a id="estados-jogo"></a>

A máquina de estados do sistema está definida em [VER: 01_arquitetura.md#diagrama-estados]. MOD_JOGO é responsável pelos estados: `SESSAO_ATIVA`, `ESTIMULO`, `AVALIANDO`, `INTERVALO`, `FIM_SESSAO` e `PAUSADO`. Os estados `BOOT`, `AGUARDANDO_CONEXAO` e `CONFIGURANDO` são responsabilidade do sistema de inicialização e de MOD_WIFI.

```cpp
enum class Estado {
  SESSAO_ATIVA,
  ESTIMULO,     // LEDs acesos, aguardando impacto
  AVALIANDO,    // impacto recebido, processando resultado
  INTERVALO,    // acerto registrado, aguardando próximo ciclo
  FIM_SESSAO,   // N interações concluídas
  PAUSADO       // desconexão detectada, estado preservado
};
```

### 3.1 Transições de responsabilidade de MOD_JOGO <a id="transicoes"></a>

| De | Para | Gatilho |
|---|---|---|
| `SESSAO_ATIVA` | `ESTIMULO` | `iniciarSessao()` recebido — sortear cor(es) e acender LED(s) |
| `ESTIMULO` | `AVALIANDO` | `onImpacto()` recebido |
| `AVALIANDO` | `ESTIMULO` | resultado = ERRO — LEDs mantidos, aguardar nova resposta |
| `AVALIANDO` | `INTERVALO` | resultado = ACERTO — apagar LEDs, aguardar `INTERVALO_MS` |
| `INTERVALO` | `ESTIMULO` | timer expirou, `acertos < n_interacoes` — sortear próxima cor |
| `INTERVALO` | `FIM_SESSAO` | timer expirou, `acertos == n_interacoes` — celebração |
| `SESSAO_ATIVA` (qualquer) | `PAUSADO` | notificação de desconexão de MOD_WIFI |
| `PAUSADO` | estado anterior | notificação de reconexão de MOD_WIFI |

---

## 4. Aleatoriedade <a id="aleatoriedade"></a>

Implementação dos dois mecanismos definidos em [VER: 00_conceito.md#aleatoriedade].

### 4.1 Mecanismo A — Shuffle por bloco <a id="mecanismo-a"></a>

Garante distribuição uniforme: cada cor aparece exatamente uma vez a cada bloco de 4 interações.

```cpp
// Estado interno (persiste durante a sessão)
Cor bloco_A[4] = {LARANJA, AZUL, AMARELO, ROXO};
uint8_t idx_A = 4;  // inicializar em 4 força shuffle na primeira chamada

Cor proximaCor_A() {
  if (idx_A >= 4) {
    // Fisher-Yates shuffle
    for (int i = 3; i > 0; i--) {
      int j = random(i + 1);
      swap(bloco_A[i], bloco_A[j]);
    }
    idx_A = 0;
  }
  return bloco_A[idx_A++];
}

// Modo 2: retorna dois valores sempre distintos
// Como o bloco tem 4 cores sem repetição, posições consecutivas são sempre distintas
pair<Cor,Cor> proximasDuas_A() {
  Cor c1 = proximaCor_A();
  Cor c2 = proximaCor_A();
  return {c1, c2};
}
```

### 4.2 Mecanismo B — Peso decrescente <a id="mecanismo-b"></a>

Reduz a previsibilidade ao diminuir a probabilidade de cores recentes. Indicado para público estendido — [VER: 00_conceito.md#publico-estendido].

```cpp
// Estado interno (persiste durante a sessão)
// Índice: 0=LARANJA, 1=AZUL, 2=AMARELO, 3=ROXO
uint8_t peso_B[4] = {4, 4, 4, 4};

Cor proximaCor_B() {
  uint8_t total = peso_B[0] + peso_B[1] + peso_B[2] + peso_B[3];
  if (total == 0) {
    // Reset: todos os pesos a 4
    for (auto& p : peso_B) p = 4;
    total = 16;
  }
  uint8_t r = random(total);
  uint8_t acum = 0;
  for (int i = 0; i < 4; i++) {
    acum += peso_B[i];
    if (r < acum) {
      if (peso_B[i] > 0) peso_B[i]--;
      return (Cor)i;
    }
  }
  return LARANJA;  // fallback nunca atingido
}

// Modo 2: garantir duas cores distintas
pair<Cor,Cor> proximasDuas_B() {
  Cor c1 = proximaCor_B();
  // Temporariamente zerar o peso de c1 para garantir c2 diferente
  uint8_t peso_salvo = peso_B[(int)c1];
  peso_B[(int)c1] = 0;
  Cor c2 = proximaCor_B();
  peso_B[(int)c1] = peso_salvo;  // restaurar
  return {c1, c2};
}
```

---

## 5. Lógica por Modo <a id="logica-por-modo"></a>

### 5.1 Modo 1 — Um Martelo <a id="logica-modo-1"></a>

Derivado de [VER: 00_conceito.md#modo-1-martelo] e [VER: 00_conceito.md#regras-sistema].

```
ESTIMULO:
  cor = proximaCor()                         // Mecanismo A ou B
  setLED({ LED::CENTRAL, cor })              // [VER: 01_arquitetura.md#interface-jogo-led]

AVALIANDO (ao receber onImpacto(evt)):
  se evt.zona == zonaParaCor(cor):
    → ACERTO
  senão:
    → ERRO (LED mantido)
```

### 5.2 Modo 2 — Dois Martelos <a id="logica-modo-2"></a>

Derivado de [VER: 00_conceito.md#modo-2-martelos] e [VER: 00_conceito.md#timings].

```
ESTIMULO:
  {corE, corD} = proximasDuas()              // cores sempre distintas
  setLED({ LED::ESQUERDO, corE })
  setLED({ LED::DIREITO,  corD })

AVALIANDO — estado interno adicional para Modo 2:
  primeiroImpacto: bool = false
  zonaCorreta1:    Zona                      // zona da corE ou corD que foi acertada primeiro
  t0:              uint32_t                  // timestamp do primeiro impacto correto

Ao receber onImpacto(evt):

  Se !primeiroImpacto:
    Se evt.zona == zonaParaCor(corE) OU evt.zona == zonaParaCor(corD):
      primeiroImpacto = true
      zonaCorreta1 = evt.zona
      t0 = evt.timestamp_ms
      // LEDs permanecem acesos — aguardar segundo impacto
    Senão:
      → ERRO (zona errada no primeiro impacto)

  Se primeiroImpacto:
    zonaEsperada = (zonaCorreta1 == zonaParaCor(corE)) ? zonaParaCor(corD) : zonaParaCor(corE)
    Se evt.zona == zonaEsperada E (evt.timestamp_ms - t0) <= JANELA_MS:
      → ACERTO
    Senão:
      → ERRO (zona errada OU fora da janela)
      primeiroImpacto = false    // reset para nova tentativa
```

Regras derivadas de [VER: 00_conceito.md#definicao-erro]:
- Acertar uma zona e errar a outra → ERRO
- Acertar ambas fora da janela → ERRO
- Hitting mesma zona duas vezes → ERRO (não é a `zonaEsperada`)

---

## 6. Tratamento de Acerto e Erro <a id="tratamento-resultados"></a>

Derivado de [VER: 00_conceito.md#definicao-acerto], [VER: 00_conceito.md#tratamento-erro] e [VER: 00_conceito.md#feedback].

```
ACERTO:
  acertos++
  setLED({ LED::TODOS, OFF })                // apagar LEDs imediatamente
  onResultado({ ACERTO, acertos, n_interacoes, duracao() })  // → MOD_WIFI → tela verde
  iniciar timer INTERVALO_MS → transição para INTERVALO

ERRO:
  // LEDs NÃO apagam — estímulo mantido
  onResultado({ ERRO, acertos, n_interacoes, duracao() })    // → MOD_WIFI → tela vermelha
  // permanecer em AVALIANDO aguardando nova resposta
```

---

## 7. Gestão de Score e Sessão <a id="gestao-score"></a>

Derivado de [VER: 00_conceito.md#contagem-score] e [VER: 00_conceito.md#armazenamento].

Estado em memória durante a sessão (MOD_JOGO é stateless entre sessões):

```cpp
struct EstadoSessao {
  String nome_crianca;
  uint32_t timestamp_inicio_ms;
  ConfigSessao::Modo modo;
  ConfigSessao::Mecanismo mecanismo;
  uint8_t n_configurado;
  uint8_t acertos;
  uint8_t erros;           // erros intermediários (dentro de interações)
  uint32_t duracao_ms;     // calculado ao fim da sessão
};
```

Score exibido na interface: `acertos / n_configurado` e taxa percentual.

**ESP32 não persiste nenhum dado** — ao fim da sessão, `EventoResultado{ FIM_SESSAO, acertos, n_configurado, duracao_ms }` é enviado a MOD_WIFI, que repassa ao browser. Persistência é responsabilidade do localStorage do pedagogo — [VER: 00_conceito.md#armazenamento].

---

## 8. Timings e Constantes <a id="timings"></a>

Derivado de [VER: 00_conceito.md#timings] e [VER: 00_conceito.md#intervalo-interacoes].

| Constante | Valor padrão | Tipo | Descrição |
|---|---|---|---|
| `INTERVALO_MS` | 2000 | uint16_t (ms) | Pausa automática após acerto antes do próximo estímulo. [CALIBRAR] |
| `JANELA_MS` | 800 | uint16_t (ms) | Janela de simultaneidade Modo 2 — configurável via interface. [CALIBRAR] |

`JANELA_MS` é recebido em `ConfigSessao.janela_ms` — [VER: 01_arquitetura.md#interface-jogo-wifi]. `INTERVALO_MS` é fixo no firmware (não configurável na v1.0).

---

## 9. Mapeamento Zona → Cor <a id="mapeamento-zona-cor"></a>

Função auxiliar usada internamente por MOD_JOGO para validar impactos:

```cpp
// Derivado de [VER: 01_arquitetura.md#mapeamento-gpios] e [VER: 03_saida_visual.md#cores-rgb]
Zona zonaParaCor(Cor cor) {
  switch (cor) {
    case Cor::LARANJA:  return Zona::LARANJA;
    case Cor::AZUL:     return Zona::AZUL;
    case Cor::AMARELO:  return Zona::AMARELO;
    case Cor::ROXO:     return Zona::ROXO;
  }
}
```

O mapeamento é 1:1 por design — cada cor identifica unicamente uma zona física.

---

## 10. Critérios de Aceitação <a id="criterios-aceitacao"></a>

| # | Teste | Condição de aprovação |
|---|---|---|
| CA-04-01 | Mecanismo A — distribuição | Em 40 interações: cada cor aparece 10 vezes ± 0 (shuffle exato por bloco) |
| CA-04-02 | Mecanismo B — variação | Em 40 interações: nenhuma cor repete mais de 3× consecutivas |
| CA-04-03 | Modo 2 — cores distintas | Em 20 pares sorteados: nenhum par com corE == corD |
| CA-04-04 | Acerto Modo 1 | Bater na zona correta: LED apaga, tela verde, score+1 |
| CA-04-05 | Erro Modo 1 | Bater em zona errada: LED mantido, tela vermelha, score inalterado |
| CA-04-06 | Acerto Modo 2 dentro da janela | Duas zonas corretas < 800ms: acerto registrado |
| CA-04-07 | Erro Modo 2 fora da janela | Segunda zona correta > 800ms: erro registrado |
| CA-04-08 | Fim de sessão | N=5 acertos: `FIM_SESSAO` emitido com totais corretos |
| CA-04-09 | Pausa/retomada | Desconexão durante ESTIMULO: estado preservado; reconexão: LEDs reacendem com mesma cor |
| CA-04-10 | Intervalo entre interações | Medição: 2000ms ± 100ms entre apagamento do LED e próximo acendimento |

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-26 | — | Criação — derivada de 00_conceito v0.1.0 e 01_arquitetura v0.1.0 com âncoras e _PADRAO v0.1.0 | 07_interface_pedagogo.md |
| 0.1.1 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referência 01_arquitetura.md v0.1.0→v0.2.0 (bump MINOR retroativo) | 07_interface_pedagogo.md |
| 0.1.2 | 2026-07-01 | depende_de, Rastreabilidade | Atualiza referência 01_arquitetura.md v0.2.0→v0.2.1 (especifica DevKitC V4) | 07_interface_pedagogo.md |
| 0.1.3 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do conceito v0.2.0 (exportação CSV+PDF com pré-visualização, M2/M3 validados): atualiza referências — 00_conceito.md v0.1.0→v0.2.0, 01_arquitetura.md v0.2.1→v0.3.0 | — |
| 0.1.4 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do registro do manual do pedagogo (12_manual_pedagogo.md no impacta de 00 e 07): atualiza referências — 00_conceito.md v0.2.0→v0.2.1, 01_arquitetura.md v0.3.0→v0.3.1 | — |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 00_conceito.md | 0.2.1 | BLOQUEADOR | #glossario, #modos-operacao, #fluxo-interacao, #regras-sistema, #feedback, #aleatoriedade, #timings, #intervalo-interacoes, #contagem-score, #armazenamento |
| Pai | 01_arquitetura.md | 0.3.1 | BLOQUEADOR | #mod-jogo, #interfaces-modulos, #diagrama-estados, #requisitos-nao-funcionais |
| Filho | 07_interface_pedagogo.md | — | CONDICIONAL: #gestao-score | #gestao-score |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
