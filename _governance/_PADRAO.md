---
documento:    _PADRAO.md
versão:       0.1.0
status:       APROVADO
data:         2026-06-26
depende_de:   nenhum (documento raiz de governança)
impacta:      todos os documentos do projeto [OBRIGATÓRIO]
---

# _PADRAO.md — Padrão de Documentação do Projeto

**Escopo:** Todos os documentos do projeto, sem exceção

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | _PADRAO.md |
| Versão | 1.1.0 |
| Status | APROVADO |
| Escopo | Todos os documentos do projeto sem exceção |
| Pai | Nenhum — documento raiz de governança |

---

## 2. Objetivo <a id="objetivo"></a>

Este documento governa o formato, estrutura, versionamento e rastreabilidade de todos os documentos do projeto. Nenhum documento pode ser criado, alterado ou aprovado sem conformidade com este padrão.

Este é o único documento sem pai. Todos os outros derivam diretamente ou indiretamente de `00_conceito.md`.

---

## 3. Cabeçalho Obrigatório <a id="cabecalho"></a>

Todo documento deve abrir com o seguinte bloco YAML antes de qualquer conteúdo:

```yaml
---
documento:    XX_nome.md
versão:       MAJOR.MINOR.PATCH
status:       RASCUNHO | REVISÃO | APROVADO | OBSOLETO
data:         YYYY-MM-DD
depende_de:
  - 00_conceito.md v0.1.0  [BLOQUEADOR]
impacta:
  - 02_sensor_impacto.md   [OBRIGATÓRIO]
  - 04_logica_jogo.md      [CONDICIONAL: #ancora-da-secao]
---
```

### 3.1 Campo `depende_de` <a id="campo-depende-de"></a>

Lista os documentos dos quais este depende, com a versão mínima exigida e o tipo de vínculo.

### 3.2 Campo `impacta` <a id="campo-impacta"></a>

Lista os documentos que devem ser revisados caso este documento mude. O responsável pela alteração é obrigado a notificar e atualizar os documentos impactados antes de marcar sua própria mudança como APROVADA.

---

## 4. Semântica de Versionamento <a id="versionamento"></a>

```
MAJOR.MINOR.PATCH
```

| Nível | Quando usar | Efeito nos dependentes |
|---|---|---|
| MAJOR | Ruptura de escopo, arquitetura ou requisito fundamental | Todos os dependentes precisam ser revistos e reaprovados |
| MINOR | Adição de requisito, clarificação sem ruptura | Dependentes podem precisar de revisão — avaliar caso a caso |
| PATCH | Correção textual, formatação, erro tipográfico | Dependentes provavelmente não afetados |

**Regra:** nenhum documento passa de RASCUNHO para APROVADO com versão MAJOR diferente do pai declarado em `depende_de`.

---

## 5. Semântica dos Vínculos <a id="vinculos"></a>

| Marcador | Significado |
|---|---|
| `[BLOQUEADOR]` | Este documento só pode ser marcado APROVADO se a versão declarada do pai estiver APROVADA |
| `[OBRIGATÓRIO]` | Qualquer mudança neste documento exige revisão do documento listado em `impacta` |
| `[CONDICIONAL: #ancora]` | Exige revisão do dependente somente se a seção identificada pela âncora for alterada |

---

## 6. Status Permitidos <a id="status"></a>

| Status | Significado |
|---|---|
| `RASCUNHO` | Em elaboração. Não usar como fonte de derivação. |
| `REVISÃO` | Completo, aguardando aprovação formal. |
| `APROVADO` | Fonte confiável. Pode ser citado por documentos filhos. |
| `OBSOLETO` | Substituído por versão mais recente. Não usar. |

**Regra:** documentos filhos só podem declarar status APROVADO se todos os seus `[BLOQUEADOR]` estiverem APROVADOS.

---

## 7. Notações Especiais no Corpo do Documento <a id="notacoes"></a>

Usadas inline para sinalizar pontos que exigem atenção:

| Notação | Significado |
|---|---|
| `[CALIBRAR]` | Valor provisório baseado em literatura. Deve ser confirmado após prototipagem. Registrar no changelog. |
| `[DECIDIR]` | Ponto em aberto que bloqueia derivação. Não pode existir em documento APROVADO. |
| `[ASSUMIDO]` | Decisão tomada sem confirmação explícita. Deve ser validada antes da aprovação. |
| `[VER: XX_doc.md#ancora-id]` | Referência cruzada estável a seção de outro documento. Ver §7.1. |

### 7.1 Convenção de Âncoras para Referências Estáveis <a id="convencao-ancoras"></a>

Todo heading `##` ou `###` que pode ser referenciado por outro documento deve ter uma âncora explícita imediatamente após o texto:

```markdown
## 6. Decoupling <a id="decoupling"></a>
```

**Regras para IDs de âncoras:**
- ASCII minúsculo, sem acentos, palavras separadas por hífen
- Semântico, não posicional: usar `decoupling`, nunca `section-6`
- Único dentro do documento
- **Nunca alterar depois de estabelecido** — quebra todas as referências externas
- Se for necessário alterar, registrar no changelog e atualizar todos os `[VER:]` que apontam para ela

**Formato da referência cruzada:**
```
[VER: 05_alimentacao.md#decoupling]
```

O script `scripts/verify_links.py` verifica automaticamente se toda referência `[VER: file#anchor]` resolve para uma âncora existente.

---

## 8. Estrutura Obrigatória de Seções <a id="estrutura"></a>

Todo documento deve conter, nesta ordem:

```
1. Identificação     → tabela de metadados do documento
2. Objetivo          → o que este documento define e para que serve
3. [Conteúdo]        → seções específicas de cada documento, com <a id="..."> em cada heading
...
N-1. Changelog       → histórico de alterações
N.   Rastreabilidade → vínculos ativos declarados
```

As seções intermediárias variam por tipo de documento, mas abertura e fechamento são sempre iguais.

---

## 9. Formato do Changelog <a id="formato-changelog"></a>

Obrigatório em todos os documentos, ao final, antes de Rastreabilidade:

```markdown
## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | YYYY-MM-DD | — | Criação inicial | Nenhum |
```

---

## 10. Formato da Seção de Rastreabilidade <a id="formato-rastreabilidade"></a>

Obrigatório como última seção de todo documento:

```markdown
## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | 00_conceito.md | 0.1.0 | BLOQUEADOR | #glossario, #modos-operacao |
| Filho | 02_sensor_impacto.md | — | OBRIGATÓRIO | #circuito-protecao |
| Filho | 04_logica_jogo.md | — | CONDICIONAL: #mapeamento-gpios | #algoritmo-deteccao |
```

A coluna "Âncora relevante" usa IDs de âncora em vez de números de seção, mantendo os vínculos estáveis mesmo se a numeração mudar.

---

## 11. Diagramas <a id="diagramas"></a>

| Situação | Formato |
|---|---|
| Fluxos, estados, sequências | Bloco ` ```mermaid ``` ` — renderiza onde suportado, degrada para código legível |
| Árvores de arquivos, tabelas simples, estruturas lineares | ASCII puro — sempre legível |

Nunca usar os dois para o mesmo diagrama no mesmo documento.

---

## 12. Nomenclatura de Arquivos <a id="nomenclatura"></a>

```
00_conceito.md           → documento raiz
01_arquitetura.md        → nível de sistema
02_sensor_impacto.md     → módulo firmware
03_saida_visual.md       → módulo firmware
04_logica_jogo.md        → módulo firmware
05_alimentacao.md        → módulo hardware — alimentação
06_privacidade_lgpd.md   → documento autônomo
07_interface_pedagogo.md → módulo firmware
08_bom.md                → lista de materiais (Bill of Materials)
09_conexoes.md           → esquemático e mapeamento de conexões
10_cablagem.md           → especificação de fios, bitolas, comprimentos
11_montagem.md           → instruções de montagem física e teste
_PADRAO.md               → este documento (prefixo _ = meta)
```

Prefixo numérico indica ordem de derivação. Prefixo `_` indica documento de governança.

---

## 13. Scripts de Verificação <a id="scripts"></a>

O diretório `scripts/` contém ferramentas que verificam automaticamente a integridade da documentação. Rodar antes de qualquer aprovação de documento:

```bash
python scripts/run_all.py
```

| Script | O que verifica |
|---|---|
| `verify_links.py` | Todos os `[VER: file#anchor]` resolvem para âncoras existentes |
| `check_versions.py` | Versões em `depende_de` batem com versões reais dos documentos |
| `check_impacta.py` | Simetria bidirecional: A impacta B ↔ B declara A como Pai |
| `run_all.py` | Executa os três em sequência, retorna exit code 1 se houver falha |

O GitHub Action em `.github/workflows/verify-docs.yml` executa `run_all.py` automaticamente em todo push ou PR que altere arquivos `.md`.

Forward references (referências a documentos ainda não criados) geram aviso, não erro.

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-26 | — | Criação inicial: padrão de documentação, convenção de âncoras, scripts de verificação e GitHub Action | Todos os documentos do projeto |

---

## Rastreabilidade

Este documento não tem pai. É a raiz do padrão.

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Governado | Todos os `.md` do projeto | — | OBRIGATÓRIO | — |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
