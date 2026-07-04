# VALIDATION — Checklist formal de validação (ETAPA 8)

Registro PASSOU/FALHOU de cada Critério de Aceitação (CA), exigido pelo gate
da ETAPA 8 (CLAUDE.md): **nenhuma tag v1.0.0 enquanto houver CA sem registro.**

Como usar:
- Executar o teste exatamente como descrito na seção `#criterios-aceitacao`
  do documento fonte de cada grupo.
- Preencher **Resultado** (`PASSOU` / `FALHOU`) e **Evidência** (data, instrumento,
  valor medido ou observação).
- `FALHOU` → corrigir em branch `fix/<ca-id>`, commitar, re-testar, atualizar aqui.
- Status `PENDENTE` = teste ainda não executado ou executado sem registro formal.
  Evidência parcial anotada não substitui a execução formal.

Estado em 2026-07-03: **7 PASSOU · 1 OBSOLETO · 57 PENDENTE** (65 CAs) + 8 cenários UI.

---

## CA-01 — Sistema ([VER: 01_arquitetura.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-01-01 | Boot completo: LEDs varrem 3×4 cores; hotspot < 5s | PENDENTE | Evidência parcial: boot + animação + AP BMI observados em bancada (2026-07-03, SESSION_STATE) — repetir com medição do tempo |
| CA-01-02 | Browser conecta e carrega interface < 3s | PENDENTE | Evidência parcial: interface carregada em bancada (2026-07-03) — repetir com medição do tempo |
| CA-01-03 | Latência impacto→LED < 50ms (osciloscópio, 10 amostras) | PENDENTE | — |
| CA-01-04 | Latência evento→tela < 200ms (inspeção visual) | PENDENTE | — |
| CA-01-05 | Sessão completa Modo 1 (N=5) | PENDENTE | — |
| CA-01-06 | Sessão completa Modo 2 (N=5) | PENDENTE | — |
| CA-01-07 | Desconexão e retomada | PENDENTE | — |
| CA-01-08 | Exportação: prévia + CSV + PDF corretos | PENDENTE | Evidência forte: CA-07-09/12/13 PASSOU em bancada (2026-07-03) — registrar execução formal deste CA de sistema |

## CA-02 — Sensor de impacto ([VER: 02_sensor_impacto.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-02-01 | 50/50 batidas detectadas | PENDENTE | — |
| CA-02-02 | 0 falsos positivos em 5 min | PENDENTE | — |
| CA-02-03 | Bater em zona X não gera evento em Y | PENDENTE | — |
| CA-02-04 | GPIO sobrevive 100 impactos fortes | PENDENTE | — |
| CA-02-05 | Uma batida → exatamente 1 evento (debounce) | PENDENTE | Lógica coberta em `pio test -e native` (test_sensor 13/13, 2026-06-28); execução física pendente |
| CA-02-06 | Latência detecção→evento < 10ms | PENDENTE | — |

## CA-03 — Saída visual ([VER: 03_saida_visual.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-03-01 | Boot animation: 3 LEDs × 4 cores, ~3s | PENDENTE | Evidência parcial: animação de boot observada em bancada (2026-07-03); lógica coberta em native (test_visual 14/14) — repetir com verificação da duração |
| CA-03-02 | Laranja distinguível de Amarelo (amostra de cor) | PENDENTE | — |
| CA-03-03 | Azul distinguível de Roxo em luz ambiente | PENDENTE | — |
| CA-03-04 | LED correto por modo | PENDENTE | Lógica coberta em native (test_visual); execução física pendente |
| CA-03-05 | Latência LED < 10ms | PENDENTE | — |
| CA-03-06 | Celebração: 5 rotações ~3s, todos apagam | PENDENTE | Lógica coberta em native (test_visual); execução física pendente |
| CA-03-07 | Sem aquecimento anormal após 60 min | PENDENTE | — |

## CA-04 — Lógica de jogo ([VER: 04_logica_jogo.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-04-01 | Mecanismo A: 40 interações → cada cor 10× ± 0 | PASSOU | `pio test -e native` (test_game, 38/38) — 2026-06-28; teste é de natureza lógica, executável em native por definição |
| CA-04-02 | Mecanismo B: nenhuma cor > 3× consecutivas em 40 | PASSOU | Idem CA-04-01 |
| CA-04-03 | Modo 2: nenhum par com corE == corD em 20 pares | PASSOU | Idem CA-04-01 |
| CA-04-04 | Acerto Modo 1: LED apaga, tela verde, score+1 | PASSOU | Idem CA-04-01 (lado lógico); comportamento físico observável coberto por CA-01-05 |
| CA-04-05 | Erro Modo 1: LED mantido, tela vermelha, score inalterado | PASSOU | Idem CA-04-04 |
| CA-04-06 | Acerto Modo 2 dentro da janela | PASSOU | Idem CA-04-01 |
| CA-04-07 | Erro Modo 2 fora da janela | PASSOU | Idem CA-04-01 |
| CA-04-08 | Fim de sessão: FIM_SESSAO com totais corretos | PASSOU | Idem CA-04-01 |
| CA-04-09 | Pausa/retomada: LEDs reacendem com mesma cor | PENDENTE | Implementado (commit 89fac38); teste físico com desconexão real pendente (junto com CA-01-07/CA-07-10) |
| CA-04-10 | Intervalo 2000ms ± 100ms entre interações | PENDENTE | Lógica coberta em native; medição física pendente |

## CA-05 — Alimentação ([VER: 05_alimentacao.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-05-01 | LM2596 em vazio: 3.30V ± 0.05V | PENDENTE | Evidência parcial: ajustado a 3.30V na bancada (2026-07-02); registrar medição formal + trava do trimpot |
| CA-05-02 | LM2596 sob carga: 3.3V ± 5% por 60 min | PENDENTE | — |
| CA-05-03 | Pino 3V3 sob carga: 3.3V ± 5% por 60 min | PENDENTE | — |
| CA-05-04 | Zero resets por brownout em 60 min | PENDENTE | Evidência parcial: sistema operou em bancada sem brownout após arquitetura 3.3V direta (2026-07-03); janela de 60 min não cronometrada |
| CA-05-05 | Temperatura LM2596 < 70°C após 30 min | PENDENTE | — |
| CA-05-06 | — | OBSOLETO | AMS1117 fora do caminho de potência (05 v0.3.0) — não aplicável |
| CA-05-07 | Ripple no rail 3.3V < 50mV pico a pico | PENDENTE | — |
| CA-05-08 | 10 boots consecutivos com init WiFi sem brownout (incl. 1º pós-erase) | PENDENTE | — |

## CA-07 — Interface do pedagogo ([VER: 07_interface_pedagogo.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-07-01 | Hotspot `BMI` visível < 5s após boot | PENDENTE | Evidência parcial: AP no ar em bancada (2026-07-02/03); tempo não medido |
| CA-07-02 | Interface carrega < 3s em 192.168.4.1 | PENDENTE | Evidência parcial: interface usada em bancada; tempo não medido |
| CA-07-03 | Validação do formulário (nome vazio bloqueado) | PENDENTE | — |
| CA-07-04 | Campo janela visível somente no Modo 2 | PENDENTE | — |
| CA-07-05 | Feedback acerto: verde + som < 200ms, some em 1500ms | PENDENTE | — |
| CA-07-06 | Feedback erro: vermelho + som < 200ms, mantido | PENDENTE | — |
| CA-07-07 | Tela de resultados completa no FIM_SESSAO | PENDENTE | — |
| CA-07-08 | Registro no localStorage ao confirmar Nova Sessão | PENDENTE | — |
| CA-07-09 | Exportação CSV via prévia (RFC 4180, UTF-8 BOM) | PASSOU | Validação física de bancada, browser da matriz RNF-05 — 2026-07-03 (SESSION_STATE; commit 60d3bb0). Pré-validação: 44/44 checks em Node |
| CA-07-10 | Desconexão pausa; reconexão retoma | PENDENTE | — |
| CA-07-11 | Offline total em todas as etapas | PENDENTE | — |
| CA-07-12 | Pré-visualização e confirmação (incl. vazio) | PASSOU | Validação física de bancada — 2026-07-03 (SESSION_STATE; commit 60d3bb0) |
| CA-07-13 | Exportação PDF (abre, paginação, acentos) | PASSOU | Validação física de bancada — 2026-07-03 (SESSION_STATE; commit 60d3bb0); PDF de amostra conferido visualmente |

## CA-09 — Conexões ([VER: 09_conexoes.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-09-01 | Continuidade + 3.30V ± 0.05V no pino 3V3 | PENDENTE | — |
| CA-09-02 | Isolamento 12V→3.3V (sem curto no LM2596) | PENDENTE | — |
| CA-09-03 | GPIOs de piezo leem 0V em repouso | PENDENTE | — |
| CA-09-04 | Zener: GPIO não ultrapassa 3.3V com impacto forte | PENDENTE | — |
| CA-09-05 | Cadeia LED: os 3 respondem ao comando | PENDENTE | Evidência parcial: 3/3 LEDs em ciclo de cores na bancada (2026-07-02) — registrar execução formal |
| CA-09-06 | Resistência ∞ entre sinais ADC | PENDENTE | — |
| CA-09-07 | Serigrafia verificada borne a borne; bornes proibidos marcados | PENDENTE | Pendência física registrada: marcar borne falso-GND (CMD) e auditar todos os rótulos (SESSION_STATE) |

## CA-10 — Cablagem ([VER: 10_cablagem.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-10-01 | Continuidade: 26 fios < 1Ω | PENDENTE | — |
| CA-10-02 | Fios de piezo identificados por cor de zona | PENDENTE | — |
| CA-10-03 | Strain relief nos cabos de piezo | PENDENTE | — |
| CA-10-04 | Comprimentos respeitados | PENDENTE | — |
| CA-10-05 | Heat shrink: sem cobre exposto | PENDENTE | — |
| CA-10-06 | Junções da cadeia LED soldadas/travadas (clipe proibido) | PENDENTE | Motivação registrada: contato marginal dos clipes comprovado em 2026-07-02 |

## Cenários de UI ([VER: WEB_STANDARD.md#criterios-aceitacao])

Complementares aos CA-07; mesma disciplina de registro.

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-07-UI-01 | Tokens MD3 aplicados, sem hex fora de `:root` | PENDENTE | — |
| CA-07-UI-02 | Layout responsivo em 375px e 768px | PENDENTE | — |
| CA-07-UI-03 | Zero magic literals no script | PENDENTE | — |
| CA-07-UI-04 | Estados mutuamente exclusivos | PENDENTE | — |
| CA-07-UI-05 | Overlay de feedback cobre 100% da viewport | PENDENTE | — |
| CA-07-UI-06 | Transição automática do overlay de ACERTO | PENDENTE | — |
| CA-07-UI-07 | Persistência de sessão no localStorage | PENDENTE | — |
| CA-07-UI-08 | Reconexão transparente sem recarregar | PENDENTE | — |

---

## Histórico

| Data | Mudança |
|---|---|
| 2026-07-03 | Criação do checklist. Status inicial derivado exclusivamente de evidência registrada (SESSION_STATE.md, TODO.md, commits): CA-07-09/12/13 PASSOU (bancada 2026-07-03); CA-04-01..08 PASSOU (native 38/38 — testes de natureza lógica); CA-05-06 OBSOLETO; demais PENDENTE |
