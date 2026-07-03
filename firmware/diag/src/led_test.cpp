// main.cpp — Diagnostico de bancada da cadeia WS2812B (GPIO5)
// Projeto separado do firmware; NAO derivado do V-model. Ver ../README.md.
//
// Fase 1 (30s apos boot/EN): onda quadrada lenta no GPIO5 — medir com
//   multimetro DC no DIN do LED1 contra GND: alterna ~3.3V / ~0V a cada 5s.
//   Apertar EN reinicia a fase 1 (nova janela de medicao).
// Fase 2 (para sempre): ciclo de cores identico nos 3 LEDs, 1s por cor.

#include <Arduino.h>
#include <FastLED.h>

static const uint8_t N_LEDS = 3;
static CRGB leds[N_LEDS];

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("LED_HW_TEST v2 — fase 1: onda quadrada lenta no GPIO5 (30s)");
    Serial.println("Medir DC no DIN do LED1 contra GND: deve alternar ~3.3V / ~0V a cada 5s");
    pinMode(5, OUTPUT);
    for (uint8_t i = 0; i < 3; i++) {
        Serial.println("LED_HW_TEST: GPIO5 = ALTO (~3.3V no DIN) por 5s");
        digitalWrite(5, HIGH);
        delay(5000);
        Serial.println("LED_HW_TEST: GPIO5 = BAIXO (~0V no DIN) por 5s");
        digitalWrite(5, LOW);
        delay(5000);
    }
    Serial.println("LED_HW_TEST v2 — fase 2: ciclo de cores FastLED (para sempre)");
    FastLED.addLeds<WS2812B, 5, GRB>(leds, N_LEDS);
    FastLED.setBrightness(150);
}

static void mostrar(const char* nome, CRGB cor) {
    Serial.print("LED_HW_TEST: ");
    Serial.println(nome);
    for (uint8_t i = 0; i < N_LEDS; i++) {
        leds[i] = cor;
    }
    FastLED.show();
    delay(1000);
}

void loop() {
    mostrar("VERMELHO", CRGB(255, 0, 0));
    mostrar("VERDE",    CRGB(0, 255, 0));
    mostrar("AZUL",     CRGB(0, 0, 255));
    mostrar("BRANCO",   CRGB(255, 255, 255));
    mostrar("APAGADO",  CRGB(0, 0, 0));
}
