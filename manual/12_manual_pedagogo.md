---
documento:    12_manual_pedagogo.md
versão:       0.1.0
status:       APROVADO
data:         2026-07-03
depende_de:
  - _PADRAO.md v0.1.0               [BLOQUEADOR]
  - 00_conceito.md v0.2.1           [BLOQUEADOR]
  - 07_interface_pedagogo.md v0.3.2 [BLOQUEADOR]
impacta: []
---

# 12_MANUAL_PEDAGOGO.md — Manual de Uso do Instrumento

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | 12_manual_pedagogo.md |
| Versão | 0.1.0 |
| Status | APROVADO |
| Audiência | Pedagogo ou profissional habilitado que conduz as sessões |
| Idioma | Português brasileiro — linguagem não-técnica |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Ensinar, passo a passo e sem termos técnicos, como preparar o instrumento, conduzir uma sessão com a criança, ler os resultados e guardar os dados. Todo o comportamento descrito aqui deriva de [VER: 00_conceito.md#identificacao] e [VER: 07_interface_pedagogo.md#identificacao] — em caso de dúvida sobre uma regra, aqueles documentos prevalecem.

As fotos indicadas ao longo do texto serão adicionadas após a produção do exemplar final. A lista completa do que fotografar está em [VER: #fotos-a-produzir].

---

## 3. O que é este instrumento <a id="o-que-e"></a>

É uma mesa de jogo com **4 zonas coloridas** (laranja, azul, amarelo e roxo), **3 luzes indicadoras** posicionadas acima das zonas e **martelos de madeira**. Uma luz acende mostrando uma cor; a criança deve bater com o martelo na zona daquela cor. Você acompanha e controla tudo pelo navegador do seu celular ou tablet — sem instalar aplicativo e sem internet.

Apesar do formato de brinquedo, este é um **instrumento de avaliação pedagógica**: ele registra acertos, erros e tempos de cada sessão para que você acompanhe o desenvolvimento da coordenação motora e da discriminação de cores da criança ao longo do tempo.

> **[FOTO 01 — a inserir]**
> *Vista geral do instrumento montado: zonas coloridas, luzes e martelos.*

**Para quem é:** crianças de 5 anos em desenvolvimento típico (modo com 1 martelo). O modo com 2 martelos é destinado a crianças com altas habilidades ou neurodivergência de alto rendimento, e a decisão de usá-lo é exclusivamente sua, como profissional responsável.

**Para quem não é:** o instrumento não foi projetado para crianças com déficit motor significativo, para uso doméstico sem orientação pedagógica, nem para uso sem a supervisão de um profissional habilitado.

---

## 4. O que compõe o conjunto <a id="componentes"></a>

| Item | Para que serve |
|---|---|
| Mesa com 4 zonas de cor | Onde a criança bate com o martelo. Cada zona sente a batida |
| 3 luzes acima das zonas | Mostram a cor-alvo. A luz central é usada no modo de 1 martelo; as luzes da esquerda e da direita, no modo de 2 martelos |
| 1 ou 2 martelos de madeira | Ferramenta da criança. Não têm eletrônica nenhuma — podem ser trocados por outro martelo semelhante se necessário |
| Fonte de energia | Liga na tomada. O instrumento não tem bateria |
| Seu celular ou tablet | O painel de controle. Qualquer aparelho com navegador serve |

> **[FOTO 02 — a inserir]**
> *Close das 4 zonas de cor com um martelo apoiado.*

> **[FOTO 03 — a inserir]**
> *As 3 luzes indicadoras acesas durante o aviso de início.*

---

## 5. Antes de começar — segurança e supervisão <a id="seguranca"></a>

- O uso é sempre **supervisionado por você**. A criança nunca opera o instrumento sozinha.
- A fonte e os fios ficam fora do alcance da criança; ela interage apenas com as zonas coloridas e os martelos.
- Antes da primeira sessão com cada criança, a instituição precisa ter o **consentimento do responsável legal** para o registro dos dados — o documento de privacidade ([VER: 06_privacidade_lgpd.md#identificacao]) explica isso em detalhe e serve de apoio para essa conversa.

---

## 6. Ligando o instrumento <a id="ligar"></a>

1. Conecte a fonte na tomada.
2. Aguarde alguns segundos. As 3 luzes farão uma **varredura passando pelas 4 cores** — é o sinal de que o instrumento está pronto e aguardando você conectar.

> **[FOTO 04 — a inserir]**
> *Fonte conectada e luzes fazendo a varredura de início.*

Se a varredura não acontecer, desconecte a fonte da tomada, aguarde alguns segundos e conecte de novo.

---

## 7. Conectando o celular ou tablet <a id="conectar"></a>

O instrumento cria a própria rede WiFi — você não precisa de internet nem da rede da instituição.

1. No seu aparelho, abra as configurações de WiFi.
2. Conecte-se à rede chamada **`BMI`**. Ela não pede senha e aparece na lista poucos segundos depois de o instrumento ligar.
3. Abra o navegador (Chrome ou Firefox) e digite na barra de endereço: **`192.168.4.1`**
4. A tela de configuração da sessão vai carregar.

> **[FOTO 05 — a inserir]**
> *Tela do celular mostrando a rede `BMI` na lista de WiFi.*

> **[FOTO 06 — a inserir]**
> *Navegador aberto em `192.168.4.1` com a tela de configuração carregada.*

Enquanto seu aparelho estiver conectado à rede `BMI`, ele fica sem internet. Isso é esperado — ao final, basta voltar para a sua rede habitual.

---

## 8. Configurando uma sessão <a id="configurar-sessao"></a>

Na tela de configuração, preencha:

| Campo | O que é |
|---|---|
| **Nome da criança** | Obrigatório. Identifica de quem é o registro |
| **Número de interações** | Quantos acertos completam a sessão (por exemplo, 10) |
| **Modo** | `1 Martelo` ou `2 Martelos` — ver [VER: #modos] |
| **Mecanismo** | `A (uniforme)` ou `B (variado)` — ver [VER: #sorteio-cores] |
| **Janela (ms)** | Aparece somente no modo 2 Martelos — ver [VER: #modos]. Se estiver em dúvida, mantenha o valor sugerido (800) |

Toque em iniciar e a primeira cor acende na mesa.

> **[FOTO 07 — a inserir]**
> *Tela de configuração preenchida, pronta para iniciar.*

---

## 9. Os dois modos <a id="modos"></a>

**Modo 1 Martelo** — o modo principal. A luz **central** acende com uma cor e a criança bate na zona daquela cor, usando um martelo. Indicado para o público principal: 5 anos, desenvolvimento típico.

**Modo 2 Martelos** — o modo estendido. As luzes da **esquerda e da direita** acendem ao mesmo tempo, sempre com **cores diferentes entre si**. A criança, com um martelo em cada mão, deve bater nas duas zonas correspondentes **quase ao mesmo tempo**. Este modo avalia coordenação bimanual e é indicado para o público estendido, a seu critério profissional.

**A "janela" do modo 2 Martelos:** é a tolerância de tempo entre a primeira e a segunda batida — não é o tempo que a criança tem para responder. A criança pode olhar e pensar pelo tempo que precisar; mas, depois que der a primeira batida, a segunda precisa vir dentro dessa janela para valer como acerto. O valor sugerido é 800 milissegundos (menos de 1 segundo).

O modo é escolhido antes de iniciar e **não muda durante a sessão**. Em nenhum dos modos existe limite de tempo para responder: pressão de tempo é contraproducente nessa faixa etária, por isso o instrumento não a impõe.

---

## 10. Como as cores são sorteadas <a id="sorteio-cores"></a>

A cor de cada interação é sorteada pelo instrumento. Existem duas formas de sortear, e você escolhe uma delas na tela de configuração, antes de iniciar a sessão.

**Mecanismo A (uniforme)** — funciona como um baralho de 4 cartas, uma de cada cor:

1. O instrumento "embaralha" as 4 cores.
2. Mostra uma por vez, até as 4 saírem — nesse trecho, nenhuma cor repete.
3. Quando as 4 saíram, embaralha de novo e recomeça.

O efeito prático: ao final da sessão, **todas as cores aparecem em quantidade praticamente igual** — e exatamente igual quando o número de interações é múltiplo de 4: numa sessão de 12 interações, cada cor sai 3 vezes, só a ordem muda. Por isso este é o mecanismo indicado para o uso padrão e para **comparar sessões entre si**: toda criança passa pelo mesmo conjunto de estímulos, em ordem diferente.

Uma consequência do "baralho": a mesma cor pode sair duas vezes seguidas na virada de um bloco para o outro (por exemplo, roxo como última cor de um bloco e primeira do seguinte). Isso é normal e não é defeito.

**Mecanismo B (variado)** — aqui não há baralho. A cada interação, as 4 cores concorrem no sorteio, mas **as que saíram há pouco ficam temporariamente menos prováveis**. Quanto mais recente a cor, menor a chance de sair de novo — sem nunca ser impossível.

O efeito prático: a sequência parece mais natural e imprevisível, mas **não há garantia de que todas as cores apareçam na mesma quantidade** ao final. Este mecanismo é indicado para o público estendido: crianças que antecipam padrões com facilidade percebem a regularidade do mecanismo A ("já saíram três cores, agora só pode vir aquela") e passam a prever em vez de discriminar — o mecanismo B remove essa previsibilidade.

**Resumo para escolher:**

| | Mecanismo A (uniforme) | Mecanismo B (variado) |
|---|---|---|
| Como sorteia | Blocos de 4 cores, todas saem antes de repetir | Cores recentes ficam menos prováveis |
| Quantidade por cor | Igual para todas | Sem garantia de igualdade |
| Previsibilidade | Maior (padrão perceptível) | Menor |
| Indicado para | Uso padrão; comparar sessões | Público estendido |

No modo 2 Martelos, as duas cores exibidas juntas são **sempre diferentes entre si**, qualquer que seja o mecanismo escolhido.

---

## 11. Durante a sessão <a id="durante-sessao"></a>

**O que a criança vê:** as luzes na mesa. Acendeu uma cor, ela bate na zona daquela cor.

**O que você vê no celular:** o andamento da sessão — quantos acertos já foram feitos do total configurado e a taxa de acerto — e um botão para **pausar** quando precisar interromper.

**Quando a criança acerta:** sua tela fica **verde** por um instante, toca um som curto ascendente, e a luz da mesa apaga. Cerca de 2 segundos depois, a próxima cor acende sozinha.

**Quando a criança erra** (bate na zona errada, ou numa zona sem luz correspondente): sua tela fica **vermelha** com um som neutro de aviso — e a luz da mesa **continua acesa**. A criança simplesmente tenta de novo, quantas vezes precisar, até acertar. O erro não elimina, não pula a vez e não é anunciado para a criança: o feedback vermelho aparece só no seu aparelho.

> **[FOTO 08 — a inserir]**
> *Criança (ou mão adulta simulando) batendo em uma zona com a luz correspondente acesa.*

> **[FOTO 09 — a inserir]**
> *Celular com a tela verde de acerto; ao lado, a tela vermelha de erro.*

---

## 12. Se a conexão cair <a id="conexao-caiu"></a>

Se o seu aparelho se desconectar da rede `BMI` no meio da sessão (bateria, distância, troca de rede), o instrumento **pausa o jogo sozinho** e guarda o ponto em que estava. Reconecte-se à rede `BMI`, abra `192.168.4.1` de novo, e a sessão **retoma de onde parou** — a luz reacende com a mesma cor. Nada se perde.

---

## 13. Fim da sessão <a id="fim-sessao"></a>

Quando o número de interações configurado é atingido:

- **Para a criança:** as luzes fazem uma varredura festiva de cerca de 3 segundos — sempre comemorativa, **independentemente do desempenho**. A criança nunca vê pontuação, taxa ou erro. Isso é deliberado: evita que ela associe o instrumento a julgamento e preserva a experiência lúdica.
- **Para você:** o celular mostra a tela de resultados da sessão.

> **[FOTO 10 — a inserir]**
> *Tela de resultados ao final de uma sessão.*

---

## 14. Lendo os resultados <a id="ler-resultados"></a>

A tela de resultados mostra:

| Informação | Como interpretar |
|---|---|
| **Acertos / total** | Cada interação termina com um acerto, então este número confirma a sessão completa |
| **Taxa de acerto (%)** | Proporção de interações resolvidas sem erro intermediário |
| **Erros** | Batidas erradas ao longo da sessão. Erros dentro de uma mesma interação não anulam o acerto final — eles mostram o *processo*, não só o resultado |
| **Duração** | Tempo total da sessão, em minutos e segundos |

Dois botões acompanham os resultados:

- **Nova Sessão** — **guarda o registro** desta sessão no seu aparelho e volta para a tela de configuração. É este toque que salva os dados: crie o hábito de encerrar por ele.
- **Exportar** — abre a conferência dos dados para gerar um arquivo ([VER: #exportar]).

---

## 15. Guardando e exportando os dados <a id="exportar"></a>

Os registros de todas as sessões ficam **somente no seu aparelho** — o instrumento não guarda nada e não há nuvem envolvida. Para gerar um arquivo com tudo o que está registrado:

1. Toque em **Exportar** na tela de resultados. Abre uma **prévia** com a tabela completa dos registros — exatamente o que vai para o arquivo. Nenhum download acontece ainda.
2. Confira os dados e escolha o formato:
   - **CSV** — para abrir em planilha (Excel, Google Planilhas). Ideal para análise e gráficos.
   - **PDF** — relatório pronto para leitura e impressão, com data e hora de geração.
3. Toque em **Baixar** para confirmar (ou **Cancelar** para sair sem gerar arquivo).

Se ainda não houver nenhum registro, a prévia avisa e o botão Baixar fica desativado.

> **[FOTO 11 — a inserir]**
> *Prévia de exportação com a tabela de registros e a escolha entre CSV e PDF.*

---

## 16. Privacidade e dados das crianças <a id="privacidade"></a>

Os dados registrados são mínimos: nome, data, configuração da sessão e desempenho. Eles ficam exclusivamente no aparelho do profissional, e a guarda é responsabilidade da instituição. Os responsáveis legais podem pedir para ver, corrigir, exportar ou apagar os dados da criança a qualquer momento.

Leia e mantenha à mão o documento de privacidade: [VER: 06_privacidade_lgpd.md#identificacao]. Ele foi escrito para ser entregue e explicado aos responsáveis.

---

## 17. Problemas comuns <a id="problemas-comuns"></a>

| Situação | O que fazer |
|---|---|
| A rede `BMI` não aparece na lista de WiFi | Confirme que a fonte está na tomada e que a varredura de luzes aconteceu ao ligar. Desconecte e reconecte a fonte e aguarde a varredura |
| A página `192.168.4.1` não abre | Confirme que o aparelho está conectado à rede `BMI` (e não voltou sozinho para outra rede). Digite o endereço na barra do navegador, sem `www` |
| A sessão parou sozinha | Provavelmente o aparelho se desconectou da rede `BMI`. Reconecte e reabra a página — a sessão retoma do ponto de pausa ([VER: #conexao-caiu]) |
| O download não começa ao exportar | Use Chrome ou Firefox no celular ou tablet. Refaça o caminho Exportar → conferir a prévia → Baixar |
| Uma zona não responde às batidas | Interrompa o uso e acione o responsável técnico pelo instrumento. Não abra o equipamento |

---

## 18. Fotos a produzir <a id="fotos-a-produzir"></a>

Lista de referência para a sessão de fotos do exemplar final. Cada foto substitui o marcador correspondente neste manual.

| Nº | O que fotografar | Entra na seção |
|---|---|---|
| 01 | Vista geral do instrumento montado | [VER: #o-que-e] |
| 02 | Close das 4 zonas de cor com martelo | [VER: #componentes] |
| 03 | As 3 luzes indicadoras acesas | [VER: #componentes] |
| 04 | Fonte conectada + varredura de início | [VER: #ligar] |
| 05 | Rede `BMI` na lista de WiFi do celular | [VER: #conectar] |
| 06 | Navegador em `192.168.4.1` com a tela de configuração | [VER: #conectar] |
| 07 | Tela de configuração preenchida | [VER: #configurar-sessao] |
| 08 | Batida na zona com luz correspondente acesa | [VER: #durante-sessao] |
| 09 | Telas de feedback verde e vermelha | [VER: #durante-sessao] |
| 10 | Tela de resultados | [VER: #fim-sessao] |
| 11 | Prévia de exportação (tabela + escolha CSV/PDF) | [VER: #exportar] |

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-07-03 | — | Criação — manual de uso em linguagem não-técnica (melhoria M4 da validação ETAPA 8), derivado de 00_conceito v0.2.1 e 07_interface_pedagogo v0.3.2, com marcadores de fotos a produzir | — |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 00_conceito.md | 0.2.1 | BLOQUEADOR | #publico-alvo, #componentes-fisicos, #modos-operacao, #fluxo-interacao, #regras-sistema, #feedback, #interface-pedagogo, #gestao-dados |
| Pai | 07_interface_pedagogo.md | 0.3.2 | BLOQUEADOR | #access-point, #estados-interface, #tela-configuracao, #tela-resultados, #pre-visualizacao, #exportacao-csv |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
