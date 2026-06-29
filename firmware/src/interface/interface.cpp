// interface.cpp — Implementacao de MOD_WIFI
// Derivado de: spec/interface/interface.json + modules/interface/07_interface_pedagogo.md
// [VER: 01_arquitetura.md#interface-jogo-wifi]
// [VER: WEB_STANDARD.md]

#include "interface.h"
#include "interface_config.h"
#include "game.h"
#include "game_config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// ---------------------------------------------------------------------------
// Constantes de protocolo — nao geradas por interface_config.h
// ---------------------------------------------------------------------------

// --- DERIVADO: spec/interface/interface.json#protocolo_mensagens.browser_para_esp32[0].campos[3].valores_validos ---
static constexpr const char* INTERFACE_MODO_DOIS_MARTELOS = "DOIS_MARTELOS";

// --- DERIVADO: spec/interface/interface.json#protocolo_mensagens.browser_para_esp32[0].campos[4].valores_validos ---
static constexpr const char* INTERFACE_MECA_B = "B_PESO";

// --- DERIVADO: spec/interface/interface.json#configuracao_ui.janela_ms_padrao ---
static constexpr uint16_t INTERFACE_JANELA_MS_PADRAO = 800u;

// --- DERIVADO: spec/interface/interface.json#protocolo_mensagens.browser_para_esp32[*].campos[0].valor_fixo ---
static constexpr const char* MSG_INICIAR  = "INICIAR";
static constexpr const char* MSG_PAUSAR   = "PAUSAR";
static constexpr const char* MSG_RETOMAR  = "RETOMAR";

// --- DERIVADO: spec/interface/interface.json#protocolo_mensagens.esp32_para_browser[*].campos[0].valor_fixo ---
static constexpr const char* MSG_ACERTO     = "ACERTO";
static constexpr const char* MSG_ERRO       = "ERRO";
static constexpr const char* MSG_FIM_SESSAO = "FIM_SESSAO";
static constexpr const char* MSG_PAUSADO    = "PAUSADO";
static constexpr const char* MSG_RETOMADO   = "RETOMADO";

// HARDCODED JUSTIFICADO: porta HTTP padrao (RFC 2616). Sem campo na spec pois e fixo.
static constexpr uint16_t INTERFACE_HTTP_PORT = 80u;

// HARDCODED JUSTIFICADO: tipo MIME HTML. Detalhe de plataforma HTTP.
static constexpr const char* INTERFACE_HTML_MIME = "text/html";

// HARDCODED JUSTIFICADO: mascara /24 para AP de cliente unico. Sem campo na spec.
static const IPAddress INTERFACE_AP_MASCARA(255u, 255u, 255u, 0u);

// ---------------------------------------------------------------------------
// Estado interno
// ---------------------------------------------------------------------------

// HARDCODED JUSTIFICADO: porta HTTP padrao declarada acima
static AsyncWebServer s_server(INTERFACE_HTTP_PORT);
static AsyncWebSocket s_ws(INTERFACE_WS_PATH);
static bool           s_sessao_ativa = false;

// ---------------------------------------------------------------------------
// HTML inline — [VER: WEB_STANDARD.md#esqueleto-html]
// Tokens MD3 em :root — [VER: WEB_STANDARD.md#design-system]
// Constantes JS — [VER: WEB_STANDARD.md#tabela-constantes-js]
// ---------------------------------------------------------------------------

static const char HTML_PAGE[] PROGMEM = R"rawhtml(<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>BMI</title>
  <style>
    /* --- Tokens MD3 — WEB_STANDARD.md#tokens-cor / #tokens-tipografia / #tokens-espacamento --- */
    :root {
      --md-sys-color-primary:          #1565C0;
      --md-sys-color-on-primary:       #FFFFFF;
      --md-sys-color-surface:          #FAFAFA;
      --md-sys-color-on-surface:       #1C1B1F;
      --md-sys-color-surface-variant:  #E7E0EC;
      --md-sys-color-outline:          #79747E;
      --md-sys-color-error:            #B3261E;
      --md-sys-color-feedback-acerto:  #2ECC40;
      --md-sys-color-feedback-erro:    #FF4136;
      --md-sys-spacing-xs:  4px;
      --md-sys-spacing-sm:  8px;
      --md-sys-spacing-md:  16px;
      --md-sys-spacing-lg:  24px;
      --md-sys-spacing-xl:  32px;
      --md-sys-shape-small:  4px;
      --md-sys-shape-medium: 12px;
      --md-sys-shape-large:  16px;
      --md-sys-elevation-1:  0 1px 2px rgba(0,0,0,.30);
      --md-sys-elevation-2:  0 2px 6px rgba(0,0,0,.30);
    }

    /* --- Reset e base --- */
    *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: system-ui, -apple-system, sans-serif;
      background: var(--md-sys-color-surface);
      color: var(--md-sys-color-on-surface);
      min-height: 100dvh;
      display: flex;
      align-items: center;
      justify-content: center;
      padding: var(--md-sys-spacing-md);
    }
    [hidden] { display: none !important; }

    /* --- Card --- */
    .card {
      background: var(--md-sys-color-surface);
      border-radius: var(--md-sys-shape-medium);
      box-shadow: var(--md-sys-elevation-1);
      padding: var(--md-sys-spacing-lg);
      width: 100%;
      max-width: 480px;
    }

    /* --- Tipografia MD3 --- */
    h1 {
      font-size: 1.5rem; font-weight: 400; line-height: 2rem;
      color: var(--md-sys-color-primary);
      margin-bottom: var(--md-sys-spacing-lg);
    }
    h2 {
      font-size: 1.5rem; font-weight: 400; line-height: 2rem;
      margin-bottom: var(--md-sys-spacing-md);
    }

    /* --- Formulario --- */
    .field { margin-bottom: var(--md-sys-spacing-md); }
    label {
      display: block;
      font-size: 0.75rem; font-weight: 500; line-height: 1rem;
      color: var(--md-sys-color-outline);
      margin-bottom: var(--md-sys-spacing-xs);
    }
    input, select {
      width: 100%;
      padding: var(--md-sys-spacing-sm) var(--md-sys-spacing-md);
      border: 1px solid var(--md-sys-color-outline);
      border-radius: var(--md-sys-shape-small);
      background: var(--md-sys-color-surface-variant);
      color: var(--md-sys-color-on-surface);
      font-family: inherit;
      font-size: 0.875rem;
    }
    input:focus, select:focus {
      outline: 2px solid var(--md-sys-color-primary);
      outline-offset: 1px;
    }
    .err-msg {
      font-size: 0.75rem; color: var(--md-sys-color-error);
      margin-top: var(--md-sys-spacing-xs);
      display: none;
    }

    /* --- Botoes --- */
    .btn {
      display: block; width: 100%;
      padding: var(--md-sys-spacing-sm) var(--md-sys-spacing-md);
      background: var(--md-sys-color-primary);
      color: var(--md-sys-color-on-primary);
      border: none;
      border-radius: var(--md-sys-shape-small);
      font-family: inherit;
      font-size: 0.75rem; font-weight: 500; letter-spacing: 0.05em;
      cursor: pointer;
      margin-top: var(--md-sys-spacing-md);
    }
    .btn:hover { opacity: 0.92; }
    .btn-outline {
      background: transparent;
      border: 1px solid var(--md-sys-color-primary);
      color: var(--md-sys-color-primary);
      margin-top: var(--md-sys-spacing-sm);
    }

    /* --- Linha de estatistica --- */
    .stat-row {
      display: flex; justify-content: space-between; align-items: center;
      padding: var(--md-sys-spacing-sm) 0;
      border-bottom: 1px solid var(--md-sys-color-surface-variant);
    }
    .stat-val {
      font-size: 2rem; font-weight: 400;
      color: var(--md-sys-color-primary);
    }

    /* --- Overlay de feedback — WEB_STANDARD.md#mapeamento-estado-elemento --- */
    #overlay-feedback {
      position: fixed; inset: 0;
      display: flex; align-items: center; justify-content: center;
    }

    /* --- Tela pausado --- */
    #tela-pausado { text-align: center; }
    #tela-pausado p {
      margin-top: var(--md-sys-spacing-md);
      font-size: 0.875rem; color: var(--md-sys-color-outline);
    }
  </style>
</head>
<body>

  <!-- CONFIGURANDO — WEB_STANDARD.md#mapeamento-estado-elemento -->
  <div id="tela-configurando" class="card">
    <h1>BMI — Configurar Sessao</h1>
    <form id="form-sessao">
      <div class="field">
        <label for="campo-nome">Nome da crianca</label>
        <input id="campo-nome" type="text" autocomplete="off">
        <div class="err-msg" id="erro-nome">Nome obrigatorio.</div>
      </div>
      <div class="field">
        <label for="campo-n">Numero de interacoes</label>
        <input id="campo-n" type="number" value="10" min="1">
      </div>
      <div class="field">
        <label for="campo-modo">Modo</label>
        <select id="campo-modo">
          <option value="UM_MARTELO">Um martelo</option>
          <option value="DOIS_MARTELOS">Dois martelos</option>
        </select>
      </div>
      <div class="field" id="campo-janela-wrapper" hidden>
        <label for="campo-janela">Janela (ms)</label>
        <input id="campo-janela" type="number" value="800" min="100">
      </div>
      <div class="field">
        <label for="campo-mecanismo">Mecanismo</label>
        <select id="campo-mecanismo">
          <option value="A_SHUFFLE">A - Shuffle</option>
          <option value="B_PESO">B - Peso</option>
        </select>
      </div>
      <button type="submit" class="btn">Iniciar Sessao</button>
    </form>
  </div>

  <!-- SESSAO_ATIVA -->
  <div id="tela-sessao" class="card" hidden>
    <h1>Sessao em andamento</h1>
    <div class="stat-row">
      <span>Acertos</span>
      <span class="stat-val" id="stat-acertos">0</span>
    </div>
    <div class="stat-row">
      <span>Total configurado</span>
      <span class="stat-val" id="stat-total">-</span>
    </div>
    <button class="btn btn-outline" id="btn-pausar">Pausar</button>
  </div>

  <!-- OVERLAY FEEDBACK — position:fixed sobre tela-sessao — WEB_STANDARD.md#mapeamento-estado-elemento -->
  <div id="overlay-feedback" hidden></div>

  <!-- PAUSADO -->
  <div id="tela-pausado" class="card" hidden>
    <h2>Sessao pausada</h2>
    <p>Reconectando ao dispositivo...</p>
    <button class="btn" id="btn-retomar">Retomar</button>
  </div>

  <!-- RESULTADOS -->
  <div id="tela-resultados" class="card" hidden>
    <h2>Resultados</h2>
    <div class="stat-row"><span>Nome</span>   <span id="res-nome">-</span></div>
    <div class="stat-row"><span>Acertos</span><span class="stat-val" id="res-acertos">-</span></div>
    <div class="stat-row"><span>Total</span>  <span id="res-total">-</span></div>
    <div class="stat-row"><span>Taxa</span>   <span id="res-taxa">-</span></div>
    <div class="stat-row"><span>Duracao</span><span id="res-duracao">-</span></div>
    <button class="btn"         id="btn-nova-sessao">Nova Sessao</button>
    <button class="btn btn-outline" id="btn-exportar">Exportar CSV</button>
  </div>

  <script>
    // =======================================================================
    // 1. Constantes derivadas — WEB_STANDARD.md#tabela-constantes-js
    // =======================================================================

    // --- DERIVADO: spec/interface/interface.json#access_point.ip + servidor_http.rotas[1].rota ---
    const WS_URL = 'ws://192.168.4.1/ws';
    // --- DERIVADO: spec/interface/interface.json#access_point.ssid ---
    const AP_SSID = 'BMI';
    // --- DERIVADO: spec/interface/interface.json#estados_interface.duracao_feedback_acerto_ms ---
    const FEEDBACK_ACERTO_MS = 1500;
    // --- DERIVADO: spec/interface/interface.json#feedback_visual.acerto_cor_hex ---
    const FEEDBACK_ACERTO_COR = '#2ECC40';
    // --- DERIVADO: spec/interface/interface.json#feedback_visual.erro_cor_hex ---
    const FEEDBACK_ERRO_COR = '#FF4136';
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.acerto.frequencias_hz ---
    const SOM_ACERTO_FREQS = [440, 880];
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.acerto.duracao_por_tom_ms ---
    const SOM_ACERTO_DURACAO_MS = 200;
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.acerto.offset_entre_tons_ms ---
    const SOM_ACERTO_OFFSET_MS = 220;
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.acerto.attack_ms / 1000 ---
    const SOM_ACERTO_ATTACK_S = 0.01;
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.acerto.release_ms / 1000 ---
    const SOM_ACERTO_RELEASE_S = 0.05;
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.acerto.ganho ---
    const SOM_ACERTO_GANHO = 0.4;
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.erro.frequencia_hz ---
    const SOM_ERRO_FREQ = 220;
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.erro.duracao_ms ---
    const SOM_ERRO_DURACAO_MS = 150;
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.erro.attack_ms / 1000 ---
    const SOM_ERRO_ATTACK_S = 0.01;
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.erro.release_ms / 1000 ---
    const SOM_ERRO_RELEASE_S = 0.05;
    // --- DERIVADO: spec/interface/interface.json#feedback_sonoro.erro.ganho ---
    const SOM_ERRO_GANHO = 0.4;
    // --- DERIVADO: spec/interface/interface.json#armazenamento.chave ---
    const LS_CHAVE = 'bmi_sessoes';
    // --- DERIVADO: spec/interface/interface.json#exportacao_csv.cabecalho ---
    const CSV_CABECALHO = 'id,nome,timestamp_inicio,modo,mecanismo,n_configurado,acertos,erros,taxa_pct,duracao_s';
    // --- DERIVADO: spec/interface/interface.json#exportacao_csv.nome_arquivo ---
    const CSV_NOME_ARQUIVO = 'bmi_sessoes.csv';
    // --- DERIVADO: spec/interface/interface.json#configuracao_ui.janela_ms_padrao ---
    const JANELA_MS_PADRAO = 800;
    // --- DERIVADO: spec/interface/interface.json#configuracao_ui.n_interacoes_min ---
    const N_MIN = 1;
    // --- DERIVADO: spec/interface/interface.json#configuracao_ui.modo_padrao ---
    const MODO_PADRAO = 'UM_MARTELO';
    // --- DERIVADO: spec/interface/interface.json#configuracao_ui.mecanismo_padrao ---
    const MECA_PADRAO = 'A_SHUFFLE';
    // HARDCODED_WEB: intervalo de reconexao WS — detalhe de plataforma sem campo na spec
    const WS_RECONEXAO_MS = 3000;
    // HARDCODED_WEB: tipo MIME CSV — detalhe de plataforma HTTP
    const CSV_MIME = 'text/csv';

    // =======================================================================
    // 2. Estado de sessao — WEB_STANDARD.md#estado-sessao-js
    // =======================================================================
    let s_nome           = '';
    let s_nConfigurando  = 0;
    let s_modo           = '';
    let s_mecanismo      = '';
    let s_acertos        = 0;
    let s_erros          = 0;
    let s_duracao_ms     = 0;
    let s_timestampInicio = null;
    let s_timerFeedback  = null;

    let _estado = 'CONFIGURANDO';
    let s_ws    = null;
    let s_audioCtx = null;

    // =======================================================================
    // 3. Referencias DOM
    // =======================================================================
    const elConfigurando  = document.getElementById('tela-configurando');
    const elSessao        = document.getElementById('tela-sessao');
    const elOverlay       = document.getElementById('overlay-feedback');
    const elPausado       = document.getElementById('tela-pausado');
    const elResultados    = document.getElementById('tela-resultados');
    const elFormSessao    = document.getElementById('form-sessao');
    const elNome          = document.getElementById('campo-nome');
    const elN             = document.getElementById('campo-n');
    const elModo          = document.getElementById('campo-modo');
    const elJanelaWrapper = document.getElementById('campo-janela-wrapper');
    const elJanela        = document.getElementById('campo-janela');
    const elMecanismo     = document.getElementById('campo-mecanismo');
    const elErroNome      = document.getElementById('erro-nome');
    const elStatAcertos   = document.getElementById('stat-acertos');
    const elStatTotal     = document.getElementById('stat-total');
    const elResNome       = document.getElementById('res-nome');
    const elResAcertos    = document.getElementById('res-acertos');
    const elResTotal      = document.getElementById('res-total');
    const elResTaxa       = document.getElementById('res-taxa');
    const elResDuracao    = document.getElementById('res-duracao');

    // =======================================================================
    // 4. Som — WEB_STANDARD.md#padrao-funcoes-js (tocar<Evento>)
    // =======================================================================
    function _audioCtxReady() {
      if (!s_audioCtx) {
        s_audioCtx = new (window.AudioContext || window.webkitAudioContext)();
      }
      if (s_audioCtx.state === 'suspended') { s_audioCtx.resume(); }
      return s_audioCtx;
    }

    // DRY: unico ponto de criacao de tom sinusoidal com envelope ADSR simplificado
    function _tocarTom(freq, inicioS, duracaoS, attackS, releaseS, ganho) {
      const ctx = _audioCtxReady();
      const osc = ctx.createOscillator();
      const env = ctx.createGain();
      osc.connect(env);
      env.connect(ctx.destination);
      osc.frequency.value = freq;
      osc.type = 'sine';
      const t = ctx.currentTime;
      env.gain.setValueAtTime(0,     t + inicioS);
      env.gain.linearRampToValueAtTime(ganho,  t + inicioS + attackS);
      env.gain.setValueAtTime(ganho, t + inicioS + duracaoS - releaseS);
      env.gain.linearRampToValueAtTime(0,      t + inicioS + duracaoS);
      osc.start(t + inicioS);
      osc.stop(t + inicioS + duracaoS);
    }

    function tocarAcerto() {
      const durS = SOM_ACERTO_DURACAO_MS / 1000;
      SOM_ACERTO_FREQS.forEach(function(freq, idx) {
        _tocarTom(freq, (idx * SOM_ACERTO_OFFSET_MS) / 1000,
                  durS, SOM_ACERTO_ATTACK_S, SOM_ACERTO_RELEASE_S, SOM_ACERTO_GANHO);
      });
    }

    function tocarErro() {
      _tocarTom(SOM_ERRO_FREQ, 0, SOM_ERRO_DURACAO_MS / 1000,
                SOM_ERRO_ATTACK_S, SOM_ERRO_RELEASE_S, SOM_ERRO_GANHO);
    }

    // =======================================================================
    // 5. WebSocket — WEB_STANDARD.md#websocket
    // =======================================================================
    function conectarWS() {
      s_ws = new WebSocket(WS_URL);
      s_ws.onopen    = aoConectarWS;
      s_ws.onclose   = aoDesconectarWS;
      s_ws.onmessage = aoReceberMensagem;
    }

    // DRY: unico ponto de envio — sem chamadas diretas a s_ws.send()
    function enviarMensagem(obj) {
      if (s_ws && s_ws.readyState === WebSocket.OPEN) {
        s_ws.send(JSON.stringify(obj));
      }
    }

    function aoConectarWS() {
      // reconexao: ESP32 envia RETOMADO automaticamente se sessao estava ativa
    }

    function aoDesconectarWS() {
      if (_estado !== 'CONFIGURANDO' && _estado !== 'RESULTADOS') {
        setState('PAUSADO');
      }
      setTimeout(conectarWS, WS_RECONEXAO_MS);
    }

    function aoReceberMensagem(evt) {
      let msg;
      try { msg = JSON.parse(evt.data); } catch (_) { return; }
      switch (msg.tipo) {
        case 'ACERTO':     aoReceberAcerto(msg);    break;
        case 'ERRO':       aoReceberErro(msg);      break;
        case 'FIM_SESSAO': aoReceberFimSessao(msg); break;
        case 'PAUSADO':    setState('PAUSADO');      break;
        case 'RETOMADO':   setState('SESSAO_ATIVA'); break;
      }
    }

    // =======================================================================
    // 6. Maquina de estados — WEB_STANDARD.md#maquina-estados-js
    // =======================================================================
    function setState(novoEstado) {
      _estado = novoEstado;
      _renderizarEstado();
    }

    function _renderizarEstado() {
      const emSessao = (_estado === 'SESSAO_ATIVA' ||
                        _estado === 'FEEDBACK_ACERTO' ||
                        _estado === 'FEEDBACK_ERRO');
      elConfigurando.hidden = (_estado !== 'CONFIGURANDO');
      elSessao.hidden       = !emSessao;
      elPausado.hidden      = (_estado !== 'PAUSADO');
      elResultados.hidden   = (_estado !== 'RESULTADOS');

      if (_estado === 'FEEDBACK_ACERTO') {
        _mostrarOverlay(FEEDBACK_ACERTO_COR);
      } else if (_estado === 'FEEDBACK_ERRO') {
        _mostrarOverlay(FEEDBACK_ERRO_COR);
      } else {
        elOverlay.hidden = true;
      }
    }

    // DRY: unico ponto de exibicao do overlay de feedback
    function _mostrarOverlay(cor) {
      elOverlay.style.background = cor;
      elOverlay.hidden = false;
    }

    // =======================================================================
    // 7. Handlers de mensagens recebidas
    // =======================================================================
    function aoReceberAcerto(msg) {
      if (s_timerFeedback) { clearTimeout(s_timerFeedback); }
      elStatAcertos.textContent = String(msg.acertos);
      tocarAcerto();
      setState('FEEDBACK_ACERTO');
      s_timerFeedback = setTimeout(function() {
        s_timerFeedback = null;
        setState('SESSAO_ATIVA');
      }, FEEDBACK_ACERTO_MS);
    }

    function aoReceberErro(msg) {
      // Se havia timer de acerto em curso, cancela — nao pode acontecer mas e defensivo
      if (s_timerFeedback) { clearTimeout(s_timerFeedback); s_timerFeedback = null; }
      elStatAcertos.textContent = String(msg.acertos);
      s_erros += 1;
      tocarErro();
      setState('FEEDBACK_ERRO');
      // overlay permanece ate proximo evento — WEB_STANDARD.md#transicoes
    }

    function aoReceberFimSessao(msg) {
      if (s_timerFeedback) { clearTimeout(s_timerFeedback); s_timerFeedback = null; }
      s_acertos    = msg.acertos;
      s_duracao_ms = msg.duracao_ms;
      // DRY: calculos de taxa e duracao em um lugar so; usados em renderizar e salvar
      const taxa    = Number((s_acertos / s_nConfigurando * 100).toFixed(1));
      const duracaoS = Number((s_duracao_ms / 1000).toFixed(1));
      renderizarResultados(taxa, duracaoS);
      salvarSessao({
        id:               String(s_timestampInicio ? s_timestampInicio.getTime() : Date.now()),
        nome:             s_nome,
        timestamp_inicio: s_timestampInicio ? s_timestampInicio.toISOString() : new Date().toISOString(),
        modo:             s_modo,
        mecanismo:        s_mecanismo,
        n_configurado:    s_nConfigurando,
        acertos:          s_acertos,
        erros:            s_erros,
        taxa_pct:         taxa,
        duracao_s:        duracaoS
      });
      setState('RESULTADOS');
    }

    // =======================================================================
    // 8. Handlers de eventos DOM
    // =======================================================================
    elFormSessao.addEventListener('submit', function(evt) {
      evt.preventDefault();
      const nome = elNome.value.trim();
      if (!nome) { elErroNome.style.display = 'block'; return; }
      elErroNome.style.display = 'none';

      s_nome           = nome;
      s_nConfigurando  = Math.max(N_MIN, parseInt(elN.value, 10) || N_MIN);
      s_modo           = elModo.value;
      s_mecanismo      = elMecanismo.value;
      s_acertos        = 0;
      s_erros          = 0;
      s_duracao_ms     = 0;
      s_timestampInicio = new Date();

      elStatTotal.textContent   = String(s_nConfigurando);
      elStatAcertos.textContent = '0';

      enviarMensagem({
        tipo:      'INICIAR',
        nome:      s_nome,
        n:         s_nConfigurando,
        modo:      s_modo,
        mecanismo: s_mecanismo,
        janela_ms: (s_modo === 'DOIS_MARTELOS')
                     ? (parseInt(elJanela.value, 10) || JANELA_MS_PADRAO)
                     : JANELA_MS_PADRAO
      });
      setState('SESSAO_ATIVA');
    });

    elModo.addEventListener('change', function() {
      elJanelaWrapper.hidden = (elModo.value !== 'DOIS_MARTELOS');
    });

    document.getElementById('btn-pausar').addEventListener('click', function() {
      enviarMensagem({ tipo: 'PAUSAR' });
      setState('PAUSADO');
    });

    document.getElementById('btn-retomar').addEventListener('click', function() {
      enviarMensagem({ tipo: 'RETOMAR' });
      setState('SESSAO_ATIVA');
    });

    document.getElementById('btn-nova-sessao').addEventListener('click', function() {
      setState('CONFIGURANDO');
    });

    document.getElementById('btn-exportar').addEventListener('click', exportarCSV);

    // =======================================================================
    // 9. Armazenamento e exportacao — WEB_STANDARD.md#armazenamento-exportacao
    // =======================================================================
    function carregarSessoes() {
      try { return JSON.parse(localStorage.getItem(LS_CHAVE) || '[]'); }
      catch (_) { return []; }
    }

    function salvarSessao(registro) {
      const lista = carregarSessoes();
      lista.push(registro);
      localStorage.setItem(LS_CHAVE, JSON.stringify(lista));
    }

    function exportarCSV() {
      const lista = carregarSessoes();
      const linhas = [CSV_CABECALHO].concat(lista.map(function(r) {
        return [r.id, r.nome, r.timestamp_inicio, r.modo, r.mecanismo,
                r.n_configurado, r.acertos, r.erros, r.taxa_pct, r.duracao_s
               ].join(',');
      }));
      const blob = new Blob([linhas.join('\n')], { type: CSV_MIME });
      const url  = URL.createObjectURL(blob);
      const a    = document.createElement('a');
      a.href     = url;
      a.download = CSV_NOME_ARQUIVO;
      a.click();
      URL.revokeObjectURL(url);
    }

    // DRY: renderizacao de resultados recebe valores ja calculados — nao recalcula
    function renderizarResultados(taxa, duracaoS) {
      elResNome.textContent    = s_nome;
      elResAcertos.textContent = String(s_acertos);
      elResTotal.textContent   = String(s_nConfigurando);
      elResTaxa.textContent    = taxa + '%';
      elResDuracao.textContent = duracaoS + 's';
    }

    // =======================================================================
    // 10. Inicializacao
    // =======================================================================
    elJanelaWrapper.hidden = (elModo.value !== 'DOIS_MARTELOS');
    conectarWS();
  </script>
</body>
</html>
)rawhtml";

// ---------------------------------------------------------------------------
// Helpers JSON — DRY: serializa e envia sem duplicar serializeJson + textAll
// ---------------------------------------------------------------------------

static void _serializarEnviarTodos(JsonDocument& doc) {
    String payload;
    serializeJson(doc, payload);
    s_ws.textAll(payload);
}

static void _serializarEnviarCliente(AsyncWebSocketClient* client, JsonDocument& doc) {
    String payload;
    serializeJson(doc, payload);
    client->text(payload);
}

// DRY: mensagem de evento com 4 campos identicos (ACERTO, ERRO, FIM_SESSAO)
static void enviarEvento(const char* tipo,
                         uint8_t acertos, uint8_t total, uint32_t duracao_ms) {
    JsonDocument doc;
    doc["tipo"]       = tipo;
    doc["acertos"]    = acertos;
    doc["total"]      = total;
    doc["duracao_ms"] = duracao_ms;
    _serializarEnviarTodos(doc);
}

// DRY: mensagem com campo unico "tipo" (PAUSADO, RETOMADO)
static void enviarStatus(const char* tipo) {
    JsonDocument doc;
    doc["tipo"] = tipo;
    _serializarEnviarTodos(doc);
}

// DRY: PAUSAR e RETOMAR sao simetricos — mesmo padrao: guardar sessao → chamar game → enviar status
static void _aplicarToggle(bool pausar, AsyncWebSocketClient* cliente) {
    if (!s_sessao_ativa) { return; }
    if (pausar) {
        gamePausarSessao();
        enviarStatus(MSG_PAUSADO);
    } else {
        gameRetomarSessao();
        if (cliente) {
            // reconexao: envia so para o cliente recem-conectado
            JsonDocument doc;
            doc["tipo"] = MSG_RETOMADO;
            _serializarEnviarCliente(cliente, doc);
        } else {
            enviarStatus(MSG_RETOMADO);
        }
    }
}

// ---------------------------------------------------------------------------
// Callback de MOD_JOGO — [VER: 01_arquitetura.md#interface-jogo-wifi]
// ---------------------------------------------------------------------------

static void aoEventoJogo(EventoJogo ev) {
    switch (ev.resultado) {
        case ResultadoJogo::ESTIMULO:
            // ESTIMULO nao gera mensagem ao browser
            return;

        case ResultadoJogo::ACERTO:
            enviarEvento(MSG_ACERTO, ev.acertos, ev.n_configurado, ev.duracao_ms);
            break;

        case ResultadoJogo::ERRO:
            enviarEvento(MSG_ERRO, ev.acertos, ev.n_configurado, ev.duracao_ms);
            break;

        case ResultadoJogo::FIM_SESSAO:
            enviarEvento(MSG_FIM_SESSAO, ev.acertos, ev.n_configurado, ev.duracao_ms);
            s_sessao_ativa = false;
            break;
    }
}

// ---------------------------------------------------------------------------
// Handler de eventos WebSocket — [VER: 07_interface_pedagogo.md#websocket]
// ---------------------------------------------------------------------------

static void onWsEvent(AsyncWebSocket*       server,
                      AsyncWebSocketClient* client,
                      AwsEventType          type,
                      void*                 arg,
                      uint8_t*              data,
                      size_t                len) {
    if (type == WS_EVT_CONNECT) {
        _aplicarToggle(false, client); // retoma sessao se estava ativa
        return;
    }

    if (type == WS_EVT_DISCONNECT) {
        _aplicarToggle(true, nullptr); // pausa sessao se estava ativa
        return;
    }

    if (type != WS_EVT_DATA) { return; }

    AwsFrameInfo* info = static_cast<AwsFrameInfo*>(arg);
    if (!info->final || info->index != 0u || info->opcode != WS_TEXT) {
        return; // fragmentado ou binario — ignorar
    }

    JsonDocument doc;
    if (deserializeJson(doc, data, len)) { return; }

    const char* tipo = doc["tipo"] | "";

    if (strcmp(tipo, MSG_INICIAR) == 0) {
        const char* modo = doc["modo"] | "";
        const char* meca = doc["mecanismo"] | "";
        ConfigSessao cfg;
        cfg.modo = (strcmp(modo, INTERFACE_MODO_DOIS_MARTELOS) == 0)
                       ? GAME_MODO_DOIS : GAME_MODO_UM;
        cfg.mecanismo     = (strcmp(meca, INTERFACE_MECA_B) == 0) ? 'B' : 'A';
        cfg.n_configurado = static_cast<uint8_t>(doc["n"]        | 1);
        cfg.janela_ms     = static_cast<uint16_t>(doc["janela_ms"] | INTERFACE_JANELA_MS_PADRAO);
        s_sessao_ativa    = true;
        gameIniciarSessao(cfg);

    } else if (strcmp(tipo, MSG_PAUSAR) == 0) {
        _aplicarToggle(true, nullptr);

    } else if (strcmp(tipo, MSG_RETOMAR) == 0) {
        _aplicarToggle(false, nullptr);
    }
}

// ---------------------------------------------------------------------------
// API publica — [VER: CODING_STANDARD.md#estrutura-h]
// ---------------------------------------------------------------------------

void interfaceInit() {
    // WiFi AP — [VER: 07_interface_pedagogo.md#acesso-rede]
    WiFi.mode(WIFI_AP);
    IPAddress ip;
    ip.fromString(INTERFACE_AP_IP);
    WiFi.softAPConfig(ip, ip, INTERFACE_AP_MASCARA);
    WiFi.softAP(INTERFACE_AP_SSID, nullptr, INTERFACE_AP_CANAL);

    s_ws.onEvent(onWsEvent);
    s_server.addHandler(&s_ws);

    s_server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, INTERFACE_HTML_MIME, HTML_PAGE);
    });

    s_server.begin();

    gameOnEvento(aoEventoJogo);
}

void interfaceLoop() {
    // ESPAsyncWebServer e assíncrono — nao ha poll necessario.
    // cleanupClients libera slots de clientes desconectados.
    s_ws.cleanupClients();
}
