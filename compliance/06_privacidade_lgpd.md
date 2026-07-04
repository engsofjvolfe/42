---
documento:    06_privacidade_lgpd.md
versão:       0.2.1
status:       APROVADO
data:         2026-06-26
depende_de:
  - _PADRAO.md v0.1.0       [BLOQUEADOR]
  - 00_conceito.md v0.2.1   [BLOQUEADOR]
impacta: []
---

# 06_PRIVACIDADE_LGPD.md — Privacidade e Proteção de Dados

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | 06_privacidade_lgpd.md |
| Versão | 0.2.1 |
| Status | APROVADO |
| Audiência | Responsáveis legais das crianças participantes |
| Idioma | Português brasileiro — linguagem acessível |
| Base legal | Lei nº 13.709/2018 — Lei Geral de Proteção de Dados (LGPD) |

---

## 2. Objetivo do Documento <a id="objetivo"></a>

Explicar, de forma direta e sem rodeios, o que acontece com os dados da criança quando este instrumento é usado. Derivado de [VER: 00_conceito.md#privacidade-lgpd] e [VER: 00_conceito.md#gestao-dados].

**Para o responsável que vai ler a partir daqui:** você não vai encontrar frases como "mediante livre, inequívoco e informado consentimento nos termos do artigo X". Vai encontrar o que de fato acontece, o que não acontece, os riscos reais e o que você pode fazer. Para quem quiser conferir na lei, cada ponto traz a referência exata.

---

## 3. O que é este instrumento <a id="o-que-e"></a>

Este é um instrumento de **avaliação pedagógica com formato de brinquedo**. A criança bate com um martelo de madeira em zonas coloridas enquanto o profissional responsável observa e registra como ela responde a estímulos visuais. É uma ferramenta de trabalho do pedagogo, não um produto de entretenimento.

O instrumento foi desenvolvido especificamente para ser usado **sob supervisão de um profissional habilitado**, em ambiente pedagógico. Não foi feito para uso doméstico ou sem orientação. **(→ LGPD Art. 14, §1 — dados de crianças exigem tratamento no melhor interesse do menor)**

---

## 4. O que coletamos — e o que não coletamos <a id="dados-coletados"></a>

### 4.1 O que coletamos <a id="o-que-sim"></a>

Quando o profissional usa o instrumento com uma criança, o seguinte é registrado no dispositivo do profissional (celular ou tablet):

| Dado | Por que existe |
|---|---|
| Nome da criança | Para identificar de quem é cada registro |
| Data e hora da sessão | Para comparar sessões ao longo do tempo |
| Modo utilizado (1 ou 2 martelos) | Para contextualizar o resultado |
| Número de tentativas configuradas | Para calcular a taxa de acerto |
| Número de acertos | O resultado principal |
| Número de erros intermediários | Para entender o processo, não só o resultado |
| Taxa de acerto (%) | Cálculo automático a partir dos dados acima |
| Duração da sessão | Para o relatório do profissional |

Só coletamos o que é necessário para a avaliação funcionar. Nenhum campo a mais. **(→ LGPD Art. 6, III — princípio da necessidade)**

### 4.2 O que não coletamos <a id="o-que-nao"></a>

Isto **não é coletado**, em nenhuma circunstância:

- Localização geográfica
- Fotos ou vídeos
- Áudio
- Documentos pessoais (CPF, certidão, prontuário)
- Dados de contato (telefone, e-mail)
- Informações de saúde ou diagnóstico
- Qualquer dado que identifique a família

**(→ LGPD Art. 6, III — coleta mínima; Art. 11 — dados sensíveis têm proteção especial e não são coletados aqui)**

---

## 5. Por que coletamos <a id="finalidade"></a>

Os dados coletados têm **uma única finalidade**: permitir ao profissional acompanhar o desenvolvimento da criança ao longo de sessões, documentar para fins pedagógicos ou clínicos, e exportar registros quando necessário.

Não existe nenhuma outra finalidade. Os dados **não serão**:
- Vendidos ou compartilhados com terceiros
- Usados para publicidade
- Usados em pesquisas sem consentimento específico e separado
- Repassados para outros profissionais sem autorização do responsável

Se a instituição quiser usar esses dados para pesquisa acadêmica ou publicação, isso exige um consentimento diferente deste, específico para esse fim. **(→ LGPD Art. 9, II — finalidade; Art. 6, I — princípio da finalidade)**

---

## 6. Onde ficam os dados <a id="onde-ficam"></a>

Os dados ficam **exclusivamente no dispositivo do profissional** — o celular ou tablet que ele usa para controlar o instrumento. Nenhum dado sai desse dispositivo.

Para ser mais exato:
- O equipamento eletrônico do instrumento (o ESP32, o computador embutido no aparelho) **não armazena nada**. Ele apenas controla as luzes e lê as batidas. Ao desligar, esquece tudo.
- Não existe servidor. Não existe nuvem. Não existe banco de dados externo.
- Não existe conexão com a internet durante o uso.
- Nós (os fabricantes do instrumento) **não temos acesso a nenhum dado** de nenhuma criança. Isso não é uma promessa — é impossível tecnicamente, porque os dados nunca saem do dispositivo do profissional.

**(→ LGPD Art. 46 — segurança técnica; Art. 6, VII — princípio da segurança)**

---

## 7. Quem tem acesso <a id="quem-acessa"></a>

Tem acesso aos dados **qualquer pessoa que tenha acesso ao dispositivo do profissional**. Isso significa:

- O próprio profissional ✓
- Quem mais usa o mesmo celular ou tablet (família, colegas)
- Qualquer pessoa que encontre o dispositivo desbloqueado

A responsabilidade pela guarda do dispositivo é **da instituição e do profissional**, não do instrumento em si. Recomendamos que o dispositivo usado para avaliações seja protegido com senha e não compartilhado para uso pessoal.

**(→ LGPD Art. 37 — o controlador dos dados é quem decide como e por que os dados são usados; neste caso, a instituição)**

---

## 8. Por quanto tempo ficam os dados <a id="retencao"></a>

Os dados permanecem no dispositivo **até serem apagados manualmente**. O instrumento não apaga automaticamente.

É responsabilidade da instituição definir uma política de retenção — por exemplo: "mantemos os dados por 12 meses após o encerramento do acompanhamento". Sem uma política definida, os dados ficam indefinidamente.

Se um profissional sair da instituição, os dados que estiverem no dispositivo dele vão com ele — a menos que a instituição tome medidas antes. Isso é um risco real que precisa de atenção. **(→ LGPD Art. 15 — término do tratamento; Art. 16 — eliminação dos dados)**

---

## 9. O que você pode pedir <a id="seus-direitos"></a>

Você, como responsável legal pela criança, tem os seguintes direitos sobre os dados dela. Para exercê-los, o caminho é falar diretamente com o profissional ou a instituição:

| O que você pode pedir | O que acontece |
|---|---|
| **Ver** os dados da criança | O profissional mostra ou exporta os registros |
| **Corrigir** um erro (nome digitado errado, por exemplo) | O profissional corrige no sistema |
| **Apagar** todos os dados da criança | O profissional deleta os registros do dispositivo |
| **Levar** os dados (portabilidade) | O profissional exporta um arquivo com todos os dados — planilha (CSV) ou relatório de leitura (PDF), conferindo o conteúdo na tela antes de baixar |
| **Retirar** o consentimento | Você pode a qualquer momento; dados anteriores precisam ser apagados manualmente |
| **Saber** se os dados foram compartilhados | A instituição é obrigada a informar |

Nenhum desses pedidos pode ser negado sem justificativa. **(→ LGPD Art. 18 — direitos do titular)**

---

## 10. Riscos reais — e o que fizemos para reduzi-los <a id="riscos"></a>

Honestidade exige falar dos riscos, não só dos cuidados:

### 10.1 Riscos que existem <a id="riscos-existem"></a>

- **Dispositivo perdido ou roubado:** se o celular do profissional for roubado sem proteção por senha, os dados das crianças ficam expostos a quem encontrar o aparelho.
- **Dispositivo compartilhado:** se o profissional usar o mesmo dispositivo para avaliações e para uso pessoal, outras pessoas com acesso ao aparelho podem ver os dados.
- **Saída do profissional:** se um profissional sair da instituição sem que os dados sejam transferidos ou apagados, eles saem junto com ele.
- **Sem criptografia automática:** os dados são armazenados como texto no browser do dispositivo. Se o dispositivo não tiver criptografia de disco (disponível em todos os smartphones modernos mas que precisa ser ativada), os dados podem ser acessados por alguém com acesso físico ao aparelho.

### 10.2 O que fizemos para reduzir os riscos <a id="riscos-reduzimos"></a>

- Os dados **nunca saem do dispositivo do profissional** — eliminamos o risco de vazamento de servidor, que é a forma mais comum de vazamento de dados em larga escala.
- O instrumento **não usa internet** — não há risco de interceptação de rede.
- **Não coletamos dados sensíveis** — nome e desempenho em um exercício motor não são dados que colocam a criança em risco significativo se expostos.
- A quantidade de dados é mínima — em caso de exposição, o dano potencial é limitado.

**(→ LGPD Art. 46, §2 — privacidade por design; Art. 6, VII — princípio da segurança)**

---

## 11. Consentimento <a id="consentimento"></a>

Para usar este instrumento com uma criança, a instituição precisa ter **consentimento explícito de um responsável legal**. Esse consentimento precisa ser:

- **Informado:** o responsável sabe o que vai ser coletado (este documento serve para isso)
- **Específico:** é para esta atividade, com esta finalidade
- **Documentado:** precisa existir um registro de que o consentimento foi dado
- **Revogável:** o responsável pode retirar o consentimento a qualquer momento

Este documento explica o que acontece com os dados — ele não é em si o formulário de consentimento. A instituição é responsável por coletar e guardar o consentimento de cada família.

Se o consentimento for retirado, os dados da criança devem ser apagados. Dados de sessões já realizadas não desaparecem automaticamente — precisam ser deletados manualmente pela instituição.

**(→ LGPD Art. 14, §1 — consentimento parental para dados de menores; Art. 7, I — consentimento como base legal)**

---

## 12. Limites do uso dos dados — para os dois lados <a id="limites"></a>

Esta seção é clara sobre o que nenhum dos lados pode fazer:

**A instituição não pode:**
- Usar os dados para qualquer fim além da avaliação pedagógica documentada
- Compartilhar dados com terceiros sem consentimento específico
- Negar ao responsável o acesso, a correção ou a exclusão dos dados da criança
- Usar o desempenho no instrumento como único critério de qualquer decisão sobre a criança

**O responsável não pode:**
- Alegar dano a partir de avaliação pedagógica adequada e consentida, sem evidência concreta
- Reter a criança de participar e depois reclamar da falta de dados para acompanhamento

**Os fabricantes não podem** (e não têm como):
- Acessar dados de nenhuma criança — a arquitetura técnica não permite

**(→ LGPD Art. 6, I ao IX — princípios gerais; Art. 42 — responsabilidade por danos)**

---

## 13. Se algo der errado <a id="incidentes"></a>

Se a instituição suspeitar que dados de crianças foram acessados por pessoas não autorizadas (dispositivo roubado, acesso indevido), ela é obrigada a:

1. Avaliar o risco real do incidente
2. Se houver risco relevante: comunicar a Autoridade Nacional de Proteção de Dados (ANPD) em até 72 horas
3. Avisar os responsáveis afetados se o risco for alto

**(→ LGPD Art. 48 — comunicação de incidentes de segurança)**

---

## 14. Contato e reclamações <a id="contato"></a>

**Dúvidas sobre os dados da criança:** fale com o profissional ou a instituição que usa o instrumento. Eles são os controladores dos dados.

**Dúvidas sobre o instrumento em si:** [a preencher com contato do fabricante]

**Reclamação formal:** se a instituição não responder adequadamente, você pode registrar reclamação na Autoridade Nacional de Proteção de Dados (ANPD) — www.gov.br/anpd **(→ LGPD Art. 18, §1 — direito de petição à ANPD)**

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-26 | — | Criação — derivado de 00_conceito v0.1.0 com linguagem acessível para responsáveis, referências LGPD inline | — |
| 0.2.0 | 2026-07-03 | #seus-direitos (portabilidade), depende_de | Cascata do conceito v0.2.0: portabilidade passa a citar os dois formatos de exportação (planilha CSV e relatório PDF) e a conferência dos dados na tela antes do download; nenhuma mudança na coleta, finalidade ou guarda dos dados | — |
| 0.2.1 | 2026-07-03 | depende_de, Rastreabilidade | Cascata do registro do manual do pedagogo (12_manual_pedagogo.md no impacta de 00 e 07): atualiza referências — 00_conceito.md v0.2.0→v0.2.1 | — |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 00_conceito.md | 0.2.1 | BLOQUEADOR | #privacidade-lgpd, #gestao-dados, #armazenamento, #responsabilidade-dados, #exportacao |
---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
