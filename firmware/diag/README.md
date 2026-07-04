# Kit de diagnóstico de bancada

Projeto PlatformIO **separado** do firmware, criado durante a investigação do
boot loop CA-07-01 (2026-07-02 — ver seção "Investigação boot loop CA-07-01 —
RESOLVIDA" em `../../SESSION_STATE.md`).

**Não faz parte do V-model.** Nada aqui deriva de spec; são ferramentas de
bancada para diagnóstico manual de hardware. Este diretório não é compilado
pelos builds normais do firmware (`pio run` no projeto principal não o enxerga).

## Conteúdo

| Arquivo | O que faz |
|---|---|
| `src/led_test.cpp` | Sketch de teste da cadeia WS2812B no GPIO5 (2 fases, ver abaixo) — env `ledtest` |
| `src/wifi_test.cpp` | Init do AP com TX reduzido + breadcrumbs na serial (diagnóstico de brownout) — env `wifitest` |
| `monitor_serial.py` | Monitor serial com timestamp por linha e reset via RTS |
| `platformio.ini` | Ambientes `ledtest` e `wifitest` (esp32dev; um sketch por env via `build_src_filter`) |

## Sketch de teste do LED (`src/led_test.cpp`)

- **Fase 1 (30s após boot ou EN):** onda quadrada lenta no GPIO5 — 5s em
  ~3.3V, 5s em ~0V, três ciclos. Medível com multímetro DC no DIN do LED1
  contra GND. Apertar EN reinicia a janela de medição.
- **Fase 2 (para sempre):** ciclo de cores idêntico nos 3 LEDs (VERMELHO,
  VERDE, AZUL, BRANCO, APAGADO), 1s por cor, com eco na serial (115200).

### Gravar o teste

```
pio run -d firmware/diag -e ledtest -t upload
```

Se falhar com "Wrong boot mode detected": segurar **BOOT**, apertar e soltar
**EN**, soltar **BOOT**, e repetir o upload (o auto-download desta placa é
recorrentemente instável).

### Voltar o firmware real depois do diagnóstico

```
pio run -d firmware -e esp32dev -t upload
```

## Sketch de teste do WiFi (`src/wifi_test.cpp`)

Sobe o AP **"BMI-DIAG"** com potência TX reduzida (8.5 dBm) imprimindo cada
passo na serial — a última linha antes de um `Brownout detector was triggered`
localiza o ponto exato da queda de tensão. Usado para condenar o AMS1117 do
DevKitC (2026-07-02): a placa morria no "passo 1" (ligada do rádio) em
qualquer fonte de energia, e passou a bootar com 3.30V direto no rail 3V3.

```
pio run -d firmware/diag -e wifitest -t upload
```

Leitura: morre no "passo 1" = o pico da ligada do rádio derruba o 3V3 (fonte
fraca ou regulador da placa incapaz); AP "BMI-DIAG" no ar = a alimentação
sustenta o rádio com TX reduzido.

## Monitor serial (`monitor_serial.py`)

```
python monitor_serial.py COM9 20 reset     # reseta a placa e captura 20s
python monitor_serial.py COM9 60 noreset   # so escuta, sem resetar
```

Requer pyserial — usar o Python do penv do PlatformIO
(`C:\Users\eflov\.platformio\penv\Scripts\python.exe`).

As primeiras linhas em `[0.000]` podem ser lixo pré-bufferizado do driver
CP210x (fragmentos repetidos); o trecho ao vivo começa no primeiro
`ets Jul 29 2019` após o reset.

**Confirmado na investigação do D2 (2026-07-04):** reproduzido de novo — uma
captura trouxe dezenas de linhas idênticas (mesmo timestamp de firmware, ex.
`[DIAG 66500] WS_EVT_DISCONNECT` repetido ~70 vezes) despejadas em poucos ms
de tempo real, antes do boot real aparecer. Não é defeito de firmware nem do
produto (só afeta captura serial de bancada; o pedagogo nunca vê isso) — é
o driver CP210x entregando de uma vez o que ficou represado no buffer da
porta enquanto nenhum monitor estava aberto. Ação: nenhuma. Ignorar tudo
antes do primeiro `ets Jul 29 2019` da captura, como já orientado acima.

## Lições de hardware registradas (2026-07-02)

- **Serigrafia do shield mente:** o borne marcado "GND" entre SD3 e 5V
  corresponde ao pino **CMD/GPIO11 (chip select da flash)** do DevKitC.
  Conferir TODO rótulo do shield contra a serigrafia do próprio DevKitC antes
  de parafusar qualquer fio. Causa raiz do boot loop CA-07-01.
- **Clipes da cadeia de LED são marginais:** contato re-assenta com manuseio
  (cadeia inteira apagada → funcionando sem mudança de código). Endereçar na
  cablagem/montagem final.
- **AMS1117 do DevKitC degradado (2026-07-02):** sustenta cargas leves mas
  colapsa no pico da ligada do rádio WiFi → brownout em loop, em qualquer
  fonte de 5V. Contorno que virou arquitetura: LM2596 ajustado para 3.30V
  alimentando o rail 3V3 direto (USB só para gravar — gravação não usa WiFi).
  Consequência operacional: USB-only com WiFi = brownout sempre; bancada de
  WiFi exige a fonte ligada. Segurança: medir 3.30V na saída do LM2596 ANTES
  de conectar ao borne 3V3 (5V nesse rail mata o ESP32; máx. absoluto 3.6V).
- **USB + fonte externa ligados AO MESMO TEMPO também causa brownout em
  loop (confirmado 2026-07-04, investigação D2/D3):** com os dois
  conectados simultaneamente, brownout batia de forma regular a cada
  ~3.4s, sempre logo após o rádio WiFi subir (`rst:0xc SW_CPU_RESET`
  precedido de `Brownout detector was triggered`), com o MESMO firmware
  que já tinha rodado limpo minutos antes só com a fonte. Hipótese: o
  regulador de bordo do DevKitC (alimentado pelo 5V do USB) e o LM2596
  externo (injetando 3.3V direto no mesmo rail) disputam o mesmo trilho.
  Confirmado na prática: desconectar o USB e rodar só com a fonte externa
  eliminou o brownout. **Para bancada de WiFi: usar SOMENTE a fonte
  externa; desconectar o USB após gravar.** Se precisar de log serial ao
  vivo durante teste de WiFi, usar um cabo USB "somente dados" (sem o fio
  de 5V/VBUS) — um cabo comum reintroduz o conflito.
