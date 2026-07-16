# Sobre o uso de Inteligência Artificial neste projeto

*Como a IA foi usada na elaboração deste projeto, onde ela foi
deliberadamente limitada, e por quê — em linguagem acessível a quem não é
da área técnica.*

---

Este projeto foi concebido a partir das aulas da disciplina de Desenvolvimento
Motor Normal, em curso de graduação na área da saúde.

Ao longo do desenvolvimento, contou-se com o auxílio de ferramentas de
inteligência artificial, buscando ao máximo construir um fluxo de trabalho
efetivo. A ideia original era partir de um conceito bem estruturado, escrito
em prosa humana, e só então transformá-lo, com apoio da IA, em algo
tecnicamente executável.

---

## Como o processo aconteceu

O ponto de partida foram inteligências artificiais comerciais de uso geral —
DeepSeek, Le Chat, Gemini e Claude. Nas interfaces web dessas ferramentas, o
conceito do projeto foi discutido e os primeiros documentos foram elaborados,
sempre com revisão humana extensiva sobre cada parágrafo produzido.

Só depois que o conceito estava completo e fechado — ou seja, aprovado, sem
pontos em aberto — é que entrou em cena uma IA de código. Junto com ela foi
elaborado o documento-guia `CLAUDE.md`: um roteiro que a IA deveria seguir do
início ao fim de qualquer sessão de trabalho.

Esse guia existe porque IAs alucinam — inventam informação com a mesma
confiança de quem está certo — e são treinadas para dar respostas que
*parecem* corretas, porque foi assim que foram recompensadas durante o
treinamento. Além disso, uma IA de código não lê um documento inteiro, linha
por linha, a menos que seja forçada a isso. Por isso, ao longo do projeto, foi
preciso voltar repetidamente e pedir: "leia este documento linha por linha",
"explique de forma determinística o que você fez em cada etapa", "diga em
qual documento você se baseou, em qual não se baseou, e por quê".

O documento [CLAUDE](CLAUDE.md) existe para não depender de lembrar isso a cada sessão nova. Ele
organiza o projeto como um V-model de engenharia — um modelo usado em
engenharia de sistemas críticos, como o setor automotivo e o médico — em que
cada linha de código só existe porque está descrita em um documento de
especificação, que por sua vez só existe porque está descrito no documento de
conceito do projeto. Nada é criado "porque a IA achou que fazia sentido":
tudo precisa apontar para trás, para uma decisão já tomada e registrada.

Um exemplo prático dessa ideia é o que, dentro do projeto, chamamos de
"cascata": quando um documento muda, existe uma regra fixa que obriga a
checar todos os outros documentos que dependem dele, atualizar o que for
necessário, e só depois disso o código pode mudar. Essa regra existe porque,
em qualquer projeto técnico, é fácil alterar uma peça e esquecer de avisar
todas as outras que dependiam dela do jeito antigo — e esse é exatamente o
tipo de esquecimento que vira defeito de hardware ou de software (o que inclui documentação) mais tarde.

---

## Os limites reais — e por que eles não são vergonha

Trabalhar com IA envolve desafios que a maioria dos usuários nunca chega a
enfrentar, porque nunca exige da ferramenta além do superficial. Para quem
não é técnico, mas entende o conceito e quer trabalhar direito — seguindo
padrões de indústria e boas práticas — é preciso forçar a ferramenta ao
máximo. Mesmo assim, não se chega a um resultado 100% equivalente ao de um
engenheiro profissional formado para isso, e isso deve ser reconhecido com
honestidade.

Mas também deve ser reconhecido o esforço de quem elaborou este projeto para
criar uma trilha coerente, organizada, modular e escalável ao longo do tempo
— uma trilha que conduza o humano e a inteligência artificial numa direção
que mantenha o projeto coerente e que permita que ele continue crescendo, em
qualquer estágio em que for retomado, por qualquer outra pessoa que queira
dar sequência a ele.

Três exemplos concretos de como esse limite foi tratado na prática, ao longo
do projeto:

- Mesmo quando tudo saiu certo nas verificações feitas dentro do computador
  — sem nenhum erro apontado — a correção só foi considerada pronta depois
  de testada de verdade, com o brinquedo ligado, sendo usado do jeito que
  uma criança usaria de verdade. Sair certo no computador não é a mesma
  coisa que funcionar direito no mundo real.
- Mesmo quando toda a lista de critérios de validação do projeto (os "CAs",
  critérios de aceitação) estava fechada, a decisão de lançar a versão 1.0
  do projeto não foi tomada pela IA sozinha. Um checklist verde não
  autoriza, por si só, a próxima etapa — quem decide é sempre a pessoa
  responsável pelo projeto.
- A intenção desde o início era que toda referência técnica ou pedagógica
  citada nos documentos deste projeto — normas técnicas, artigos científicos
  revisados por pares — fosse conferida contra a fonte original antes de
  entrar em qualquer documento aprovado. Isso não foi cumprido na prática:
  uma auditoria posterior encontrou que, das 7 referências científicas
  citadas originalmente no documento de conceito, 5 não resistiram à
  conferência contra o texto real da fonte (população errada, conceito
  errado, ou o oposto do que o autor citado realmente propõe). Foram
  corrigidas assim que encontradas — e o próprio fato de terem entrado sem
  essa conferência é a prova do que este ponto afirma: IA inventa citação
  com a mesma fluência com que inventa qualquer outro texto; citação não
  conferida não é fundamentação, é risco disfarçado de rigor.

Esses três exemplos resumem bem o espírito do processo: a IA executa e
sugere, mas não decide sozinha o que é verdade, o que está testado, nem o
que está pronto para o mundo real.

---

## Um percurso de idas e vindas

O percurso é de idas e vindas, e certamente ainda há erros a serem sanados.
Certamente já houve momentos em que a IA criou algo que não foi pedido,
mesmo quando, à primeira vista de uma revisão humana, parecia que tudo havia
sido executado exatamente conforme o planejado. É por isso que a revisão
linha por linha, mesmo cansativa, não é opcional neste processo.

---

## Quem faz este projeto

Este projeto não é feito por técnicos especialistas. E também não é feito
por leigos que apenas digitam um pedido e esperam uma resposta pronta. É um
projeto feito por quem entende a responsabilidade inerente que significa
transmitir conhecimento e tentar levar um produto final de qualidade a quem
possa se interessar.

---

## Para quem quiser verificar

Nada neste processo depende de confiar cegamente em quem escreveu este texto
— mas o esforço pra verificar não é igual em todos os níveis.

O nível fácil: o documento [CLAUDE](CLAUDE.md) (as regras que a IA deveria
seguir) e `concept/00_conceito.md` (o que o projeto é, desde o início) são
textos comuns. Dá pra ler direto, sem instalar nada.

O nível de verdade, passo a passo, é outra história. Esse histórico completo
— o que mudou, quando mudou e por quê, cada alteração registrada sem poder
ser apagada ou reescrita escondida — fica guardado numa ferramenta chamada
Git, que é o que hospeda este projeto no GitHub. E aqui vale ser honesto: Git
não é simples pra quem nunca usou. Mas, sem exagero, é a ferramenta de
controle de histórico mais usada e mais poderosa que existe pra esse tipo de
registro — nada parecido chega perto em capacidade de rastrear cada mudança.
Ela tem uma curva de aprendizado de verdade. Então, se alguém quiser auditar
de verdade, e não só confiar na palavra de quem escreveu este texto: é aqui
que entra aquilo que, às vezes, a gente precisa mesmo aprender um pouco.
Vale a pena — é exatamente pra isso que a ferramenta existe.

---

## Não é sobre usar ou não usar IA

Um projeto como este poderia muito bem ter sido realizado por alguém que
estudou um pouco dos fundamentos da engenharia de software, que usa IA
entendendo como cada modelo funciona, que lê a documentação das ferramentas
e tem acesso à internet para buscar o que não sabe. O uso de IA acelera a
parte mecânica do trabalho e libera espaço para a criatividade e para a
organização — e, principalmente, para o bem mais precioso de todos: tempo.
