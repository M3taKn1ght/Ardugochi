// Ardugochi — 8x8 status icons (monochrome, PROGMEM)
// SPDX-License-Identifier: MIT
#ifndef ARDUGOCHI_ICONS_H
#define ARDUGOCHI_ICONS_H

#include <Arduino.h>

constexpr uint8_t ICON_W = 8;
constexpr uint8_t ICON_H = 8;

// Heart (Health)
const uint8_t PROGMEM ICON_HEART[] = {
    B00000000,
    B00011000,
    B00111100,
    B01111110,
    B01111110,
    B00111100,
    B00011000,
    B00000000
};

// Burger/Food (Hunger — empty plate with steam)
const uint8_t PROGMEM ICON_FOOD[] = {
    B00000000,
    B00011000,
    B00100100,
    B00000000,
    B00111100,
    B01111110,
    B00111100,
    B00000000
};

// Star/Music (Fun/Happiness)
const uint8_t PROGMEM ICON_FUN[] = {
    B00000000,
    B00010000,
    B00111000,
    B01111100,
    B00010000,
    B00010000,
    B00101000,
    B00000000
};

// Crown/Shield (Discipline)
const uint8_t PROGMEM ICON_DISC[] = {
    B00000000,
    B00111100,
    B01111110,
    B01011010,
    B00011000,
    B00011000,
    B00111100,
    B00000000
};

// Clock (Age / cycles)
const uint8_t PROGMEM ICON_CLOCK[] = {
    B00000000,
    B00111100,
    B01000010,
    B01011010,
    B01001010,
    B01000010,
    B00111100,
    B00000000
};

#endif // ARDUGOCHI_ICONS_H
