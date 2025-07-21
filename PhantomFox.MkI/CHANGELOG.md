# 📋 PhantomFox Mk.I - Project Changelog

> **Current Version**: v2.3.0 | **Last Updated**: July 20, 2025  
> **Project**: ESP32-based Penetration Testing Device  
> **Hardware**: ESP32 + SSD1306 OLED + 2x2 Keypad + SD Card

---

## 📖 Overview

This changelog documents the evolution of the PhantomFox Mk.I firmware from its initial release (v1.2.0) through the current version (v2.3.0). Each version represents significant improvements in functionality, security capabilities, and code quality.

---

## [v2.3.0] - 2025-07-20 (phantomFoxMkIV3.ino)

### 🚀 Added
- **File Explorer System**: Complete SD card file management
  - Directory navigation with breadcrumb display
  - File and folder listing with visual indicators
  - Log file viewer with scrollable content
- **Evil Twin Attack**: Advanced WiFi credential harvesting
  - Fake access point creation with target network mimicking
  - Real-time credential capture and logging
  - Professional captive portal interface
- **Handshake Capture System**: WPA/WPA2 security analysis
  - Four-way handshake detection and capture
  - PCAP file generation for offline analysis
  - Automated handshake validation
- **Network Actions Menu**: Streamlined attack workflow
  - Direct access to attack tools from WiFi scanner
  - Target network information display
  - Attack status monitoring
- **Enhanced Display Management**: Improved user experience
  - Screen timeout with touch-to-wake functionality
  - Display recovery mechanisms for I2C issues
  - Status indicators and real-time feedback

### 🔧 Changed
- **Firmware Version**: Updated from v2.1.0 to v2.3.0
- **Menu Structure**: Reorganized for better workflow
  - File Explorer now accessible from main menu
  - Network Actions integrated with WiFi Scanner
  - Streamlined navigation between related functions
- **Code Quality**: Professional-grade improvements
  - Consistent tab-based indentation throughout
  - Humanized comments for better readability
  - Improved error handling and recovery mechanisms
- **File System**: Enhanced directory management
  - Automatic directory creation for logs and captures
  - Improved file handling with better error recovery
  - Organized storage structure for different data types

### 🏗️ Technical Improvements
- **Memory Management**: Optimized data structures
  - Efficient WiFi network storage (up to 20 networks)
  - Smart file entry management for directory listing
  - Improved string handling for large file paths
- **Error Handling**: Robust file operations
  - Better SD card error recovery
  - Improved display connection stability
  - Enhanced WiFi mode switching
- **Network Processing**: Advanced WiFi capabilities
  - Complete network information storage (SSID, BSSID, RSSI, Channel)
  - Improved network selection and targeting
  - Better attack coordination between modules

### 🛡️ Security Enhancements
- **Ethical Use Framework**: Enhanced responsible disclosure
  - Improved logging for security audit trails
  - Better attack documentation for authorized testing
  - Clear separation between testing and production modes

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

| Feature | v1.2.0 | v2.1.0 | v2.3.0 |
|---------|--------|--------|--------|
| **Basic Menu System** | ✅ | ✅ | ✅ |
| **File Explorer** | ✅ | ❌ (Removed) | ✅ Enhanced |
| **WiFi Scanner** | ✅ | ✅ (Enhanced) | ✅ Advanced |
| **System Info** | Basic | ✅ Advanced | ✅ Advanced |
| **Penetration Testing** | ❌ | ✅ Full Suite | ✅ Enhanced |
| **Captive Portal** | ❌ | ✅ | ❌ (Replaced) |
| **Evil Twin Attack** | ❌ | ❌ | ✅ |
| **Deauth Attacks** | ❌ | ✅ | ✅ Enhanced |
| **Handshake Capture** | ❌ | ❌ | ✅ |
| **Performance Monitor** | ❌ | ✅ | ✅ |
| **Web Server** | ❌ | ✅ | ✅ |
| **Real-time Monitoring** | ❌ | ✅ | ✅ |
| **Attack Status Display** | ❌ | ✅ | ✅ Enhanced |
| **Log File Viewer** | ❌ | ❌ | ✅ |
| **Network Actions Menu** | ❌ | ❌ | ✅ |
| **Directory Navigation** | ❌ | ❌ | ✅ |
| **Screen Timeout** | 30s | 60s | 60s |
| **Code Quality** | Standard | ✅ Cleaned | ✅ Professional |

---

## 🛠️ Migration Guide

### From v2.1.0 to v2.3.0

**✨ New Features:**
- File Explorer has been restored and enhanced
- Evil Twin attack replaces basic captive portal
- Handshake capture system for WPA/WPA2 testing
- Network Actions menu for streamlined attack workflow

**🔧 Configuration Updates:**
- No hardware changes required
- SD card directory structure automatically created
- Enhanced logging requires sufficient SD card space
- Attack results stored in organized directory structure

**⚙️ Usage Changes:**
- WiFi Scanner now leads to Network Actions menu
- File Explorer accessible from main menu
- Enhanced attack logging and result storage
- Improved navigation between attack modules

### From v1.2.0 to v2.3.0

**⚠️ Major Changes:**
- Complete menu system redesign
- Advanced penetration testing capabilities
- Enhanced file management system
- Professional security testing framework

**📦 Additional Dependencies:**
```cpp
// All libraries from v2.1.0 plus enhanced functionality
#include <WebServer.h>
#include <DNSServer.h>
```

**🔧 Hardware Compatibility:**
- Same hardware requirements as previous versions
- Enhanced SD card usage for comprehensive logging
- Improved memory management for complex operations

---

## 🚨 Legal Notice

**IMPORTANT**: The penetration testing features in PhantomFox Mk.I are intended for:
- ✅ Authorized security testing
- ✅ Educational purposes
- ✅ Your own networks
- ✅ Controlled lab environments

**NEVER** use these tools on networks you don't own or without explicit permission. Unauthorized network attacks are illegal in most jurisdictions.

---

## 🤝 Contributing

When contributing to PhantomFox Mk.I:
1. Document all changes in this changelog
2. Follow the existing code style and formatting
3. Test thoroughly on actual hardware
4. Update version numbers appropriately
5. Consider security and legal implications
6. Update the main project README with new features

---

## 📞 Support

For PhantomFox Mk.I specific questions:
- Check the project README.md for setup instructions
- Review the code comments for technical details
- Test changes on non-production hardware first
- Report issues through the main repository

---

*Last Updated: July 20, 2025 - PhantomFox Mk.I v2.3.0*
