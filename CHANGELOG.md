# CHANGELOG

Historico de releases do projeto. Atualizado a cada tag de versao.

Formato: baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.0.0/).
Versionamento: [SemVer](https://semver.org/lang/pt-BR/).

---

## [Nao lancado]

### Adicionado
- Documentacao completa do V-model: conceito, arquitetura, modulos, hardware
- Protocolo V-model de engenharia (`v_model_protocol/`)
- Padroes de governanca (`_governance/`)
- Scripts de verificacao de documentacao (`scripts/`)
- Pipeline CI/CD (`.github/workflows/verify-docs.yml`)
- Licenca GPL-3.0
- `CHANGELOG.md` (este arquivo)
- Script de automacao de versionamento (`_governance/scripts/bump_version.py`)
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

### Alterado
- `CLAUDE.md` ETAPA 6: adiciona regra obrigatoria de campos sem narrativa nos arquivos JSON
- `CLAUDE.md` retorno de sessao: referencia SESSION_STATE.md antes de ir para ETAPA 3
- `CLAUDE.md` ETAPA 5 checklist: adiciona meta-analise de cadeia como item pre-commit
- `CLAUDE.md` ETAPA 6 procedimento: adiciona passo 0 (ler SESSION_STATE), passos 9-11 (merge, proxima branch, atualizar SESSION_STATE)

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

---

## Proximas releases planejadas

| Tag | Conteudo |
|---|---|
| v0.2.0 | JSON Spec e Schema de todos os 5 modulos (ETAPA 6) |
| v0.3.0 | Firmware MVP — todos os modulos implementados (ETAPA 7) |
| v1.0.0 | V-model fechado — validacao completa (ETAPA 8) |
