// wifi_test.cpp — Diagnostico de bancada do init WiFi (brownout)
// Projeto separado do firmware; NAO derivado do V-model. Ver ../README.md.
//
// Sobe o AP "BMI-DIAG" com potencia TX reduzida (8.5 dBm) e imprime cada
// passo na serial — os prints sao breadcrumbs: a ultima linha antes de um
// "Brownout detector was triggered" localiza o ponto exato da queda.

#include <Arduino.h>
#include <WiFi.h>

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("WIFI_HW_TEST: iniciando");
    Serial.println("WIFI_HW_TEST: passo 1 — WiFi.mode(WIFI_AP)...");
    WiFi.mode(WIFI_AP);
    Serial.println("WIFI_HW_TEST: passo 2 — mode OK; TX -> 8.5 dBm");
    WiFi.setTxPower(WIFI_POWER_8_5dBm);
    Serial.println("WIFI_HW_TEST: passo 3 — subindo AP BMI-DIAG...");
    const bool ok = WiFi.softAP("BMI-DIAG", nullptr, 1);
    Serial.print("WIFI_HW_TEST: passo 4 — softAP -> ");
    Serial.println(ok ? "OK" : "FALHOU");
    Serial.print("WIFI_HW_TEST: IP ");
    Serial.println(WiFi.softAPIP());
}

void loop() {
    Serial.print("WIFI_HW_TEST: vivo; txpower=");
    Serial.print((int)WiFi.getTxPower());
    Serial.print("; estacoes=");
    Serial.println(WiFi.softAPgetStationNum());
    delay(2000);
}
