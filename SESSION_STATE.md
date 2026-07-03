---
# ANCORAGEM DE SESSÃO — arquivo2-protocolo-ia.md PARTE 5
# Atualizar ao encerrar cada sessão. Enviar junto com arquivo2 ao retomar.
---

CONTEXTO: continuando projeto Instrumento Ludico-Pedagogico ESP32 — 2026-07-02
Processo: V-Model (ISO 26262 / IEC 61508 / IEC 62304)
Estado verificado em 2026-07-02: run_all.py → TODOS OS CHECKS PASSARAM (47 constantes, 6 seções, 4 _config.h em sync)

Camada atual: VALIDAÇÃO
Fase V-Model atual: ETAPA 8 — Validação com hardware físico

Fases concluídas e aprovadas:
  [x] Fase 1 — Requisitos          (concept/00_conceito.md v0.1.0 APROVADO)
  [x] Fase 2 — Arquitetura Sistema  (system/01_arquitetura.md v0.1.0 APROVADO)
  [x] Fase 3 — Design de Hardware   (hardware/08–11 v0.1.0 APROVADO)
  [x] Fase 4 — Arquitetura Firmware (modules/02–07 v0.1.0 APROVADO)
  [x] GATE Formalização             (spec/power, spec/sensor, spec/visual, spec/game, spec/interface MERGEADOS em develop — tag v0.2.0)
  [x] GATE pré-ETAPA 7              (CODING_STANDARD.md v0.1.0 + firmware_constants.json + _config.h gerados — mergeados em develop)
  [x] ETAPA 7 — Firmware MVP        (MOD_SENSOR, MOD_LED, MOD_JOGO, MOD_WIFI — feat/interface mergeado em develop — tag v0.3.0 — 38/38 PASSED)
  [ ] Fase 5 — Testes de Módulos    (requer hardware físico)
  [ ] Fase 6 — Validação de Hardware
  [ ] Fase 7 — Teste de Integração
  [ ] Fase 8 — Teste de Sistema

Branch ativa: feat/export-preview-pdf (criada de fix/export-csv em
2026-07-03 — melhorias M2/M3 do TODO.md: pré-visualização com confirmação
antes do download e escolha de formato CSV/PDF, com exportação PDF gerada
em JS puro).
Estado da branch feat/export-preview-pdf — cadeia completa commitada na
ordem md → padrão → spec → código, run_all.py verde, jsonschema OK e
CHANGELOG atualizado antes de cada commit:
  1. docs(interface) 28e8191 — 07 v0.3.0: §8 reestruturada (fluxo Exportar
     → prévia → formato → confirmar), §8.3 PRE-01..05, §8.4 PDF-01..06 com
     DECISAO formal (PDF 1.4 em JS puro, base-14, WinAnsi, A4 paisagem,
     35 linhas/página), CA-07-09 ajustado, CA-07-12/13 novos
  2. docs(padrao) 24cd106 — WEB_STANDARD v0.3.0: overlay-exportacao,
     constantes EXPORT_*/PDF_*, §10.3/§10.4, cenários CA-07-12/13
  3. spec(interface) 1b1150c — exportacao_ui + exportacao_pdf no
     interface.json/schema, 13 CAs, versao_fonte 0.3.0
  4. feat(interface) 72ba5cb — interface.cpp: overlay de prévia (tabela
     via textContent, radios de EXPORT_FORMATOS, Baixar/Cancelar,
     PRE-05 com vazio), gerador _pdfGerar (xref byte-exata), download
     base64 pelo mesmo mecanismo data: URI do D1; baixarArquivo() como
     ponto único de âncora; exportarCSV() só via confirmação da prévia
Verificação já feita nesta branch: harness Node novo (JS real embutido,
DOM mockado, valores esperados lidos do interface.json) — 44/44 checks
cobrindo CA-07-12 (prévia/cancelar/vazio), CA-07-09 via prévia (regressão
D1 completa: BOM, RFC 4180, data: URI) e CA-07-13 (estrutura PDF: %PDF-1.4,
xref byte-exata, /Length, WinAnsi de acentos, escaping de parênteses,
paginação 40 registros → 2 páginas); PDF de amostra aberto e conferido
visualmente (título, data/hora, tabela, acentos); pio run -e esp32dev
SUCCESS sem warnings (Flash 66.7%); pio test -e native 38/38.
Harness em scratchpad da sessão (previa_pdf_check.js) — recriável a
partir dos cenários do WEB_STANDARD §11.5.
Próxima ação: VALIDAÇÃO FÍSICA de CA-07-09 (ainda pendente do D1),
CA-07-12 e CA-07-13 — flashar feat/export-preview-pdf (contém o fix do
D1) e validar na bancada com browser da matriz RNF-05. PASSOU → merge
fast-forward de fix/export-csv em develop, depois feat/export-preview-pdf
em develop; baixa de D1 e M2/M3 no TODO.md. FALHOU → iterar.
Nota de derivação registrada: M2/M3 derivam do 07 (trâmite do TODO.md,
igual ao D1); 00_conceito.md §12.2 segue citando só CSV — se o usuário
quiser registrar PDF no conceito, é branch docs/conceito-exportacao com
bump 00 v0.1.0→v0.2.0 + cascata de depende_de em ~10 documentos
(mecânica, não bloqueia).

Contexto herdado da branch fix/export-csv (D1):
Estado da branch — cadeia completa commitada na ordem md → json → schema →
código, run_all.py verde e jsonschema OK em todos os commits:
  1. docs(interface) cf97ed9 — 07 v0.2.0: §8 re-especificada — mecanismo
     data: URI + âncora no DOM (DECISAO formal), requisitos CSV-01..04
     (UTF-8 BOM, escaping RFC 4180, charset, mecanismo), CA-07-09 estendido
  2. docs(padrao) 639a97b — WEB_STANDARD v0.2.0: constantes CSV_MIME/
     CSV_CHARSET/CSV_DATA_URI_PREFIX/CSV_BOM derivadas, csvEscapar(),
     §10.2 com proibição de blob+revoke na exportação
  3. spec(interface) a9c198a — interface.json + schema re-derivados:
     charset/bom/escaping_rfc4180 em exportacao_csv, versao_fonte 0.2.0
  4. fix(interface) 3739a9d — exportarCSV() reescrita em interface.cpp:
     data: URI + appendChild/removeChild, csvEscapar(), BOM; causa raiz
     tripla documentada (revoke síncrono, click fora do DOM, blob: em
     WebView). NADA hardcoded: todos os valores derivam da spec.
Verificação já feita: pio run -e esp32dev SUCCESS sem warnings; pio test -e
native 38/38; pré-validação em Node executando o JS real embutido (DOM
mockado) — 11/11 checks do CA-07-09, incluindo vírgula/aspas em coluna
única e BOM. (pio run do env native falha por design fora de pio test —
mocks vivem em test/; condição pré-existente, não tocar.)
Próxima ação: VALIDAÇÃO FÍSICA do CA-07-09 — flashar fix/export-csv,
exportar com ≥2 sessões (uma com nome contendo vírgula e acento) e conferir
o arquivo em planilha. Browser da matriz RNF-05 (Chrome/Firefox Android);
DuckDuckGo/WebView deve funcionar com data: URI mas está fora da matriz
formal. PASSOU → merge fast-forward em develop + baixa do D1 no TODO.md.
FALHOU → iterar nesta branch.
Depois, nesta ordem: (1) checklist formal de CAs da ETAPA 8 (PASSOU/FALHOU
por CA, incluindo CA-05-08, CA-09-07, CA-10-06 novos); (2) melhorias M1–M5
em branches próprias; (3) só então tag v1.0.0 e merge em main. Pendências
físicas: marcar borne falso-GND (CMD) como proibido; auditar rótulos do
shield contra a serigrafia do DevKitC; travar/marcar o trimpot do LM2596 em
3.30V (CA-09-07/CA-05-01).

---

## Investigação boot loop CA-07-01 — RESOLVIDA (2026-07-02)

**CAUSA RAIZ ENCONTRADA E CORRIGIDA FISICAMENTE — sem osciloscópio.**

### Causa raiz

A serigrafia do shield marca "GND" num borne cuja posição no soquete corresponde
ao pino **CMD (GPIO11)** do DevKitC — o **chip select da flash SPI** interna do
WROOM-32 (posição entre SD3 e 5V na coluna). O fio do barramento de terra de
TODO o sistema (piezos, LEDs, fonte) e o negativo do eletrolítico de 1000 µF do
5V estavam parafusados nesse borne: o "terra" do sistema estava amarrado na
linha CS da flash.

Mecanismo: com o CS carregado, a flash não responde — toda leitura devolve 0xFF
(`invalid header: 0xffffffff`, `flash read err, 988`). Contato marginal explica
a intermitência histórica: quando a carga no CS era fraca, a placa bootava e a
flash só falhava sob o tráfego pesado do init do WiFi (leitura de calibração
RF) — produzindo o `TG1WDT_SYS_RESET` registrado como sintoma original.
Confirmado por inspeção: serigrafia "GND" do shield vs serigrafia "CMD" do
DevKitC na mesma posição. CS em repouso é dirigido em nível alto (3.3V), o que
explica as medições anômalas (3V3↔borne "GND" = <1V; 5V↔borne ≈ 2V).

### Cadeia de evidência (2026-07-02 — multímetro + monitor serial, sem osciloscópio)

1. Firmware íntegro (HEAD 7fe6738, zero chamadas `disable*WDT` em src/) na
   bancada: boot limpo, AP BMI no ar.
2. No shield (configuração completa): ROM bootloader NÃO LÊ a flash desde o
   primeiro boot (`rst:0x1 POWERON_RESET` → `invalid header` imediato) — falha
   PRÉ-firmware. WiFi/NVS/firmware nunca foram a causa. Leituras parciais
   morriam no meio da carga (`load:0x3fff0030,len:1184` OK →
   `load:0xffffff0f,len:-1`).
3. Mesma imagem de flash, shield pelado (zero fios/componentes nos bornes):
   boot limpo + AP no ar → a flash NUNCA esteve corrompida; soquete e PCB do
   shield inocentados.
4. Bisseção: LM2596+fonte removidos → crash continuou (eliminados). Só
   capacitores nos bornes → crash voltou. Negativo do eletrolítico do 5V movido
   do borne falso-GND para GND verdadeiro → boot limpo. Remontagem completa
   passo a passo (terra → piezos → dado do LED → LM2596 + fonte 12V) → boot
   limpo em TODOS os passos, AP no ar, modo iniciado via celular.

### Fechamento das 20 hipóteses

Todas as hipóteses da tabela histórica (mantida abaixo como registro) são
explicadas pela causa raiz: os testes por jumper fora do shield (12, 15) eram
limpos porque usavam o GND real da placa; tudo que passava pelo barramento de
terra do shield herdava o CS da flash como "terra". A pista aberta "rodar sem
disable*WDT" foi coberta pelo item 1 (HEAD não tem nenhuma chamada).
Osciloscópio não é mais necessário para este caso.

### Correções físicas aplicadas (hardware; cascata de docs ainda NÃO executada)

- Fio do barramento de terra movido para borne GND verdadeiro (entre GPIO12 e
  GPIO13; continuidade com pino GND do DevKitC verificada antes do aperto).
- Negativo do eletrolítico 1000 µF do 5V movido para GND verdadeiro.
- Capacitor removido do pino de DADOS (GPIO5) — estava alterando o strapping
  (`boot:0x12` em vez de `0x13`); decoupling correto é no VDD da cadeia, junto
  ao primeiro LED, quando houver espaço físico.
- VDD da cadeia WS2812B de volta a 3V3 conforme decisão de spec (mudança para
  5V do teste 11 revertida; em 5V o dado de 3.3V viola VIH ≥ 0.7·VDD).
- PENDENTE físico: marcar o borne falso-GND (CMD) como proibido; auditar TODOS
  os rótulos do shield contra a serigrafia do DevKitC, pino a pino.

### LEDs não acendiam — RESOLVIDO (2026-07-02)

- Causa: contato marginal dos CLIPES da cadeia WS2812B. Sem nenhuma mudança
  de código, o manuseio/re-assentamento dos conectores fez LED1 e LED2
  acenderem; o trecho LED2→LED3 resolvido re-assentando o clipe. 3/3 LEDs
  funcionando com o sketch de diagnóstico (ciclo de cores completo).
- Antes disso, verificado: caminho do dado íntegro GPIO5 → borne → resistor
  série (medido 299 Ω) → clipe → DIN do LED1; VDD presente nos 3 LEDs.
  MOD_LED comprovado vivo em hardware pela primeira vez (14 testes anteriores
  rodavam contra mock do FastLED).
- Pendência de projeto (cascata futura): conexão por clipe é frágil para o
  contexto de uso (instrumento para crianças de 5 anos, impactos constantes)
  — endereçar em 10_cablagem.md/11_montagem.md quando a cascata for
  autorizada.
- Nota de procedimento: um primeiro upload do sketch gravou o ambiente errado
  (`pio run -t upload` sem `-e ledtest` grava esp32dev primeiro); o
  diagnóstico só começou de fato com `pio run -e ledtest -t upload`.

### Brownout no init do rádio — AMS1117 condenado; arquitetura 3.3V direta (2026-07-02)

- Com os LEDs finalmente funcionais, o firmware real entrou em loop de
  brownout (`Brownout detector was triggered` → `rst:0xc SW_CPU_RESET`) no
  instante da ligada do rádio WiFi — localizado por breadcrumbs do
  `wifi_test` do kit diag (morte sempre no "passo 1 — WiFi.mode(WIFI_AP)").
- Eliminados com evidência: firmware (src byte-idêntico ao tag v0.3.0; a
  mensagem vem de comparador analógico do chip), cabo/porta USB (carregador
  de parede + cabo diferente → mesmo resultado), LEDs (VDD desconectado →
  mesmo resultado), fonte externa (LM2596 medindo 4.96V no pino 5V durante o
  loop).
- CONDENADO: o AMS1117 (5V→3.3V) do DevKitC — sustenta cargas leves, colapsa
  no pico da ligada do rádio. Prova: LM2596 recalibrado para 3.30V
  alimentando o rail 3V3 DIRETO (contornando o AMS1117) → AP no ar, placa
  viva. Nota: de manhã o mesmo boot passava; o regulador degradou ao longo
  do dia (possível estresse residual da era do falso-GND).
- ARQUITETURA NOVA (validada fisicamente; cascata de docs em execução):
  12V → LM2596 @ 3.30V → rail 3V3 direto. Rail 5V sem função operacional
  (borne esvaziado). USB somente para gravação (gravação não usa WiFi).
  Consequência operacional: USB-only com WiFi = brownout SEMPRE nesta placa;
  bancada de WiFi exige fonte ligada.
- Segurança (vai para a cascata): medir 3.30V na saída do LM2596 ANTES de
  conectar ao borne 3V3 (máx. absoluto do ESP32: 3.6V); travar/marcar o
  trimpot.
- Capacitores: LM2596 intocado (100µF entrada / 470µF saída, soldados no
  módulo); borne 3V3 mantém eletrolítico 1000µF + cerâmico (1000µF é
  EXTRA-SPEC consciente, relíquia útil da investigação — valor definitivo
  decidido na cascata do 05); borne 5V esvaziado.
- SISTEMA COMPLETO FUNCIONANDO DE PONTA A PONTA (2026-07-03): boot +
  animação nos 3 LEDs + AP BMI + interface. Primeira vez na história do
  projeto com todos os subsistemas vivos simultaneamente.
- Defeito conhecido em aberto: EXPORT CSV da interface (CA-07-*) — branch
  fix em sessão futura; bloqueia o gate v1.0.0.

### Kit de diagnóstico de bancada — consolidado em firmware/diag/ (2026-07-02)

Os artefatos temporários da investigação foram consolidados em
`firmware/diag/` — projeto PlatformIO SEPARADO com README próprio (uso,
truque BOOT/EN, lições de hardware). Fora do V-model (nada deriva de spec) e
fora dos builds normais do firmware. Conteúdo: sketch de teste da cadeia
WS2812B (onda quadrada p/ multímetro + ciclo de cores) e `monitor_serial.py`
(timestamps por linha + reset via RTS). Os temporários do projeto principal
(`firmware/src/led_hw_test.cpp` e `[env:ledtest]`, nunca commitados) foram
removidos — projeto principal de volta ao estado limpo.

---

### Histórico da investigação original (registro — tudo explicado pela causa raiz)

### Sintoma

Firmware funciona indefinidamente fora do shield (USB, bancada). Ao conectar
o DevKitC ao shield — mesmo sem energizar a fonte externa 12V/LM2596, só USB
já é suficiente — trava imediatamente:

```
[XX][E][esp32-hal-misc.c:143] disableCore1WDT(): Failed to remove Core 1 IDLE task from WDT
rst:0x8 (TG1WDT_SYS_RESET)  [as vezes rst:0x7 TG0WDT_SYS_RESET nos ciclos seguintes]
invalid header: 0xffffffff  (repete, cascateando em "flash read err, 988")
```

`TG1WDT_SYS_RESET` = Interrupt Watchdog (Timer Group 1) — dispara quando
interrupções ficam desabilitadas tempo demais em algum core. A causa mais
provável é uma operação de flash/NVS do driver WiFi (que exige interrupções
desabilitadas nos dois cores) sendo interrompida no meio, corrompendo a
imagem — daí o `invalid header` e a cascata de resets subsequentes.

O `disableCore1WDT(): Failed to remove...` que aparece em TODO boot (inclusive
os que funcionam) é um aviso benigno — a idle task do Core 1 provavelmente
nunca esteve inscrita no TWDT nesta config; não é a causa.

### Hipóteses testadas e ELIMINADAS (com evidência, não suposição)

| # | Hipótese | Teste | Resultado |
|---|---|---|---|
| 1 | Escrita NVS de credenciais WiFi durante init | `WiFi.persistent(false)` | Sem mudança |
| 2 | Task WDT (TG0) com timeout curto | `esp_task_wdt_init(30s)` | TG0 parou de disparar primeiro, mas TG1 continuou — não resolve |
| 3 | Brownout/subtensão real no pico RF | Medição direta com multímetro no instante do crash, múltiplas vezes | Tensão nunca cai; chegou a medir 5.06V (fora de spec alto, não baixo) |
| 4 | Capacitor insuficiente no 5V | 1000μF no pino 5V do DevKitC | Sem mudança |
| 5 | Capacitor insuficiente no 3V3 | 1000μF no pino 3V3 (além do 5V) | Sem mudança |
| 6 | LM2596 descalibrado | Recalibrado de 5.06V para 5.00V exato | Sem mudança |
| 7 | Fonte externa fraca | LM2596/12V ligado direto no DevKitC, testado sozinho fora do shield | Funciona limpo — fonte não é o gargalo |
| 8 | GPIO0 tocado pelo shield (entrada em download mode) | Verificado — foi botão físico pressionado manualmente | Descartado |
| 9 | GPIO12 (strapping, tensão da flash) em conflito | Conferido no mapeamento de GPIOs do projeto (34,35,32,33,5) | Não usado, descartado |
| 10 | Curto GPIO5↔GPIO34 | Multímetro em **resistência** mostrou valor finito | **Falso alarme** — resistência sempre mostra algo finito via diodos ESD internos de qualquer GPIO; **continuidade** (teste correto) não apitou. Sem curto real |
| 11 | Rail de alimentação do LED (3.3V, abaixo do spec do WS2812B) vs 5V | VDD do LED movido de 3V3 para 5V | Sem mudança |
| 12 | Cadeia de 3 LEDs (capacitância/volume) vs 1 LED | 1 LED avulso por jumper fora do shield: limpo. 3 LEDs em cadeia por jumper fora do shield: limpo | Nenhum reproduziu fora do shield |
| 13 | Borne de parafuso do shield com mau contato | Continuidade + tensão testadas ponta a ponta, aperto cuidadoso | Conexão boa, não é isso |
| 14 | GPIO5 é pino de strapping, soquete do shield com contato ruim nesse pino específico | Dado do LED movido de GPIO5 para GPIO18 (sem strapping, sem restrição) | Travou igual — não é o pino específico |
| 15 | LED em si (qualquer rail, qualquer pino) é necessário pro crash | Cadeia de LED **removida completamente** (nem dado nem energia conectados) — só sensor+game+interface | Travou igual — LED nunca foi a causa |
| 16 | `sensorInit()` (ADC1 real nos piezos, fiação real do shield) sozinho | `sensorInit()` + `interfaceInit()`, sem visual/game | Travou igual |
| 17 | Ordem/tempo de chamada — WiFi chamado tarde demais | `interfaceInit()` sozinho + `delay(50)` antes: travou. `interfaceInit()` chamado PRIMEIRO no firmware completo (sensor/visual/game depois): travou igual, no mesmo timestamp que sempre foi limpo quando isolado | Não é sobre ordem — binário maior (mesmo não executado ainda) muda a duração da seção crítica dentro do WiFi init |
| 18 | Biblioteca Arduino `WiFi.h` vs `esp_wifi` puro do IDF | Reescrito com `esp_wifi_init/set_mode/set_config/start`, replicando config de IP do netif, handlers de evento, e a mitigação de brownout (DM-05) do interfaceInit() original | Travou igual — não é a biblioteca |
| 19 | Placa física específica com defeito | Testado em 2 unidades diferentes de DevKitC/shield | Mesmo resultado nas duas |
| 20 | Flash suja de crashes anteriores confundindo os testes | Usuário confirmou: faz `erase_flash` completo antes de CADA upload, sempre | Não é confusão de estado — resultados são reais |

### Pistas em aberto da época (OBSOLETAS — supersedidas pela causa raiz)

Osciloscópio no 3V3, teste sem `disable*WDT` e sonda SPI ficaram
desnecessários: a falha era o CS da flash amarrado ao "terra" do sistema via
borne com serigrafia errada. Ironia registrada: a pista "sonda direta nas
linhas SPI da flash (CS/CLK)" apontava literalmente para o ponto defeituoso.

### Estado do código

Branch `fix/ca-07-01` resetada para `7fe6738` (ponto de divergência de
`develop`, antes de qualquer alteração desta investigação). Todo o histórico
anterior (commits reais + toda a exploração desta sessão) preservado na tag
`backup/fix-ca-07-01-abandonado-20260702` — usar `git show
backup/fix-ca-07-01-abandonado-20260702` ou `git log
backup/fix-ca-07-01-abandonado-20260702` pra recuperar qualquer coisa
específica, incluindo os commits da sessão anterior (6h, antes desta) que já
tinham tentado: `WiFi.setTxPower` limitado a 8.5dBm, brownout desabilitado
durante `WiFi.mode()`, `board_build.partitions = no_ota.csv`, `disableLoopWDT()`
antes de `interfaceInit()` — nenhuma dessas resolveu o problema de fato (o
sintoma só sumia às vezes por acaso de timing/estado de flash, não por a causa
raiz estar corrigida — só percebemos isso nesta sessão ao isolar variável por
variável).

**Antes de retomar**: a causa raiz está corrigida fisicamente. Não
re-investigar nada desta seção histórica; o trabalho restante está em
"Problema novo em aberto: LEDs não acendem" e nas pendências do topo.

---

Ordem de implementação dos módulos (ETAPA 7):
  1. MOD_SENSOR  [CONCLUÍDO — feat/sensor] ← commits: a2d2fef, 59b25ea, 9d313ac
  2. MOD_LED     [CONCLUÍDO — feat/visual] ← commit: 48bbe36
  3. MOD_JOGO    [CONCLUÍDO — feat/game]   ← commits: 51246f3, 89fac38
  4. MOD_WIFI    [CONCLUÍDO — feat/interface] ← commit: 2308d61

Gate ETAPA 7 — CONCLUÍDO (2026-06-28):
  [x] Todos os módulos implementados (SENSOR, LED, JOGO, WIFI)
  [x] pio test -e native → 38/38 PASSED (confirmado 2026-06-28 com firmware completo)
  [x] pio run → SUCCESS (0 errors, 0 warnings — 2026-06-28; RAM 13.6%, Flash 65.4%)
  [x] tag v0.3.0 — criada em develop (ad0dea3)

MOD_WIFI — artefatos criados (branch feat/interface):
  - _governance/WEB_STANDARD.md v0.1.0: governanca HTML/CSS/JS embutido; MD3 offline; zero magic numbers; cenarios CA-07-01..11
  - firmware/src/interface/interface.h: API publica interfaceInit() e interfaceLoop()
  - firmware/src/interface/interface.cpp: WiFi AP + ESPAsyncWebServer + WebSocket /ws + ArduinoJson 7.x
    HTML/CSS/JS embutido: maquina de estados 6 telas; WebAudio; localStorage; exportação CSV
    Helpers DRY: enviarEvento(), enviarStatus(), _aplicarToggle(), _mostrarOverlay()
  - firmware/src/game/game.h: adiciona declaracoes gamePausarSessao() e gameRetomarSessao()
  - system/01_arquitetura.md: ArduinoJson >= 7.0 registrado na tabela de stack

Commits do usuário feitos entre as sessões (já no HEAD de feat/interface):
  - 5a6ed41 build(interface): refatora platformio.ini para [env] base compartilhada
      → lib_extra_dirs = src movido para [env] compartilhado; corrige include path esp32dev
  - afc0661 fix(sensor): corrige tipo ADC de adc_atten_t para adc_attenuation_t
      → sensor.cpp: SENSOR_ADC_ATENUACAO = ADC_11db (Arduino ESP32 3.x)
  - 89fac38 feat(game): integra visualSetLED e visualRunCelebracao em MOD_JOGO
      → game.cpp: helper cor_para_led(); visualSetLED no ESTIMULO; visualRunCelebracao no FIM_SESSAO
      → game.cpp: gameRetomarSessao() reacende LEDs (CA-04-09); game.cpp: gamePausarSessao() implementado
      → main.cpp: remove [DECIDIR]; add [VER: 01_arquitetura.md#interface-jogo-led]
      → test_game: mock FastLED adicionado; 38/38 PASSED

MOD_JOGO — artefatos criados (branch feat/game):
  - firmware/src/game/game.h: Cor, ParCores, ResultadoJogo, EventoJogo, ConfigSessao; API publica + API de teste
  - firmware/src/game/game.cpp: maquina de estados; Mec A (Fisher-Yates) e Mec B (peso decrescente); logica Modo 1 e Modo 2
    visualSetLED, visualRunCelebracao e gamePausarSessao/gameRetomarSessao integrados via commits do usuario
  - firmware/test/test_game/test_main.cpp: 11 testes TDD; 38/38 PASSED (com visual mock); cobre CA-04-01..08, CA-04-10
  - firmware/platformio.ini: build_src_filter atualizado para +<sensor/> +<visual/> +<game/>
  - firmware/test/mock/Arduino.h: adiciona String (mock minimo) e declaracao random(long)

MOD_LED — artefatos criados (branch feat/visual):
  - firmware/test/mock/FastLED.h: mock nativo (CRGB, CFastLED, EOrder/GRB, WS2812B); padrao declare->define
  - firmware/test/test_visual/test_main.cpp: 14 testes TDD; 14/14 PASSED; cobre CA-03-01, CA-03-04, CA-03-06
  - firmware/src/visual/visual.h: ComandoLED (struct + enums LED/Cor), visualInit, visualLoop, visualSetLED, visualRunCelebracao
  - firmware/src/visual/visual.cpp: animacoes boot e celebracao non-blocking via millis(); paleta RGB constexpr
  - firmware/src/main.cpp: visualInit() em setup(); visualLoop() em loop()

MOD_SENSOR — artefatos criados (branch feat/sensor):
  - firmware/platformio.ini: [env:esp32dev] + [env:native]; lib_extra_dirs=src
  - firmware/test/mock/Arduino.h: mock nativo mínimo (declare sem define)
  - firmware/test/test_sensor/test_main.cpp: 13 testes TDD; 13/13 PASSED
  - firmware/src/sensor/sensor.h: EventoImpacto, sensorInit, sensorOnImpacto, sensorLoop
  - firmware/src/sensor/sensor.cpp: implementação completa; debounce por zona independente
  - firmware/src/main.cpp: stub Arduino (setup/loop)
  - _governance/TESTING_STANDARD.md: v0.1.0 → v0.1.1 (lib_extra_dirs documentado)

Descoberta registrada — NÃO re-investigar:
  - PlatformIO 6.1.x: build_src_filter NÃO afeta pio test. Para compilar src/ em test builds,
    usar lib_extra_dirs = src. O LDF detecta dependências via #include e compila automaticamente.
  - Compilar como biblioteca via lib_extra_dirs requer include relativo ao diretório da biblioteca:
    dentro de sensor.cpp, usar #include "sensor.h" (não "sensor/sensor.h"). Idem game.cpp.
  - Mock de biblioteca de terceiros (FastLED): arquivo <Lib.h> em test/mock/ é resolvido via
    -I $PROJECT_DIR/test/mock no [env:native]. test_main.cpp deve incluir <FastLED.h> explicitamente
    para usar tipos do mock (CRGB, CFastLED). Padrão idêntico ao Arduino.h.
  - game.h inclui "sensor.h" (não "sensor/sensor.h") — mesmo padrão de biblioteca via lib_extra_dirs.
  - test_game/test_main.cpp deve incluir <Arduino.h> explicitamente para ter adc_atten_t disponível.
  - gameIniciarSessao() emite ESTIMULO via callback — testes que verificam s_cb_count após impacto
    devem resetar s_cb_count = 0u após gameIniciarSessao() para contar apenas o evento do impacto.
  - ESPAsyncWebServer 3.x: send_P() está deprecated; usar send() com os mesmos argumentos.
  - MOD_WIFI não é testável em native (WiFi, WebSocket). TDD equivalente: cenários CA-07-* em WEB_STANDARD.md.
  - ArduinoJson v7: JsonDocument sem tamanho (alocacao dinâmica); deserializeJson(doc, data, len) para dados binários WS.
  - _aplicarToggle(pausar, client): abstrai PAUSAR/RETOMAR de forma simétrica; client != nullptr → send só ao cliente reconectado.

Decisões aprovadas:
  - LM2596 módulo pré-montado → 5V ± 0.1V (eficiência 85–90%)
  - WS2812B em 3.3V sem level shifter (GPIO5 a 3.3V compatível)
  - ADC1 exclusivo para piezos: GPIOs 34/35/32/33 (ADC2 incompatível com WiFi)
  - GPIO 5: dados WS2812B (cadeia de 3 LEDs)
  - THRESHOLD piezo: 500/4095 [CALIBRAR após montagem]
  - DEBOUNCE_MS: 200ms [não CALIBRAR — cobertura de ringing mecânico]
  - Timing B (janela simultaneidade Modo 2): 800ms padrão [CALIBRAR]
  - Intervalo entre interações: 2000ms [CALIBRAR]
  - Zener proteção GPIO: 3.3V BZX55C3V3 ou 1N5226B (não usar 1N4728A — 3.9V inadequado)
  - R1 proteção piezo: 1MΩ 1/4W
  - Constantes com tipo de plataforma (adc_atten_t, EOrder) declaradas no .cpp após includes de plataforma — não no _config.h (flag tipo_plataforma: true em firmware_constants.json)
  - Animações de boot e celebração: non-blocking via millis() (blocking via delay() incompatível com stack WiFi — [VER: 01_arquitetura.md#stack-tecnologico])
  - visualRunCelebracao() exposta em visual.h: MOD_LED é responsável pela animação de celebração ([VER: 01_arquitetura.md#mod-led]); não altera interface cross-module definida em 01_arquitetura.md#interface-jogo-led
  - N_PALETA (4 cores) não existe em visual_config.h — derivado implicitamente de sizeof(s_paleta) em visual.cpp; em testes usa T_VISUAL_N_PALETA = 4u (HARDCODED_TESTE com justificativa)
  - rand() em game.cpp requer #include <stdlib.h> em native (MinGW não fornece via Arduino.h)
  - API de teste (gameGetCorAtual, gameGetParAtual, gameZonaParaCor, etc.) exposta em game.h com comentário "Nao chamar em producao" — necessária para observabilidade dos CAs sem acoplar test_main.cpp ao estado interno
  - gameOnEvento: único slot de callback (um consumidor). MOD_JOGO chama visualSetLED diretamente ([VER: 01_arquitetura.md#interface-jogo-led]); interfaceInit() registra o callback para MOD_WIFI
  - ArduinoJson ≥ 7.0: escolhido sobre parsing manual (frágil); documentado em 01_arquitetura.md#stack-tecnologico e CODING_STANDARD.md#modularidade-pode
  - WEB_STANDARD.md em _governance/: governa HTML/CSS/JS embutido; mesma estrutura de CODING_STANDARD.md e TESTING_STANDARD.md
  - Bump de versão de documentos: NÃO necessário quando o conteúdo é atualizado durante branch de desenvolvimento ativa (scripts verificam sincronismo, não versão de documento de governança)

Pendências:
  - pio test -e native → re-confirmar 38/38 PASSED com firmware completo (main.cpp wiring novo)
  - merge feat/interface → develop (fast-forward, sem --no-ff)
  - tag v0.3.0 após gate ETAPA 7 completo
  - ETAPA 8: Validação (fecha o V-model) — requer hardware físico

Artefatos em develop — sessão anterior (feat/gerar-config-h — mergeado, FECHADO):
  NOTA: estes artefatos já existem em disco e estão em sync. Não recriar.
  - scripts/generate_coding_standard.py: gera firmware/src/<modulo>/<modulo>_config.h
    (a promessa do CODING_STANDARD.md seção 2 está honrada — confirmado em 2026-06-28)
  - firmware/src/sensor/sensor_config.h, visual/visual_config.h, game/game_config.h, interface/interface_config.h
  - spec/firmware_constants.json: campo tipo_plataforma: true em DM-02 e DM-03
  - spec/firmware_constants.schema.json: aceita tipo_plataforma opcional
  - _governance/CODING_STANDARD.md: seção 2 corrigida + registro no changelog interno

Premissas verificadas (NÃO re-analisar em sessões futuras):
  - generate_coding_standard.py GERA _config.h (linhas 414-421 do script): config_path.write_text(...)
  - Modo --check VERIFICA sync dos _config.h em disco (linhas 361-382)
  - run_all.py confirma: "OK: 4 arquivos _config.h em sync" — verificado em 2026-06-28
  - A promessa do CODING_STANDARD.md seção 4.1 ("conteúdo gerado pelo script") está honrada
  - Cascata de ponta a ponta está fechada: spec JSON → firmware_constants.json → _config.h → CI detecta divergência
  - Opção A (estender o script) foi implementada na sessão feat/gerar-config-h (já mergeada)
  - pio test -e native: 38/38 PASSED (11 game + 13 sensor + 14 visual) — confirmado em 2026-06-28 (pré-feat/interface)

Desvios desta sessão feat/interface (2026-06-28) — registrados para não repetir:
  - Tentou usar Task agent (proibido neste repositório) — corrigido imediatamente ao ser alertado
  - Leu módulos existentes para entender padrão na prática — padrão correto: ler _governance/CODING_STANDARD.md
  - Saltou direto para interface.h sem escrever cenários de teste primeiro (ETAPA 7 passo 3) — corrigido: WEB_STANDARD.md#padrao-testes escrito antes do HTML
  - send_P() não detectado como deprecated antes de rodar pio run — corrigido após primeira compilação
  - CHANGELOG editado após staging em vez de antes — inverter ordem nas próximas sessões
  - game.h com declarações gamePausarSessao/gameRetomarSessao staged mas game.cpp já implementado pelo usuário em commit anterior — registrar este padrão: usuário pode commitar implementações entre sessões; verificar commits recentes ao iniciar

Desvios desta sessão feat/game (2026-06-28) — registrados para não repetir:
  - game.h incluiu "sensor/sensor.h" em vez de "sensor.h" — erro de compilacao; corrigido pelo erro real
  - test_main.cpp não incluiu <Arduino.h> — adc_atten_t nao declarado; corrigido pelo erro real
  - game.cpp não incluiu <stdlib.h> — rand() nao declarado em MinGW; corrigido pelo erro real
  - s_cb_count não resetado após gameIniciarSessao nos testes de Modo 1 — Expected 1 Was 2; corrigido pela falha de asserção real

Desvios desta sessão feat/visual (2026-06-28) — registrados para não repetir:
  - test_main.cpp inicial não incluía <FastLED.h> explicitamente — tipos CRGB/CFastLED
    invisíveis para o compilador; corrigido adicionando #include <FastLED.h> em test_main.cpp
  - Constante T_VISUAL_N_PALETA definida dentro de função (escopo local) — corrigido para escopo
    de arquivo conforme TESTING_STANDARD.md#nomenclatura-constantes-teste

Desvios da sessão feat/sensor (2026-06-28) — registrados para não repetir:
  - VER link em sensor.h usou ancora inexistente #interfaces; ancora correta e #interfaces-modulos
    (corrigido antes do merge; o script verify_links.py nao valida .cpp/.h — vigilancia manual necessária)

Desvios da sessão feat/sensor abertura (2026-06-28):
  - Re-análise de premissa já resolvida: sessão questionou se generate_coding_standard.py gera _config.h —
    premissa era falsa; o script já fazia isso desde feat/gerar-config-h; SESSION_STATE.md e CHANGELOG
    atualizados para bloquear recorrência

Desvios da sessão feat/gerar-config-h (registrados para não repetir):
  - Branch não criada antes de iniciar o trabalho — iniciado em develop, branch criada tardiamente
  - Merge commit criado com --no-ff e depois desfeito — usar fast-forward (sem --no-ff)
  - Changelog interno do CODING_STANDARD.md não incluído no commit original — corrigido via amend

Desvios da sessão anterior (registrados para não repetir):
  - CHANGELOG não atualizado antes da revisão do diff — terceira reincidência consecutiva (identificado pelo usuário, corrigido antes do commit)

Desvios de sessões anteriores (registrados para não repetir):
  - ETAPA 4 e ETAPA 5 não relidas como ato explícito na primeira tentativa — corrigido na segunda passagem
  - CHANGELOG não atualizado antes do commit (identificado na meta-análise, corrigido antes do commit)
  - CHANGELOG não atualizado antes do commit spec(sensor) — corrigido via amend
  - Merge commit tentado antes de verificar CHANGELOG — bloqueado pelo usuário
  - Commits em main antes de criar develop + branch de trabalho
  - CHANGELOG não atualizado antes dos commits spec(power) e docs(padrao)
  - Gate ETAPA 3 não validado com as 4 perguntas antes de avançar
  - Meta-análise executada após entrega (deve ser pré-commit)
