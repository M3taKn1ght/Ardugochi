// Ardugochi — input manager declarations (3 buttons, active LOW with pull-ups)
// SPDX-License-Identifier: MIT
#ifndef ARDUGOCHI_INPUT_H
#define ARDUGOCHI_INPUT_H

#include <Arduino.h>
#include "config.h"

enum class InputEvent : uint8_t {
    NONE = 0,
    FEED,
    PLAY,
    STATUS,
    RESET   // long-press on STATUS (3s hold)
};

class InputManager {
public:
    void begin() const;
    InputEvent poll();

private:
    static constexpr uint16_t DEBOUNCE_MS    = 30;
    static constexpr uint16_t RESET_HOLD_MS  = 3000;

    uint8_t mLastFeed   = HIGH;
    uint8_t mLastPlay   = HIGH;
    uint8_t mLastStatus = HIGH;

    unsigned long mDebounceFeed   = 0;
    unsigned long mDebouncePlay   = 0;
    unsigned long mDebounceStatus = 0;

    bool checkButton(uint8_t pin, uint8_t& lastState,
                     unsigned long& debounceTs, unsigned long now);
};

#endif // ARDUGOCHI_INPUT_H
