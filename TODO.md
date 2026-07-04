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
| D2 | Primeira interação após energizar trava na primeira batida | fix | `fix/primeira-interacao` — `firmware/src/game/game.cpp`, `firmware/src/main.cpp` | **RESOLVIDO (2026-07-04):** validado na bancada. Sintoma original (ambos os modos): 1ª interação após energizar acendia sempre a cor roxa e travava na 1ª batida; destravava com PAUSAR e depois RETOMAR. Duas causas raiz, ambas sem lacuna de spec (`04_logica_jogo.md` e `01_arquitetura.md` já especificavam o comportamento correto — ver `CHANGELOG.md`): (1) `game.cpp` usava `rand()` (seed fixa) em vez de `random()` (já era o que `04_logica_jogo.md#mecanismo-a`/`#mecanismo-b` especificava); (2) `main.cpp` subia `interfaceInit()` em paralelo com a animação de boot de 3s, permitindo 1º comando de LED sobrescrito pelo passo final da animação — corrigido para respeitar `01_arquitetura.md#diagrama-estados`. Validação física (log serial timestampado, `firmware/diag/monitor_serial.py`): sessão completa com 3 interações, todas ACERTO, nenhuma auto-pausa após o 1º hit |
| D3 | Modo 2 (dois martelos): pares de cor sempre os mesmos dois | fix | `firmware/src/game/game.cpp` (`shuffle_bloco_A`/`random_uint32`) — nenhuma alteração de código feita | **RESOLVIDO — SEM CAUSA RAIZ CONFIRMADA (2026-07-04).** Sintoma original: no Modo 2/Mecanismo A, os pares exibidos apareciam sempre como {Roxo,Amarelo} e {Laranja,Azul} — nunca outras combinações; testado na build já com o fix do D2 (`random()`, não `rand()`). Coincidiu no tempo com um episódio de brownout por conflito USB+fonte externa ligados simultaneamente (ver `firmware/diag/README.md`). Re-teste em bancada, já com energia estável (só fonte externa, sem USB): pares variaram normalmente — usuário confirmou "agora funcionou". **Não foi possível confirmar a causa raiz por instrumentação** (log serial via `monitor_serial.py` exige USB, que reintroduz o conflito de energia) — fechado por observação visual de bancada, não por evidência de log. **RESSALVA para reabrir se reaparecer:** hipótese não descartada de que ruído/instabilidade elétrica (do mesmo conflito USB+fonte) tenha afetado a qualidade do RNG de hardware (`esp_random()`, baseado em ruído RF/térmico) durante os testes originais do D3 — se os pares travarem de novo em {Roxo,Amarelo}/{Laranja,Azul}, primeiro checar se USB e fonte externa estão ligados ao mesmo tempo; se estiverem só com a fonte e o sintoma persistir, aí sim investigar `shuffle_bloco_A()`/`random_uint32()` com instrumentação (cabo USB "somente dados" para não reintroduzir o brownout) |

**Regra do protocolo:** nenhuma tag v1.0.0 enquanto houver defeito aberto — o
gate da ETAPA 8 exige todos os CAs aprovados. **D1 baixado em 2026-07-03.
D2 baixado em 2026-07-04 (validado na bancada). D3 baixado em 2026-07-04
(observação de bancada, sem causa raiz confirmada — ver ressalva na
tabela).**

### Melhorias de produto (requisito novo → atualizar 07_interface_pedagogo.md antes)

| # | Item | Motivação | Critério de conclusão |
|---|---|---|---|
| M1 | Botão de encerrar sessão e iniciar nova sessão | Hoje não há caminho explícito na interface para encerrar e recomeçar sem recarregar/reiniciar | **CONCLUÍDO (2026-07-04):** CA-01-09/CA-04-11/CA-07-14 — botão "Encerrar Sessão" na tela de sessão ativa (com confirmação), reaproveitando o evento `FIM_SESSAO` já existente; validado manualmente no código (`pio test -e native` 42/42, `pio run` sem warnings) e na bancada pelo usuário antes da cascata; `branch feat/encerrar-nova-sessao` |
| M2 | Visualização do CSV na própria interface | Pedagogo precisa conferir os dados antes de baixar | **CONCLUÍDO (2026-07-03):** CA-07-12 PASSOU na bancada; `feat/export-preview-pdf` mergeada em develop |
| M3 | Exportação em formato legível além do CSV | CSV é para planilha; falta formato para leitura humana (relatório) | **CONCLUÍDO (2026-07-03):** CA-07-13 (PDF) PASSOU na bancada; `feat/export-preview-pdf` mergeada em develop |
| M4 | Documento de instrução do jogo | Não existe manual de uso para o pedagogo | **CONCLUÍDO (2026-07-03):** `manual/12_manual_pedagogo.md` v0.1.0 criado (branch `docs/manual-pedagogo`, mergeada em develop) — preparação, modos, mecanismos de sorteio, sessão, resultados, exportação, privacidade; 11 marcadores de fotos a produzir com o exemplar final |
| M5 | Atualização do README | README desatualizado frente ao estado real (validação, arquitetura 3.3V) | **CONCLUÍDO (2026-07-03):** README reescrito com foco não-técnico (branch `docs/readme-v1`, mergeada em develop) — fotos a inserir, aponta para o manual (M4) e para `VALIDATION.md` |

**Quando executar:** D1 imediatamente após a cascata de alimentação; M1–M5 em
branches próprias, priorizadas pelo usuário.

---
