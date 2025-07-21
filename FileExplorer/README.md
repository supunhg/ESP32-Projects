# FileExplorer - ESP32 SD Card File Browser 📁

A simple yet effective SD card file browser for ESP32 with OLED display and keypad navigation. Perfect for viewing files stored on an SD card using a minimal hardware setup.

## 🎯 Project Overview

This project demonstrates how to create a basic file browser interface using an ESP32, OLED display, and a simple 2x2 keypad. It's an excellent starting point for projects that need file management capabilities or educational purposes to understand SD card interfacing with ESP32.

## 🛠️ Hardware Requirements

### Core Components
- **ESP32 Development Board** (any variant with sufficient GPIO pins)
- **128x64 OLED Display** (SSD1306, I2C interface)
- **2x2 Keypad Matrix** (or 4 individual push buttons)
- **MicroSD Card Module** (SPI interface)
- **MicroSD Card** (formatted as FAT32)

### Optional Components
- **Breadboard** and **Jumper Wires** for prototyping
- **Pull-up Resistors** (10kΩ) if keypad doesn't have internal pull-ups

## 🔌 Wiring Diagram

### I2C OLED Display (SSD1306)
| OLED Pin | ESP32 Pin | Description |
|----------|-----------|-------------|
| VCC      | 3.3V      | Power supply |
| GND      | GND       | Ground |
| SDA      | GPIO 21   | I2C Data |
| SCL      | GPIO 22   | I2C Clock |

### 2x2 Keypad Matrix
| Keypad | ESP32 Pin | Function |
|--------|-----------|----------|
| Row 1  | GPIO 26   | Up/Down row |
| Row 2  | GPIO 27   | Left/Right row |
| Col 1  | GPIO 32   | Up/Left column |
| Col 2  | GPIO 33   | Down/Right column |

**Keypad Layout:**
```
[U] [D]
[L] [R]
```

### MicroSD Card Module (SPI)
| SD Module Pin | ESP32 Pin | Description |
|---------------|-----------|-------------|
| VCC           | 3.3V      | Power supply |
| GND           | GND       | Ground |
| CS            | GPIO 5    | Chip Select |
| MOSI          | GPIO 23   | Master Out Slave In |
| MISO          | GPIO 19   | Master In Slave Out |
| SCK           | GPIO 18   | Serial Clock |

## 🎮 Navigation Controls

| Key | Function |
|-----|----------|
| **U** | Navigate up in file list |
| **D** | Navigate down in file list |
| **R** | Open/view selected file |
| **L** | Back to file list (when viewing file) |

## 🚀 Getting Started

### 1. Hardware Setup
1. Connect all components according to the wiring diagram above
2. Insert a formatted MicroSD card (FAT32) into the SD card module
3. Add some text files to the SD card for testing

### 2. Software Installation
1. **Install Arduino IDE** with ESP32 board support
2. **Install Required Libraries:**
   ```
   - Adafruit SSD1306 (by Adafruit)
   - Adafruit GFX Library (by Adafruit)
   - Keypad (by Mark Stanley, Alexander Brevig)
   ```
3. **Add ESP32 Board Support:**
   - File → Preferences → Additional Board Manager URLs
   - Add: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Tools → Board → Boards Manager → Search "ESP32" → Install

### 3. Upload and Run
1. Open `FileExplorer.ino` in Arduino IDE
2. Select your ESP32 board: Tools → Board → ESP32 Dev Module
3. Select the correct COM port: Tools → Port
4. Upload the sketch to your ESP32
5. Open Serial Monitor (115200 baud) to see debug messages

## 💾 Supported File Types

The file browser can display any text-based files, including:
- `.txt` - Text files
- `.log` - Log files
- `.csv` - Comma-separated values
- `.json` - JSON configuration files
- `.md` - Markdown files
- Any other text-based file format

**Note:** Binary files may display garbled text but won't damage the system.

## 📱 User Interface

### File List View
```
> filename1.txt
  filename2.log
  config.json
  data.csv
```
- Files are displayed 4 at a time
- `>` indicates the currently selected file
- Use U/D keys to navigate
- Press R to open the selected file

### File Content View
```
File contents are displayed
line by line on the OLED.
Up to 8 lines can be shown
at once due to display size.

Use L key to go back to
the file list.
```

## 🔧 Customization Options

### Modify Display Settings
```cpp
// Change number of files shown per page (max 8 due to display size)
// In displayFiles() function, change the loop condition:
for (int i = 0; i < 6 && (offset + i) < fileCount; i++) // Shows 6 files
```

### Change Maximum File Limit
```cpp
// Increase or decrease max files to scan
#define MAX_FILES 100  // Default is 50
```

### Modify Keypad Layout
```cpp
// Change key assignments in the keys array:
char keys[ROWS][COLS] = {
    {'1', '2'},  // Custom key mapping
    {'3', '4'}
};
```

## 🐛 Troubleshooting

### Common Issues

#### OLED Display Not Working
- **Check I2C Address:** Default is 0x3C, some displays use 0x3D
- **Verify Wiring:** Ensure SDA/SCL are connected to correct pins
- **Power Supply:** Make sure display gets stable 3.3V

#### SD Card Not Detected
- **Format:** Ensure SD card is formatted as FAT32
- **Size Limit:** Use cards 32GB or smaller for best compatibility
- **Wiring:** Double-check SPI connections
- **Power:** SD modules can be power-hungry, ensure adequate supply

#### Keypad Not Responsive
- **Debouncing:** If keys register multiple times, add delay
- **Pull-up Resistors:** Some keypads need external 10kΩ pull-ups
- **Wiring:** Verify row/column connections

#### Files Not Displaying
- **File Format:** Ensure files are text-based
- **File Names:** Avoid special characters in file names
- **Directory:** Files must be in root directory (/) of SD card

### Debug Tips
1. **Serial Monitor:** Check Serial Monitor for error messages
2. **Test Components:** Test OLED, keypad, and SD card separately
3. **Simple Files:** Start with simple .txt files for testing

## 📈 Potential Enhancements

### Easy Additions
- **File Size Display:** Show file sizes next to names
- **File Type Icons:** Add simple icons for different file types
- **Timestamp Display:** Show file creation/modification dates
- **File Count:** Display total number of files

### Advanced Features
- **Directory Navigation:** Support for subdirectories
- **File Operations:** Delete, rename, copy files
- **File Search:** Search for files by name
- **Scroll in Files:** Navigate through long files
- **Multiple File Types:** Support for images, audio (with additional hardware)

## 📚 Educational Value

This project teaches:
- **SD Card Interfacing:** SPI communication with storage devices
- **OLED Display Control:** I2C communication and graphics
- **Keypad Input Handling:** Matrix scanning and debouncing
- **File System Operations:** Reading files and directories
- **User Interface Design:** Creating navigable menus
- **Memory Management:** Handling file lists and buffers

## 🔗 Related Projects

This FileExplorer is part of the **ESP32 Projects Library**. Check out other projects:
- **[PhantomFox.MkI](../PhantomFox.MkI/)** - Advanced penetration testing device
- **[Main Repository](../)** - Complete ESP32 project collection

## 🤝 Contributing

Contributions are welcome! Areas for improvement:
- **Code Optimization:** Memory usage improvements
- **Feature Additions:** New functionality
- **Bug Fixes:** Issue reports and fixes
- **Documentation:** Better examples and guides
- **Hardware Compatibility:** Testing with different modules

## 📄 License

This project is part of the ESP32 Projects Library and is provided for educational and personal use.

## 📞 Support

For questions or issues:
- **Check Troubleshooting:** Review common issues above
- **Hardware Testing:** Verify connections and components
- **GitHub Issues:** Report bugs or request features
- **Community:** Join ESP32 development discussions

---

**Happy File Browsing!** 📁⚡

*Part of the ESP32 Projects Library - Making file management simple and accessible.*
