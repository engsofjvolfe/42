---
# ANCORAGEM DE SESSÃO — arquivo2-protocolo-ia.md PARTE 5
# Atualizar ao encerrar cada sessão. Enviar junto com arquivo2 ao retomar.
---

CONTEXTO: continuando projeto Instrumento Ludico-Pedagogico ESP32 — 2026-06-27
Processo: V-Model (ISO 26262 / IEC 61508 / IEC 62304)

Camada atual: FORMALIZAÇÃO
Fase V-Model atual: GATE — Formalização: JSON Spec e Schema (ETAPA 6 do CLAUDE.md)

Fases concluídas e aprovadas:
  [x] Fase 1 — Requisitos          (concept/00_conceito.md v0.1.0 APROVADO)
  [x] Fase 2 — Arquitetura Sistema  (system/01_arquitetura.md v0.1.0 APROVADO)
  [x] Fase 3 — Design de Hardware   (hardware/08–11 v0.1.0 APROVADO)
  [x] Fase 4 — Arquitetura Firmware (modules/02–07 v0.1.0 APROVADO)
  [~] GATE Formalização             (spec/power MERGEADO em develop — 4 módulos pendentes)
  [ ] Fase 5 — Testes de Módulos
  [ ] Fase 6 — Validação de Hardware
  [ ] Fase 7 — Teste de Integração
  [ ] Fase 8 — Teste de Sistema

Branch ativa: develop
Próxima ação: criar branch spec/sensor a partir de develop, depois criar spec/sensor/sensor.json + sensor.schema.json

Ordem obrigatória dos módulos restantes (ETAPA 6):
  2. spec/sensor/  ← deriva de spec/power/ + 02_sensor_impacto.md
  3. spec/visual/  ← deriva de spec/power/ + 03_saida_visual.md
  4. spec/game/    ← deriva de spec/sensor/ + spec/visual/ + 04_logica_jogo.md
  5. spec/interface/ ← deriva de spec/game/ + 07_interface_pedagogo.md

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
  - spec/sensor/sensor.json + sensor.schema.json
  - spec/visual/visual.json + visual.schema.json
  - spec/game/game.json + game.schema.json
  - spec/interface/interface.json + interface.schema.json
  - tag v0.2.0 ao concluir gate ETAPA 6 (todos os 5 módulos mergeados em develop)

Desvios da sessão anterior (registrados para não repetir):
  - Commits em main antes de criar develop + branch de trabalho
  - CHANGELOG não atualizado antes dos commits spec(power) e docs(padrao)
  - Gate ETAPA 3 não validado com as 4 perguntas antes de avançar
  - Meta-análise executada após entrega (deve ser pré-commit)
