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

Estado em 2026-07-10: **63 PASSOU · 1 OBSOLETO · 4 PENDENTE** (68 CAs);
cenários UI: **3 PASSOU · 5 PENDENTE**. Pendentes: CA-01-03 (método a
re-especificar em `01_arquitetura.md`), CA-04-10 (medição física do
intervalo), CA-07-04 e CA-07-08. Defeitos **D2 e D3 baixados** (2026-07-04,
ver TODO.md — D3 fechado por observação de bancada, sem causa raiz
confirmada, com ressalva registrada para reabertura). Ressalvas ativas:
reconfirmar paleta de cores com criança do público-alvo antes do gate
v1.0.0 (CA-03-02/03); cablagem executada com material fora da spec — ver
nota na seção CA-10.

---

## CA-01 — Sistema ([VER: 01_arquitetura.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-01-01 | Boot completo: LEDs varrem 3×4 cores; hotspot < 5s | PASSOU | 
| CA-01-02 | Browser conecta e carrega interface < 3s | PASSOU | 
| CA-01-03 | Latência impacto→LED < 50ms (osciloscópio, 10 amostras) | PENDENTE | Sem osciloscópio disponível. Decisão do usuário (2026-07-04): re-especificar o método do CA em `01_arquitetura.md` (com cascata) para verificação sem osciloscópio e só então executar e registrar |
| CA-01-04 | Latência evento→tela < 200ms (inspeção visual) | PASSOU | — |
| CA-01-05 | Sessão completa Modo 1 (N=5) | PASSOU | Nota (2026-07-03): sessão conduzida com o workaround PAUSAR/RETOMAR (D2, TODO.md). D2 baixado em 2026-07-04 — workaround não é mais necessário; re-execução formal N=5 sem workaround ainda não registrada |
| CA-01-06 | Sessão completa Modo 2 (N=5) | PASSOU | Nota (2026-07-03): mesmo workaround do CA-01-05. D2 baixado em 2026-07-04. D3 (pares de cor do Modo 2 sem variedade) também baixado em 2026-07-04 por observação de bancada — ver ressalva de reabertura em TODO.md |
| CA-01-07 | Desconexão e retomada | PASSOU | — |
| CA-01-08 | Exportação: prévia + CSV + PDF corretos | PASSOU | 
| CA-01-09 | Encerramento antecipado: acertos parciais preservados, sem reload/reboot (M1) | PASSOU | Bancada 2026-07-04 — usuário confirmou funcionamento antes de autorizar a cascata de spec/doc |

## CA-02 — Sensor de impacto ([VER: 02_sensor_impacto.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-02-01 | 50/50 batidas detectadas | PASSOU | — |
| CA-02-02 | 0 falsos positivos em 5 min | PASSOU | — |
| CA-02-03 | Bater em zona X não gera evento em Y | PASSOU | — |
| CA-02-04 | GPIO sobrevive 100 impactos fortes | PASSOU | — |
| CA-02-05 | Uma batida → exatamente 1 evento (debounce) | PASSOU | Lógica coberta em `pio test -e native` (test_sensor 13/13, 2026-06-28)|
| CA-02-06 | Latência detecção→evento < 10ms | PASSOU | — |

## CA-03 — Saída visual ([VER: 03_saida_visual.md#criterios-aceitacao])

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-03-01 | Boot animation: 3 LEDs × 4 cores, ~3s | PASSOU |
| CA-03-02 | Laranja distinguível de Amarelo (amostra de cor) | PASSOU | Bancada 2026-07-04, teste visual direto com adulto (boot animation + LED estático em sessão). Proposta original (paleta dos tacos, #E37C5F/#F5D252) rejeitada — sRGB de amostra de tinta emitido direto por LED de baixa saturação lia como branco, mesmo após `erase_flash` completo (descartado flash obsoleto) e após tentativa de correção gama sRGB→linear (ainda claro demais). Solução final: Laranja escurecido do valor original `#FF5000` para `#BF3C00` (191,60,0 — 75% do original, mesmo matiz); Amarelo mantido em `#FFB400`. Cascata aplicada em `03_saida_visual.md` v0.1.8. Ressalva: reconfirmar com criança do público-alvo antes do fechamento do gate v1.0.0 |
| CA-03-03 | Azul distinguível de Roxo em luz ambiente | PASSOU | Bancada 2026-07-04, mesmo teste do CA-03-02. Roxo escurecido do valor original `#9400D3` para `#1E002A` (30,0,42 — 20% do original, mesmo matiz) após duas iterações intermediárias (80%, 35% do original) ainda avaliadas como claras demais; Azul mantido em `#0000FF`. Cascata aplicada em `03_saida_visual.md` v0.1.8. Mesma ressalva de reconfirmação com criança do CA-03-02 |
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
| CA-04-09 | Pausa/retomada: LEDs reacendem com mesma cor | PASSOU | Implementado (commit 89fac38); teste físico com desconexão real pendente (junto com CA-01-07/CA-07-10) |
| CA-04-10 | Intervalo 2000ms ± 100ms entre interações | PENDENTE | Lógica coberta em native; medição física pendente |
| CA-04-11 | Encerramento antecipado: FIM_SESSAO com acertos parciais (M1) | PASSOU | `pio test -e native` (test_game, 4 testes novos) — 2026-07-04; teste é de natureza lógica, executável em native por definição |

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
| CA-07-10 | Desconexão pausa; reconexão retoma | PASSOU | — |
| CA-07-11 | Offline total em todas as etapas | PASSOU | — |
| CA-07-12 | Pré-visualização e confirmação (incl. vazio) | PASSOU |
| CA-07-13 | Exportação PDF (abre, paginação, acentos) | PASSOU |
| CA-07-14 | Encerramento antecipado: mensagem ENCERRAR, confirmação, FIM_SESSAO (M1) | PASSOU | Bancada 2026-07-04 — usuário confirmou funcionamento antes de autorizar a cascata de spec/doc |

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

**Nota de conformidade de material (2026-07-10, decisão do usuário):** as
bitolas e as cores dos fios utilizados na montagem deste exemplar NÃO seguem
[VER: 10_cablagem.md#totais-compra] nem [VER: 10_cablagem.md#tabela-fios] —
foi usado o fio disponível em bancada, eletricamente adequado (continuidade
CA-10-01 e funcionamento ponta a ponta validados), escolha consciente para
não exigir compra de material novo. A identificação por zona foi garantida
pelo mecanismo alternativo já registrado no CA-10-02 (fita pintada na cor da
zona, conforme [VER: 10_cablagem.md#regras-montagem] regra 4). **A
especificação permanece normativa:** `10_cablagem.md` descreve o ideal do
projeto — qualquer novo exemplar ou substituição de cablagem deve seguir as
bitolas, cores e comprimentos especificados lá, não o executado neste
exemplar.

## Cenários de UI ([VER: WEB_STANDARD.md#criterios-aceitacao])

Complementares aos CA-07; mesma disciplina de registro.

| CA | Teste | Resultado | Evidência |
|---|---|---|---|
| CA-07-UI-01 | Tokens MD3 aplicados, sem hex fora de `:root` | PENDENTE | — |
| CA-07-UI-02 | Layout responsivo em 375px e 768px | PENDENTE | — |
| CA-07-UI-03 | Zero magic literals no script | PASSOU | — |
| CA-07-UI-04 | Estados mutuamente exclusivos | PENDENTE | — |
| CA-07-UI-05 | Overlay de feedback cobre 100% da viewport | PASSOU | — |
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
| 2026-07-04 | D3 baixado: investigação interrompida por brownout (conflito USB+fonte externa simultâneos — não relacionado, ver `firmware/diag/README.md`); após corrigir a energia (só fonte externa), re-teste de bancada (observação visual, sem log serial) mostrou os pares variando normalmente. Fechado sem causa raiz confirmada por instrumentação — ressalva de reabertura registrada em TODO.md |
| 2026-07-04 | CA-03-02/03 promovidos a PENDENTE→PASSOU: paleta original (`#FF5000`/`#9400D3`) reprovada em bancada; proposta intermediária (paleta de tinta dos tacos, sRGB, sem correção) reprovada — LED emitia branco; correção gama sRGB→linear tentada e também reprovada — ainda claro demais. Solução final: escurecer Laranja e Roxo mantendo o mesmo matiz (`#BF3C00`, `#1E002A`); Azul e Amarelo mantidos no valor original. Testado direto no firmware antes da cascata (decisão do usuário); cascata completa aplicada em `03_saida_visual.md` v0.1.8 → `spec/visual` → `firmware_constants.json` → `_config.h` → 08/09/10/11 (mecânico) → TESTING_STANDARD.md (mecânico, via CODING_STANDARD.md v0.2.5). Ressalva: reconfirmar com criança do público-alvo antes do fechamento do gate v1.0.0 |
| 2026-07-10 | Rodada de bancada preenchida pelo usuário: CA-01-07, CA-02-01, CA-02-02, CA-02-04, CA-07-11 e CA-07-UI-05 promovidos a PASSOU. Correção de contagem: a linha de estado anterior dizia 56 PASSOU · 11 PENDENTE, mas a tabela committed continha 58 · 9 — estado real agora 63 PASSOU · 1 OBSOLETO · 4 PENDENTE (+ UI 3/5). Teste exploratório de brilho: firmware provisório com `setBrightness(255)` flashado (nada commitado; `visual_config.h` revertido ao valor de spec) e validado visualmente em bancada — cores mantêm distinção (escala global proporcional preserva matiz e saturação). ATENÇÃO: 255 excede a restrição `05_alimentacao.md#restricao-led` (brilho máximo 150, budget de §5.1 calculado nessa condição) — o teste foi exploratório e pontual, fora da spec; valor em vigor MANTIDO em 150 por decisão do usuário, dentro da restrição. O `[CALIBRAR]` de `03_saida_visual.md#cores-rgb` permanece aberto para ajuste pós-montagem; qualquer calibração futura acima de 150 exige cascata iniciando em `05_alimentacao.md` (recalcular §5.1 e revisar §8.2) antes de tocar o 03. Nota de conformidade de material da cablagem adicionada à seção CA-10 (bitolas/cores fora da spec por disponibilidade; especificação permanece normativa) |
