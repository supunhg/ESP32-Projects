# 📋 ESP32 Projects Library - CHANGELOG

> **Current Version**: v3.1.0 | **Last Updated**: July 20, 2025  
> **Project**: Comprehensive ESP32 Development Library  
> **Structure**: Multi-Project Repository with Individual Project Folders

---

## 📖 Overview

This changelog documents the evolution of our ESP32 projects library, starting from the single PhantomFox project through the current multi-project library structure. This repository now serves as a comprehensive collection of ESP32 projects for learning, development, and practical applications.

---

## [v3.1.0] - 2025-07-20 - New Project Addition

### 📁 **Added FileExplorer Project**
- **New Standalone Project**: Created FileExplorer - SD card file browser
  - Simple file browser with OLED display and keypad navigation
  - Text file viewing capabilities
  - Basic file system operations
  - Complete documentation and setup guide

### 📚 **Documentation Enhancements**
- **FileExplorer README**: Comprehensive project documentation
  - Hardware requirements and wiring diagrams
  - Setup instructions and troubleshooting guide
  - Navigation controls and usage examples
- **Main Repository Updates**: Updated to reflect new project
  - Added FileExplorer to project structure
  - Updated featured projects section

### 🔧 **Code Quality**
- **Consistent Formatting**: Applied professional code standards
  - Tab-based indentation throughout
  - Clear function documentation
  - Proper header comments

---

## [v3.0.1] - 2025-07-20 - Repository Cleanup

### 🧹 Repository Optimization
- **Removed Placeholder Project**: Removed DasaiMochi project placeholder
  - Simplified repository structure to focus on completed projects
  - Updated all documentation to reflect current project status
  - Streamlined future project planning approach

### 📚 Documentation Updates
- **Main README**: Removed all placeholder project references
- **Changelog**: Updated to reflect current repository state
- **Project Structure**: Simplified to show only active projects

---

## [v3.0.0] - 2025-07-20 - Repository Restructuring

### 🏗️ Major Changes - Repository Transformation
- **Repository Restructure**: Transformed from single PhantomFox project to comprehensive ESP32 Projects Library
  - Created dedicated project folders for better organization
  - Established library-style structure for multiple ESP32 projects
  - Implemented comprehensive documentation system

### 📁 New Project Structure
```
ESP32 Projects Library/
├── PhantomFox.MkI/           # Advanced ESP32 Penetration Testing Suite
│   ├── phantomFoxMkIV1.ino   # v1.2.0 - Basic firmware
│   ├── phantomFoxMkIV2.ino   # v2.1.0 - Enhanced pentest suite
│   ├── phantomFoxMkIV3.ino   # v2.3.0 - Advanced capabilities
│   ├── README.md             # Project-specific documentation
│   └── CHANGELOG.md          # Project version history
├── FileExplorer/             # SD Card File Browser
│   ├── FileExplorer.ino      # Arduino sketch
│   └── README.md             # Project documentation
├── README.md                 # Main library documentation
└── CHANGELOG.md              # This changelog
```

### 🚀 Added Projects
- **PhantomFox.MkI**: Complete penetration testing device firmware
  - Professional ESP32-based security auditing tool
  - Advanced WiFi attack capabilities and network analysis
  - Comprehensive file system and SD card integration
  - Individual project documentation and changelog

### 📚 Documentation Enhancements
- **Main Repository README**: Comprehensive library overview
  - Project catalog with descriptions and capabilities
  - Hardware requirements and setup guides for all projects
  - Learning resources and educational pathways
- **Project-Specific Documentation**: Detailed documentation for PhantomFox.MkI
  - Individual setup instructions and requirements
  - Project-specific features and capabilities
  - Usage examples and troubleshooting guides
  - Dedicated project changelog for version history

### 🎯 Vision & Purpose
- **Educational Focus**: Comprehensive learning resource for ESP32 development
- **Project Quality**: Focus on well-documented, production-ready projects
- **Best Practices**: Professional code organization and documentation standards
- **Community Resource**: Open-source library for ESP32 enthusiasts

---

## 🔄 PhantomFox Project Integration

The PhantomFox Mk.I project has been successfully integrated into this ESP32 Projects Library. For detailed firmware version history, migration guides, and project-specific changes, please see:

📋 **[PhantomFox.MkI/CHANGELOG.md](PhantomFox.MkI/CHANGELOG.md)**

### Current PhantomFox Status
- **Latest Firmware**: v2.3.0 (phantomFoxMkIV3.ino)
- **Project Status**: Mature, fully functional penetration testing device
- **Documentation**: Complete with hardware setup and usage guides
- **Code Quality**: Professional-grade with consistent formatting and comments

---

## 📁 Future Projects

### Project Expansion Plans
The library structure is designed to accommodate additional ESP32 projects as they are developed. Future additions will follow the established pattern:
- Dedicated project folders with comprehensive documentation
- Individual project changelogs for version tracking
- Integration with main library documentation
- Professional code quality and organization standards

### Additional ESP32 Projects
Watch this space for new ESP32 projects covering various applications:
- IoT automation and smart home integration
- Sensor networks and data logging
- Communication protocol implementations
- Educational modules and learning resources

### 🚀 Added
- **File Explorer System**: Complete SD card file management
  - Directory navigation with breadcrumb display
  - File and folder listing with visual indicators
  - Log file viewer with scrollable content
  - Support for multiple file types and deep directory structures
- **Enhanced Evil Twin Attack**: Advanced wireless social engineering
  - Automatic SSID cloning from WiFi scanner
  - Improved credential harvesting interface
  - Real-time client connection monitoring
  - Enhanced logging with timestamps and network details
- **Advanced Network Actions Menu**: Streamlined attack workflow
  - Target selection from WiFi scanner results
  - Integrated deauth and evil twin attacks
  - Handshake capture with packet counting
  - Organized attack result storage
- **Handshake Capture System**: WPA/WPA2 security testing
  - Real-time handshake packet monitoring
  - Automatic handshake file generation
  - Visual progress indicators during capture
  - Organized storage in dedicated /handshakes directory

### 🔧 Changed
- **Firmware Version**: Updated from v2.1.0 to v2.3.0
- **Menu Architecture**: Enhanced navigation system
  - Added File Explorer to main menu
  - Improved network action workflow
  - Better state management between menus
- **Code Quality**: Professional formatting and structure
  - Consistent tab-based indentation throughout
  - Removed excessive comment headers for cleaner code
  - Improved variable naming and function organization
- **User Interface**: Enhanced visual feedback
  - Better selection highlighting in all menus
  - Improved status indicators ([ATK], [EVL], [CAP], [RDY])
  - More intuitive navigation controls
- **Attack Logging**: Comprehensive result tracking
  - Detailed deauth attack logs with target information
  - Enhanced credential capture logging
  - Automatic directory creation for organized storage

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

**IMPORTANT**: The penetration testing features in PhantomFox.MkI are intended for:
- ✅ Authorized security testing
- ✅ Educational purposes
- ✅ Your own networks
- ✅ Controlled lab environments

**NEVER** use these tools on networks you don't own or without explicit permission. Unauthorized network attacks are illegal in most jurisdictions.

---

## 🤝 Contributing

When contributing to the ESP32 Projects Library:
1. **Project Organization**: Follow the established folder structure
2. **Documentation**: Update both project-specific and main library documentation
3. **Code Quality**: Follow existing code style and formatting standards
4. **Testing**: Test thoroughly on actual hardware before submitting
5. **Legal Compliance**: Consider security and legal implications for all projects
6. **Changelog Updates**: Document all changes in this changelog
7. **Version Management**: Update version numbers appropriately

### Adding New Projects
1. Create project folder with descriptive name
2. Include comprehensive README.md with setup instructions
3. Follow hardware documentation standards
4. Update main library README with project information
5. Add entry to this changelog

---

## 📞 Support

For questions about the ESP32 Projects Library:
- **General Setup**: Check the main README.md for library overview
- **Project-Specific Issues**: Review individual project README files
- **PhantomFox Support**: Review the code comments for technical details
- **Hardware Testing**: Test changes on non-production hardware first
- **Migration Help**: Check version comparison tables above

### Project-Specific Support
- **PhantomFox.MkI**: Advanced penetration testing device support
- **FileExplorer**: SD card file browser with OLED display

---

*Last Updated: July 20, 2025 - ESP32 Projects Library v3.1.0*
