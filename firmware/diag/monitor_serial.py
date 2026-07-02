# monitor_serial.py — Monitor serial com timestamp por linha e reset via RTS.
# Kit de diagnostico de bancada; NAO faz parte do V-model.
#
# Uso:
#   python monitor_serial.py [porta] [duracao_s] [reset|noreset]
#   Ex.: C:\Users\eflov\.platformio\penv\Scripts\python.exe monitor_serial.py COM9 20 reset
#
# reset   -> pulsa EN via RTS antes de capturar (pega o boot desde o inicio)
# noreset -> so escuta, sem resetar a placa
#
# Requer pyserial (o python do penv do PlatformIO ja tem).
# Nota: as primeiras linhas em [0.000] podem ser lixo pre-bufferizado do
# driver CP210x (fragmentos repetidos) — ignorar; o trecho ao vivo comeca
# no primeiro "ets Jul 29 2019" apos o reset.
import serial
import sys
import time

port = sys.argv[1] if len(sys.argv) > 1 else 'COM9'
dur = float(sys.argv[2]) if len(sys.argv) > 2 else 20.0
do_reset = (len(sys.argv) < 4) or (sys.argv[3] != 'noreset')

ser = serial.Serial()
ser.port = port
ser.baudrate = 115200
ser.timeout = 0.05
ser.dtr = False          # nunca segurar GPIO0
ser.rts = do_reset       # RTS=True -> EN em nivel baixo (reset segurado)
ser.open()
if do_reset:
    time.sleep(0.1)
    ser.rts = False      # solta EN -> boot normal

t0 = time.time()
end = t0 + dur
line = b''
n_lines = 0
while time.time() < end:
    chunk = ser.read(256)
    if not chunk:
        continue
    line += chunk
    while b'\n' in line:
        one, line = line.split(b'\n', 1)
        n_lines += 1
        print('[%8.3f] %s' % (time.time() - t0,
                              one.decode('utf-8', errors='replace').rstrip()))
if line:
    print('[%8.3f] %s' % (time.time() - t0,
                          line.decode('utf-8', errors='replace').rstrip()))
ser.close()
print('--- fim da captura: %.1fs, %d linhas ---' % (dur, n_lines))
