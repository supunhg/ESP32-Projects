# 🦊 PhantomFox Mk.I

A modular ESP32-based cybersecurity multitool inspired by the Flipper Zero. Designed for wireless auditing, local recon, payload deployment, and file interaction — all via OLED interface and keypad navigation.

---

## 🚀 Features

- 📁 File Explorer with SD card support
- 📡 Wi-Fi Scanner with live logging
- 📋 System stats with uptime and usage
- 🔐 Payload Launcher *(coming soon)*
- 🧠 Custom UI navigation via keypad
- 💾 Auto-log saving and file explorer
- 📦 Compact OLED CLI and scrollable UI

---

## 🖼️ Images

> OLED UI Demo

(Coming soon)

> Wiring Diagram

(Coming soon)

---

## ⚙️ Hardware Requirements

| Module           | Specs / Example                    |
|------------------|------------------------------------|
| ESP32            | NodeMCU ESP32 (Chipset ESP32-WROOM-32) |
| OLED Display     | SSD1306 128x64 I2C                 |
| SD Card Module   | SPI via CS=5, SCK=18, MOSI=23, MISO=19 |
| Keypad Module    | 4x4 Keypad (For further advancements) |
| Power Supply     | 5V via USB or battery regulator    |

---

## 📦 Installation

1. Clone this repo
2. Open in **Arduino IDE** or **PlatformIO**
3. Select board: `ESP32 Dev Module`
4. Install dependencies:
    - Adafruit_GFX
    - Adafruit_SSD1306
    - Keypad
    - SD
5. Flash to device

---

## 🔓 Roadmap

- [x] OLED Menu System
- [x] File Explorer
- [x] Wi-Fi Scanner & Logger
- [ ] Payload Launcher (Deauther, RF)
- [ ] Settings Config
- [ ] Log Viewer
- [ ] AP Mode Web Interface (planned)

---

## ⭐ Showcase

If you build your own PhantomFox Mk.I, submit a Pull Request with your build photos, improvements, or payloads!

