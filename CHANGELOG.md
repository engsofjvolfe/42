# CHANGELOG

Historico de releases do projeto. Atualizado a cada tag de versao.

Formato: baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.0.0/).
Versionamento: [SemVer](https://semver.org/lang/pt-BR/).

---

## [Nao lancado]

### Adicionado
- `_governance/CODING_STANDARD.md` v0.1.0 — padrao de codigo firmware derivado de `01_arquitetura.md` v0.1.0 e de todos os specs v0.2.0; secoes derivadas geradas automaticamente por script
- `spec/firmware_constants.json` — mapeamento canonico de 47 constantes DERIVADO e 3 HARDCODED_JUSTIFICADO; liga cada constante C++ ao campo JSON de origem no spec correspondente
- `spec/firmware_constants.schema.json` — schema JSON (draft/2020-12) para validacao de firmware_constants.json
- `scripts/generate_coding_standard.py` — gera secoes derivadas de CODING_STANDARD.md a partir de firmware_constants.json; valida cascata (valores spec vs valor mapeado) em modo --check
- `firmware/src/sensor/sensor_config.h`, `firmware/src/visual/visual_config.h`, `firmware/src/game/game_config.h`, `firmware/src/interface/interface_config.h` — gerados por `generate_coding_standard.py`; cascata completa de ponta a ponta (spec JSON → firmware_constants.json → _config.h); compilaveis em native (constantes com tipo de plataforma emitidas como stub `[PLATAFORMA]`)

### Alterado
- `scripts/run_all.py`: adiciona 4 check — `generate_coding_standard.py --check` (cascata specs -> CODING_STANDARD.md)
- `scripts/generate_coding_standard.py`: estende para gerar `_config.h` em modo generate e verificar sync dos arquivos em modo --check; suporte a flag `tipo_plataforma` em DMs — emite stub comentado `[PLATAFORMA]` em vez de declaracao C++ para tipos que requerem headers de plataforma
- `spec/firmware_constants.json`: adiciona `tipo_plataforma: true` em DM-02 (`adc_atten_t`) e DM-03 (`EOrder`)
- `spec/firmware_constants.schema.json`: aceita campo opcional `tipo_plataforma: boolean` em `decisoes_manuais_inerentes`

### Corrigido
- `scripts/verify_links.py`: corrige varredura de `.md` de `glob('*.md')` (so raiz) para `rglob` recursivo com registry `{nome: path}` — scripts nao verificavam nenhum documento em subdiretorios
- `scripts/check_impacta.py`: mesma correcao de varredura recursiva com registry
- `scripts/check_versions.py`: mesma correcao de varredura recursiva com registry
- `SESSION_STATE.md`: corrige premissa falsa registrada implicitamente — `generate_coding_standard.py` JA gera `_config.h` (linhas 414-421 do script) e JA verifica sync em modo --check; `run_all.py` confirma `OK: 4 arquivos _config.h em sync`; premissa de que "o script nao tem logica de saida para firmware/src/" e falsa e nao deve ser re-analisada em sessoes futuras

---

## [0.2.0] — 2026-06-28

### Adicionado
- `spec/power/power.json` — spec derivada de `05_alimentacao.md` v0.1.0
- `spec/power/power.schema.json` — schema de validacao do power spec
- `spec/sensor/sensor.json` — spec derivada de `02_sensor_impacto.md` v0.1.0
- `spec/sensor/sensor.schema.json` — schema de validacao do sensor spec
- `spec/visual/visual.json` — spec derivada de `03_saida_visual.md` v0.1.0
- `spec/visual/visual.schema.json` — schema de validacao do visual spec
- `spec/game/game.json` — spec derivada de `04_logica_jogo.md` v0.1.0
- `spec/game/game.schema.json` — schema de validacao do game spec
- `spec/interface/interface.json` — spec derivada de `07_interface_pedagogo.md` v0.1.0
- `spec/interface/interface.schema.json` — schema de validacao do interface spec
- `README.md` — documentacao de entrada do repositorio
- Script de automacao de versionamento (`_governance/scripts/bump_version.py`)
- `CHANGELOG.md` (este arquivo)

### Alterado
- `CLAUDE.md` ETAPA 6: adiciona regra obrigatoria de campos sem narrativa nos arquivos JSON
- `CLAUDE.md` retorno de sessao: referencia SESSION_STATE.md antes de ir para ETAPA 3
- `CLAUDE.md` ETAPA 5 checklist: adiciona meta-analise de cadeia como item pre-commit
- `CLAUDE.md` ETAPA 6 procedimento: adiciona passo 0 (ler SESSION_STATE), passos 9-11 (merge, proxima branch, atualizar SESSION_STATE)
- `CLAUDE.md` ETAPA 5: padroniza corpo de mensagem de commit (obrigatorio para spec/feat/fix/docs) com modelos e regra heredoc
- `CLAUDE.md` ETAPA 5: adiciona Protocolo CHANGELOG com arvore de decisao e gate imediato pre-commit

### Corrigido
- Scripts de verificacao movidos de `_governance/scripts/` para `scripts/` (alinhamento com CLAUDE.md e CI)
- `scripts/run_all.py`: corrige UnicodeEncodeError no Windows ao imprimir caracteres UTF-8
- `_governance/_PADRAO.md`: corrige versao de 1.1.0 para 0.1.0 (numero incorreto introduzido no HEAD de develop)

---

## [0.1.0] — 2026-06-27

### Adicionado
- Commit inicial: V-model Niveis 0 a 2 aprovados
  - `concept/00_conceito.md` v0.1.0
  - `system/01_arquitetura.md` v0.1.0
  - `modules/sensor/02_sensor_impacto.md` v0.1.0
  - `modules/visual/03_saida_visual.md` v0.1.0
  - `modules/game/04_logica_jogo.md` v0.1.0
  - `modules/power/05_alimentacao.md` v0.1.0
  - `compliance/06_privacidade_lgpd.md` v0.1.0
  - `modules/interface/07_interface_pedagogo.md` v0.1.0
  - `hardware/08_bom.md` v0.1.0
  - `hardware/09_conexoes.md` v0.1.0
  - `hardware/10_cablagem.md` v0.1.0
  - `hardware/11_montagem.md` v0.1.0
  - `_governance/_PADRAO.md` v0.1.0
- Protocolo V-model de engenharia (`v_model_protocol/`)
- Padroes de governanca (`_governance/`)
- Scripts de verificacao de documentacao (`scripts/`)
- Pipeline CI/CD (`.github/workflows/verify-docs.yml`)
- Licenca GPL-3.0

---

## Proximas releases planejadas

| Tag | Conteudo |
|---|---|
| v0.3.0 | Firmware MVP — todos os modulos implementados (ETAPA 7) |
| v1.0.0 | V-model fechado — validacao completa (ETAPA 8) |
