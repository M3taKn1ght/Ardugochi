// Ardugochi — pet core logic declarations
// SPDX-License-Identifier: MIT
#ifndef ARDUGOCHI_PET_H
#define ARDUGOCHI_PET_H

#include <Arduino.h>
#include "config.h"

// ---------------------------------------------------------------------------
// EEPROM layout (versioned so we can evolve it later)
// ---------------------------------------------------------------------------
extern const uint8_t EEPROM_HEADER;
extern const uint8_t EEPROM_VERSION;
extern const uint16_t EEPROM_START;
extern const uint16_t EEPROM_SIZE;

// ---------------------------------------------------------------------------
// Pet states
// ---------------------------------------------------------------------------
enum class PetState : uint8_t {
    NORMAL = 0,
    SLEEPING,
    DEAD,
    EATING,
    PLAYING,
    ANGRY
};

// ---------------------------------------------------------------------------
// Pet class
// ---------------------------------------------------------------------------
class Pet {
public:
    void reset();
    void tick();
    void feed();
    void play();
    void toggleInfoScreen();

    bool load();
    void save() const;

    // Getters
    uint8_t hunger()     const;
    uint8_t happiness()  const;
    uint8_t health()     const;
    uint8_t discipline() const;
    uint16_t ageTicks()  const;
    PetState state()     const;
    bool infoScreen()    const;

private:
    uint8_t  mHunger     = 0;
    uint8_t  mHappiness  = 0;
    uint8_t  mHealth     = 0;
    uint8_t  mDiscipline = 0;
    uint16_t mAgeTicks   = 0;
    uint8_t  mTickCounter = 0;
    PetState mState      = PetState::NORMAL;
    bool     mInfoScreen = false;
};

#endif // ARDUGOCHI_PET_H
