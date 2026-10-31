// Ardugochi — input manager implementation
// SPDX-License-Identifier: MIT
#include "input.h"

void InputManager::begin() const {
    pinMode(BTN_FEED_PIN, INPUT_PULLUP);
    pinMode(BTN_PLAY_PIN, INPUT_PULLUP);
    pinMode(BTN_STATUS_PIN, INPUT_PULLUP);
}

InputEvent InputManager::poll() {
    const unsigned long now = millis();
    InputEvent ev = InputEvent::NONE;

    if (checkButton(BTN_FEED_PIN, mLastFeed, mDebounceFeed, now))
        ev = InputEvent::FEED;
    else if (checkButton(BTN_PLAY_PIN, mLastPlay, mDebouncePlay, now))
        ev = InputEvent::PLAY;
    else if (checkButton(BTN_STATUS_PIN, mLastStatus, mDebounceStatus, now))
        ev = InputEvent::STATUS;

    // Long-press detection on STATUS → RESET
    static unsigned long statusPressTs = 0;
    static bool resetFired = false;
    static uint8_t prevStatusState = HIGH;
    uint8_t statusState = digitalRead(BTN_STATUS_PIN);

    if (statusState == LOW && prevStatusState == HIGH) {
        statusPressTs = now;
        resetFired = false;
    }

    if (statusState == LOW && prevStatusState == LOW) {
        if (!resetFired && (now - statusPressTs) >= RESET_HOLD_MS) {
            resetFired = true;
            prevStatusState = HIGH;
            return InputEvent::RESET;
        }
    }

    prevStatusState = statusState;
    return ev;
}

bool InputManager::checkButton(uint8_t pin, uint8_t& lastState,
                               unsigned long& debounceTs, unsigned long now) {
    uint8_t s = digitalRead(pin);
    if (s == LOW && lastState == HIGH) {
        if ((now - debounceTs) >= DEBOUNCE_MS) {
            debounceTs = now;
            lastState = LOW;
            return true;
        }
    }
    if (s == HIGH && lastState == LOW) {
        lastState = HIGH;
    }
    return false;
}
