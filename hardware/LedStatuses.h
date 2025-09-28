#pragma once
#include "LedStatus.h"

// WiFi statuses
const LedStatus WIFI_CONNECTED(1, "Wi-Fi is Connected", 0x00FF00);
const LedStatus WIFI_CONNECTING(0, "Wi-Fi is Connecting", 0xFFFF00);
const LedStatus WIFI_DISCONNECTED(-1, "Wi-Fi is Disconnected", 0xFF0000);

// PSU statuses
const LedStatus PSU_ON(1, "PSU is ON", 0x00FF00);
const LedStatus PSU_OFF(0, "PSU is OFF", 0xFF0000);

// PC statuses
const LedStatus PC_ON(1, "PC is ON", 0x00FF00);
const LedStatus PC_OFF(0, "PC is OFF", 0xFF0000);