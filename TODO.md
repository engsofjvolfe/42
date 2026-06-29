# TODO — Tarefas futuras

Tarefas identificadas mas não executadas na sessão corrente.
Cada item indica onde o trabalho acontece e qual é o critério de conclusão.

---

## Metadocumentos e índices de navegação

**Contexto:** o README raiz é o ponto de entrada do repositório. Dentro de cada
diretório com múltiplos artefatos, a navegação depende de conhecer os nomes dos
arquivos. Alguns diretórios precisam de um `INDEX.md` (ou `README.md` de diretório)
que liste e contextualize o que está ali, com o mesmo papel que o índice do
README raiz exerce para o projeto inteiro.

**Diretórios candidatos a index (avaliar na execução):**

| Diretório | Motivo |
|---|---|
| `modules/` | 5 subdiretórios, cada um com um doc de módulo |
| `spec/` | 5 subdiretórios, cada um com `.json` + `.schema.json` |
| `hardware/` | 4 documentos (08–11) em sequência de dependência |
| `_governance/` | 4 padrões com relações entre si |

**Critério de conclusão por diretório:**
- Um arquivo de índice que liste os artefatos com uma linha de contexto cada
- Links corretos validados por `verify_links.py` (se o arquivo usar `[VER:]`)
- Ou simplesmente links relativos simples (não requerem verificação pelo script)

**Quando executar:** após ETAPA 8 (validação com hardware), antes de v1.0.0.

---
