# 📋 PhantomFox Mk.I Changelog

All notable changes to the PhantomFox Mk.I firmware will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [v2.1.0] - 2025-07-20 (phantomFoxMkIV2.ino)

### 🚀 Added
- **Penetration Testing Suite**: Complete security auditing framework
  - Deauthentication attack capabilities with target scanning
  - Captive portal for credential harvesting
  - Real-time attack monitoring with packet counters
- **Advanced System Monitoring**: Enhanced system metrics and monitoring
  - Real-time performance monitoring with memory usage tracking
  - Storage analyzer for flash and SD card usage
  - Network information display with signal strength
  - System uptime tracking and CPU frequency monitoring
- **Web Server Integration**: HTTP server capabilities for captive portal
  - DNS server for traffic redirection
  - Credential logging to SD card with timestamps
  - Web-based login interface for social engineering attacks
- **Enhanced Menu System**: Improved navigation and user interface
  - Status indicators for attack modes ([ATK], [CPT], [RDY])
  - Menu validation with implementation status tracking
  - Streamlined navigation with better state management
- **Code Quality Improvements**: Clean, maintainable codebase
  - Consistent tab-based indentation throughout
  - Removed unnecessary comments for cleaner code
  - Improved error handling and I2C recovery mechanisms

### 🔧 Changed
- **Firmware Version**: Updated from v1.2.0 to v2.1.0
- **Menu Structure**: Reorganized into specialized categories
  - System Monitor (System Info, Performance, Storage)
  - Network Tools (WiFi Scanner, Captive Portal)
  - Pentest Suite (Deauth Attack)
- **Display Header**: Added real-time status indicators
- **Screen Timeout**: Increased from 30 seconds to 60 seconds
- **Memory Management**: Improved heap monitoring and metrics collection
- **WiFi Scanner**: Enhanced with deauth attack integration

### 🏗️ Technical Changes
- **New Dependencies**: Added WebServer and DNSServer libraries
- **Configuration Constants**: Added captive portal configuration
- **State Management**: Extended menu states for new features
- **Data Structures**: Added SystemMetrics structure for monitoring
- **Display Functions**: Enhanced with status message system

### 🔒 Security Features
- **Ethical Use Notice**: All penetration testing features are for authorized testing only
- **Logging Capabilities**: Comprehensive attack result logging
- **Network Isolation**: Proper WiFi mode management

---

## [v1.2.0] - Previous Release (phantomFoxMkIV1.ino)

### 🚀 Features
- **Core Menu System**: Basic OLED-based navigation interface
  - 2x2 keypad navigation (Up, Down, Left, Right)
  - Scrollable menu system with visual indicators
- **File Explorer**: SD card file management capabilities
  - File listing and navigation
  - Basic file operations support
- **WiFi Scanner**: Network discovery and monitoring
  - SSID detection with signal strength
  - Real-time network scanning
- **System Information**: Basic device status display
  - Firmware version display
  - Basic system metrics
- **Logging System**: File-based logging capabilities
  - SD card integration for data storage
  - EEPROM support for configuration

### 🔧 Technical Specifications
- **Hardware Support**: ESP32 with SSD1306 OLED display
- **Storage**: SD card via SPI interface
- **Display**: 128x64 OLED with I2C communication
- **Input**: 2x2 keypad matrix for navigation
- **Screen Timeout**: 30-second auto-sleep

### 📁 Menu Structure (v1.2.0)
```
Main Menu
├── File Explorer
├── Payload Launcher (WIP)
├── Logs Viewer
├── Settings Menu
├── WiFi Scanner
└── System Info
```

---

## 🔄 Version Comparison

| Feature | v1.2.0 | v2.1.0 |
|---------|--------|--------|
| **Basic Menu System** | ✅ | ✅ |
| **File Explorer** | ✅ | ❌ (Removed) |
| **WiFi Scanner** | ✅ | ✅ (Enhanced) |
| **System Info** | Basic | ✅ Advanced |
| **Penetration Testing** | ❌ | ✅ Full Suite |
| **Captive Portal** | ❌ | ✅ |
| **Deauth Attacks** | ❌ | ✅ |
| **Performance Monitor** | ❌ | ✅ |
| **Web Server** | ❌ | ✅ |
| **Real-time Monitoring** | ❌ | ✅ |
| **Attack Status Display** | ❌ | ✅ |
| **Screen Timeout** | 30s | 60s |
| **Code Quality** | Standard | ✅ Cleaned |

---

## 🛠️ Migration Guide

### From v1.2.0 to v2.1.0

**⚠️ Breaking Changes:**
- File Explorer functionality has been removed in v2.1.0
- Menu structure has been completely reorganized
- Some basic functions replaced with advanced security tools

**📦 Required Dependencies:**
```cpp
// Additional libraries needed for v2.1.0
#include <WebServer.h>
#include <DNSServer.h>
```

**🔧 Hardware Compatibility:**
- Same hardware requirements as v1.2.0
- No additional components needed
- Existing wiring configuration remains unchanged

**⚙️ Configuration Updates:**
- Update firmware version references
- Review new menu navigation paths
- Configure captive portal settings if needed

---

## 🚨 Legal Notice

**IMPORTANT**: The penetration testing features in v2.1.0 are intended for:
- ✅ Authorized security testing
- ✅ Educational purposes
- ✅ Your own networks
- ✅ Controlled lab environments

**NEVER** use these tools on networks you don't own or without explicit permission. Unauthorized network attacks are illegal in most jurisdictions.

---

## 🤝 Contributing

When contributing to this project, please:
1. Document all changes in this changelog
2. Follow the existing code style and formatting
3. Test thoroughly on actual hardware
4. Update version numbers appropriately
5. Consider security and legal implications

---

## 📞 Support

For questions about specific versions or migration assistance:
- Check the README.md for general setup instructions
- Review the code comments for technical details
- Test changes on non-production hardware first

---

*Last Updated: July 20, 2025*
