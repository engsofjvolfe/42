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
| D1 | Exportação CSV não funciona | fix | `fix/export-csv` — `firmware/src/interface/interface.cpp` (HTML/JS embutido) | **RESOLVIDO (2026-07-03):** CA-07-09 PASSOU na validação física de bancada; branch mergeada em develop |
| D2 | Primeira interação após energizar trava na primeira batida | fix | branch `fix/primeira-interacao` — investigar `firmware/src/game/game.cpp` (sorteio e estado inicial) e `firmware/src/interface/interface.cpp` | **ABERTO (registrado 2026-07-04).** Sintoma (bancada, ambos os modos): na primeira conexão à rede após ligar o dispositivo, a primeira interação acende **sempre a cor roxa** e trava na primeira batida; destrava com PAUSAR e depois RETOMAR na interface. Reproduz em todo ciclo de energização — é preciso tirar da tomada para reproduzir de novo. Pistas (hipóteses a verificar, não conclusões): cor sempre igual sugere sorteio determinístico no primeiro uso (seed do RNG); destravar via PAUSAR/RETOMAR sugere estado inicial que `gameRetomarSessao()` corrige. Critério de conclusão: primeira interação pós-boot sorteia cor conforme o mecanismo configurado e registra a primeira batida sem exigir pausar/retomar |

**Regra do protocolo:** nenhuma tag v1.0.0 enquanto houver defeito aberto — o
gate da ETAPA 8 exige todos os CAs aprovados. **D1 baixado em 2026-07-03.
D2 aberto em 2026-07-04 — bloqueia o gate v1.0.0.**

### Melhorias de produto (requisito novo → atualizar 07_interface_pedagogo.md antes)

| # | Item | Motivação | Critério de conclusão |
|---|---|---|---|
| M1 | Botão de encerrar sessão e iniciar nova sessão | Hoje não há caminho explícito na interface para encerrar e recomeçar sem recarregar/reiniciar | Pedagogo encerra sessão ativa e inicia nova sem recarregar a página nem reiniciar o ESP32 |
| M2 | Visualização do CSV na própria interface | Pedagogo precisa conferir os dados antes de baixar | **CONCLUÍDO (2026-07-03):** CA-07-12 PASSOU na bancada; `feat/export-preview-pdf` mergeada em develop |
| M3 | Exportação em formato legível além do CSV | CSV é para planilha; falta formato para leitura humana (relatório) | **CONCLUÍDO (2026-07-03):** CA-07-13 (PDF) PASSOU na bancada; `feat/export-preview-pdf` mergeada em develop |
| M4 | Documento de instrução do jogo | Não existe manual de uso para o pedagogo | **CONCLUÍDO (2026-07-03):** `manual/12_manual_pedagogo.md` v0.1.0 criado (branch `docs/manual-pedagogo`, mergeada em develop) — preparação, modos, mecanismos de sorteio, sessão, resultados, exportação, privacidade; 11 marcadores de fotos a produzir com o exemplar final |
| M5 | Atualização do README | README desatualizado frente ao estado real (validação, arquitetura 3.3V) | **CONCLUÍDO (2026-07-03):** README reescrito com foco não-técnico (branch `docs/readme-v1`, mergeada em develop) — fotos a inserir, aponta para o manual (M4) e para `VALIDATION.md` |

**Quando executar:** D1 imediatamente após a cascata de alimentação; M1–M5 em
branches próprias, priorizadas pelo usuário.

---
