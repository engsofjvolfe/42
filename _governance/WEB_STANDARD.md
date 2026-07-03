---
documento:    WEB_STANDARD.md
versão:       0.3.0
status:       APROVADO
data:         2026-06-28
depende_de:
  - _PADRAO.md v0.1.0                [BLOQUEADOR]
  - 01_arquitetura.md v0.2.1         [BLOQUEADOR]
  - 07_interface_pedagogo.md v0.3.0  [BLOQUEADOR]
impacta:
  - firmware/src/interface/ (interface.cpp — arquivo HTML/CSS/JS embutido)
---

# WEB_STANDARD.md — Padrão de Interface Web Embutida

**Escopo:** Todo HTML, CSS e JavaScript embutido em `firmware/src/interface/interface.cpp`, sem exceção.

---

## 1. Identificação <a id="identificacao"></a>

| Campo | Valor |
|---|---|
| Documento | WEB_STANDARD.md |
| Versão | 0.3.0 |
| Status | APROVADO |
| Escopo | HTML/CSS/JS embutido no firmware como string literal em interface.cpp |
| Pais | _PADRAO.md v0.1.0, 01_arquitetura.md v0.2.1, 07_interface_pedagogo.md v0.3.0 |

---

## 2. Objetivo <a id="objetivo"></a>

Definir de forma determinística as regras que governam todo código HTML, CSS e JavaScript embutido no firmware. Cada regra deriva de um documento aprovado. O mesmo princípio de zero magic numbers e rastreabilidade de valores que governa o C++ governa o código web.

**Princípio central:** nenhum valor com significado de domínio (cor, duração, frequência, chave de armazenamento, rota, SSID) aparece como literal no corpo de funções JS nem em propriedades CSS inline. Todo valor é declarado como `const` nomeada no topo do bloco `<script>`, com comentário rastreável a `spec/interface/interface.json`.

**Sistema de design:** Material Design 3 — implementado offline via CSS custom properties. Sem CDN externo. Derivado de [VER: 01_arquitetura.md#requisitos-nao-funcionais] RNF-04 e RNF-07.

---

## 3. Restrições de Plataforma <a id="restricoes-plataforma"></a>

Derivadas de [VER: 01_arquitetura.md#requisitos-nao-funcionais] e [VER: 01_arquitetura.md#stack-tecnologico].

| Restrição | Regra | Origem |
|---|---|---|
| Offline total | Nenhuma requisição a host externo (CDN, fonte, imagem, API) | RNF-04 |
| Sem framework | HTML + CSS + JS puro. Proibido React, Vue, Angular, jQuery, lit-element | RNF-07 |
| Browser mobile moderno | Chrome/Safari/Firefox em Android/iOS modernos. Sem polyfill IE | RNF-05 |
| Arquivo único | Todo HTML, CSS e JS num único arquivo servido por `/` | [VER: 07_interface_pedagogo.md#servidor-http] |
| Tipografia | `system-ui, -apple-system, sans-serif` — sem fonte externa | RNF-04 |

---

## 4. Sistema de Design — Material Design 3 <a id="design-system"></a>

### 4.1 Implementação offline <a id="md3-implementacao"></a>

Material Design 3 é implementado via CSS custom properties (variáveis CSS) declaradas em `:root`. Nenhuma folha de estilo externa é carregada. Os tokens abaixo são a fonte única de verdade para todos os valores visuais.

### 4.2 Tokens de cor <a id="tokens-cor"></a>

Derivados de [VER: 07_interface_pedagogo.md#tela-feedback] e [VER: 07_interface_pedagogo.md#identificacao]. Paleta MD3 gerada com `Primary: #1565C0` (azul pedagógico, contraste WCAG AA com branco).

| Token CSS | Valor | Derivação |
|---|---|---|
| `--md-sys-color-primary` | `#1565C0` | MD3 Primary — decisão de design deste documento |
| `--md-sys-color-on-primary` | `#FFFFFF` | MD3 On-Primary — contraste sobre primary |
| `--md-sys-color-surface` | `#FAFAFA` | MD3 Surface — fundo da tela |
| `--md-sys-color-on-surface` | `#1C1B1F` | MD3 On-Surface — texto principal |
| `--md-sys-color-surface-variant` | `#E7E0EC` | MD3 Surface Variant — cards, inputs |
| `--md-sys-color-outline` | `#79747E` | MD3 Outline — bordas e divisores |
| `--md-sys-color-error` | `#B3261E` | MD3 Error — estado de erro de formulário |
| `--md-sys-color-feedback-acerto` | `#2ECC40` | [VER: 07_interface_pedagogo.md#tela-feedback] — spec/interface/interface.json#feedback_visual.acerto_cor_hex |
| `--md-sys-color-feedback-erro` | `#FF4136` | [VER: 07_interface_pedagogo.md#tela-feedback] — spec/interface/interface.json#feedback_visual.erro_cor_hex |

### 4.3 Tokens de tipografia <a id="tokens-tipografia"></a>

Escala MD3 simplificada para este contexto pedagógico.

| Token CSS | Propriedades | Uso |
|---|---|---|
| `--md-sys-typescale-display` | `font-size:2rem; font-weight:400; line-height:2.5rem` | Números grandes (score, timer) |
| `--md-sys-typescale-headline` | `font-size:1.5rem; font-weight:400; line-height:2rem` | Títulos de tela |
| `--md-sys-typescale-title` | `font-size:1.125rem; font-weight:500; line-height:1.5rem` | Subtítulos, labels |
| `--md-sys-typescale-body` | `font-size:0.875rem; font-weight:400; line-height:1.25rem` | Texto de corpo |
| `--md-sys-typescale-label` | `font-size:0.75rem; font-weight:500; line-height:1rem` | Labels de campo, botão |

### 4.4 Tokens de espaçamento e forma <a id="tokens-espacamento"></a>

Grid MD3: múltiplos de 4dp.

| Token CSS | Valor | Uso |
|---|---|---|
| `--md-sys-spacing-xs` | `4px` | Espaçamento interno mínimo |
| `--md-sys-spacing-sm` | `8px` | Padding de label, gap interno |
| `--md-sys-spacing-md` | `16px` | Padding padrão de card e campo |
| `--md-sys-spacing-lg` | `24px` | Padding de seção |
| `--md-sys-spacing-xl` | `32px` | Margem entre blocos |
| `--md-sys-shape-small` | `4px` | Border-radius botão, campo |
| `--md-sys-shape-medium` | `12px` | Border-radius card |
| `--md-sys-shape-large` | `16px` | Border-radius dialog, overlay |
| `--md-sys-elevation-1` | `0 1px 2px rgba(0,0,0,.30)` | Elevação 1 — card padrão |
| `--md-sys-elevation-2` | `0 2px 6px rgba(0,0,0,.30)` | Elevação 2 — card com foco |

---

## 5. Estrutura HTML <a id="estrutura-html"></a>

### 5.1 Esqueleto obrigatório <a id="esqueleto-html"></a>

```html
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>BMI</title>     <!-- titulo = INTERFACE_AP_SSID -->
  <style>/* tokens + layout */</style>
</head>
<body>
  <div id="tela-configurando">...</div>
  <div id="tela-sessao"      hidden>...</div>
  <div id="overlay-feedback" hidden>...</div>
  <div id="tela-pausado"     hidden>...</div>
  <div id="tela-resultados"  hidden>...</div>
  <div id="overlay-exportacao" hidden>...</div>
  <script>/* constantes + estado + handlers */</script>
</body>
</html>
```

Regras:
- Atributo `hidden` controla visibilidade. Nunca `display:none` inline.
- IDs de elemento: `kebab-case`, prefixo `tela-` para telas de estado, `overlay-` para overlays.
- Nenhum inline `style=""` nos elementos — apenas classes e variáveis CSS.
- O `<title>` usa o valor de `INTERFACE_AP_SSID` (constante JS derivada de `interface.json#access_point.ssid`).

### 5.2 Mapeamento estado → elemento <a id="mapeamento-estado-elemento"></a>

Derivado de [VER: 07_interface_pedagogo.md#estados-interface].

| Estado UI | Elemento visível | Elemento oculto |
|---|---|---|
| `CONFIGURANDO` | `#tela-configurando` | todos os outros |
| `SESSAO_ATIVA` | `#tela-sessao` | todos os outros |
| `FEEDBACK_ACERTO` | `#tela-sessao` + `#overlay-feedback` | `#tela-configurando`, `#tela-pausado`, `#tela-resultados` |
| `FEEDBACK_ERRO` | `#tela-sessao` + `#overlay-feedback` | idem |
| `PAUSADO` | `#tela-pausado` | todos os outros |
| `RESULTADOS` | `#tela-resultados` | todos os outros |

`#overlay-feedback` é um `<div>` posicionado `position:fixed; inset:0` sobre `#tela-sessao`.

`#overlay-exportacao` é a pré-visualização de exportação ([VER: 07_interface_pedagogo.md#pre-visualizacao]) — `position:fixed; inset:0` sobre `#tela-resultados`. Não é um estado da máquina: só pode ficar visível enquanto `_estado === 'RESULTADOS'`, e o estado não muda com sua abertura/fechamento.

---

## 6. Máquina de Estados JavaScript <a id="maquina-estados-js"></a>

Derivada de [VER: 07_interface_pedagogo.md#estados-interface].

### 6.1 Representação do estado <a id="estado-js"></a>

```js
// Estado atual da UI — nunca acessar diretamente fora de setState()
let _estado = 'CONFIGURANDO';

function setState(novoEstado) {
    _estado = novoEstado;
    _renderizarEstado();
}
```

`_estado` assume exatamente os 6 valores de `spec/interface/interface.json#estados_interface.lista`:
`CONFIGURANDO | SESSAO_ATIVA | FEEDBACK_ACERTO | FEEDBACK_ERRO | PAUSADO | RESULTADOS`

### 6.2 Transições permitidas <a id="transicoes"></a>

| De | Para | Gatilho |
|---|---|---|
| `CONFIGURANDO` | `SESSAO_ATIVA` | envio de `INICIAR` via WebSocket |
| `SESSAO_ATIVA` | `FEEDBACK_ACERTO` | recebimento de mensagem `ACERTO` |
| `SESSAO_ATIVA` | `FEEDBACK_ERRO` | recebimento de mensagem `ERRO` |
| `FEEDBACK_ACERTO` | `SESSAO_ATIVA` | timer de `FEEDBACK_ACERTO_MS` expira |
| `FEEDBACK_ERRO` | `SESSAO_ATIVA` | recebimento de qualquer próximo evento |
| `SESSAO_ATIVA` | `PAUSADO` | recebimento de mensagem `PAUSADO` |
| `PAUSADO` | `SESSAO_ATIVA` | recebimento de mensagem `RETOMADO` |
| `SESSAO_ATIVA` | `RESULTADOS` | recebimento de mensagem `FIM_SESSAO` |
| `RESULTADOS` | `CONFIGURANDO` | clique em "Nova Sessão" |

---

## 7. Padrão JavaScript — Zero Magic Numbers <a id="zero-magic-numbers-js"></a>

Derivado de [VER: CODING_STANDARD.md#constantes-zero-magic-numbers].

### 7.1 Declaração de constantes <a id="declaracao-constantes-js"></a>

Todas as constantes de domínio são declaradas no topo do bloco `<script>`, antes de qualquer função, com comentário indicando a origem em `spec/interface/interface.json`.

```js
// Formato obrigatório:
// --- DERIVADO: spec/interface/interface.json#<campo> ---
const NOME_CONSTANTE = valor;
```

### 7.2 Tabela de constantes obrigatórias <a id="tabela-constantes-js"></a>

| Constante JS | Valor | Origem JSON |
|---|---|---|
| `WS_URL` | `"ws://192.168.4.1/ws"` | `access_point.ip` + `servidor_http.rotas[1].rota` |
| `AP_SSID` | `"BMI"` | `access_point.ssid` |
| `FEEDBACK_ACERTO_MS` | `1500` | `estados_interface.duracao_feedback_acerto_ms` |
| `FEEDBACK_ACERTO_COR` | `"#2ECC40"` | `feedback_visual.acerto_cor_hex` |
| `FEEDBACK_ERRO_COR` | `"#FF4136"` | `feedback_visual.erro_cor_hex` |
| `SOM_ACERTO_FREQS` | `[440, 880]` | `feedback_sonoro.acerto.frequencias_hz` |
| `SOM_ACERTO_DURACAO_MS` | `200` | `feedback_sonoro.acerto.duracao_por_tom_ms` |
| `SOM_ACERTO_OFFSET_MS` | `220` | `feedback_sonoro.acerto.offset_entre_tons_ms` |
| `SOM_ACERTO_ATTACK_S` | `0.01` | `feedback_sonoro.acerto.attack_ms` ÷ 1000 |
| `SOM_ACERTO_RELEASE_S` | `0.05` | `feedback_sonoro.acerto.release_ms` ÷ 1000 |
| `SOM_ACERTO_GANHO` | `0.4` | `feedback_sonoro.acerto.ganho` |
| `SOM_ERRO_FREQ` | `220` | `feedback_sonoro.erro.frequencia_hz` |
| `SOM_ERRO_DURACAO_MS` | `150` | `feedback_sonoro.erro.duracao_ms` |
| `SOM_ERRO_ATTACK_S` | `0.01` | `feedback_sonoro.erro.attack_ms` ÷ 1000 |
| `SOM_ERRO_RELEASE_S` | `0.05` | `feedback_sonoro.erro.release_ms` ÷ 1000 |
| `SOM_ERRO_GANHO` | `0.4` | `feedback_sonoro.erro.ganho` |
| `LS_CHAVE` | `"bmi_sessoes"` | `armazenamento.chave` |
| `CSV_CABECALHO` | `"id,nome,..."` | `exportacao_csv.cabecalho` |
| `CSV_NOME_ARQUIVO` | `"bmi_sessoes.csv"` | `exportacao_csv.nome_arquivo` |
| `CSV_MIME` | `"text/csv"` | `exportacao_csv.tipo_mime` |
| `CSV_CHARSET` | `"utf-8"` | `exportacao_csv.charset` |
| `CSV_DATA_URI_PREFIX` | `'data:' + CSV_MIME + ';charset=' + CSV_CHARSET + ','` | composta — [VER: 07_interface_pedagogo.md#mecanismo-download] |
| `CSV_BOM` | `"\uFEFF"` | `exportacao_csv.bom` — [VER: 07_interface_pedagogo.md#requisitos-csv] CSV-01 |
| `JANELA_MS_PADRAO` | `800` | `configuracao_ui.janela_ms_padrao` |
| `N_MIN` | `1` | `configuracao_ui.n_interacoes_min` |
| `MODO_PADRAO` | `"UM_MARTELO"` | `configuracao_ui.modo_padrao` |
| `MECA_PADRAO` | `"A_SHUFFLE"` | `configuracao_ui.mecanismo_padrao` |
| `EXPORT_FORMATOS` | `["CSV", "PDF"]` | `exportacao_ui.formatos` |
| `EXPORT_FORMATO_PADRAO` | `"CSV"` | `exportacao_ui.formato_padrao` |
| `PDF_NOME_ARQUIVO` | `"bmi_sessoes.pdf"` | `exportacao_pdf.nome_arquivo` |
| `PDF_MIME` | `"application/pdf"` | `exportacao_pdf.tipo_mime` |
| `PDF_DATA_URI_PREFIX` | `'data:' + PDF_MIME + ';base64,'` | composta — [VER: 07_interface_pedagogo.md#exportacao-pdf] PDF-06 |
| `PDF_VERSAO` | `"1.4"` | `exportacao_pdf.versao_pdf` |
| `PDF_TITULO` | `"Relatório de Sessões — BMI"` | `exportacao_pdf.titulo` |
| `PDF_FONTE_TITULO` | `"Helvetica-Bold"` | `exportacao_pdf.fontes.titulo.nome` |
| `PDF_FONTE_TITULO_PT` | `16` | `exportacao_pdf.fontes.titulo.tamanho_pt` |
| `PDF_FONTE_META` | `"Helvetica"` | `exportacao_pdf.fontes.metadados.nome` |
| `PDF_FONTE_META_PT` | `10` | `exportacao_pdf.fontes.metadados.tamanho_pt` |
| `PDF_FONTE_TABELA` | `"Courier"` | `exportacao_pdf.fontes.tabela.nome` |
| `PDF_FONTE_TABELA_PT` | `8` | `exportacao_pdf.fontes.tabela.tamanho_pt` |
| `PDF_PAG_LARGURA_PT` | `842` | `exportacao_pdf.pagina.largura_pt` |
| `PDF_PAG_ALTURA_PT` | `595` | `exportacao_pdf.pagina.altura_pt` |
| `PDF_MARGEM_PT` | `40` | `exportacao_pdf.pagina.margem_pt` |
| `PDF_ALTURA_LINHA_PT` | `12` | `exportacao_pdf.altura_linha_pt` |
| `PDF_ESPACO_POS_TITULO_PT` | `20` | `exportacao_pdf.espaco_pos_titulo_pt` |
| `PDF_ESPACO_POS_META_PT` | `24` | `exportacao_pdf.espaco_pos_meta_pt` |
| `PDF_LINHAS_DADOS_POR_PAGINA` | `35` | `exportacao_pdf.linhas_dados_por_pagina` |
| `PDF_COLUNAS` | array `{campo, titulo, largura_chars}` (10 itens) | `exportacao_pdf.colunas` |

### 7.3 Classificação de constantes web <a id="classificacao-constantes-js"></a>

| Classe | Definição | Ação |
|---|---|---|
| `DERIVADO` | Campo existe em `spec/interface/interface.json` | Declarar com comentário `--- DERIVADO: interface.json#campo ---` |
| `HARDCODED_WEB` | Detalhe de plataforma web (ex: `"application/json"`, `WebSocket.OPEN`) | Declarar com comentário técnico obrigatório |
| `MAGIC NUMBER` | Literal sem nome no corpo de função | **Proibido. Sem exceção.** |

---

## 8. Padrão de Funções JavaScript <a id="padrao-funcoes-js"></a>

### 8.1 Nomenclatura <a id="nomenclatura-funcoes-js"></a>

| Categoria | Padrão | Exemplos |
|---|---|---|
| Handlers de estado | `ao<Evento>()` | `aoReceberAcerto()`, `aoEnviarIniciar()` |
| Renderização | `renderizar<Tela>()` | `renderizarSessao()`, `renderizarResultados()` |
| Som | `tocar<Evento>()` | `tocarAcerto()`, `tocarErro()` |
| WebSocket | `conectarWS()`, `enviarMensagem(obj)` | — |
| Armazenamento | `salvarSessao(registro)`, `carregarSessoes()` | — |
| Exportação | `exportarCSV()`, `exportarPDF()`, `csvEscapar(campo)`, `baixarArquivo(href, nome)` | — |
| Pré-visualização | `abrirPrevia()`, `fecharPrevia()`, `renderizarPrevia(lista)` | — |
| Geração PDF (internas) | `_pdfWinAnsi(s)`, `_pdfEscapar(s)`, `_pdfGerar(lista)`, `_formatarDataGeracao(data)` | — |

### 8.2 Estrutura obrigatória do bloco `<script>` <a id="estrutura-script"></a>

```
1. Constantes derivadas (seção 7.2)
2. Estado da sessão (variáveis let mutáveis)
3. Referências a elementos DOM (const pelo id)
4. Funções de som (Web Audio API)
5. Funções de WebSocket
6. Funções de estado (setState, renderizarEstado)
7. Handlers de mensagens recebidas
8. Handlers de eventos DOM (submit, click)
9. Funções de armazenamento e exportação
10. Inicialização (chamada única ao carregar)
```

### 8.3 Estado de sessão <a id="estado-sessao-js"></a>

Variáveis de sessão declaradas como `let` no escopo do script, nomeadas com prefixo `s_`:

```js
let s_nome = '';
let s_nConfigurando = 0;
let s_modo = '';
let s_mecanismo = '';
let s_acertos = 0;
let s_erros = 0;
let s_timestampInicio = null;
let s_timerFeedback = null;
```

---

## 9. WebSocket <a id="websocket"></a>

Derivado de [VER: 07_interface_pedagogo.md#websocket].

### 9.1 Conexão e reconexão <a id="ws-conexao"></a>

```js
function conectarWS() {
    // derivado: WS_URL = "ws://192.168.4.1/ws"
    const ws = new WebSocket(WS_URL);
    ws.onopen    = aoConectarWS;
    ws.onclose   = aoDesconectarWS;
    ws.onmessage = aoReceberMensagem;
}
```

Reconexão automática: ao detectar `onclose`, agendar nova tentativa com `setTimeout`. Derivado de [VER: 07_interface_pedagogo.md#websocket] (`acao_reconexao: retomarSessao`).

### 9.2 Envio de mensagens <a id="ws-envio"></a>

Toda mensagem enviada ao ESP32 passa por `enviarMensagem(obj)`:

```js
function enviarMensagem(obj) {
    if (s_ws && s_ws.readyState === WebSocket.OPEN) {
        s_ws.send(JSON.stringify(obj));
    }
}
```

Nenhuma chamada direta a `s_ws.send()` fora desta função.

---

## 10. Armazenamento e Exportação <a id="armazenamento-exportacao"></a>

Derivado de [VER: 07_interface_pedagogo.md#armazenamento-dados] e [VER: 07_interface_pedagogo.md#exportacao-csv].

### 10.1 Estrutura do registro <a id="estrutura-registro"></a>

Campos e tipos derivados de `spec/interface/interface.json#armazenamento.campos_registro`:

| Campo | Tipo JS | Fórmula / Origem |
|---|---|---|
| `id` | `string` | `String(Date.now())` ao iniciar sessão |
| `nome` | `string` | campo UI |
| `timestamp_inicio` | `string` | `new Date().toISOString()` ao iniciar |
| `modo` | `string` | `"UM_MARTELO"` ou `"DOIS_MARTELOS"` |
| `mecanismo` | `string` | `"A_SHUFFLE"` ou `"B_PESO"` |
| `n_configurado` | `integer` | campo UI |
| `acertos` | `integer` | `EventoJogo.acertos` do `FIM_SESSAO` |
| `erros` | `integer` | contador local de eventos `ERRO` |
| `taxa_pct` | `number` | `(acertos / n_configurado * 100).toFixed(1)` |
| `duracao_s` | `number` | `(duracao_ms / 1000).toFixed(1)` |

### 10.2 Exportação CSV <a id="exportacao-csv"></a>

O cabeçalho CSV é exatamente `CSV_CABECALHO` (constante derivada de `interface.json#exportacao_csv.cabecalho`). A ordem das colunas no CSV segue a ordem dos campos na tabela acima. O nome do arquivo é `CSV_NOME_ARQUIVO`.

Requisitos do arquivo e mecanismo de download: [VER: 07_interface_pedagogo.md#requisitos-csv] e [VER: 07_interface_pedagogo.md#mecanismo-download]. Regras derivadas:

- Todo campo passa por `csvEscapar()` (RFC 4180 §2) antes do `join(',')` — CSV-02.
- O conteúdo é prefixado com `CSV_BOM` (`interface.json#exportacao_csv.bom`) — CSV-01.
- Download via `data:` URI: `CSV_DATA_URI_PREFIX + encodeURIComponent(csv)` em âncora com atributo `download`, anexada ao `document.body` antes de `click()` e removida após — CSV-04. Proibido: `URL.createObjectURL`/`revokeObjectURL` para exportação e `click()` em âncora fora do DOM (causa raiz do defeito D1).
- A âncora de download é criada em um único ponto: `baixarArquivo(href, nome)` — usada por `exportarCSV()` e `exportarPDF()`. Nenhuma outra função cria âncora de download.

### 10.3 Pré-visualização e confirmação <a id="pre-visualizacao-js"></a>

Derivado de [VER: 07_interface_pedagogo.md#pre-visualizacao] (PRE-01..05).

- `exportarCSV()` e `exportarPDF()` **nunca** são chamadas diretamente por botão da tela de resultados — somente pela confirmação da prévia. O botão "Exportar" chama `abrirPrevia()`.
- `abrirPrevia()`: chama `renderizarPrevia(carregarSessoes())` e exibe `#overlay-exportacao`. Nenhum download é iniciado.
- `renderizarPrevia(lista)`: monta tabela HTML com as 10 colunas de `armazenamento.campos_registro`, na mesma ordem do CSV, uma linha por registro. Lista vazia: exibe aviso e desabilita o botão Baixar (`disabled`) — PRE-05.
- Seletor de formato: valores exatamente `EXPORT_FORMATOS`, seleção inicial `EXPORT_FORMATO_PADRAO` — PRE-03.
- Botão Baixar: despacha para `exportarCSV()` ou `exportarPDF()` conforme o formato selecionado e chama `fecharPrevia()` — PRE-04.
- Botão Cancelar: chama apenas `fecharPrevia()` — nenhum download — PRE-04.
- Conteúdo da tabela via `textContent` (nunca `innerHTML` com dado de sessão) — nome de criança é entrada de usuário.

### 10.4 Geração de PDF <a id="geracao-pdf-js"></a>

Derivado de [VER: 07_interface_pedagogo.md#exportacao-pdf] (PDF-01..06).

- Todo valor de layout (página, margem, fontes, tamanhos, alturas, larguras de coluna, linhas por página) vem das constantes `PDF_*` da seção 7.2 — zero magic numbers também dentro do gerador.
- `_pdfWinAnsi(s)`: converte string JS (UTF-16) para bytes WinAnsi; code points latin-1 (≤ `0xFF`, fora de `0x80–0x9F`) passam direto; caracteres tipográficos com posição própria no WinAnsi (ex: `…` `—` `–`) usam a posição do Annex D.2; qualquer outro vira `?` — PDF-02. A tabela de mapeamento é `HARDCODED_WEB` (conteúdo normativo do ISO 32000-1 Annex D.2, não é dado de domínio).
- `_pdfEscapar(s)`: escapa `\`, `(` e `)` — PDF-05. Aplicado após `_pdfWinAnsi`.
- `_pdfGerar(lista)`: monta o arquivo como string binária (1 char = 1 byte); os offsets da tabela `xref` são calculados sobre o comprimento acumulado dessa string — byte-exatos por construção. Retorna a string binária completa (`%PDF-` … `%%EOF`).
- Célula da tabela: valor convertido, truncado em `largura_chars − 2` + `…` quando excede, e completado com espaços até `largura_chars` (`padEnd`) — Courier garante alinhamento.
- Download: `PDF_DATA_URI_PREFIX + btoa(_pdfGerar(lista))` via `baixarArquivo()` — PDF-06. `btoa` é seguro aqui porque após `_pdfWinAnsi` todos os code points são ≤ `0xFF`.
- Data/hora de geração: `_formatarDataGeracao(new Date())` no formato `dd/mm/aaaa hh:mm` (PDF-03), montado com `getDate()/getMonth()/getFullYear()/getHours()/getMinutes()` + `padStart` — sem `toLocaleString` (saída varia por engine; determinismo obrigatório).

---

## 11. Padrão de Testes — Derivação Sem Overfit <a id="padrao-testes"></a>

Derivado de [VER: TESTING_STANDARD.md#objetivo] — o mesmo princípio que governa testes Unity governa cenários de teste web.

### 11.1 Princípio central <a id="teste-principio"></a>

**O cenário de teste define o comportamento esperado derivado da especificação. O cenário não é escrito após ler o HTML — é escrito derivando de `spec/interface/interface.json` e `07_interface_pedagogo.md` antes de qualquer linha de HTML/CSS/JS existir.**

Regra: se um cenário de teste só passa porque o código foi escrito de um jeito específico (em vez de porque a spec exige aquele comportamento), o cenário está com overfit. Reescrever.

### 11.2 Ordem de trabalho obrigatória <a id="teste-ordem"></a>

```
1. Ler interface.json#criterios_aceitacao e 07_interface_pedagogo.md#criterios-aceitacao
2. Para cada CA, escrever o cenário de teste ANTES de escrever o HTML
3. O cenário descreve: precondição → ação → resultado esperado
   Os valores esperados vêm EXCLUSIVAMENTE de interface.json (ex: 1500ms, #2ECC40)
4. Somente após todos os cenários estarem escritos: implementar o HTML/CSS/JS
5. Verificar cada cenário contra a implementação
```

### 11.3 Formato obrigatório de cenário <a id="teste-formato"></a>

```
[CA-07-NN] Nome do CA (fonte: interface.json#criterios_aceitacao[N].id)
Precondição : estado da UI e do WebSocket antes do teste
Ação        : o que o testador faz ou o que chega via WebSocket
Resultado   : comportamento observável esperado, com valor derivado de interface.json
              ex: "tela fica verde (#2ECC40) por exatamente 1500ms"
              NÃO: "tela fica verde por um tempo razoável"
Não testável: se o CA requer hardware ou infra além do browser, documentar aqui
```

### 11.4 Proibições de cenário <a id="teste-proibicoes"></a>

| Proibido | Correto |
|---|---|
| "Verificar que o código envia o JSON correto" | "Inspecionar DevTools Network: payload é `{\"tipo\":\"INICIAR\",...}`" |
| "Verificar que a variável `s_acertos` é 3" | "Verificar que o elemento `#acertos` exibe `3`" |
| "Testar que `tocarAcerto()` foi chamada" | "Verificar que som positivo ascendente é audível após acerto" |
| Valor esperado inventado: "verde por ~1s" | Valor derivado: "verde por 1500ms (interface.json#estados_interface.duracao_feedback_acerto_ms)" |

Testes de caixa branca (inspecionar variáveis JS internas) são proibidos. Testes de caixa preta (comportamento observável) são obrigatórios.

### 11.5 Cenários obrigatórios (derivados de interface.json#criterios_aceitacao) <a id="teste-cenarios"></a>

Os cenários abaixo são derivados diretamente de `spec/interface/interface.json#criterios_aceitacao` e `07_interface_pedagogo.md#criterios-aceitacao`. Devem ser escritos e revisados **antes** de qualquer HTML ser escrito.

**[CA-07-01] Hotspot visível após boot**
- Precondição: ESP32 energizado, firmware flashado
- Ação: ligar o ESP32, aguardar
- Resultado: SSID `"BMI"` (interface.json#access_point.ssid) aparece na lista WiFi em < 5s
- Não testável em browser — requer hardware

**[CA-07-02] Carregamento da interface**
- Precondição: dispositivo conectado ao AP `BMI`, browser aberto
- Ação: navegar para `192.168.4.1` (interface.json#access_point.ip)
- Resultado: tela de configuração carrega em < 3s (interface.json#criterios_aceitacao[1].carga_max_s)

**[CA-07-03] Validação do formulário**
- Precondição: estado CONFIGURANDO, campo nome vazio
- Ação: submeter formulário com nome vazio
- Resultado: submissão bloqueada, mensagem de erro visível; nenhuma mensagem enviada ao ESP32
- Variante: preencher todos os campos → sessão inicia (estado muda para SESSAO_ATIVA)

**[CA-07-04] Visibilidade do campo janela**
- Precondição: estado CONFIGURANDO, modo selecionado = `"UM_MARTELO"`
- Ação: observar campo janela_ms
- Resultado: campo janela oculto
- Variante: selecionar `"DOIS_MARTELOS"` → campo janela aparece com valor `800` (interface.json#configuracao_ui.janela_ms_padrao)

**[CA-07-05] Feedback de acerto**
- Precondição: estado SESSAO_ATIVA, WebSocket conectado
- Ação: servidor envia `{"tipo":"ACERTO","acertos":1,"total":5,"duracao_ms":3000}`
- Resultado: tela fica verde `#2ECC40` (interface.json#feedback_visual.acerto_cor_hex) em < 200ms; som positivo ascendente audível; após exatamente 1500ms (interface.json#estados_interface.duracao_feedback_acerto_ms) tela retorna para SESSAO_ATIVA

**[CA-07-06] Feedback de erro**
- Precondição: estado SESSAO_ATIVA, WebSocket conectado
- Ação: servidor envia `{"tipo":"ERRO","acertos":1,"total":5,"duracao_ms":3000}`
- Resultado: tela fica vermelha `#FF4136` (interface.json#feedback_visual.erro_cor_hex) em < 200ms; som neutro audível; overlay permanece até próximo evento

**[CA-07-07] Tela de resultados**
- Precondição: estado SESSAO_ATIVA
- Ação: servidor envia `{"tipo":"FIM_SESSAO","acertos":8,"total":10,"duracao_ms":45000}`
- Resultado: tela de resultados exibe nome da criança, `8/10`, `80.0%`, `45.0s`

**[CA-07-08] Persistência localStorage**
- Precondição: estado RESULTADOS, localStorage vazio
- Ação: clicar "Nova Sessão"
- Resultado: DevTools → Application → localStorage → chave `"bmi_sessoes"` (interface.json#armazenamento.chave) contém array com 1 registro com os 10 campos de interface.json#armazenamento.campos_registro

**[CA-07-09] Exportação CSV**
- Precondição: localStorage com ≥ 1 registro, incluindo um registro com nome contendo vírgula e um com acento (ex: `"Silva, João"`)
- Ação: clicar "Exportar", manter o formato `CSV` (interface.json#exportacao_ui.formato_padrao) selecionado, clicar "Baixar"
- Resultado: download de arquivo `"bmi_sessoes.csv"` (interface.json#exportacao_csv.nome_arquivo) com primeira linha exatamente `"id,nome,timestamp_inicio,modo,mecanismo,n_configurado,acertos,erros,taxa_pct,duracao_s"` (interface.json#exportacao_csv.cabecalho), precedida apenas do BOM U+FEFF (interface.json#exportacao_csv.bom — invisível em editor de texto); aberto em planilha: nome com vírgula ocupa uma única coluna (RFC 4180) e acentos exibidos corretamente

**[CA-07-12] Pré-visualização e confirmação**
- Precondição: estado RESULTADOS, localStorage com ≥ 2 registros
- Ação: clicar "Exportar"
- Resultado: `#overlay-exportacao` visível com tabela contendo as 10 colunas de interface.json#armazenamento.campos_registro, na mesma ordem, e uma linha por registro com valores idênticos ao localStorage; nenhum download iniciado (DevTools → sem navegação/download); seletor exibe `CSV` e `PDF` (interface.json#exportacao_ui.formatos) com `CSV` selecionado
- Variante: clicar "Cancelar" → overlay fecha, nenhum download
- Variante: localStorage vazio → aviso de ausência de registros visível e botão "Baixar" com atributo `disabled`

**[CA-07-13] Exportação PDF**
- Precondição: estado RESULTADOS, localStorage com ≥ 1 registro com acento no nome (ex: `"João"`)
- Ação: clicar "Exportar", selecionar `PDF`, clicar "Baixar"
- Resultado: download de `"bmi_sessoes.pdf"` (interface.json#exportacao_pdf.nome_arquivo); arquivo abre sem erro em leitor de PDF; contém o título (interface.json#exportacao_pdf.titulo), data/hora de geração no formato `dd/mm/aaaa hh:mm`, `Página 1 de 1` e tabela com todas as sessões e as 10 colunas (interface.json#exportacao_pdf.colunas); acentos exibidos corretamente
- Variante: > 35 registros (interface.json#exportacao_pdf.linhas_dados_por_pagina) → arquivo com 2 páginas, metadados `Página 1 de 2` e `Página 2 de 2`, cabeçalho de tabela repetido na página 2

**[CA-07-10] Desconexão e retomada**
- Precondição: estado SESSAO_ATIVA, WiFi ativo
- Ação: desligar WiFi do dispositivo, aguardar detecção; religar WiFi
- Resultado: estado muda para PAUSADO; ao reconectar estado retoma onde parou (interface.json#websocket.acao_desconexao e acao_reconexao)
- Requer hardware para teste completo

**[CA-07-11] Operação offline total**
- Precondição: dispositivo conectado ao AP sem internet
- Ação: usar a interface completa (configurar, iniciar sessão, exportar CSV)
- Resultado: nenhuma requisição a host externo (verificar DevTools → Network)

---

## 12. Critérios de Aceitação <a id="criterios-aceitacao"></a>

Verificação manual no browser após flash do firmware.

| # | Teste | Condição de aprovação |
|---|---|---|
| CA-07-UI-01 | Tokens MD3 aplicados | Tela de configuração usa cor `--md-sys-color-primary` em botão e foco; nenhum `#` hexadecimal hardcoded em CSS fora de `:root` |
| CA-07-UI-02 | Layout responsivo | Formulário legível em 375px (iPhone SE) e 768px (tablet) sem scroll horizontal |
| CA-07-UI-03 | Zero magic literals no script | Inspecionar fonte: nenhum número ou string de domínio fora do bloco de constantes |
| CA-07-UI-04 | Estados mutuamente exclusivos | Em cada estado, exatamente um elemento de tela está visível (verificar com DevTools) |
| CA-07-UI-05 | Overlay fullscreen | `#overlay-feedback` cobre 100% da viewport em ambas as cores (testar com `setState('FEEDBACK_ACERTO')` no console) |
| CA-07-UI-06 | Transição automática ACERTO | Overlay verde desaparece após `FEEDBACK_ACERTO_MS` sem ação do usuário |
| CA-07-UI-07 | Persistência de sessão no localStorage | Após "Nova Sessão", abrir DevTools → Application → localStorage → verificar registro com todos os 10 campos |
| CA-07-UI-08 | Reconexão transparente | Desligar e religar WiFi do dispositivo: interface reconecta e exibe estado correto sem recarregar a página |

---

## Changelog

| Versão | Data | Seção | Mudança | Impacto em dependentes |
|---|---|---|---|---|
| 0.1.0 | 2026-06-28 | — | Criação: padrão de UI web embutida — MD3 offline, zero magic numbers JS/CSS, estrutura HTML, máquina de estados, nomenclatura de funções | firmware/src/interface/interface.cpp |
| 0.1.1 | 2026-07-01 | depende_de, Rastreabilidade, #identificacao | Atualiza referências: 01_arquitetura.md v0.1.0→v0.2.0 (bump MINOR retroativo), 07_interface_pedagogo.md v0.1.0→v0.1.1 | — |
| 0.1.2 | 2026-07-01 | depende_de | Atualiza referências: 01_arquitetura.md v0.2.0→v0.2.1 (especifica DevKitC V4), 07_interface_pedagogo.md v0.1.1→v0.1.2 | — |
| 0.2.0 | 2026-07-03 | #tabela-constantes-js, #classificacao-constantes-js, #nomenclatura-funcoes-js, #exportacao-csv, #teste-cenarios | Cascata do 07 v0.2.0 (correção D1): novas constantes obrigatórias CSV_MIME, CSV_CHARSET, CSV_DATA_URI_PREFIX, CSV_BOM (todas derivadas de exportacao_csv); csvEscapar() na nomenclatura; §10.2 com regras de escaping/BOM/data URI e proibição de blob+revoke na exportação; cenário CA-07-09 estendido; corrige versões desatualizadas em #identificacao e Rastreabilidade (01 v0.2.0→v0.2.1, 07 v0.1.1→v0.2.0) | firmware/src/interface/interface.cpp |
| 0.3.0 | 2026-07-03 | #esqueleto-html, #mapeamento-estado-elemento, #tabela-constantes-js, #nomenclatura-funcoes-js, #pre-visualizacao-js, #geracao-pdf-js, #teste-cenarios | Cascata do 07 v0.3.0 (melhorias M2/M3): `#overlay-exportacao` no esqueleto e no mapeamento; constantes obrigatórias EXPORT_* e PDF_* (derivadas de exportacao_ui e exportacao_pdf); nomenclatura exportarPDF/baixarArquivo/abrirPrevia/fecharPrevia/renderizarPrevia e internas de geração PDF; §10.3 regras da pré-visualização (PRE-01..05, textContent obrigatório); §10.4 regras do gerador PDF (WinAnsi, escaping, xref byte-exato, btoa, data determinística); cenário CA-07-09 ajustado ao fluxo com prévia; cenários CA-07-12 e CA-07-13 novos | firmware/src/interface/interface.cpp |

---

## Rastreabilidade

| Tipo | Documento | Versão | Vínculo | Âncora relevante |
|---|---|---|---|---|
| Pai | _PADRAO.md | 0.1.0 | BLOQUEADOR | — |
| Pai | 01_arquitetura.md | 0.2.1 | BLOQUEADOR | #requisitos-nao-funcionais, #stack-tecnologico |
| Pai | 07_interface_pedagogo.md | 0.3.0 | BLOQUEADOR | #estados-interface, #feedback-sonoro, #websocket, #armazenamento-dados, #exportacao-csv, #requisitos-csv, #mecanismo-download, #pre-visualizacao, #exportacao-pdf, #tela-feedback |
| Governa | firmware/src/interface/ | — | OBRIGATÓRIO | interface.cpp (HTML/CSS/JS embutido) |

---

Licenca: GPL-3.0 — consulte `/LICENSE` na raiz do repositorio.
