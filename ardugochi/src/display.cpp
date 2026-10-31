// Ardugochi — display manager implementation (SSD1306 OLED)
// SPDX-License-Identifier: MIT
#include "display.h"

DisplayManager::DisplayManager()
    : mOled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST_PIN) {}

bool DisplayManager::begin() {
    Wire.begin(4, 5);  // SDA=GPIO4(D2), SCL=GPIO5(D1) on D1 Mini
    if (!mOled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        return false;
    }
    mOled.display();
    delay(100);
    mOled.clearDisplay();
    return true;
}

void DisplayManager::showSplash() {
    mOled.clearDisplay();
    mOled.setTextSize(2);
    mOled.setTextColor(SSD1306_WHITE);
    mOled.setCursor(10, 20);
    mOled.print(F("Ardugochi"));
    mOled.setTextSize(1);
    mOled.setCursor(30, 45);
    mOled.print(F("v0.2 booting..."));
    mOled.display();
}

void DisplayManager::render(const Pet& pet) {
    mOled.clearDisplay();
    if (pet.infoScreen()) {
        drawInfoScreen(pet);
    } else {
        drawPetScreen(pet);
    }
    mOled.display();
}

void DisplayManager::drawPetScreen(const Pet& pet) {
    drawStatusLine(pet);

    const uint8_t* bmp = nullptr;
    switch (pet.state()) {
        case PetState::EATING:   bmp = SPRITE_EAT;   break;
        case PetState::PLAYING:  bmp = SPRITE_PLAY;  break;
        case PetState::ANGRY:    bmp = SPRITE_ANGRY; break;
        case PetState::DEAD:     bmp = SPRITE_DEAD;  break;
        case PetState::SLEEPING: bmp = SPRITE_SLEEP; break;
        default:                 bmp = SPRITE_IDLE;  break;
    }

    int16_t x = (SCREEN_WIDTH - SPRITE_W) / 2;
    int16_t y = 8;
    mOled.drawBitmap(x, y, bmp, SPRITE_W, SPRITE_H, SSD1306_WHITE);

    if (pet.state() == PetState::SLEEPING) {
        mOled.setTextSize(1);
        mOled.setTextColor(SSD1306_WHITE);
        bool phase = (millis() / 800) % 2;
        if (phase) {
            mOled.setCursor(x + SPRITE_W + 2, y + 4);
            mOled.print(F("z"));
            mOled.setCursor(x + SPRITE_W + 8, y - 2);
            mOled.print(F("Z"));
        }
    }

    drawStatBar(2, SCREEN_HEIGHT - 6, pet.health(), F("HP"));
}

void DisplayManager::drawInfoScreen(const Pet& pet) {
    mOled.setTextSize(1);
    mOled.setTextColor(SSD1306_WHITE);

    mOled.drawBitmap((SCREEN_WIDTH - ICON_W) / 2, 0, ICON_HEART, ICON_W, ICON_H, SSD1306_WHITE);
    mOled.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    drawInfoBar(0, 13, ICON_HEART, pet.health(),    pet.health()    < 30);
    drawInfoBar(0, 26, ICON_FOOD,  pet.hunger(),    pet.hunger()    > 70);
    drawInfoBar(0, 39, ICON_FUN,   pet.happiness(), pet.happiness() < 30);

    mOled.drawBitmap(0, 52, ICON_CLOCK, ICON_W, ICON_H, SSD1306_WHITE);
    mOled.setCursor(12, 54);
    mOled.print(pet.ageTicks());
}

void DisplayManager::drawInfoBar(uint8_t x, uint8_t y, const uint8_t* icon,
                                 uint8_t value, bool critical) {
    constexpr uint8_t BAR_W = 80;
    constexpr uint8_t BAR_H = 8;

    mOled.drawBitmap(x, y, icon, ICON_W, ICON_H, SSD1306_WHITE);

    uint8_t barX = x + 12;
    mOled.drawRect(barX, y, BAR_W, BAR_H, SSD1306_WHITE);

    uint8_t fill = (value * (BAR_W - 2)) / STAT_MAX;
    if (fill > 0) {
        if (critical) {
            for (uint8_t fx = 0; fx < fill; fx += 3) {
                uint8_t segW = (fx + 3 <= fill) ? 2 : (fill - fx);
                mOled.fillRect(barX + 1 + fx, y + 2, segW, BAR_H - 4, SSD1306_WHITE);
            }
            mOled.fillRect(barX + 1, y + 1, fill, 1, SSD1306_WHITE);
            mOled.fillRect(barX + 1, y + BAR_H - 2, fill, 1, SSD1306_WHITE);
        } else {
            mOled.fillRect(barX + 1, y + 1, fill, BAR_H - 2, SSD1306_WHITE);
        }
    }
}

void DisplayManager::drawStatusLine(const Pet& pet) {
    uint16_t days = pet.ageTicks() / 6;

    mOled.setCursor(2, 0);
    mOled.print(F("Tag "));
    mOled.print(days);

    const uint8_t* face = nullptr;
    if (pet.state() == PetState::DEAD) {
        face = ICON_DISC;
    } else if (pet.health() > 80 && pet.happiness() > 60) {
        face = ICON_FUN;
    } else if (pet.health() < 30 || pet.hunger() > 80) {
        face = ICON_HEART;
    } else {
        face = ICON_FOOD;
    }

    if (face) {
        mOled.drawBitmap(SCREEN_WIDTH - ICON_W - 2, 0, face, ICON_W, ICON_H, SSD1306_WHITE);
    }
}

void DisplayManager::drawStatBar(uint8_t x, uint8_t y, uint8_t value,
                                 const __FlashStringHelper* label) {
    constexpr uint8_t barW = 30;
    constexpr uint8_t barH = 4;
    mOled.setTextSize(1);
    mOled.setTextColor(SSD1306_WHITE);
    mOled.setCursor(x, y - 8);
    mOled.print(label);
    mOled.drawRect(x, y, barW, barH, SSD1306_WHITE);
    uint8_t fill = (value * (barW - 2)) / STAT_MAX;
    mOled.fillRect(x + 1, y + 1, fill, barH - 2, SSD1306_WHITE);
}
