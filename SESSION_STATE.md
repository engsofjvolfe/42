---
# ANCORAGEM DE SESSÃO — arquivo2-protocolo-ia.md PARTE 5
# Atualizar ao encerrar cada sessão. Enviar junto com arquivo2 ao retomar.
---

CONTEXTO: continuando projeto Instrumento Ludico-Pedagogico ESP32 — 2026-06-28
Processo: V-Model (ISO 26262 / IEC 61508 / IEC 62304)
Estado verificado em 2026-06-28: run_all.py → TODOS OS CHECKS PASSARAM (47 constantes, 6 seções, 4 _config.h em sync)

Camada atual: DERIVAÇÃO
Fase V-Model atual: ETAPA 7 — Firmware (MOD_SENSOR, MOD_LED e MOD_JOGO concluídos)

Fases concluídas e aprovadas:
  [x] Fase 1 — Requisitos          (concept/00_conceito.md v0.1.0 APROVADO)
  [x] Fase 2 — Arquitetura Sistema  (system/01_arquitetura.md v0.1.0 APROVADO)
  [x] Fase 3 — Design de Hardware   (hardware/08–11 v0.1.0 APROVADO)
  [x] Fase 4 — Arquitetura Firmware (modules/02–07 v0.1.0 APROVADO)
  [x] GATE Formalização             (spec/power, spec/sensor, spec/visual, spec/game, spec/interface MERGEADOS em develop — tag v0.2.0)
  [x] GATE pré-ETAPA 7              (CODING_STANDARD.md v0.1.0 + firmware_constants.json + _config.h gerados — mergeados em develop)
  [ ] Fase 5 — Testes de Módulos
  [ ] Fase 6 — Validação de Hardware
  [ ] Fase 7 — Teste de Integração
  [ ] Fase 8 — Teste de Sistema

Branch ativa: develop (feat/game mergeado FF em 2026-06-28)
Próxima ação: criar branch feat/interface a partir de develop; implementar MOD_WIFI (ETAPA 7, passo 4)

Ordem de implementação dos módulos (ETAPA 7):
  1. MOD_SENSOR  [CONCLUÍDO — feat/sensor] ← commits: a2d2fef, 59b25ea, 9d313ac
  2. MOD_LED     [CONCLUÍDO — feat/visual] ← commit: 48bbe36
  3. MOD_JOGO    [CONCLUÍDO — feat/game]   ← commit: 51246f3
  4. MOD_WIFI    ← depende da interface de MOD_JOGO  ← PRÓXIMO

MOD_JOGO — artefatos criados (branch feat/game):
  - firmware/src/game/game.h: Cor, ParCores, ResultadoJogo, EventoJogo, ConfigSessao; API publica + API de teste
  - firmware/src/game/game.cpp: maquina de estados; Mec A (Fisher-Yates) e Mec B (peso decrescente); logica Modo 1 e Modo 2
  - firmware/test/test_game/test_main.cpp: 11 testes TDD; 11/11 PASSED; cobre CA-04-01..08, CA-04-10
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

Pendências:
  - ETAPA 7: MOD_WIFI restante (TDD — não testável em native; requer hardware)
  - Após MOD_WIFI: pio run → compilação sem erro ou warning
  - tag v0.3.0 ao concluir gate ETAPA 7 (todos os módulos + pio test -e native zero falhas + pio run sem erro)

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
  - pio test -e native: 38/38 PASSED (11 game + 13 sensor + 14 visual) — confirmado em 2026-06-28

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
