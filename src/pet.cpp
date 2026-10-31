// Ardugochi — pet core logic implementation
// SPDX-License-Identifier: MIT
#include "pet.h"
#include <EEPROM.h>

// ---------------------------------------------------------------------------
// EEPROM constants
// ---------------------------------------------------------------------------
const uint8_t EEPROM_HEADER  = 0xAD;
const uint8_t EEPROM_VERSION = 0x01;
const uint16_t EEPROM_START  = 0x00;
const uint16_t EEPROM_SIZE   = 64;

// ---------------------------------------------------------------------------
// Public methods
// ---------------------------------------------------------------------------
void Pet::reset() {
    mHunger     = 50;
    mHappiness  = 50;
    mHealth     = 80;
    mDiscipline = 50;
    mAgeTicks   = 0;
    mTickCounter = 0;
    mState       = PetState::NORMAL;
    mInfoScreen  = false;
}

void Pet::tick() {
    if (mState == PetState::DEAD) return;

    // ── SLEEPING STATE ──────────────────────────────────────────
    if (mState == PetState::SLEEPING) {
        if (mHunger < STAT_MAX) ++mHunger;
        if (mHappiness < STAT_MAX) ++mHappiness;
        if (mHealth < 100) ++mHealth;

        if (++mTickCounter >= 8) {
            mTickCounter = 0;
            ++mAgeTicks;
        }

        if (mHunger >= 50) {
            mState = PetState::NORMAL;
        }
        return;
    }

    // ── NORMAL DECAY ────────────────────────────────────────────
    if (mHunger < STAT_MAX) ++mHunger;
    if (mHappiness > STAT_MIN) --mHappiness;

    if (mHunger > 80 || mHappiness < 20) {
        if (mHealth > STAT_MIN) --mHealth;
    } else if (mHunger < 50 && mHappiness > 50) {
        if (mHealth < 100) ++mHealth;
    }

    if (++mTickCounter >= 8) {
        mTickCounter = 0;
        ++mAgeTicks;
    }

    if (mHealth == 0) {
        mState = PetState::DEAD;
        return;
    }

    // ── CLEAR TRANSIENT STATES ─────────────────────────────────
    if (mState == PetState::EATING || mState == PetState::PLAYING) {
        mState = PetState::NORMAL;
    }

    // ── STATE TRANSITIONS (from NORMAL) ────────────────────────
    if (mState == PetState::NORMAL && (mHunger > 80 || mHealth < 30 || mHappiness <= 50)) {
        mState = PetState::ANGRY;
    } else if (mState == PetState::NORMAL && mHappiness > 80 && mHunger < 20) {
        mState = PetState::SLEEPING;
    }
}

void Pet::feed() {
    if (mState == PetState::DEAD) return;
    if (mState == PetState::SLEEPING && mHunger < 50) return;

    if (mHunger >= 10) mHunger -= 10; else mHunger = STAT_MIN;
    mHappiness = constrain(mHappiness + 5, STAT_MIN, STAT_MAX);
    if (mHealth < 100) mHealth += 2;
    if (mHealth > 100) mHealth = 100;
    mState = PetState::EATING;
}

void Pet::play() {
    if (mState == PetState::DEAD) return;

    if (mState == PetState::SLEEPING) {
        mState = PetState::NORMAL;
        mHappiness = random(10, 50);
        if (mHunger < 80) mHunger += 5; else mHunger = STAT_MAX;
        return;
    }

    if (mHunger < 80) mHunger += 5; else mHunger = STAT_MAX;
    mHappiness = constrain(mHappiness + 15, STAT_MIN, STAT_MAX);
    mDiscipline = constrain(mDiscipline + 2, STAT_MIN, STAT_MAX);
    if (mHealth < 100) mHealth += 3;
    if (mHealth > 100) mHealth = 100;
    mState = PetState::PLAYING;
}

void Pet::toggleInfoScreen() {
    mInfoScreen = !mInfoScreen;
}

bool Pet::load() {
    EEPROM.begin(EEPROM_SIZE);
    uint8_t header = EEPROM.read(EEPROM_START + 0);
    uint8_t ver    = EEPROM.read(EEPROM_START + 1);
    if (header != EEPROM_HEADER || ver != EEPROM_VERSION) {
        EEPROM.end();
        return false;
    }

    mHunger     = EEPROM.read(EEPROM_START + 2);
    mHappiness  = EEPROM.read(EEPROM_START + 3);
    mHealth     = EEPROM.read(EEPROM_START + 4);
    mDiscipline = EEPROM.read(EEPROM_START + 5);
    mAgeTicks   = (uint16_t(EEPROM.read(EEPROM_START + 6)) << 8)
                | uint16_t(EEPROM.read(EEPROM_START + 7));
    mState      = static_cast<PetState>(EEPROM.read(EEPROM_START + 8));
    EEPROM.end();
    return true;
}

void Pet::save() const {
    EEPROM.begin(EEPROM_SIZE);
    uint16_t addr = EEPROM_START;
    EEPROM.write(addr++, EEPROM_HEADER);
    EEPROM.write(addr++, EEPROM_VERSION);
    EEPROM.write(addr++, mHunger);
    EEPROM.write(addr++, mHappiness);
    EEPROM.write(addr++, mHealth);
    EEPROM.write(addr++, mDiscipline);
    EEPROM.write(addr++, uint8_t(mAgeTicks >> 8));
    EEPROM.write(addr++, uint8_t(mAgeTicks & 0xFF));
    EEPROM.write(addr++, static_cast<uint8_t>(mState));
    EEPROM.commit();
    EEPROM.end();
}

// Getters
uint8_t Pet::hunger()     const { return mHunger; }
uint8_t Pet::happiness()  const { return mHappiness; }
uint8_t Pet::health()     const { return mHealth; }
uint8_t Pet::discipline() const { return mDiscipline; }
uint16_t Pet::ageTicks()  const { return mAgeTicks; }
PetState Pet::state()     const { return mState; }
bool Pet::infoScreen()    const { return mInfoScreen; }
