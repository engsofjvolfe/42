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

## Interface do pedagogo — defeito conhecido e melhorias (2026-07-03)

Achados da validação com hardware físico (ETAPA 8). Cada item em branch
própria, derivando de `07_interface_pedagogo.md` (que deve ser atualizado
primeiro, com cascata, para os itens que criam requisito novo).

### Defeito — bloqueia o gate v1.0.0

| # | Item | Tipo | Onde | Critério de conclusão |
|---|---|---|---|---|
| D1 | Exportação CSV não funciona | fix | `fix/export-csv` — `firmware/src/interface/interface.cpp` (HTML/JS embutido) | CA-07-* de exportação PASSOU: arquivo CSV com cabeçalho baixado e aberto com dados corretos da sessão |

**Regra do protocolo:** nenhuma tag v1.0.0 enquanto D1 não passar — o gate da
ETAPA 8 exige todos os CAs aprovados.

### Melhorias de produto (requisito novo → atualizar 07_interface_pedagogo.md antes)

| # | Item | Motivação | Critério de conclusão |
|---|---|---|---|
| M1 | Botão de encerrar sessão e iniciar nova sessão | Hoje não há caminho explícito na interface para encerrar e recomeçar sem recarregar/reiniciar | Pedagogo encerra sessão ativa e inicia nova sem recarregar a página nem reiniciar o ESP32 |
| M2 | Visualização do CSV na própria interface | Pedagogo precisa conferir os dados antes de baixar | Tabela renderizada na interface com os mesmos dados do arquivo exportado |
| M3 | Exportação em formato legível além do CSV | CSV é para planilha; falta formato para leitura humana (relatório) | Pedagogo escolhe o formato no momento do download (CSV ou legível) |
| M4 | Documento de instrução do jogo | Não existe manual de uso para o pedagogo | Documento em linguagem não-técnica: preparação, modos, sessão, leitura de resultados |
| M5 | Atualização do README | README desatualizado frente ao estado real (validação, arquitetura 3.3V) | README reflete o estado corrente do projeto e aponta para o manual (M4) |

**Quando executar:** D1 imediatamente após a cascata de alimentação; M1–M5 em
branches próprias, priorizadas pelo usuário.

---
