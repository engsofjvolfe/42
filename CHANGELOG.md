# CHANGELOG

Historico de releases do projeto. Atualizado a cada tag de versao.

Formato: baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.0.0/).
Versionamento: [SemVer](https://semver.org/lang/pt-BR/).

---

## [Nao lancado]

### Corrigido
- `modules/visual/03_saida_visual.md` v0.1.7 → v0.1.8: corrige RGB de Laranja (`#FF5000`→`#BF3C00`, 191,60,0) e Roxo (`#9400D3`→`#1E002A`, 30,0,42) após validação física apontar CA-03-02/CA-03-03 como não distinguíveis o suficiente (teste com adulto, bancada 2026-07-04). Ambas escurecidas por escala proporcional em direção ao preto, mesmo matiz preservado; Azul e Amarelo mantidos no valor original — validados como suficientes após o ajuste dos outros dois. Proposta intermediária (paleta de tinta medida dos tacos pintados, sRGB direto) e tentativa de correção gama sRGB→linear avaliadas e descartadas nesta sessão — ambas ainda liam como branco ou claras demais quando emitidas por LED; valores finais testados direto no firmware (`visual_config.h`) antes desta cascata, por decisão do usuário
- **D2** — primeira interação após energizar acendia sempre a cor roxa e travava na primeira batida (ambos os modos), destravando apenas com PAUSAR/RETOMAR. Duas causas raiz, ambas correções de código sem lacuna de spec (`04_logica_jogo.md` e `01_arquitetura.md` já especificavam o comportamento correto):
  - `firmware/src/game/game.cpp` — `random_uint32()` usava `rand()` (libc, seed fixa) em vez de `random()` (Arduino/ESP32, `esp_random()` — RNG de hardware, não determinístico desde a 1ª chamada), divergindo do pseudocódigo já aprovado em `04_logica_jogo.md#mecanismo-a`/`#mecanismo-b` (`random(i+1)`, `random(total)`). Corrigido para chamar `random()` diretamente; `firmware/test/test_game/test_main.cpp` ganha a definição de `random()` (já declarada em `test/mock/Arduino.h`, padrão declare→define) delegando a `rand()` em native — determinismo aceitável pois os testes verificam invariantes do algoritmo, não valores sorteados específicos
  - `firmware/src/main.cpp` — `interfaceInit()` (AP + servidor HTTP/WS) subia imediatamente em `setup()`, em paralelo com a animação de boot de 3s, permitindo que uma sessão fosse iniciada e seu primeiro comando de LED fosse sobrescrito pelo passo final `todos_OFF` da animação de boot — violava a ordem já especificada em `01_arquitetura.md#diagrama-estados` (`BOOT --> AGUARDANDO_CONEXAO` só com "varredura LEDs concluída") e `04_logica_jogo.md#estados-jogo` (BOOT/AGUARDANDO_CONEXAO/CONFIGURANDO são responsabilidade do sistema de inicialização). Corrigido: `interfaceInit()` só é chamado em `loop()` após `millis() >= VISUAL_BOOT_DURACAO_MS`
  - Validação: `pio test -e native` 38/38 PASSED; `pio run -e esp32dev` SUCCESS sem warnings (RAM 13.6%, Flash 66.7%). Validação física em bancada (usuário, log serial timestampado via `firmware/diag/monitor_serial.py`): sessão completa, 3 interações seguidas, todas ACERTO, sem auto-pausa — D2 baixado (TODO.md)

### Adicionado
- `firmware/src/game/game.h`, `firmware/src/game/game.cpp` — `gameEncerrarSessao()` (melhoria M1, TODO.md): encerra a sessão ativa antes do N configurado, preservando os acertos parciais, reaproveitando o evento `FIM_SESSAO` já existente; derivado de `01_arquitetura.md#interface-jogo-wifi` e `04_logica_jogo.md#encerramento-antecipado` (CA-04-11); 4 testes novos em `test/test_game/test_main.cpp`
- `firmware/src/interface/interface.cpp` — mensagem WebSocket `ENCERRAR` (browser→ESP32) e botão "Encerrar Sessão" na tela de sessão ativa, com confirmação obrigatória antes de enviar; derivado de `07_interface_pedagogo.md#tela-sessao-ativa` (CA-07-14); nenhum estado novo na máquina de estados do browser — reaproveita a transição `SESSAO_ATIVA → RESULTADOS` já existente ao receber `FIM_SESSAO`
- `VALIDATION.md` — checklist formal de validação da ETAPA 8: os 65 CAs (+ 8 cenários de UI do WEB_STANDARD) com Resultado (PASSOU/FALHOU/PENDENTE/OBSOLETO) e Evidência; status inicial derivado exclusivamente de evidência registrada — CA-07-09/12/13 PASSOU (bancada 2026-07-03), CA-04-01..08 PASSOU (native 38/38, testes de natureza lógica), CA-05-06 OBSOLETO, demais PENDENTE; o gate v1.0.0 exige todas as linhas resolvidas
- `manual/12_manual_pedagogo.md` v0.1.0 — manual de uso do instrumento em linguagem não-técnica (melhoria M4 da validação ETAPA 8), derivado de `00_conceito.md` v0.2.1 e `07_interface_pedagogo.md` v0.3.2: preparação (ligar, conectar à rede `BMI`, abrir `192.168.4.1`), configuração de sessão, os dois modos, os dois mecanismos de sorteio explicados com exemplos, condução da sessão, leitura de resultados, exportação com prévia (CSV/PDF), privacidade e problemas comuns; 11 marcadores de fotos a produzir com lista de referência para a sessão de fotos
- `firmware/src/interface/interface.cpp` — pré-visualização de exportação com confirmação (M2) e exportação PDF (M3), derivadas de `spec/interface/interface.json#exportacao_ui` e `#exportacao_pdf`: overlay `#overlay-exportacao` com tabela das 10 colunas via `textContent`, seletor CSV/PDF gerado de `EXPORT_FORMATOS`, botões Baixar/Cancelar (Baixar desabilitado com localStorage vazio); gerador de PDF 1.4 em JS puro (`_pdfGerar` — fontes base-14, WinAnsiEncoding, A4 paisagem, 35 linhas/página, xref byte-exata) baixado por `data:application/pdf;base64,`; `baixarArquivo()` como ponto único de criação da âncora de download; `exportarCSV()` passa a receber a lista e só é chamada pela confirmação da prévia; pré-validado em Node com o JS real embutido (44/44 checks — CA-07-09/12/13)
- `firmware/diag/` — kit de diagnóstico de bancada (projeto PlatformIO separado, fora do V-model e dos builds normais): `led_test.cpp` (onda quadrada medível com multímetro + ciclo de cores FastLED), `wifi_test.cpp` (init do AP com TX reduzido + breadcrumbs — diagnóstico de brownout) e `monitor_serial.py` (timestamps por linha + reset via RTS); origem: investigação CA-07-01, diagnóstico dos LEDs e condenação do AMS1117 (2026-07-02)
- `TODO.md` — lista de tarefas futuras: metadocumentos e índices de navegação por diretório (`modules/`, `spec/`, `hardware/`, `_governance/`)

### Alterado
- `spec/visual/visual.json`, `spec/firmware_constants.json`: re-derivados de `03_saida_visual.md` v0.1.8 — `cores_rgb.Laranja`/`cores_rgb.Roxo` e `criterios_aceitacao` CA-03-02 atualizados; `meta.versao_fonte` 0.1.0 → 0.1.8. `firmware/src/visual/visual_config.h` e `_governance/CODING_STANDARD.md` (tabela gerada `tabela_visual`, v0.2.4→v0.2.5) regenerados via `generate_coding_standard.py` — validado sem divergências de cascata
- Cascata mecânica de `03_saida_visual.md` v0.1.8 (sem mudança de conteúdo — CA-03-02/03 não afeta lista de materiais, conexões, cablagem ou montagem física): `08_bom.md` v0.3.3→v0.3.4, `09_conexoes.md` v0.3.3→v0.3.4, `10_cablagem.md` v0.2.3→v0.2.4, `11_montagem.md` v0.3.3→v0.3.4; cascata de `CODING_STANDARD.md` v0.2.5: `TESTING_STANDARD.md` v0.1.6→v0.1.7
- `VALIDATION.md` — CA-03-02 e CA-03-03 promovidos de PENDENTE para PASSOU (paleta de Laranja/Roxo corrigida e validada em bancada, 2026-07-04); estado 56 PASSOU · 1 OBSOLETO · 11 PENDENTE (68 CAs). Ressalva registrada: reconfirmar com criança do público-alvo antes do fechamento do gate v1.0.0 (teste atual foi feito com adulto)
- `concept/00_conceito.md` v0.2.1 → v0.3.0: nova seção §11.4 "Encerramento antecipado de sessão" (melhoria M1) — pedagogo pode encerrar a sessão ativa antes do N configurado e iniciar nova sessão sem recarregar a página nem reiniciar o ESP32
- `system/01_arquitetura.md` v0.3.1 → v0.4.0: cascata do conceito v0.3.0 — nova função `encerrarSessao()` na interface MOD_JOGO↔MOD_WIFI; nova transição `SESSAO_ATIVA → FIM_SESSAO` no diagrama de estados; CA-01-09 novo
- `modules/game/04_logica_jogo.md` v0.1.4 → v0.2.0: nova transição `SESSAO_ATIVA (qualquer) → FIM_SESSAO` via `encerrarSessao()` (§3.2, nova); CA-04-11 novo
- `modules/interface/07_interface_pedagogo.md` v0.3.2 → v0.4.0: nova mensagem `ENCERRAR` no protocolo (§4.1); botão "Encerrar Sessão" na tela de sessão ativa (§5.2); nota em `#armazenamento-dados` sobre registros de sessão encerrada antecipadamente (`acertos < n_configurado` identifica o registro, sem campo novo); CA-07-14 novo
- `spec/interface/interface.json` + `spec/interface/interface.schema.json` — re-derivados de `07_interface_pedagogo.md` v0.4.0: nova mensagem `ENCERRAR` em `protocolo_mensagens.browser_para_esp32` (3→4 itens); novo CA-07-14 em `criterios_aceitacao` (13→14 itens); `meta.versao_fonte` 0.3.0 → 0.4.0; validado com jsonschema
- `manual/12_manual_pedagogo.md` v0.1.1 → v0.2.0: descreve o novo botão "Encerrar Sessão" (§11 Durante a sessão; §13 Fim da sessão)
- `_governance/WEB_STANDARD.md` v0.3.2 → v0.4.0: novo cenário CA-07-14 (§11.5 Cenários obrigatórios), derivado da mensagem `ENCERRAR` e do botão "Encerrar Sessão"; sem mudança em §5/§6 — a máquina de estados JS já cobria a transição `SESSAO_ATIVA → RESULTADOS` via `FIM_SESSAO`, reaproveitada pelo encerramento antecipado sem nenhum estado novo
- Cascata mecânica do conceito v0.3.0 e de `01_arquitetura.md` v0.4.0 (bump PATCH, `depende_de`/Identificação/Rastreabilidade, sem mudança de conteúdo — melhoria M1 não afeta hardware/alimentação/sensor/visual/montagem/padrões de teste): `05_alimentacao.md` v0.3.2→v0.3.3, `02_sensor_impacto.md` v0.2.2→v0.2.3, `03_saida_visual.md` v0.1.6→v0.1.7, `06_privacidade_lgpd.md` v0.2.1→v0.2.2, `08_bom.md` v0.3.2→v0.3.3, `09_conexoes.md` v0.3.2→v0.3.3, `10_cablagem.md` v0.2.2→v0.2.3, `11_montagem.md` v0.3.2→v0.3.3, `CODING_STANDARD.md` v0.2.3→v0.2.4, `TESTING_STANDARD.md` v0.1.5→v0.1.6
- `TODO.md` — melhoria M1 (botão de encerrar sessão e iniciar nova sessão) concluída
- `VALIDATION.md` — CA-01-09, CA-04-11 e CA-07-14 novos (melhoria M1), todos PASSOU; estado 54 PASSOU · 1 OBSOLETO · 13 PENDENTE (68 CAs)
- `TODO.md`, `VALIDATION.md` — D3 (Modo 2: pares de cor sempre os mesmos dois) baixado: investigação interrompida por um brownout não relacionado (conflito USB + fonte externa ligados simultaneamente — ver `firmware/diag/README.md`); após corrigir a energia (só fonte externa), re-teste de bancada (observação visual, sem log serial) mostrou os pares variando normalmente. Fechado sem causa raiz confirmada por instrumentação — ressalva registrada em TODO.md para reabertura se reaparecer (checar primeiro se USB e fonte estão ligados juntos)
- `firmware/diag/README.md` — nova entrada nas "Lições de hardware": USB + fonte externa ligados ao mesmo tempo também causa brownout em loop (confirmado 2026-07-04), distinto do brownout USB-only já documentado; recomenda usar somente a fonte externa em bancada de WiFi, desconectando o USB após gravar
- `TODO.md` — D2 baixado (validado na bancada); registra novo defeito conhecido D3 (Modo 2: pares de cor sempre os mesmos dois — Roxo+Amarelo e Laranja+Azul — mesmo no Mecanismo A; hipótese não confirmada de shuffle sem efeito). D3 bloqueia o gate v1.0.0
- `VALIDATION.md` — notas de D2 em CA-01-05/06 atualizadas (workaround não é mais necessário); nota de D3 adicionada em CA-01-06; estado de defeitos no cabeçalho atualizado (D2 baixado, D3 aberto)
- `firmware/diag/README.md` — reforça a nota já existente sobre lixo pré-bufferizado do driver CP210x no `monitor_serial.py`, confirmando reprodução durante a investigação do D2 (2026-07-04); não é defeito de firmware/produto, sem ação necessária
- `VALIDATION.md` — rodada de validação física de bancada (2026-07-04): 41 CAs promovidos a PASSOU (+2 cenários UI), estado 52 PASSOU · 1 OBSOLETO · 12 PENDENTE; ressalvas de protocolo anotadas sem alterar vereditos (CA-01-03 sem osciloscópio; CA-10-02 fio único vermelho); notas de D2 em CA-01-05/06; correção da contagem inicial (11 PASSOU, não 7)
- `TODO.md` — registra defeito conhecido D2 (primeira interação após energizar acende sempre roxo e trava na primeira batida; workaround PAUSAR/RETOMAR; bloqueia o gate v1.0.0) e corrige formatação da linha D1
- `README.md` — badge do GitHub Actions removido (apontava para owner errado `eflov/42`; remote real é `engsofjvolfe/42` — imagem quebrada) e substituído por badges estáticos informativos: licença GPL-3.0, plataforma ESP32, framework PlatformIO + Arduino, testes nativos 38/38 e status de validação (link para `VALIDATION.md`)
- `README.md` — reescrito com foco em leitor não-técnico (melhoria M5 da validação ETAPA 8): descrição humana do brinquedo, seção de fotos a inserir, "Como funciona" em prosa, seção "Para usar" apontando para o manual (`manual/12_manual_pedagogo.md`) e o documento de privacidade; estado do projeto por marcos com link para `VALIDATION.md`; conteúdo técnico consolidado em "Para desenvolvedores" (hardware, comandos, índice de documentos incluindo manual, spec/ e VALIDATION.md)
- `concept/00_conceito.md` v0.2.0 → v0.2.1 e `modules/interface/07_interface_pedagogo.md` v0.3.1 → v0.3.2: registram `12_manual_pedagogo.md` no `impacta` como dependente OBRIGATÓRIO (o manual descreve o comportamento visível ao usuário definido nesses documentos) e adicionam a linha Filho correspondente na Rastreabilidade
- Cascata mecânica do registro do manual (bump PATCH, `depende_de`/Identificação/Rastreabilidade, sem mudança de conteúdo): `01_arquitetura.md` v0.3.0→v0.3.1, `02_sensor_impacto.md` v0.2.1→v0.2.2, `03_saida_visual.md` v0.1.5→v0.1.6, `04_logica_jogo.md` v0.1.3→v0.1.4, `05_alimentacao.md` v0.3.1→v0.3.2, `06_privacidade_lgpd.md` v0.2.0→v0.2.1, `08_bom.md` v0.3.1→v0.3.2, `09_conexoes.md` v0.3.1→v0.3.2, `10_cablagem.md` v0.2.1→v0.2.2, `11_montagem.md` v0.3.1→v0.3.2, `CODING_STANDARD.md` v0.2.2→v0.2.3, `TESTING_STANDARD.md` v0.1.4→v0.1.5, `WEB_STANDARD.md` v0.3.1→v0.3.2
- Cascata mecânica do conceito v0.2.0 (bump PATCH, `depende_de`/Identificação/Rastreabilidade, sem mudança de conteúdo): `02_sensor_impacto.md` v0.2.0→v0.2.1, `03_saida_visual.md` v0.1.4→v0.1.5, `04_logica_jogo.md` v0.1.2→v0.1.3, `05_alimentacao.md` v0.3.0→v0.3.1, `07_interface_pedagogo.md` v0.3.0→v0.3.1, `08_bom.md` v0.3.0→v0.3.1, `09_conexoes.md` v0.3.0→v0.3.1, `10_cablagem.md` v0.2.0→v0.2.1, `11_montagem.md` v0.3.0→v0.3.1, `CODING_STANDARD.md` v0.2.1→v0.2.2, `TESTING_STANDARD.md` v0.1.3→v0.1.4, `WEB_STANDARD.md` v0.3.0→v0.3.1; em CODING e TESTING corrige também versões pré-existentes desatualizadas nas tabelas de Identificação e Rastreabilidade
- `compliance/06_privacidade_lgpd.md` v0.1.0 → v0.2.0: cascata do conceito v0.2.0 — direito de portabilidade passa a citar os dois formatos de exportação (planilha CSV e relatório PDF) e a conferência dos dados na tela antes do download; nenhuma mudança na coleta, finalidade ou guarda dos dados; atualiza `depende_de` 00 v0.2.0
- `system/01_arquitetura.md` v0.2.1 → v0.3.0: cascata do conceito v0.2.0 — CA-01-08 estendido (exportação com pré-visualização e dois formatos, CSV e PDF); atualiza `depende_de` 00 v0.2.0; corrige versão desatualizada na tabela de Identificação (0.2.0)
- `concept/00_conceito.md` v0.1.0 → v0.2.0: registra no conceito as decisões validadas na ETAPA 8 (M2/M3) — §12.2 exportação em dois formatos (CSV para planilha, PDF como relatório legível), escolha do formato no momento da exportação e pré-visualização com confirmação obrigatória; §15 escopo v1.0 atualizado; corrige lacuna de rastreabilidade: `07_interface_pedagogo.md` adicionado ao `impacta`
- `spec/interface/interface.json` + `spec/interface/interface.schema.json` — re-derivados de `07_interface_pedagogo.md` v0.3.0: novos objetos `exportacao_ui` (formatos CSV/PDF, padrão CSV, pré-visualização e confirmação obrigatórias, Baixar desabilitado com vazio) e `exportacao_pdf` (nome de arquivo, MIME, versão 1.4, título, WinAnsiEncoding, formato de data, 3 fontes, página A4 paisagem, espaçamentos, 35 linhas/página, 10 colunas com larguras); `criterios_aceitacao` 11 → 13 (CA-07-12, CA-07-13; CA-07-09 com `via_pre_visualizacao`); `meta.versao_fonte` 0.2.0 → 0.3.0; schema exige os novos objetos com `const`
- `_governance/WEB_STANDARD.md` v0.2.0 → v0.3.0: cascata do 07 v0.3.0 (M2/M3) — `#overlay-exportacao` no esqueleto HTML e no mapeamento estado→elemento; constantes obrigatórias `EXPORT_*` e `PDF_*` derivadas de `exportacao_ui`/`exportacao_pdf`; nomenclatura `exportarPDF`/`baixarArquivo`/`abrirPrevia`/`fecharPrevia`/`renderizarPrevia` + internas de geração PDF; §10.3 regras da pré-visualização (PRE-01..05, `textContent` obrigatório para dado de sessão); §10.4 regras do gerador PDF (WinAnsi, escaping, xref byte-exata, btoa, data determinística sem `toLocaleString`); cenário CA-07-09 ajustado; cenários CA-07-12 e CA-07-13 novos
- `modules/interface/07_interface_pedagogo.md` v0.2.0 → v0.3.0: melhorias M2/M3 da validação ETAPA 8 — pré-visualização com confirmação obrigatória antes de qualquer download (requisitos PRE-01..05, §8.3); escolha de formato CSV/PDF no momento da exportação; exportação PDF gerada em JS puro no browser (PDF 1.4, fontes base-14, WinAnsiEncoding, A4 paisagem com paginação — requisitos PDF-01..06 e DECISAO formal, §8.4); botão da tela de resultados passa de "Exportar CSV" para "Exportar" (abre a prévia); CA-07-09 ajustado ao fluxo com prévia; CA-07-12 e CA-07-13 novos
- `spec/interface/interface.json` + `spec/interface/interface.schema.json` — re-derivados de `07_interface_pedagogo.md` v0.2.0: novos campos em `exportacao_csv` (`charset: "utf-8"`, `bom: "\uFEFF"`, `escaping_rfc4180: true`); CA-07-09 com campos de verificação (`campo_com_virgula_em_coluna_unica`, `acentos_corretos_em_planilha`); `meta.versao_fonte` 0.1.0 → 0.2.0; schema exige os novos campos com `const`
- `_governance/WEB_STANDARD.md` v0.1.2 → v0.2.0: cascata do 07 v0.2.0 — novas constantes obrigatórias `CSV_MIME`, `CSV_CHARSET`, `CSV_DATA_URI_PREFIX`, `CSV_BOM` (todas derivadas de `exportacao_csv`); `csvEscapar()` na nomenclatura; §10.2 com regras de escaping/BOM/data URI e proibição de blob+revoke na exportação; cenário CA-07-09 estendido; corrige versões desatualizadas em Identificação e Rastreabilidade (01 v0.2.0→v0.2.1, 07 v0.1.1→v0.2.0)
- `modules/interface/07_interface_pedagogo.md` v0.1.2 → v0.2.0: re-especifica a exportação CSV a partir do defeito D1 (validação ETAPA 8) — mecanismo `data:` URI com âncora anexada ao DOM substitui blob + `revokeObjectURL` síncrono (falha silenciosa em Firefox/WebView); novos requisitos CSV-01..04 (UTF-8 BOM, escaping RFC 4180, charset explícito, mecanismo de download); CA-07-09 estendido; corrige versão desatualizada na tabela de Identificação
- `modules/power/05_alimentacao.md` v0.2.2 → v0.3.0: arquitetura de alimentação revisada pela Fase 6 (validação com hardware) — LM2596 ajustado a 3.30V ± 0.05V alimenta o rail 3.3V DIRETO; AMS1117 fora do caminho de potência (unidade degradada, comprovado 2026-07-02); USB somente gravação; bulk 1000µF no pino 3V3; CA-05-06 OBSOLETO, CA-05-08 novo (10 boots com rádio sem brownout); bump MAJOR adiado por decisão de projeto
- `modules/sensor/02_sensor_impacto.md` v0.1.3 → v0.2.0: calibração do THRESHOLD obrigatória com topologia final energizada (rail sem PSRR do AMS1117); atualiza depende_de 05 v0.3.0
- `modules/visual/03_saida_visual.md` v0.1.3 → v0.1.4: corrige origem do rail 3.3V (saída direta do LM2596); VDD e nível de dado inalterados; atualiza depende_de 05 v0.3.0
- `hardware/08_bom.md` v0.2.2 → v0.3.0: E01 nota AMS1117 fora do caminho de potência; E02 alerta de serigrafia não confiável (falso-GND = CMD/GPIO11, causa raiz CA-07-01); P05 reduzido a 3; P06 pino 3V3; P07 novo (1000µF/≥10V pino 3V3); atualiza depende_de 02/03/05
- `hardware/09_conexoes.md` v0.2.3 → v0.3.0: cadeia 12V→LM2596(3.30V)→pino 3V3; seção 4.1 nova — verificação obrigatória de serigrafia do shield; CA-09-01/02 revisados; CA-09-07 novo; atualiza depende_de 02/03/05/08
- `hardware/10_cablagem.md` v0.1.4 → v0.2.0: F03–F06 barramento 3.3V com destino Shield pino 3V3; CA-10-06 novo — junções da cadeia LED soldadas/travadas, clipe de pressão proibido; atualiza depende_de 09 v0.3.0
- `hardware/11_montagem.md` v0.2.3 → v0.3.0: ajuste do LM2596 para 3.30V com trava de trimpot e proibição >3.6V; verificação de serigrafia como passo 1 da instalação; P07 no pino 3V3; checklist final com CA-05-08/CA-09-07/CA-10-06; atualiza depende_de 08/09/10
- `spec/power/power.json` + `spec/power/power.schema.json` — re-derivados de `05_alimentacao.md` v0.3.0: tensão alvo 3.3V ± 0.05V com máximo de conexão 3.6V, rails somente r3v3, AMS1117 `em_caminho_potencia: false`, decoupling `pino_3v3_devkit` 1000µF, orçamento/margens recalculados, CA-05-06 obsoleto e CA-05-08 novos campos
- `TODO.md` — registra defeito conhecido D1 (exportação CSV não funciona — bloqueia gate v1.0.0) e melhorias M1–M5 da interface do pedagogo (botão encerrar/nova sessão, visualização do CSV, formato legível de exportação, manual do jogo, atualização do README)
- `README.md` — reescrito: descrição com hardware em prosa derivada de `00_conceito.md` v0.1.0 e `01_arquitetura.md` v0.1.0; status V-model atualizado para v0.3.0 (38/38 testes); removidas seções duplicadas (hardware, módulos, protocolo) que vivem nos documentos fonte; índice de documentação inclui `_governance/`
- `modules/power/05_alimentacao.md` v0.1.0 → v0.2.0: adiciona nota em `#cadeia-alimentacao` declarando que AMS1117-3.3 é o regulador linear integrado ao ESP32 DevKit (E01), não componente externo; cascata obrigatória em 6 documentos dependentes
- `modules/sensor/02_sensor_impacto.md` v0.1.0 → v0.1.1: atualiza `depende_de` — `05_alimentacao.md` v0.1.0 → v0.2.0
- `modules/visual/03_saida_visual.md` v0.1.0 → v0.1.1: atualiza `depende_de` — `05_alimentacao.md` v0.1.0 → v0.2.0
- `hardware/08_bom.md` v0.1.0 → v0.2.0: adiciona nota em E01 (AMS1117-3.3 onboard); atualiza `depende_de` (02 v0.1.1, 03 v0.1.1, 05 v0.2.0)
- `hardware/09_conexoes.md` v0.1.0 → v0.2.0: adiciona nota em `#visao-geral` declarando AMS1117-3.3 como regulador onboard do DevKit; atualiza `depende_de` (02 v0.1.1, 03 v0.1.1, 05 v0.2.0, 08 v0.2.0)
- `hardware/10_cablagem.md` v0.1.0 → v0.1.1: atualiza `depende_de` — `09_conexoes.md` v0.1.0 → v0.2.0
- `hardware/11_montagem.md` v0.1.0 → v0.2.0: adiciona nota em `#instalacao-devkit` declarando ausência de etapa de instalação do AMS1117; atualiza `depende_de` (08 v0.2.0, 09 v0.2.0, 10 v0.1.1)

### Corrigido
- `manual/12_manual_pedagogo.md` v0.1.0 → v0.1.1 e `README.md` — descrição dos dados de sessão estava incompleta (citava apenas acertos/erros/taxa/duração): passa a enumerar os 10 campos do registro guardado e exportado (identificador, nome, data e hora de início, modo, mecanismo, interações configuradas, acertos, erros, taxa, duração); o manual distingue o resumo da tela de resultados do registro completo e corrige a definição da taxa
- `firmware/src/interface/interface.cpp` — corrige exportação CSV (defeito D1, CA-07-09): `exportarCSV()` passa a usar `data:` URI com âncora anexada ao DOM em vez de blob + `revokeObjectURL` síncrono em âncora fora do DOM (download nunca iniciava em Firefox/WebView Android); adiciona `csvEscapar()` (RFC 4180), prefixo BOM UTF-8 e charset explícito; constantes `CSV_MIME` reclassificada como DERIVADO e novas `CSV_CHARSET`, `CSV_BOM`, `CSV_DATA_URI_PREFIX` derivadas de `interface.json#exportacao_csv`
- `firmware/platformio.ini` — corrige `upload_port` de COM8 para COM9 em `[env:esp32dev]`; a placa (CP210x) enumera atualmente em COM9 e COM8 deixou de existir nesta máquina, impedindo upload e monitor
- `firmware/platformio.ini` — adiciona `upload_port = COM8` e `upload_speed = 115200` em `[env:esp32dev]`; velocidade padrão 921600 causava falha de transferência (`Packet content transfer stopped`)
- `hardware/09_conexoes.md` v0.2.0 → v0.2.1: corrige sintaxe Mermaid em `#visao-geral` — labels de aresta com parênteses envolvidos em aspas (`|"Piezo(+)"|`); sem aspas o `(` era interpretado como início de nó stadium
- `hardware/10_cablagem.md` v0.1.1 → v0.1.2: atualiza `depende_de` — `09_conexoes.md` v0.2.0 → v0.2.1 (cascata PATCH)
- `hardware/11_montagem.md` v0.2.0 → v0.2.1: atualiza `depende_de` — `10_cablagem.md` v0.1.1 → v0.1.2 (cascata PATCH)
- `system/01_arquitetura.md` v0.1.0 → v0.2.0: bump MINOR retroativo — entrada de 2026-06-28 (ArduinoJson em `#stack-tecnologico`) estava registrada como v0.1.0 (adição deveria ter bumped MINOR para v0.2.0)
- `_governance/CODING_STANDARD.md` v0.1.1 → v0.2.0: bump MINOR retroativo — entrada de 2026-06-28 (ArduinoJson em `#8.1`) estava duplicada em v0.1.1 (adição deveria ter bumped MINOR para v0.2.0); atualiza `depende_de` 01_arquitetura v0.1.0→v0.2.0
- `modules/sensor/02_sensor_impacto.md` v0.1.1 → v0.1.2: cascata — atualiza `depende_de` 01_arquitetura v0.1.0→v0.2.0, 05_alimentacao v0.2.0→v0.2.1
- `modules/visual/03_saida_visual.md` v0.1.1 → v0.1.2: cascata — atualiza `depende_de` 01_arquitetura v0.1.0→v0.2.0, 05_alimentacao v0.2.0→v0.2.1
- `modules/game/04_logica_jogo.md` v0.1.0 → v0.1.1: cascata — atualiza `depende_de` 01_arquitetura v0.1.0→v0.2.0
- `modules/power/05_alimentacao.md` v0.2.0 → v0.2.1: cascata — atualiza `depende_de` 01_arquitetura v0.1.0→v0.2.0
- `modules/interface/07_interface_pedagogo.md` v0.1.0 → v0.1.1: cascata — atualiza `depende_de` 01_arquitetura v0.1.0→v0.2.0; rastreabilidade 04_logica_jogo v0.1.0→v0.1.1
- `hardware/08_bom.md` v0.2.0 → v0.2.1: cascata — atualiza `depende_de` 01 v0.1.0→v0.2.0, 02 v0.1.1→v0.1.2, 03 v0.1.1→v0.1.2, 05 v0.2.0→v0.2.1
- `_governance/WEB_STANDARD.md` v0.1.0 → v0.1.1: cascata — atualiza `depende_de` 01_arquitetura v0.1.0→v0.2.0, 07_interface_pedagogo v0.1.0→v0.1.1
- `_governance/TESTING_STANDARD.md` v0.1.1 → v0.1.2: cascata — atualiza `depende_de` CODING_STANDARD v0.1.1→v0.2.0, 01_arquitetura v0.1.0→v0.2.0
- `hardware/09_conexoes.md` v0.2.1 → v0.2.2: cascata — atualiza `depende_de` 01 v0.1.0→v0.2.0, 02 v0.1.1→v0.1.2, 03 v0.1.1→v0.1.2, 05 v0.2.0→v0.2.1, 08 v0.2.0→v0.2.1
- `hardware/10_cablagem.md` v0.1.2 → v0.1.3: cascata — atualiza `depende_de` 09_conexoes v0.2.1→v0.2.2
- `hardware/11_montagem.md` v0.2.1 → v0.2.2: cascata — atualiza `depende_de` 08 v0.2.0→v0.2.1, 09 v0.2.1→v0.2.2, 10 v0.1.2→v0.1.3

---

## [0.3.0] — 2026-06-28

### Corrigido
- `spec/firmware_constants.json` DM-02: `adc_atten_t`/`ADC_ATTEN_DB_11` (ESP-IDF direto) corrigido para `adc_attenuation_t`/`ADC_11db` (camada Arduino ESP32 3.x / IDF5)
- `_governance/CODING_STANDARD.md` secao 7: tabela DM-02 regenerada automaticamente — reflete tipo correto `adc_attenuation_t`
- `firmware/src/sensor/sensor_config.h`: stub [PLATAFORMA] de DM-02 regenerado — reflete tipo correto `adc_attenuation_t`/`ADC_11db`
- `firmware/src/sensor/sensor.cpp`: `SENSOR_ADC_ATENUACAO` corrigida de `adc_atten_t`/`ADC_ATTEN_DB_11` para `adc_attenuation_t`/`ADC_11db`
- `firmware/test/mock/Arduino.h`: typedef e define de ADC corrigidos de `adc_atten_t`/`ADC_ATTEN_DB_11` para `adc_attenuation_t`/`ADC_11db`; assinatura de `analogSetAttenuation` atualizada
- `firmware/test/test_game/test_main.cpp`: definicao de `analogSetAttenuation` atualizada para `adc_attenuation_t`

### Adicionado
- `_governance/WEB_STANDARD.md` v0.1.0 — governança de HTML/CSS/JS embutido: MD3 offline via CSS custom properties, zero magic numbers JS/CSS, maquina de estados, nomenclatura de funcoes, cenarios de teste CA-07-* derivados de `spec/interface/interface.json`
- `firmware/src/interface/interface.h` — interface publica de MOD_WIFI: `interfaceInit()` e `interfaceLoop()` derivados de `01_arquitetura.md#interface-jogo-wifi`
- `firmware/src/interface/interface.cpp` — implementacao de MOD_WIFI: WiFi AP (SSID BMI, IP 192.168.4.1, canal 1), ESPAsyncWebServer, WebSocket bidirecional `/ws`, parsing JSON com ArduinoJson 7.x, callback `gameOnEvento`; HTML/CSS/JS embutido conforme `WEB_STANDARD.md`; cobre CA-07-01..11
- `firmware/src/game/game.h` — adiciona declaracoes publicas de `gamePausarSessao()` e `gameRetomarSessao()` derivadas de `01_arquitetura.md#interface-jogo-wifi` (implementacao ja presente em `game.cpp` via feat(game) anterior)
- `firmware/src/game/game.cpp`: integra chamadas a `visualSetLED()` e `visualRunCelebracao()` derivadas de `01_arquitetura.md#interface-jogo-led` e `04_logica_jogo.md#logica-modo-1/2#tratamento-resultados`; resolve `[DECIDIR]` registrado em `main.cpp`; `gameRetomarSessao()` reacende LEDs (CA-04-09)
- `firmware/src/game/game.h` — interface publica de MOD_JOGO: `Cor`, `ParCores`, `ResultadoJogo`, `EventoJogo`, `ConfigSessao`, `GameCallback`; API publica (`gameInit`, `gameOnEvento`, `gameIniciarSessao`, `gameOnImpacto`, `gameLoop`); API de teste (`gameZonaParaCor`, `gameProximaCorA`, `gameProximaCorB`, `gameProximasDuasA`, `gameProximasDuasB`, `gameGetCorAtual`, `gameGetParAtual`)
- `firmware/src/game/game.cpp` — implementacao de MOD_JOGO derivada de `spec/game/game.json` e `04_logica_jogo.md`; maquina de estados (OCIOSO, ESTIMULO, AVALIANDO, INTERVALO, FIM_SESSAO); Mecanismo A (Fisher-Yates por bloco de 4) e Mecanismo B (peso decrescente); logica de acerto/erro Modo 1 e Modo 2 com janela de simultaneidade; callback de resultado e fim de sessao
- `firmware/test/test_game/test_main.cpp` — 11 testes Unity para MOD_JOGO; cobre CA-04-01 (distribuicao Mec A), CA-04-02 (variacao Mec B), CA-04-03 (pares distintos Modo 2), CA-04-04 (acerto Modo 1), CA-04-05 (erro Modo 1), CA-04-06 (acerto Modo 2 dentro da janela), CA-04-07 (erro Modo 2 fora da janela), CA-04-08 (fim de sessao), CA-04-10 (intervalo); CA-04-09 documentado como REQUER HARDWARE

### Alterado
- `firmware/src/main.cpp` — fiacao completa dos 4 modulos: inclui `game.h` e `interface.h`; adiciona `gameInit()` e `interfaceInit()` em `setup()`; adiciona `gameLoop()` e `interfaceLoop()` em `loop()`; `sensorOnImpacto(gameOnImpacto)` conecta MOD_SENSOR a MOD_JOGO
- `firmware/platformio.ini` — adiciona `bblanchon/ArduinoJson@^7.0.0` a `lib_deps` de `[env:esp32dev]`; derivado de `01_arquitetura.md#stack-tecnologico`
- `system/01_arquitetura.md` — adiciona ArduinoJson >= 7.0 a tabela de stack tecnologico
- `_governance/CODING_STANDARD.md` v0.1.1 — atualiza MOD_WIFI em `#modularidade-pode`: adiciona `<ArduinoJson.h>` as includes permitidas
- `firmware/platformio.ini`: refatora para `[env]` base compartilhada — `build_flags = -std=gnu++17` e `lib_extra_dirs = src` movidos para secao comum; `[env:native]` estende via `${env.build_flags}`; corrige include path em `[env:esp32dev]` que nao resolvia `"game.h"` e `"sensor.h"` — derivado de `CODING_STANDARD.md#estrutura-cpp`
- `firmware/platformio.ini`: atualiza `build_src_filter` para `+<sensor/> +<visual/> +<game/>` — inclui MOD_JOGO no build principal
- `firmware/test/mock/Arduino.h`: adiciona mock minimo de `String` (construtor default, `const char*`, copia, atribuicao) rastreado a `ConfigSessao.nome_crianca`; adiciona declaracao de `long random(long max)` rastreada ao mecanismo de aleatoriedade de `game.cpp`

- `firmware/test/mock/FastLED.h` — mock minimo de FastLED para compilacao nativa: `CRGB`, `CFastLED`, `EOrder`/`GRB`, `WS2812B`; `show()` copia estado para `g_mock_led_buf` (observavel nos testes); padrao declare->define
- `firmware/test/test_visual/test_main.cpp` — 14 testes Unity para MOD_LED; cobre CA-03-01 (boot animation), CA-03-04 (LED correto por modo), CA-03-06 (celebracao); constantes T_ derivadas de `visual_config.h`; CAs hardware documentados como comentario
- `firmware/src/visual/visual.h` — interface publica de MOD_LED: `ComandoLED` (struct + enums LED/Cor), `visualInit()`, `visualLoop()`, `visualSetLED()`, `visualRunCelebracao()`
- `firmware/src/visual/visual.cpp` — implementacao de MOD_LED derivada de `spec/visual/visual.json`; animacoes boot e celebracao non-blocking via millis(); paleta e sequencias de boot derivadas de tabelas constexpr; DM-03 (`EOrder`/`GRB`) declarado apos includes de plataforma
- `firmware/platformio.ini` — configuracao de build: `[env:esp32dev]` (ESP32 Arduino) e `[env:native]` (testes Unity com mingw64); `lib_extra_dirs = src` para LDF detectar modulos de `src/` em testes nativos
- `firmware/test/mock/Arduino.h` — mock minimo de Arduino para compilacao nativa: `adc_atten_t`, `ADC_ATTEN_DB_11`, `INPUT`, `OUTPUT`, declaracoes de `analogRead`, `millis`, `pinMode`, `analogSetAttenuation`
- `firmware/test/test_sensor/test_main.cpp` — 13 testes Unity para MOD_SENSOR; cobre CA-02-01 (deteccao), CA-02-02 (falso positivo), CA-02-05 (debounce); constantes T_ derivadas de `sensor_config.h`; CAs hardware documentados como comentario
- `firmware/src/sensor/sensor.h` — interface publica de MOD_SENSOR: `EventoImpacto` (struct + enum Zona), `sensorInit()`, `sensorOnImpacto()`, `sensorLoop()`
- `firmware/src/sensor/sensor.cpp` — implementacao de MOD_SENSOR derivada de `spec/sensor/sensor.json`; tabelas GPIO→Zona, debounce por zona independente, DM-02 (`ADC_ATTEN_DB_11`) declarado apos include de plataforma
- `firmware/src/main.cpp` — stub de entry point Arduino; chama `sensorInit()` e `sensorLoop()`; callback placeholder para MOD_JOGO
- `_governance/TESTING_STANDARD.md` v0.1.0 — padrão genérico de testes de firmware; define zero magic numbers em testes, constantes T_ derivadas de `_config.h`, padrão mock declare→define, rastreabilidade de CAs
- `_governance/CODING_STANDARD.md` v0.1.0 — padrao de codigo firmware derivado de `01_arquitetura.md` v0.1.0 e de todos os specs v0.2.0; secoes derivadas geradas automaticamente por script
- `spec/firmware_constants.json` — mapeamento canonico de 47 constantes DERIVADO e 3 HARDCODED_JUSTIFICADO; liga cada constante C++ ao campo JSON de origem no spec correspondente
- `spec/firmware_constants.schema.json` — schema JSON (draft/2020-12) para validacao de firmware_constants.json
- `scripts/generate_coding_standard.py` — gera secoes derivadas de CODING_STANDARD.md a partir de firmware_constants.json; valida cascata (valores spec vs valor mapeado) em modo --check
- `firmware/src/sensor/sensor_config.h`, `firmware/src/visual/visual_config.h`, `firmware/src/game/game_config.h`, `firmware/src/interface/interface_config.h` — gerados por `generate_coding_standard.py`; cascata completa de ponta a ponta (spec JSON → firmware_constants.json → _config.h); compilaveis em native (constantes com tipo de plataforma emitidas como stub `[PLATAFORMA]`)

### Alterado
- `firmware/platformio.ini`: atualiza `build_src_filter` para `+<sensor/> +<visual/>` — inclui MOD_LED no build principal
- `firmware/src/main.cpp`: adiciona `visualInit()` em `setup()` e `visualLoop()` em `loop()` conforme ordem de init CODING_STANDARD.md#modularidade-ordem-init
- `_governance/TESTING_STANDARD.md` v0.1.0 → v0.1.1: corrige secao 9.1 — adiciona `lib_extra_dirs = src` obrigatorio; documenta que `build_src_filter` nao afeta test builds em PlatformIO 6.x; bump PATCH
- `firmware/platformio.ini`: adiciona `lib_extra_dirs = src` em `[env:native]` — corrige compilacao de modulos `src/` em `pio test` (PlatformIO 6.1 nao inclui `src/` em test builds via `build_src_filter`; LDF detecta automaticamente via `lib_extra_dirs`)
- `_governance/CODING_STANDARD.md` v0.1.0 → v0.1.1: adiciona `TESTING_STANDARD.md` em `impacta` (CONDICIONAL); bump PATCH
- `scripts/run_all.py`: adiciona 4 check — `generate_coding_standard.py --check` (cascata specs -> CODING_STANDARD.md)
- `scripts/generate_coding_standard.py`: estende para gerar `_config.h` em modo generate e verificar sync dos arquivos em modo --check; suporte a flag `tipo_plataforma` em DMs — emite stub comentado `[PLATAFORMA]` em vez de declaracao C++ para tipos que requerem headers de plataforma
- `spec/firmware_constants.json`: adiciona `tipo_plataforma: true` em DM-02 (`adc_atten_t`) e DM-03 (`EOrder`)
- `spec/firmware_constants.schema.json`: aceita campo opcional `tipo_plataforma: boolean` em `decisoes_manuais_inerentes`

### Corrigido
- `firmware/src/main.cpp`: remove comentario `[DECIDIR]` sobre fiacao game→visual — resolvido em `game.cpp`
- `firmware/test/test_game/test_main.cpp`: adiciona definicoes do mock FastLED (padrao declare->define); adiciona `visualInit()` em `setUp()`; estende CA-04-04 com assertiva de LED apagado e CA-04-05 com assertiva de LED mantido
- `firmware/src/sensor/sensor.h`: corrige referencia `[VER:]` de `#interfaces` para `#interfaces-modulos` — ancora inexistente; ancora real em `01_arquitetura.md` e `#interfaces-modulos`
- `scripts/verify_links.py`: corrige varredura de `.md` de `glob('*.md')` (so raiz) para `rglob` recursivo com registry `{nome: path}` — scripts nao verificavam nenhum documento em subdiretorios
- `scripts/check_impacta.py`: mesma correcao de varredura recursiva com registry
- `scripts/check_versions.py`: mesma correcao de varredura recursiva com registry
- `SESSION_STATE.md`: corrige premissa falsa registrada implicitamente — `generate_coding_standard.py` JA gera `_config.h` (linhas 414-421 do script) e JA verifica sync em modo --check; `run_all.py` confirma `OK: 4 arquivos _config.h em sync`; premissa de que "o script nao tem logica de saida para firmware/src/" e falsa e nao deve ser re-analisada em sessoes futuras

---

## [0.2.0] — 2026-06-28

### Adicionado
- `spec/power/power.json` — spec derivada de `05_alimentacao.md` v0.1.0
- `spec/power/power.schema.json` — schema de validacao do power spec
- `spec/sensor/sensor.json` — spec derivada de `02_sensor_impacto.md` v0.1.0
- `spec/sensor/sensor.schema.json` — schema de validacao do sensor spec
- `spec/visual/visual.json` — spec derivada de `03_saida_visual.md` v0.1.0
- `spec/visual/visual.schema.json` — schema de validacao do visual spec
- `spec/game/game.json` — spec derivada de `04_logica_jogo.md` v0.1.0
- `spec/game/game.schema.json` — schema de validacao do game spec
- `spec/interface/interface.json` — spec derivada de `07_interface_pedagogo.md` v0.1.0
- `spec/interface/interface.schema.json` — schema de validacao do interface spec
- `README.md` — documentacao de entrada do repositorio
- Script de automacao de versionamento (`_governance/scripts/bump_version.py`)
- `CHANGELOG.md` (este arquivo)

### Alterado
- `CLAUDE.md` ETAPA 6: adiciona regra obrigatoria de campos sem narrativa nos arquivos JSON
- `CLAUDE.md` retorno de sessao: referencia SESSION_STATE.md antes de ir para ETAPA 3
- `CLAUDE.md` ETAPA 5 checklist: adiciona meta-analise de cadeia como item pre-commit
- `CLAUDE.md` ETAPA 6 procedimento: adiciona passo 0 (ler SESSION_STATE), passos 9-11 (merge, proxima branch, atualizar SESSION_STATE)
- `CLAUDE.md` ETAPA 5: padroniza corpo de mensagem de commit (obrigatorio para spec/feat/fix/docs) com modelos e regra heredoc
- `CLAUDE.md` ETAPA 5: adiciona Protocolo CHANGELOG com arvore de decisao e gate imediato pre-commit

### Corrigido
- Scripts de verificacao movidos de `_governance/scripts/` para `scripts/` (alinhamento com CLAUDE.md e CI)
- `scripts/run_all.py`: corrige UnicodeEncodeError no Windows ao imprimir caracteres UTF-8
- `_governance/_PADRAO.md`: corrige versao de 1.1.0 para 0.1.0 (numero incorreto introduzido no HEAD de develop)

---

## [0.1.0] — 2026-06-27

### Adicionado
- Commit inicial: V-model Niveis 0 a 2 aprovados
  - `concept/00_conceito.md` v0.1.0
  - `system/01_arquitetura.md` v0.1.0
  - `modules/sensor/02_sensor_impacto.md` v0.1.0
  - `modules/visual/03_saida_visual.md` v0.1.0
  - `modules/game/04_logica_jogo.md` v0.1.0
  - `modules/power/05_alimentacao.md` v0.1.0
  - `compliance/06_privacidade_lgpd.md` v0.1.0
  - `modules/interface/07_interface_pedagogo.md` v0.1.0
  - `hardware/08_bom.md` v0.1.0
  - `hardware/09_conexoes.md` v0.1.0
  - `hardware/10_cablagem.md` v0.1.0
  - `hardware/11_montagem.md` v0.1.0
  - `_governance/_PADRAO.md` v0.1.0
- Protocolo V-model de engenharia (`v_model_protocol/`)
- Padroes de governanca (`_governance/`)
- Scripts de verificacao de documentacao (`scripts/`)
- Pipeline CI/CD (`.github/workflows/verify-docs.yml`)
- Licenca GPL-3.0

---

## Proximas releases planejadas

| Tag | Conteudo |
|---|---|
| v1.0.0 | V-model fechado — validacao completa (ETAPA 8) |
