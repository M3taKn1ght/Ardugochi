// Ardugochi — D1 Mini (ESP8266) Tamagotchi Clone
// SPDX-License-Identifier: MIT
// Board: Wemos D1 Mini (ESP8266EX)

#include <Arduino.h>
#include "config.h"
#include "pet.h"
#include "display.h"
#include "input.h"

static Pet gPet;
static DisplayManager gDisplay;
static InputManager gInput;

static unsigned long gLastTick = 0;  // last game tick (ms)
static unsigned long gLastSave = 0;  // last EEPROM save (ms)

static void handleInput(InputEvent ev);

void setup() {
    Serial.begin(115200);
    Serial.println(F("Ardugochi v0.2 — D1 Mini"));

    if (!gDisplay.begin()) {
        Serial.println(F("SSD1306 init failed — halting"));
        for (;;) { delay(100); }
    }

    gDisplay.showSplash();
    delay(SPLASH_MS);

    gInput.begin();

    // Try to load persisted state; fall back to newborn
    if (!gPet.load()) {
        gPet.reset();
    }

    gLastTick = millis();
    gLastSave = millis();
}

void loop() {
    const unsigned long now = millis();

    // ---- periodic game tick (every TICK_INTERVAL_MS) ----
    if ((now - gLastTick) >= TICK_INTERVAL_MS) {
        gLastTick += TICK_INTERVAL_MS;
        gPet.tick();

        // Persist state every SAVE_INTERVAL_MS (wear-aware)
        if ((now - gLastSave) >= SAVE_INTERVAL_MS) {
            gLastSave += SAVE_INTERVAL_MS;
            gPet.save();
        }
    }

    // ---- input handling ----
    const InputEvent ev = gInput.poll();
    if (ev != InputEvent::NONE) {
        handleInput(ev);
    }

    // ---- render ----
    gDisplay.render(gPet);
    delay(16);  // ~60 FPS cap
}

static void handleInput(InputEvent ev) {
    switch (ev) {
        case InputEvent::FEED:
            gPet.feed();
            break;
        case InputEvent::PLAY:
            gPet.play();
            break;
        case InputEvent::STATUS:
            gPet.toggleInfoScreen();
            break;
        case InputEvent::RESET:
            // Tamagotchi "paperclip reset": only works when pet is dead
            if (gPet.state() == PetState::DEAD) {
                gPet.reset();
                gPet.save();
                gDisplay.showSplash();
                delay(SPLASH_MS);
            }
            break;
        default:
            break;
    }
}
