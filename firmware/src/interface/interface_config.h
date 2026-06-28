#pragma once

// interface_config.h
// Fonte: spec/interface/interface.json (via spec/firmware_constants.json)
// Nenhum valor neste arquivo e inventado.
// Toda alteracao exige atualizacao de firmware_constants.json e regeneracao.

#include <stdint.h>

// --- DERIVADO: spec/interface/interface.json#access_point.ssid ---
constexpr const char* INTERFACE_AP_SSID = "BMI";

// --- DERIVADO: spec/interface/interface.json#access_point.ip ---
constexpr const char* INTERFACE_AP_IP = "192.168.4.1";

// --- DERIVADO: spec/interface/interface.json#access_point.canal ---
constexpr uint8_t INTERFACE_AP_CANAL = 1;

// --- DERIVADO: spec/interface/interface.json#estados_interface.duracao_feedback_acerto_ms ---
constexpr uint16_t INTERFACE_FEEDBACK_ACERTO_MS = 1500;

// --- DERIVADO: spec/interface/interface.json#servidor_http.rotas[1].rota ---
constexpr const char* INTERFACE_WS_PATH = "/ws";
