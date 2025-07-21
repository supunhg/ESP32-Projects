# PhantomFox Mk.I - ESP32 Penetration Testing Device

## Overview
PhantomFox Mk.I is a portable penetration testing device built on the ESP32 platform. It combines multiple security testing capabilities into a single handheld device with an OLED display and keypad interface.

## 🛠️ Hardware Requirements

### Core Components
- **ESP32 Development Board** (any variant with sufficient GPIO pins)
- **128x64 OLED Display** (SSD1306, I2C interface)
- **2x2 Keypad** (directional navigation: Up, Down, Left, Right)
- **MicroSD Card Module** (SPI interface)
- **MicroSD Card** (for data storage)

### Wiring Configuration

#### I2C Display (SSD1306)
- **SDA** → GPIO 21
- **SCL** → GPIO 22
- **VCC** → 3.3V
- **GND** → Ground

#### 2x2 Keypad Matrix
- **Row 1** → GPIO 26
- **Row 2** → GPIO 27
- **Col 1** → GPIO 32
- **Col 2** → GPIO 33

#### MicroSD Module (SPI)
- **CS** → GPIO 5
- **MOSI** → GPIO 23
- **MISO** → GPIO 19
- **SCK** → GPIO 18
- **VCC** → 3.3V
- **GND** → Ground

## 🔧 Software Features

### System Monitoring
- **System Information**: View firmware version, CPU frequency, and system uptime
- **Performance Monitor**: Real-time RAM usage tracking
- **Storage Analyzer**: Monitor flash memory and SD card usage

### Network Tools
- **WiFi Scanner**: Scan and analyze nearby wireless networks
- **Network Analysis**: View SSID, signal strength, and channel information

### Penetration Testing Suite
- **Evil Twin Attack**: Create fake access points for credential harvesting
- **Deauthentication Attack**: Disconnect clients from target networks
- **Handshake Capture**: Capture WPA/WPA2 handshakes for offline analysis
- **PCAP File Generation**: Save captured packets in standard format

### File Management
- **File Explorer**: Browse SD card contents with directory navigation
- **Log Viewer**: View captured credentials and attack logs
- **Data Storage**: Organized storage of handshakes and logs

## 📁 Firmware Versions

For detailed version history, migration guides, and complete changelog, see: **[CHANGELOG.md](CHANGELOG.md)**

### Quick Overview

### v1.2.0 - Basic Framework
- Basic OLED display and keypad functionality
- Simple menu system implementation
- Initial system monitoring features

### v2.1.0 - Network Capabilities
- WiFi scanning and network detection
- Basic penetration testing tools
- File system integration

### v2.3.0 - Advanced Features (Current)
- Complete file explorer with SD card support
- Evil Twin attack with credential capture
- Handshake capture and PCAP generation
- Enhanced user interface and navigation
- Robust error handling and display recovery

## 🚀 Getting Started

### 1. Hardware Assembly
1. Connect all components according to the wiring diagram
2. Insert a formatted MicroSD card
3. Power the ESP32 via USB or external power supply

### 2. Software Installation
1. Install Arduino IDE with ESP32 board support
2. Install required libraries:
   ```
   - Adafruit SSD1306
   - Adafruit GFX Library
   - Keypad Library
   - SD Library (built-in)
   - WiFi Library (built-in)
   ```
3. Upload the latest firmware (`phantomFoxMkIV3.ino`)

### 3. Operation
- **Navigation**: Use keypad arrows (U/D/L/R) to navigate menus
- **Selection**: Right arrow to select/enter
- **Back**: Left arrow to go back/exit
- **Auto Sleep**: Display turns off after 60 seconds of inactivity

## 📊 Menu Structure

```
Main Menu
├── System Monitor
│   ├── System Info
│   ├── Performance
│   └── Storage
├── Network Tools
│   └── WiFi Scanner
│       └── Network Actions
│           ├── Deauth Attack
│           └── Evil Twin
├── Pentest Suite
│   └── Evil Twin
└── File Explorer
    ├── Browse Files
    └── View Logs
```

## 💾 Data Storage

### File Organization
```
/
├── captured.txt          # Harvested credentials
├── handshakes/           # WPA handshake files
│   ├── [BSSID].pcap     # Raw packet captures
│   └── [BSSID].hccapx   # Converted for hashcat
└── logs/                # System and attack logs
```

### Captured Data Format
```
Network: [Target SSID]
Password: [Captured Password]
Time: [Timestamp]
----------------
```

## ⚠️ Legal Disclaimer

**IMPORTANT**: This device is designed for authorized penetration testing and educational purposes only. 

- Only use on networks you own or have explicit permission to test
- Unauthorized access to computer networks is illegal in most jurisdictions
- Users are responsible for compliance with local laws and regulations
- The developers assume no liability for misuse of this software

## 🛡️ Security Research Applications

### Authorized Use Cases
- **Penetration Testing**: Assess wireless network security for clients
- **Security Education**: Demonstrate wireless attack vectors in training
- **Red Team Exercises**: Simulate realistic attack scenarios
- **Personal Networks**: Test security of your own wireless infrastructure

### Best Practices
- Always obtain written authorization before testing
- Document all testing activities
- Follow responsible disclosure for discovered vulnerabilities
- Use in controlled, isolated environments when possible

## 🔧 Troubleshooting

### Common Issues

#### Display Not Working
- Check I2C connections (SDA/SCL)
- Verify display address (0x3C)
- Try display recovery function (automatic on startup)

#### SD Card Not Detected
- Ensure proper SPI wiring
- Format card as FAT32
- Check SD card compatibility (up to 32GB recommended)

#### Keypad Not Responsive
- Verify keypad matrix wiring
- Check for proper pull-up resistors
- Test individual key connections

#### WiFi Issues
- Ensure ESP32 has adequate power supply
- Check antenna connection
- Verify regulatory domain compliance

## 📈 Future Development

### Planned Features
- **Bluetooth Analysis**: BLE scanning and testing capabilities
- **RFID/NFC Integration**: Additional wireless protocol testing
- **Web Interface**: Remote control via built-in web server
- **Advanced Logging**: Enhanced data analysis and reporting
- **Custom Payloads**: User-defined attack scripts

### Hardware Upgrades
- **Larger Display**: 128x64 → 256x64 for more information
- **Additional Sensors**: GPS, accelerometer for location tracking
- **External Antenna**: Improved range and sensitivity
- **Battery Management**: Built-in charging and power monitoring

## 🤝 Contributing

Contributions are welcome! Please consider:
- Bug reports and feature requests
- Code improvements and optimizations
- Documentation updates
- Hardware compatibility testing
- Security vulnerability reports

## 🔗 Related Projects

Part of the **ESP32 Projects Library** - check out other projects in this repository for more ESP32 development resources and examples.

---

**Happy Hacking!** 🦊⚡
