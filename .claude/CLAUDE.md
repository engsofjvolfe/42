# CLAUDE.md

Este documento é um guia linear de procedimentos.
Leia uma etapa. Execute a ação. Valide. Só então leia a próxima.
Nunca leia à frente antes de validar a etapa atual.

---

## Como saber em qual etapa você está

Se é a primeira vez neste repositório: comece na **ETAPA 1**.

Se está retornando:
- `git status` mostra branch de trabalho ativa → ler `SESSION_STATE.md` → **ETAPA 3**
- Nenhuma branch ativa, mas `run_all.py` passa → **ETAPA 3**
- `run_all.py` com erros → **ETAPA 2**

---

## ETAPA 1 — Orientação

Este projeto é um instrumento lúdico-pedagógico com ESP32 para avaliação de
coordenação motora em crianças de 5 anos. O repositório segue o V-model de
engenharia: toda linha de firmware é rastreável a um documento de especificação,
que é rastreável ao documento de conceito.

**Posição atual no V-model:**

```
[x] Conceito             → concept/00_conceito.md
[x] Arquitetura          → system/01_arquitetura.md
[x] Especificacao modulos → modules/*
[x] Especificacao hardware → hardware/*
[ ] JSON Spec + Schema   → spec/* (a criar)
[ ] Firmware             → firmware/* (a criar)
[ ] Testes               → tests/* (a criar)
[ ] Validacao            → fecha o V-model
```

A documentação está completa e aprovada. O próximo trabalho é `spec/`.

**Gate:** Leia esta etapa uma vez. Avance para a ETAPA 2.

---

## ETAPA 2 — Verificação do estado do repositório

**Ação:**

```bash
python scripts/run_all.py
```

**Validação:**

```
TODOS OS CHECKS PASSARAM
```

Se houver erros: corrija-os agora. Não avance com erros.

Os três checks verificam:
- `verify_links.py` — todo `[VER: file#anchor]` resolve para âncora que existe
- `check_versions.py` — versões em `depende_de` batem com versões reais dos docs
- `check_impacta.py` — se A declara B em `impacta`, B declara A como `Pai`

**Gate:** `run_all.py` retorna exit code 0. Avance para a ETAPA 3.

---

## ETAPA 3 — Leitura obrigatória antes de qualquer trabalho

**Ação — leia nesta ordem, integralmente:**

1. `v_model_protocol/arquivo2-protocolo-ia.md` — protocolo de processo V-model ativo
2. `concept/00_conceito.md` — fonte única de verdade do projeto
3. O documento pai da tarefa planejada (ex: `01_arquitetura.md` para firmware)

O `arquivo2-protocolo-ia.md` define o processo de trabalho que rege toda a sessão:
as 5 camadas (Captura → Derivação), o formato obrigatório de toda decisão técnica,
as restrições absolutas (sem magic numbers, sem condições ideais, sem valores vagos)
e os gates de cada fase do V-model. Leia-o primeiro, antes de qualquer documento
de conteúdo. Se algo na sessão conflitar com o protocolo: o protocolo prevalece.

Leitura completa não é análise. Ao ler cada seção, verifique:
- Esta âncora `[VER: file#anchor]` existe no arquivo referenciado?
- O conteúdo desta seção é consistente com o que o documento pai afirma?
- Existe algum `[DECIDIR]` não resolvido? Se sim, pare e resolva antes de derivar.

**Validação:** você consegue responder, sem consultar o arquivo:
- Quais são os 4 módulos de firmware e suas responsabilidades?
- Quais GPIOs são usados para os piezos e por quê?
- Qual é a diferença entre Timing A e Timing B?
- Quais são as 5 camadas do processo e o que é proibido em cada uma?

Se não consegue responder: releia. Não avance por memória.

**Gate:** Leitura completa e verificada. Avance para a ETAPA 4.

---

## ETAPA 4 — Regras de trabalho

Estas regras aplicam-se a toda sessão. Leia uma vez. Aplique sempre.

### Derivação

Todo artefato (documento, spec, código) deriva exclusivamente dos documentos
declarados em `depende_de`. Nada é inventado ou inferido.

Se a informação existe em outro documento: use `[VER: file#anchor]`.
Se não existe em nenhum documento pai: não pertence a este artefato.

### Âncoras

Toda seção `##` ou `###` que pode ser referenciada leva âncora explícita:

```markdown
## 6. Decoupling <a id="decoupling"></a>
```

IDs: minúsculo, sem acentos, hífen entre palavras. Semântico: `#decoupling`,
nunca `#secao-6`. Uma vez criado, nunca alterar sem atualizar todos os `[VER:]`
que apontam para ele.

### Atualização de documentos e cascata de dependências

Quando um documento é alterado, o procedimento é:

```
1. Identificar qual seção mudou
2. Ler o campo impacta do documento alterado
3. Para cada documento B em impacta:
   a. Ler B integralmente
   b. Identificar quais seções de B referenciam a seção alterada
   c. Atualizar B onde necessário
   d. Registrar no Changelog de B
   e. Bumpar versão de B (PATCH se correção, MINOR se adição)
   f. Verificar se B também tem impacta → repetir recursivamente
4. Registrar no Changelog do documento original
5. Bumpar versão do documento original
6. Executar run_all.py → zero erros antes de qualquer commit
```

Nenhuma mudança é completa até que a cascata inteira esteja resolvida e
`run_all.py` passe.

### Versionamento dos documentos

| Mudança | Nível |
|---|---|
| Ruptura: escopo, arquitetura, requisito fundamental | MAJOR |
| Adição: nova seção, novo requisito, clarificação | MINOR |
| Correção: texto, link, âncora, formatação | PATCH |

Nenhum documento passa de RASCUNHO para APROVADO enquanto qualquer `[BLOQUEADOR]`
em `depende_de` não estiver APROVADO na versão declarada.

Nenhum documento com `[DECIDIR]` no corpo pode ser marcado APROVADO.

### Status dos documentos

```
RASCUNHO  → em elaboração. Não usar como fonte de derivação.
REVISÃO   → completo, aguardando validação.
APROVADO  → único status que autoriza derivação.
OBSOLETO  → substituído. Não usar.
```

**Gate:** Estas regras estão claras. Avance para a ETAPA 5.

---

## ETAPA 5 — Contrato Git

### Branches

```
main      ← apenas releases tagueadas. Push direto: proibido.
develop   ← integração. Merge só com CI verde.
  ├── docs/descrição-curta
  ├── feat/nome-do-modulo
  ├── fix/descrição-curta
  ├── spec/nome-do-modulo
  ├── test/nome-do-modulo
  └── chore/descrição-curta
```

Fluxo obrigatório: `branch de trabalho → develop → main`.
Merge para `develop` exige CI verde. Merge para `main` exige tag de versão.

### Conventional Commits

Formato obrigatório:

```
<tipo>(<escopo>): <descrição no imperativo, em português>

<corpo: o que mudou e por quê>
```

Tipos:

| Tipo | Quando |
|---|---|
| `feat` | Nova funcionalidade |
| `fix` | Correção de bug |
| `docs` | Alteração em `.md` |
| `spec` | JSON spec ou schema |
| `test` | Testes |
| `build` | Build system, platformio.ini |
| `ci` | GitHub Actions |
| `chore` | Manutenção sem impacto funcional |
| `calibrate` | Ajuste de parâmetro `[CALIBRAR]` após prototipagem |

Escopos: `sensor`, `visual`, `game`, `power`, `interface`, `bom`, `conexoes`,
`cablagem`, `montagem`, `conceito`, `arquitetura`, `padrao`, `scripts`, `ci`.

Proibido:
```
git commit -m "fix"
git commit -m "atualização"
git commit -m "wip"
git commit -m "Co-authored-by: ..."
```

A linha `Co-authored-by` não é incluída em nenhum commit deste repositório.

### Corpo da mensagem — regras obrigatórias

O corpo é **obrigatório** para os tipos abaixo. Omiti-lo é erro de protocolo.

| Tipo | Corpo obrigatório | O que incluir no corpo |
|---|---|---|
| `spec` | SIM | Documento fonte (nome + versão), campos derivados relevantes, CAs cobertos |
| `feat` | SIM | Funcionalidade implementada, spec que a origina, módulo afetado |
| `fix` | SIM | O que estava errado, causa raiz, o que foi corrigido |
| `docs` | SIM, se altera regra ou procedimento | A regra que mudou e por quê |
| `chore` | NÃO, se o assunto for completamente autoexplicativo | — |
| `test` | SIM | Cenário testado, CA coberto, resultado esperado |

Modelo para `spec`:
```
spec(sensor): cria spec e schema derivados de 02_sensor_impacto.md v0.1.0

Derivados de 02_sensor_impacto.md v0.1.0 (APROVADO) e spec/power/power.json.
sensor.json: pinos GPIO, thresholds, janela de deteccao, decoupling,
criterios de aceitacao CA-02-*.
sensor.schema.json: schema JSON (draft/2020-12) para validacao automatica.
```

Modelo para `fix`:
```
fix(padrao): corrige versao de _PADRAO.md de 1.1.0 para 0.1.0

Versao 1.1.0 foi introduzida incorretamente. O documento nao passou
por nenhuma alteracao que justificasse bumpar de 0.1.0; versao correta
e 0.1.0 conforme commit inicial aprovado.
```

Sempre usar `git commit` com heredoc para garantir o formato:
```bash
git commit -m "$(cat <<'EOF'
tipo(escopo): descricao no imperativo

Corpo explicando o que mudou e por que.
Segunda linha do corpo se necessario.
EOF
)"
```

### Commits atômicos

**Um commit = uma razão lógica de mudança.**

Use commit único quando a mudança é inseparável:
```bash
# Exemplo: renomear âncora afeta 5 arquivos — uma razão, um commit
git commit -m "docs(sensor): renomeia âncora #circuito para #circuito-protecao"
```

Use commits separados quando as mudanças são independentes:
```bash
# Exemplo: cascata de dependências — cada documento tem sua razão
git commit -m "docs(conceito): adiciona regra de isolamento entre zonas"
git commit -m "docs(sensor): atualiza isolamento conforme conceito v0.3.0"
git commit -m "docs(conexoes): atualiza gap mínimo conforme sensor v0.2.0"
```

Nunca misture tipos em um commit:
```bash
# Proibido
git commit -m "fix e docs: corrige sensor e atualiza conceito"

# Correto: dois commits
git commit -m "fix(sensor): corrige polaridade do Zener no diagrama"
git commit -m "docs(sensor): atualiza changelog versão 0.1.1"
```

### Protocolo CHANGELOG — quando e o que atualizar

O CHANGELOG é atualizado **antes** de escrever a mensagem de commit.
Nunca após. Nunca via amend por esquecimento.

**Decisão obrigatória antes de qualquer commit:**

```
1. Este commit cria ou altera um artefato de especificação
   (.json, .schema.json, .md de módulo)?
   → SIM: adicionar em ### Adicionado ou ### Alterado

2. Este commit corrige um erro em artefato existente?
   → SIM: adicionar em ### Corrigido

3. Este commit é chore (SESSION_STATE, CI interno, reorganização)?
   → Verificar: o chore altera comportamento ou estrutura visível?
     → SIM: adicionar em ### Alterado
     → NÃO: CHANGELOG não é necessário para este commit

4. Qualquer dúvida sobre se o CHANGELOG precisa ser atualizado?
   → Atualizar. Custo de entrada desnecessária é zero.
   → Custo de entrada faltando é amend + reescrita de histórico.
```

**Commits que nunca dispensam atualização de CHANGELOG:**

| Tipo | Seção no CHANGELOG |
|---|---|
| `spec(*)` — criação | `### Adicionado` |
| `spec(*)` — alteração | `### Alterado` |
| `feat(*)` | `### Adicionado` |
| `fix(*)` | `### Corrigido` |
| `docs(*)` — altera regra/procedimento | `### Alterado` |

**Formato das entradas:**

```markdown
- `spec/sensor/sensor.json` — spec derivada de `02_sensor_impacto.md` v0.1.0
- `spec/sensor/sensor.schema.json` — schema de validacao do sensor spec
- `_governance/_PADRAO.md`: corrige versao de 1.1.0 para 0.1.0 (descricao do erro)
```

**Gate imediato antes do commit:**

```
□ Abri o CHANGELOG.md antes de escrever a mensagem de commit?
□ Identifiquei qual seção (Adicionado / Alterado / Corrigido) corresponde?
□ A entrada foi adicionada e salva?
□ git diff --staged inclui CHANGELOG.md (se aplicável)?
```

Se qualquer resposta for NÃO: atualizar CHANGELOG antes de continuar.

### Tags e releases

```bash
git tag -a v0.1.0 -m "docs: documentação completa — V-model Níveis 0–2 aprovados"
git tag -a v0.2.0 -m "spec: JSON spec e schema de todos os módulos"
git tag -a v0.3.0 -m "feat: firmware MVP — todos os módulos implementados"
git tag -a v1.0.0 -m "release: V-model fechado — validação completa"
```

### Checklist antes de qualquer commit

```
□ run_all.py retorna zero erros
□ git diff --staged revisado linha a linha
□ Protocolo CHANGELOG executado (ver seção acima) — CHANGELOG.md atualizado se aplicável
□ Versões bumpadas onde necessário
□ Tipo e escopo do commit corretos
□ Corpo da mensagem presente (obrigatório para spec, feat, fix, docs — ver seção acima)
□ Mensagem escrita com heredoc (git commit -m "$(cat <<'EOF' ... EOF)")
□ Sem Co-authored-by
□ Meta-análise de cadeia: cada etapa foi seguida na ordem correta? (executar ANTES do commit)
```

**Gate:** Contrato Git claro. Crie a branch de trabalho agora. Avance para ETAPA 6.

---

## ETAPA 6 — Tarefa atual: JSON Spec e Schema

Esta é a próxima fase do V-model. Cada módulo recebe dois artefatos derivados
dos documentos de especificação já aprovados.

### Ordem de criação (respeitar dependências)

```
1. spec/power/      ← deriva de docs/modules/power/05_alimentacao.md
2. spec/sensor/     ← deriva de spec/power/ + docs/modules/sensor/02_sensor_impacto.md
3. spec/visual/     ← deriva de spec/power/ + docs/modules/visual/03_saida_visual.md
4. spec/game/       ← deriva de spec/sensor/ + spec/visual/ + docs/modules/game/04_logica_jogo.md
5. spec/interface/  ← deriva de spec/game/ + docs/modules/interface/07_interface_pedagogo.md
```

### Por módulo, criar:

```
spec/<modulo>/
├── <modulo>.json          ← especificação de dados e parâmetros
└── <modulo>.schema.json   ← schema JSON para validação
```

### Regra obrigatória para o .json

Todo campo deve conter um dado utilizável diretamente em código.
Texto narrativo ou descritivo é proibido em qualquer valor.

```
ERRADO:  "tipo": "saída do regulador buck"
CORRETO: "tensao_saida_alvo_v": 5.0

ERRADO:  "descricao": "capacitor de bulk na entrada do LM2596"
CORRETO: "lm2596_in": {"capacitancia_uf": 100, "tensao_v": 25, "tipo": "electrolitico", "quantidade": 1}
```

Se um valor exige interpretação humana para extrair um dado: está errado.

### Procedimento por módulo

```
0. Ao retomar sessão: ler SESSION_STATE.md → identificar módulos já criados → começar pelo próximo
1. Ler o documento .md do módulo integralmente (ETAPA 3 se não fez ainda)
2. Ler os specs dos módulos pai (se existirem)
3. Criar <modulo>.json derivando dos campos dos documentos .md
4. Criar <modulo>.schema.json derivando do .json
5. Validar o .json contra o .schema com: python -c "import jsonschema, json;
   jsonschema.validate(json.load(open('<modulo>.json')),
   json.load(open('<modulo>.schema.json'))); print('OK')"
6. run_all.py → zero erros
7. Executar checklist da ETAPA 5 (incluindo meta-análise) antes de commitar
8. Commit: spec(<modulo>): cria spec e schema derivados de <doc>.md v<X.Y.Z>
9. Merge branch spec/<modulo> → develop
10. Criar branch spec/<proximo-modulo> a partir de develop
11. Atualizar SESSION_STATE.md na nova branch
```

### Gate desta etapa

```
□ spec/ de todos os 5 módulos criados
□ Cada .json valida contra seu .schema sem erros
□ run_all.py → zero erros
□ Commits feitos para cada módulo separadamente
□ Tag: git tag -a v0.2.0 -m "spec: JSON spec e schema de todos os módulos"
```

**Quando o gate passar: avance para ETAPA 7.**

---

## ETAPA 7 — Tarefa: Firmware

### Pré-condição

ETAPA 6 completa e tagueada como `v0.2.0`.

### Estrutura

```
firmware/
├── platformio.ini
├── src/
│   ├── main.cpp
│   ├── sensor/        ← MOD_SENSOR — deriva de spec/sensor/
│   ├── visual/        ← MOD_LED    — deriva de spec/visual/
│   ├── game/          ← MOD_JOGO   — deriva de spec/game/
│   └── interface/     ← MOD_WIFI   — deriva de spec/interface/
└── test/
    ├── test_sensor/
    ├── test_visual/
    └── test_game/     ← MOD_WIFI não é testável em native
```

### Ordem de implementação

```
1. MOD_SENSOR  ← sem dependência de outros módulos
2. MOD_LED     ← sem dependência de outros módulos
3. MOD_JOGO    ← depende das interfaces de MOD_SENSOR e MOD_LED
4. MOD_WIFI    ← depende da interface de MOD_JOGO
```

### Por módulo, o procedimento é

```
1. Ler spec/<modulo>/<modulo>.json e .schema
2. Ler os Critérios de Aceitação do documento .md correspondente
   (seção #criterios-aceitacao de cada módulo)
3. Escrever o teste antes da implementação (TDD)
4. Implementar o módulo
5. pio test -e native → testes passam
6. Commit: feat(<modulo>): implementa <função> derivada de spec/<modulo>.json
```

### Referências normativas ativas

| Norma | Onde se aplica |
|---|---|
| ABNT NBR ISO 8124-1 | Segurança física — 11_montagem.md |
| ABNT NBR ISO 8124-2 | Inflamabilidade — 11_montagem.md |
| IEC 62115 | Segurança elétrica — 09_conexoes.md, 11_montagem.md |
| ISO/IEC 25010 | Qualidade de software — RNFs em 01_arquitetura.md |
| LGPD Lei 13.709/2018 | Dados de crianças — 06_privacidade_lgpd.md |

### Gate desta etapa

```
□ Todos os módulos implementados
□ pio test -e native → zero falhas nos módulos testáveis
□ pio run → compilação sem erro ou warning
□ Tag: git tag -a v0.3.0 -m "feat: firmware MVP implementado"
```

**Quando o gate passar: avance para ETAPA 8.**

---

## ETAPA 8 — Tarefa: Validação (fecha o V-model)

### Pré-condição

ETAPA 7 completa e tagueada como `v0.3.0`.

### O que validar

Cada Critério de Aceitação nos documentos de módulo é um teste verificável:

```
CA-01-* em 01_arquitetura.md#criterios-aceitacao → testes de sistema
CA-02-* em 02_sensor_impacto.md#criterios-aceitacao → MOD_SENSOR
CA-03-* em 03_saida_visual.md#criterios-aceitacao  → MOD_LED
CA-04-* em 04_logica_jogo.md#criterios-aceitacao   → MOD_JOGO
CA-05-* em 05_alimentacao.md#criterios-aceitacao   → hardware de alimentação
CA-07-* em 07_interface_pedagogo.md#criterios-aceitacao → MOD_WIFI
CA-09-* em 09_conexoes.md#criterios-aceitacao      → hardware de conexões
CA-10-* em 10_cablagem.md#criterios-aceitacao      → cablagem física
```

### Procedimento

```
1. Para cada CA-XX-NN: executar o teste descrito no documento
2. Registrar resultado: PASSOU / FALHOU
3. Se FALHOU: corrigir na branch fix/<ca-id>, commitar, re-testar
4. Se PASSOU: marcar no checklist de validação
5. Quando todos os CAs passarem: run_all.py → zero erros
```

### Gate final

```
□ Todos os CAs verificados e documentados
□ run_all.py → zero erros
□ git tag -a v1.0.0 -m "release: V-model fechado — validação completa"
□ Merge develop → main
```

**V-model fechado. Projeto em v1.0.0.**

---

## CI/CD

O pipeline `.github/workflows/verify-docs.yml` executa em todo push e PR:

```
push ou PR em .md ou scripts/
  └── python scripts/run_all.py
        ├── verify_links.py    → [VER: file#anchor] resolve
        ├── check_versions.py  → versões em depende_de corretas
        └── check_impacta.py   → simetria impacta ↔ Rastreabilidade
```

Quando `firmware/` existir, adicionar ao workflow:

```yaml
- run: pip install platformio
- run: cd firmware && pio run
- run: cd firmware && pio test -e native
```

Nenhum merge para `develop` ou `main` com CI vermelho.