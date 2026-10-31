// Ardugochi — global configuration
// SPDX-License-Identifier: MIT
#ifndef ARDUGOCHI_CONFIG_H
#define ARDUGOCHI_CONFIG_H

#include <cstdint>

// ---------------------------------------------------------------------------
// Board: Wemos D1 Mini (ESP8266)
// ---------------------------------------------------------------------------
// I2C pins on D1 Mini:
//   SDA = GPIO4 (D2)
//   SCL = GPIO5 (D1)
// ---------------------------------------------------------------------------

// Display (SSD1306 128x64 OLED over I2C)
constexpr uint8_t SCREEN_WIDTH   = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr uint8_t OLED_ADDR      = 0x3C;   // common I2C address
constexpr int8_t  OLED_RST_PIN   = -1;       // -1 = no separate reset pin

// Input pins (3 tactile buttons, active LOW with internal pull-ups)
// Safe GPIOs on D1 Mini: D5 (GPIO14), D6 (GPIO12), D7 (GPIO13)
// NEVER use GPIO0 (D3) — forces flash mode on boot.
constexpr uint8_t BTN_FEED_PIN   = 14;   // D5
constexpr uint8_t BTN_PLAY_PIN   = 12;   // D6
constexpr uint8_t BTN_STATUS_PIN = 13;   // D7

// ---------------------------------------------------------------------------
// Timing
// ---------------------------------------------------------------------------
constexpr uint16_t SPLASH_MS          = 1500;   // splash screen duration
constexpr uint16_t TICK_INTERVAL_MS   = 10000;  // stat decay every 10 s
constexpr uint16_t SAVE_INTERVAL_MS   = 60000; // EEPROM persist every 60 s
constexpr uint16_t DEBOUNCE_MS        = 30;     // button debounce

// ---------------------------------------------------------------------------
// Stat bounds
// ---------------------------------------------------------------------------
constexpr uint8_t STAT_MIN = 0;
constexpr uint8_t STAT_MAX = 100;

#endif // ARDUGOCHI_CONFIG_H
