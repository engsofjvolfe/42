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

Estado em 2026-07-04: **51 PASSOU · 1 OBSOLETO · 13 PENDENTE** (65 CAs);
cenários UI: **2 PASSOU · 6 PENDENTE**. Defeito **D2 baixado** (2026-07-04,
validado na bancada). Defeito **D3 aberto** (TODO.md — pares de cor sem
variedade no Modo 2) — bloqueia o gate mesmo com CAs aprovados.

---

## CA-01 — Sistema ([VER: 01_arquitetura.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-01-01 | Boot completo: LEDs varrem 3×4 cores; hotspot < 5s | PASSOU | 
| CA-01-02 | Browser conecta e carrega interface < 3s | PASSOU | 
| CA-01-03 | Latência impacto→LED < 50ms (osciloscópio, 10 amostras) | PENDENTE | Sem osciloscópio disponível. Decisão do usuário (2026-07-04): re-especificar o método do CA em `01_arquitetura.md` (com cascata) para verificação sem osciloscópio e só então executar e registrar |
| CA-01-04 | Latência evento→tela < 200ms (inspeção visual) | PASSOU | — |
| CA-01-05 | Sessão completa Modo 1 (N=5) | PASSOU | Nota (2026-07-03): sessão conduzida com o workaround PAUSAR/RETOMAR (D2, TODO.md). D2 baixado em 2026-07-04 — workaround não é mais necessário; re-execução formal N=5 sem workaround ainda não registrada |
| CA-01-06 | Sessão completa Modo 2 (N=5) | PASSOU | Nota (2026-07-03): mesmo workaround do CA-01-05. D2 baixado em 2026-07-04. Ressalva nova: D3 (TODO.md) aberto — pares de cor do Modo 2 sem variedade suficiente; não invalida este CA (critério é sessão completar, não variedade de pares — isso é CA-04-01/02/03), mas relevante para nova rodada de bancada |
| CA-01-07 | Desconexão e retomada | PENDENTE | — |
| CA-01-08 | Exportação: prévia + CSV + PDF corretos | PASSOU | 

## CA-02 — Sensor de impacto ([VER: 02_sensor_impacto.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-02-01 | 50/50 batidas detectadas | PENDENTE | — |
| CA-02-02 | 0 falsos positivos em 5 min | PENDENTE | — |
| CA-02-03 | Bater em zona X não gera evento em Y | PASSOU | — |
| CA-02-04 | GPIO sobrevive 100 impactos fortes | PENDENTE | — |
| CA-02-05 | Uma batida → exatamente 1 evento (debounce) | PASSOU | Lógica coberta em `pio test -e native` (test_sensor 13/13, 2026-06-28)|
| CA-02-06 | Latência detecção→evento < 10ms | PASSOU | — |

## CA-03 — Saída visual ([VER: 03_saida_visual.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-03-01 | Boot animation: 3 LEDs × 4 cores, ~3s | PASSOU |
| CA-03-02 | Laranja distinguível de Amarelo (amostra de cor) | PENDENTE | melhorar a diferenciação utilizando as cores #F5D252 e #E37C5F — proposta de mudança de paleta: aplicar via atualização de `03_saida_visual.md` (+ spec/visual, com cascata), nunca direto no código |
| CA-03-03 | Azul distinguível de Roxo em luz ambiente | PENDENTE | melhorar a diferenciação utilizando as cores #79D0F2 e #8F78BF — mesma via da linha acima |
| CA-03-04 | LED correto por modo | PASSOU |
| CA-03-05 | Latência LED < 10ms | PASSOU | — |
| CA-03-06 | Celebração: 5 rotações ~3s, todos apagam | PASSOU | 
| CA-03-07 | Sem aquecimento anormal após 60 min | PASSOU | — |

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
| CA-05-01 | LM2596 em vazio: 3.30V ± 0.05V | PASSOU |
| CA-05-02 | LM2596 sob carga: 3.3V ± 5% por 60 min | PASSOU | — |
| CA-05-03 | Pino 3V3 sob carga: 3.3V ± 5% por 60 min | PASSOU | — |
| CA-05-04 | Zero resets por brownout em 60 min | PASSOU | Evidência parcial: sistema operou em bancada sem brownout após arquitetura 3.3V direta (2026-07-03); janela de 60 min não cronometrada |
| CA-05-05 | Temperatura LM2596 < 70°C após 30 min | PASSOU | — |
| CA-05-06 | — | OBSOLETO | AMS1117 fora do caminho de potência (05 v0.3.0) — não aplicável |
| CA-05-07 | Ripple no rail 3.3V < 50mV pico a pico | PASSOU  | — |
| CA-05-08 | 10 boots consecutivos com init WiFi sem brownout (incl. 1º pós-erase) | PASSOU | — |

## CA-07 — Interface do pedagogo ([VER: 07_interface_pedagogo.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-07-01 | Hotspot `BMI` visível < 5s após boot | PASSOU |
| CA-07-02 | Interface carrega < 3s em 192.168.4.1 | PASSOU |
| CA-07-03 | Validação do formulário (nome vazio bloqueado) | PASSOU | — |
| CA-07-04 | Campo janela visível somente no Modo 2 | PENDENTE | — |
| CA-07-05 | Feedback acerto: verde + som < 200ms, some em 1500ms | PASSOU | — |
| CA-07-06 | Feedback erro: vermelho + som < 200ms, mantido | PASSOU | — |
| CA-07-07 | Tela de resultados completa no FIM_SESSAO | PASSOU | — |
| CA-07-08 | Registro no localStorage ao confirmar Nova Sessão | PENDENTE | — |
| CA-07-09 | Exportação CSV via prévia (RFC 4180, UTF-8 BOM) | PASSOU |
| CA-07-10 | Desconexão pausa; reconexão retoma | PENDENTE | — |
| CA-07-11 | Offline total em todas as etapas | PENDENTE | — |
| CA-07-12 | Pré-visualização e confirmação (incl. vazio) | PASSOU |
| CA-07-13 | Exportação PDF (abre, paginação, acentos) | PASSOU |

## CA-09 — Conexões ([VER: 09_conexoes.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-09-01 | Continuidade + 3.30V ± 0.05V no pino 3V3 | PASSOU | — |
| CA-09-02 | Isolamento 12V→3.3V (sem curto no LM2596) | PASSOU | — |
| CA-09-03 | GPIOs de piezo leem 0V em repouso | PASSOU | — |
| CA-09-04 | Zener: GPIO não ultrapassa 3.3V com impacto forte | PASSOU | — |
| CA-09-05 | Cadeia LED: os 3 respondem ao comando | PASSOU |
| CA-09-06 | Resistência ∞ entre sinais ADC | PASSOU | — |
| CA-09-07 | Serigrafia verificada borne a borne; bornes proibidos marcados | PASSOU | 

## CA-10 — Cablagem ([VER: 10_cablagem.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-10-01 | Continuidade: 26 fios < 1Ω | PASSOU | — |
| CA-10-02 | Fios de piezo identificados por cor de zona | PASSOU | Fios vermelhos em todos os piezos (aproveitamento de material); identificação conforme o CA: fita pintada na cor da zona aplicada aos pares de fios (confirmado pelo usuário, 2026-07-04 — ressalva resolvida) |
| CA-10-03 | Strain relief nos cabos de piezo | PASSOU | — |
| CA-10-04 | Comprimentos respeitados | PASSOU | comprimentos de cabo utilizados conforme cabos disponíveis |
| CA-10-05 | Heat shrink: sem cobre exposto | PASSOU | — |
| CA-10-06 | Junções da cadeia LED soldadas/travadas (clipe proibido) | PASSOU |

## Cenários de UI ([VER: WEB_STANDARD.md#criterios-aceitacao])

Complementares aos CA-07; mesma disciplina de registro.

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-07-UI-01 | Tokens MD3 aplicados, sem hex fora de `:root` | PENDENTE | — |
| CA-07-UI-02 | Layout responsivo em 375px e 768px | PENDENTE | — |
| CA-07-UI-03 | Zero magic literals no script | PASSOU | — |
| CA-07-UI-04 | Estados mutuamente exclusivos | PENDENTE | — |
| CA-07-UI-05 | Overlay de feedback cobre 100% da viewport | PENDENTE | — |
| CA-07-UI-06 | Transição automática do overlay de ACERTO | PASSOU | — |
| CA-07-UI-07 | Persistência de sessão no localStorage | PENDENTE | — |
| CA-07-UI-08 | Reconexão transparente sem recarregar | PENDENTE | — |

---

## Histórico

| Data | Mudança |
|---|---|
| 2026-07-03 | Criação do checklist. Status inicial derivado exclusivamente de evidência registrada (SESSION_STATE.md, TODO.md, commits): CA-07-09/12/13 PASSOU (bancada 2026-07-03); CA-04-01..08 PASSOU (native 38/38 — testes de natureza lógica); CA-05-06 OBSOLETO; demais PENDENTE. (Correção de contagem: a linha de estado dizia 7 PASSOU; eram 11 — 8 do CA-04 + 3 do CA-07) |
| 2026-07-04 | Rodada de validação física de bancada preenchida pelo usuário: 41 CAs promovidos a PASSOU (+2 cenários UI). Restam 12 CAs e 6 cenários UI PENDENTES. Ressalvas anotadas sem alterar vereditos: CA-01-03 (sem osciloscópio) e CA-10-02 (fio vermelho em todos os piezos); CA-03-02/03 com proposta de nova paleta (via doc, com cascata) |
| 2026-07-04 | Defeito D2 registrado (TODO.md): primeira interação pós-energização acende sempre roxo e trava na primeira batida; workaround PAUSAR/RETOMAR; notas adicionadas em CA-01-05/06. D2 bloqueia o gate v1.0.0 |
| 2026-07-04 | Decisões do usuário sobre as ressalvas: CA-01-03 re-marcado PENDENTE — método será re-especificado em `01_arquitetura.md` para verificação sem osciloscópio (51 PASSOU · 13 PENDENTE); CA-10-02 confirmado conforme — fita pintada na cor da zona aplicada aos pares de fios |
| 2026-07-04 | D2 corrigido (`fix/primeira-interacao`, branch validada na bancada com log serial timestampado — sessão completa, 3 interações ACERTO, sem auto-pausa) e baixado. Notas de D2 em CA-01-05/06 atualizadas. Defeito D3 registrado durante a mesma rodada de bancada: pares de cor do Modo 2 sem variedade (sempre Roxo+Amarelo e Laranja+Azul) — bloqueia o gate v1.0.0 |
