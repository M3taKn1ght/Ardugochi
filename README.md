# Ardugochi

A Tamagotchi-style virtual pet for the **Wemos D1 Mini (ESP8266)** with a 128×64 SSD1306 OLED display.

## Hardware

| Component | Pin / Interface |
|-----------|-----------------|
| Wemos D1 Mini | — |
| SSD1306 128×64 OLED | I2C (SDA=GPIO4/D2, SCL=GPIO5/D1), addr `0x3C` |
| Feed button | GPIO14 (D5) |
| Play button | GPIO12 (D6) |
| Status / Reset button | GPIO13 (D7) |

**Note:** GPIO0 (D3) is intentionally avoided — it forces flash mode on boot.

## Build (PlatformIO)

```bash
cd Ardugochi
pio run
pio run --target upload
```

Or use VS Code with the PlatformIO extension.

## Dependencies (managed by PlatformIO)

- `adafruit/Adafruit SSD1306`
- `adafruit/Adafruit GFX Library`

## Controls

| Button | Short press | Long press (≥3 s) |
|--------|-------------|-------------------|
| D5 (Feed) | Feed the pet | — |
| D6 (Play) | Play with pet | — |
| D7 (Status) | Toggle info screen | **Reset** (only when dead) |

## Pet States

| State | Trigger | Behaviour |
|-------|---------|-----------|
| **IDLE** | Default | Normal decay, standard sprite |
| **EATING** | `feed()` | Single-tick animation, then back to NORMAL |
| **PLAYING** | `play()` | Single-tick animation, then back to NORMAL |
| **ANGRY** | Hunger > 80 **or** Health < 30 | Pet is neglected; clears after next interaction |
|| **SLEEPING** | Happiness > 80 **and** Hunger < 20 | Slower hunger, +happiness, +health; wakes at Hunger ≥ 50 **or when play() is called (grumpy wake-up, Happiness drops to 30–60%)** |
| **DEAD** | Health reaches 0 | Game over — long-press D7 to reset |

### Sleeping Rules

- While sleeping and **Hunger < 50**: `feed()` and `play()` calls are **ignored**
- Sleeping pet regenerates slowly (happiness and health rise)
- Cat wakes up automatically when hunger reaches 50

## Stats

| Stat | Range | Critical |
|------|-------|----------|
| Hunger | 0 – 100 | > 80 |
| Happiness | 0 – 100 | < 30 |
| Health | 0 – 100 | < 30 |
| Age | 0 – ∞ (ticks / days) | — |

**Tick interval:** 10 seconds real time.  
**One in-game day:** 6 ticks ≈ 1 minute.

## Display

### Main Screen
```
┌─────────────────────────────┐
│ Tag 12          ★           │  ← Status line (age + wellbeing icon)
│                             │
│        [48×48 sprite]       │  ← Pet state sprite
│                             │
│ HP ████████░░░░░░░░         │  ← Health bar
└─────────────────────────────┘
```

### Info Screen
Shows four stat bars with icons:
- Heart → Health (hatched pattern when critical)
- Food → Hunger (hatched pattern when critical)
- Star → Happiness (hatched pattern when critical)
- Clock → Age in ticks

Critical bars use a **striped / hatched fill** instead of solid white.

### Sprites

Hand-crafted 48×48 pixel-art sprites:
- **IDLE** — big round eyes, upright ears, whiskers
- **EAT** — open mouth chewing
- **PLAY** — happy ^^ eyes
- **ANGRY** — slanted brows, flat ears
- **SLEEP** — closed eyes with animated "zZ" float
- **DEAD** — X eyes, droopy ears

## EEPROM Persistence

- Auto-save every **60 seconds** (wear-aware)
- Saved: hunger, happiness, health, age, current state
- Magic byte + version header for forward compatibility

## Reset / New Game

If the pet dies, **hold the STATUS button (D7) for 3 seconds** to perform a "paperclip reset". This clears the EEPROM and starts a new pet.

**Important:** Reset only works when the pet is **DEAD**. Living pets cannot be reset.

## Project Structure

```
Ardugochi/
├── platformio.ini      # D1 Mini target, C++17, library deps
├── src/
│   └── main.cpp        # setup/loop, input dispatch, reset handler
└── include/
    ├── config.h        # Pins, timing, stat bounds, screen dimensions
    ├── display.h       # SSD1306 render manager (sprites, bars, status)
    ├── input.h         # Button debounce + long-press detection
    ├── pet.h           # Core game logic + state machine + EEPROM save/load
    ├── sprites.h       # 48×48 monochrome PROGMEM bitmaps (6 states)
    └── icons.h         # 8×8 stat-bar icons (heart, food, star, clock)
```

## License

This project is licensed under the **Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License** (CC BY-NC-SA 4.0).

You are free to share and adapt this work for non-commercial purposes.
