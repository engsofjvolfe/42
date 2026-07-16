---
documento:    escopo_e_limitacoes.md
versão:       0.1.1
status:       RASCUNHO
data:         2026-07-16
depende_de:
  - 00_conceito.md v0.5.0  [BLOQUEADOR]
impacta: nenhum
---

# escopo_e_limitacoes.md — O que o 42 é, honestamente

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | escopo_e_limitacoes.md |
| Versão | 0.1.0 |
| Status | RASCUNHO |
| Escopo | Posicionamento honesto do 42: o que é, o que não é, para quem serve, e o que fica em aberto |
| Pai | 00_conceito.md v0.5.0 [BLOQUEADOR] |

---

## 2. Objetivo <a id="objetivo"></a>

Dizer com todas as letras o que o 42 é e o que ele não é, sem se apoiar em nome de autor ou citação para parecer mais do que é.

---

## 3. O que o 42 é <a id="o-que-e"></a>

O 42 é um instrumento lúdico de estímulo e registro de dados para coordenação motora e discriminação de cor em crianças pequenas. Ele é pensado para custar pouco, ser simples de montar e de operar, e gerar um registro (CSV, PDF) que acompanha o progresso de uma criança ao longo de várias sessões.

A proposta de ser barato e acessível existe para que um profissional — fisioterapeuta, terapeuta ocupacional, pedagogo, profissional de educação física — possa usar o instrumento com o que ele já sabe do próprio ofício, sem precisar de um curso de certificação novo só para operar o aparelho.

---

## 4. O que o 42 não é <a id="o-que-nao-e"></a>

O 42 não é um instrumento de avaliação clínica testado ou validado. Ninguém mediu, até hoje, se ele mede o que promete medir, com que precisão, ou em que margem de erro.

Ele não substitui e não é equivalente a instrumentos padronizados de avaliação motora infantil que já existem no mercado — aqueles passaram por um processo de validação científica que o 42 não passou. "Barato e acessível" é uma característica de projeto, uma escolha de engenharia — não é uma prova de que o instrumento funciona ou de que é cientificamente equivalente a algo validado.

Nenhuma decisão de design do 42 (as cores usadas, os modos de jogo, os tempos configurados) deve ser lida como se tivesse sido comprovada por pesquisa feita especificamente sobre este instrumento. Algumas dessas decisões se inspiram em ideias e teorias que existem por aí — mas inspiração não é prova, e isso não deveria ser confundido.

Versões anteriores da documentação deste projeto (visíveis no histórico do git) chegaram a citar autores e teorias específicas para essas mesmas decisões, com uma confiança que uma auditoria posterior não confirmou: das 7 referências científicas citadas originalmente, 5 não resistiram à conferência contra o texto real da fonte — população errada, conceito errado, ou até o oposto do que o autor realmente propõe. Qualquer citação ou certeza científica encontrada numa versão anterior deste projeto deve ser tratada como não verificada até prova em contrário, mesmo que tenha sido escrita com tom de fato estabelecido.

---

## 5. Como isso deve ser usado na prática <a id="como-usar"></a>

O profissional que usa o 42 deve fazer isso com o próprio julgamento clínico ou pedagógico — o instrumento é um ponto de estímulo e um registro de dado, não um protocolo fechado que dita uma conclusão. Cabe a quem usa decidir o que aquele dado significa, dentro do que sabe da própria área.

O 42 não deve ser usado para diagnóstico clínico formal.

Os autores do projeto não se responsabilizam por uso indevido do instrumento — incluindo, mas não se limitando a, seu uso como ferramenta de diagnóstico.

---

## 6. O que fica em aberto <a id="campo-aberto"></a>

Nada aqui foi validado cientificamente, e este documento não finge que foi. Quem quiser levar o 42 a sério como instrumento de pesquisa ou de avaliação profissional tem, pela frente, o trabalho de desenhar um estudo de verdade: testar confiabilidade, testar validade, comparar contra instrumentos já estabelecidos, definir uma amostra.

Também é possível — e bem-vindo — que alguém encontre, na literatura de desenvolvimento motor, de terapia ocupacional ou de educação, ideias e autores cujas teorias pareçam se encaixar bem com uma ou outra decisão deste projeto. Isso deve ser dito exatamente como o que é: uma inspiração encontrada depois, não uma pesquisa que já comprova a decisão. Esse trabalho — encontrar essas ideias, verificar se elas realmente se sustentam, e declarar com honestidade até onde cada uma vale — ainda não foi feito e fica aberto para quem quiser continuar.

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-07-13 | — | Criação: posicionamento honesto do 42 — o que é (3), o que não é (4), como deve ser usado na prática (5), o que fica em aberto para pesquisa futura (6) | Nenhum — RASCUNHO, ainda não referenciado por documento aprovado |
| 0.1.1 | 2026-07-16 | depende_de, Rastreabilidade | Cascata mecânica do conceito v0.5.0 (§6.3 martelos — bater com a mão diretamente na zona também é detectado, martelo é opcional, sem impacto neste documento): atualiza referência 00_conceito.md v0.4.0→v0.5.0 | — |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | 00_conceito.md | 0.5.0 | BLOQUEADOR | #identificacao, #contexto, #fora-de-escopo |

---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
