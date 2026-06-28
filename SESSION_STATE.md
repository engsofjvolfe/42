---
# ANCORAGEM DE SESSÃO — arquivo2-protocolo-ia.md PARTE 5
# Atualizar ao encerrar cada sessão. Enviar junto com arquivo2 ao retomar.
---

CONTEXTO: continuando projeto Instrumento Ludico-Pedagogico ESP32 — 2026-06-27
Processo: V-Model (ISO 26262 / IEC 61508 / IEC 62304)

Camada atual: DERIVAÇÃO
Fase V-Model atual: ETAPA 7 — Firmware

Fases concluídas e aprovadas:
  [x] Fase 1 — Requisitos          (concept/00_conceito.md v0.1.0 APROVADO)
  [x] Fase 2 — Arquitetura Sistema  (system/01_arquitetura.md v0.1.0 APROVADO)
  [x] Fase 3 — Design de Hardware   (hardware/08–11 v0.1.0 APROVADO)
  [x] Fase 4 — Arquitetura Firmware (modules/02–07 v0.1.0 APROVADO)
  [x] GATE Formalização             (spec/power, spec/sensor, spec/visual, spec/game, spec/interface MERGEADOS em develop — tag v0.2.0)
  [ ] Fase 5 — Testes de Módulos
  [ ] Fase 6 — Validação de Hardware
  [ ] Fase 7 — Teste de Integração
  [ ] Fase 8 — Teste de Sistema

Branch ativa: develop (gate ETAPA 6 concluído, pronto para ETAPA 7)
Próxima ação: criar branch feat/sensor a partir de develop → ETAPA 7 (Firmware), começando por MOD_SENSOR

Ordem de implementação dos módulos (ETAPA 7):
  1. MOD_SENSOR  ← sem dependência de outros módulos  ← PRÓXIMO
  2. MOD_LED     ← sem dependência de outros módulos
  3. MOD_JOGO    ← depende das interfaces de MOD_SENSOR e MOD_LED
  4. MOD_WIFI    ← depende da interface de MOD_JOGO

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

Pendências:
  - ETAPA 7: Firmware (4 módulos, TDD, pio test -e native)
  - tag v0.3.0 ao concluir gate ETAPA 7

Desvios desta sessão (registrados para não repetir):
  - Nenhum desvio de protocolo identificado na meta-análise

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
