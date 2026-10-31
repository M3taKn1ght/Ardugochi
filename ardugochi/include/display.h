// Ardugochi — display manager declarations (SSD1306 OLED)
// SPDX-License-Identifier: MIT
#ifndef ARDUGOCHI_DISPLAY_H
#define ARDUGOCHI_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "pet.h"
#include "sprites.h"
#include "icons.h"

class DisplayManager {
public:
    explicit DisplayManager();
    bool begin();
    void showSplash();
    void render(const Pet& pet);

private:
    Adafruit_SSD1306 mOled;

    void drawPetScreen(const Pet& pet);
    void drawInfoScreen(const Pet& pet);
    void drawInfoBar(uint8_t x, uint8_t y, const uint8_t* icon,
                     uint8_t value, bool critical);
    void drawStatusLine(const Pet& pet);
    void drawStatBar(uint8_t x, uint8_t y, uint8_t value,
                     const __FlashStringHelper* label);
};

#endif // ARDUGOCHI_DISPLAY_H
