#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <SD.h>
#include <WiFi.h>
#include <SPI.h>
#include <EEPROM.h>

// Device configuration settings
#define FIRMWARE_VERSION "v1.2.0"
#define DEVICE_NAME "PhantomFox Mk.I"

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// Button keypad configuration
#define KEYPAD_ROWS 2
#define KEYPAD_COLS 2
byte ROW_PINS[KEYPAD_ROWS] = {26, 27};
byte COL_PINS[KEYPAD_COLS] = {32, 33};
char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
	{'U', 'D'},
	{'L', 'R'}
};

// SD card SPI pin assignments
#define SD_CS_PIN 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

// User interface settings
#define MAX_DISPLAY_ITEMS 4
#define SCROLL_DELAY 200
#define SCREEN_TIMEOUT 30000 // Screen turns off after 30 seconds

// Initialize hardware objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Keypad keypad = Keypad(makeKeymap(KEYPAD_KEYS), ROW_PINS, COL_PINS, KEYPAD_ROWS, KEYPAD_COLS);

// Menu system definitions
enum MenuState {
	MAIN_MENU,
	FILE_EXPLORER,
	PAYLOAD_LAUNCHER,
	LOGS_VIEWER,
	SETTINGS_MENU,
	WIFI_SCANNER,
	SYSTEM_INFO
};

struct MenuItem {
	String name;
	String icon;
	MenuState state;
};

MenuItem mainMenuItems[] = {
	{"File Explorer", "📁", FILE_EXPLORER},
	{"Payload Launcher", "🚀", PAYLOAD_LAUNCHER},
	{"WiFi Scanner", "📡", WIFI_SCANNER},
	{"Logs Viewer", "📋", LOGS_VIEWER},
	{"System Info", "ℹ️", SYSTEM_INFO},
	{"Settings", "⚙️", SETTINGS_MENU}
};

const int MAIN_MENU_SIZE = sizeof(mainMenuItems) / sizeof(mainMenuItems[0]);

// Global state variables
MenuState currentState = MAIN_MENU;
int selectedItem = 0;
int menuOffset = 0;
unsigned long lastActivity = 0;
bool screenOn = true;
String statusMessage = "";
unsigned long statusTimeout = 0;

// Track system statistics
struct SystemStats {
	unsigned long uptime;
	int wifiScansCount;
	int filesScanned;
	long sdCardSize;
	long sdCardUsed;
};

SystemStats stats = {0, 0, 0, 0, 0};

// Helper functions for system management
void resetActivity() {
	lastActivity = millis();
	if (!screenOn) {
		screenOn = true;
		display.ssd1306_command(SSD1306_DISPLAYON);
	}
}

void setStatus(String message, unsigned long timeout = 3000) {
	statusMessage = message;
	statusTimeout = millis() + timeout;
}

String formatFileSize(long bytes) {
	if (bytes < 1024) return String(bytes) + " B";
	else if (bytes < 1024 * 1024) return String(bytes / 1024) + " KB";
	else return String(bytes / (1024 * 1024)) + " MB";
}

String formatUptime(unsigned long ms) {
	unsigned long seconds = ms / 1000;
	unsigned long minutes = seconds / 60;
	unsigned long hours = minutes / 60;
	
	if (hours > 0) return String(hours) + "h " + String(minutes % 60) + "m";
	else if (minutes > 0) return String(minutes) + "m " + String(seconds % 60) + "s";
	else return String(seconds) + "s";
}

// Display rendering functions
void drawHeader() {
	display.setTextSize(1);
	display.setCursor(0, 0);
	display.print(DEVICE_NAME);
	
	// Show device status in top right
	display.setCursor(100, 0);
	display.print("[ON]");
	
	// Draw a line under the header
	display.drawLine(0, 10, SCREEN_WIDTH - 1, 10, SSD1306_WHITE);
}

void drawStatus() {
	if (statusMessage.length() > 0 && millis() < statusTimeout) {
		display.setCursor(0, 56);
		display.setTextSize(1);
		display.print(statusMessage);
	}
}

void drawMainMenu() {
	display.clearDisplay();
	drawHeader();
	
	display.setTextSize(1);
	int startY = 15;
	
	for (int i = 0; i < MAX_DISPLAY_ITEMS && (menuOffset + i) < MAIN_MENU_SIZE; i++) {
		int itemIndex = menuOffset + i;
		int y = startY + (i * 12);
		
		display.setCursor(0, y);
		
		if (itemIndex == selectedItem) {
			// Highlight the currently selected menu item
			display.fillRect(0, y - 1, SCREEN_WIDTH, 10, SSD1306_WHITE);
			display.setTextColor(SSD1306_BLACK);
			display.print(">");
		} else {
			display.setTextColor(SSD1306_WHITE);
			display.print(" ");
		}
		
		display.print(mainMenuItems[itemIndex].name);
		display.setTextColor(SSD1306_WHITE);
	}
	
	// Show scrollbar when there are more items than can fit on screen
	if (MAIN_MENU_SIZE > MAX_DISPLAY_ITEMS) {
		int scrollBarHeight = (MAX_DISPLAY_ITEMS * 40) / MAIN_MENU_SIZE;
		int scrollBarY = 15 + (menuOffset * 40) / MAIN_MENU_SIZE;
		display.fillRect(122, scrollBarY, 2, scrollBarHeight, SSD1306_WHITE);
	}
	
	drawStatus();
	display.display();
}

void drawLoadingScreen(String message) {
	display.clearDisplay();
	drawHeader();
	
	display.setTextSize(1);
	display.setCursor(0, 25);
	display.print(message);
	
	// Simple animated dots to show activity
	static int loadingDots = 0;
	for (int i = 0; i < (loadingDots % 4); i++) {
		display.print(".");
	}
	loadingDots++;
	
	display.display();
}

// WiFi network scanning functionality
void performWiFiScan() {
	setStatus("Initializing WiFi...", 2000);
	drawLoadingScreen("WiFi Scan");
	
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	
	setStatus("Scanning networks...", 0);
	drawLoadingScreen("Scanning Networks");
	
	int networkCount = WiFi.scanNetworks();
	stats.wifiScansCount++;
	
	if (networkCount == 0) {
		setStatus("No networks found", 3000);
		delay(3000);
		currentState = MAIN_MENU;
		return;
	}
	
	// Save scan results to SD card for later analysis
	String logFileName = "/wifi_scan_" + String(millis()) + ".txt";
	File logFile = SD.open(logFileName.c_str(), FILE_WRITE);
	if (logFile) {
		logFile.println("=== PhantomFox WiFi Scan ===");
		logFile.println("Timestamp: " + String(millis()));
		logFile.println("Networks found: " + String(networkCount));
		logFile.println("----------------------------");
	}

	int currentPage = 0;
	int totalPages = (networkCount + MAX_DISPLAY_ITEMS - 1) / MAX_DISPLAY_ITEMS;
	bool viewing = true;
	
	setStatus("Found " + String(networkCount) + " networks", 3000);
	
	while (viewing) {
		display.clearDisplay();
		drawHeader();
		
		display.setTextSize(1);
		display.setCursor(0, 15);
		display.print("WiFi Networks (");
		display.print(currentPage + 1);
		display.print("/");
		display.print(totalPages);
		display.println(")");
		
		for (int i = 0; i < MAX_DISPLAY_ITEMS; i++) {
			int networkIndex = currentPage * MAX_DISPLAY_ITEMS + i;
			if (networkIndex >= networkCount) break;
			
			int y = 27 + (i * 9);
			display.setCursor(0, y);
			
			String ssid = WiFi.SSID(networkIndex);
			int rssi = WiFi.RSSI(networkIndex);
			bool isOpen = (WiFi.encryptionType(networkIndex) == WIFI_AUTH_OPEN);
			
			// Shorten long network names to fit on screen
			if (ssid.length() > 10) {
				ssid = ssid.substring(0, 10) + "..";
			}
			
			display.print(ssid);
			display.print(" ");
			display.print(rssi);
			display.print("dB");
			if (!isOpen) display.print(" 🔒");
			
			// Record network details to log file
			if (logFile) {
				logFile.print("SSID: ");
				logFile.print(WiFi.SSID(networkIndex));
				logFile.print(", RSSI: ");
				logFile.print(rssi);
				logFile.print("dB, Security: ");
				logFile.println(isOpen ? "Open" : "Secured");
			}
		}
    
		display.setCursor(0, 56);
		display.print("U/D:Page L:Back R:Rescan");
		display.display();
		
		char key = keypad.waitForKey();
		resetActivity();
		
		switch (key) {
			case 'U':
				if (currentPage > 0) currentPage--;
				break;
			case 'D':
				if (currentPage < totalPages - 1) currentPage++;
				break;
			case 'L':
				viewing = false;
				break;
			case 'R':
				viewing = false;
				performWiFiScan(); // Start a fresh scan
				return;
		}
	}
	
	if (logFile) {
		logFile.println("--- End of Scan ---");
		logFile.close();
		setStatus("Log saved: " + logFileName, 3000);
	}
	
	WiFi.mode(WIFI_OFF);
	currentState = MAIN_MENU;
}

// File browsing functionality
void showFileExplorer() {
	display.clearDisplay();
	drawHeader();
	
	display.setTextSize(1);
	display.setCursor(0, 15);
	display.println("📁 File Explorer");
	
	File root = SD.open("/");
	if (!root || !root.isDirectory()) {
		display.setCursor(0, 30);
		display.println("SD Card Error!");
		display.display();
		delay(2000);
		currentState = MAIN_MENU;
		return;
	}
	
	int fileCount = 0;
	String files[20]; // Keep memory usage reasonable
	
	File file = root.openNextFile();
	while (file && fileCount < 20) {
		if (!file.isDirectory()) {
			files[fileCount] = String(file.name()) + " (" + formatFileSize(file.size()) + ")";
			fileCount++;
		}
		file.close();
		file = root.openNextFile();
	}
	root.close();

	stats.filesScanned = fileCount;
	
	if (fileCount == 0) {
		display.setCursor(0, 30);
		display.println("No files found");
		display.display();
		delay(2000);
		currentState = MAIN_MENU;
		return;
	}
	
	int selectedFile = 0;
	int fileOffset = 0;
	bool browsing = true;
	
	while (browsing) {
		display.clearDisplay();
		drawHeader();
		
		display.setTextSize(1);
		display.setCursor(0, 15);
		display.print("Files: ");
		display.println(fileCount);
		
		for (int i = 0; i < 3 && (fileOffset + i) < fileCount; i++) {
			int y = 27 + (i * 9);
			display.setCursor(0, y);
			
			if ((fileOffset + i) == selectedFile) {
				display.print("> ");
			} else {
				display.print("  ");
			}
			
			String fileName = files[fileOffset + i];
			if (fileName.length() > 18) {
				fileName = fileName.substring(0, 18) + "..";
			}
			display.println(fileName);
		}
		
		display.setCursor(0, 56);
		display.print("U/D:Nav L:Back");
		display.display();
		
		char key = keypad.waitForKey();
		resetActivity();
		
		switch (key) {
			case 'U':
				if (selectedFile > 0) {
					selectedFile--;
					if (selectedFile < fileOffset) fileOffset--;
				}
				break;
			case 'D':
				if (selectedFile < fileCount - 1) {
					selectedFile++;
					if (selectedFile > fileOffset + 2) fileOffset++;
				}
				break;
			case 'L':
				browsing = false;
				break;
		}
	}
	
	currentState = MAIN_MENU;
}

// System information display
void showSystemInfo() {
	display.clearDisplay();
	drawHeader();
	
	display.setTextSize(1);
	display.setCursor(0, 15);
	display.println("📊 System Info");
	
	display.setCursor(0, 27);
	display.print("Firmware: ");
	display.println(FIRMWARE_VERSION);
	
	display.setCursor(0, 36);
	display.print("Uptime: ");
	display.println(formatUptime(millis()));
	
	display.setCursor(0, 45);
	display.print("WiFi Scans: ");
	display.println(stats.wifiScansCount);
	
	display.setCursor(0, 56);
	display.print("L:Back");
	display.display();
	
	while (true) {
		char key = keypad.waitForKey();
		resetActivity();
		if (key == 'L') {
			currentState = MAIN_MENU;
			break;
		}
	}
}

// Handle navigation through the main menu
void handleMainMenuNavigation(char key) {
	switch (key) {
		case 'U':
			if (selectedItem > 0) {
				selectedItem--;
				if (selectedItem < menuOffset) menuOffset--;
			}
			break;
			
		case 'D':
			if (selectedItem < MAIN_MENU_SIZE - 1) {
				selectedItem++;
				if (selectedItem > menuOffset + MAX_DISPLAY_ITEMS - 1) menuOffset++;
			}
			break;
			
		case 'R':
			currentState = mainMenuItems[selectedItem].state;
			setStatus("Loading " + mainMenuItems[selectedItem].name + "...", 1000);
			break;
	}
}

// Main initialization and program loop
void setup() {
	Serial.begin(115200);
	Serial.println("Initializing " DEVICE_NAME " " FIRMWARE_VERSION);
	
	// Get the OLED display ready
	if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
		Serial.println("OLED initialization failed!");
		while (true) delay(1000);
	}
	
	display.clearDisplay();
	display.setTextSize(2);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0, 5);
	display.println("PhantomFox");
	display.setTextSize(1);
	display.setCursor(0, 25);
	display.println("Mk.I " FIRMWARE_VERSION);
	display.println("");
	display.println("Initializing...");
	display.display();
	delay(2000);
	
	// Set up SPI communication and SD card
	SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS_PIN);
	if (!SD.begin(SD_CS_PIN)) {
		display.clearDisplay();
		display.setCursor(0, 20);
		display.println("SD Card Failed!");
		display.println("Check connection");
		display.display();
		while (true) delay(1000);
	}
	
	// Get information about the SD card
	uint8_t cardType = SD.cardType();
	if (cardType != CARD_NONE) {
		stats.sdCardSize = SD.totalBytes() / (1024 * 1024);
		stats.sdCardUsed = SD.usedBytes() / (1024 * 1024);
	}

	display.clearDisplay();
	display.setCursor(0, 20);
	display.println("✓ OLED Ready");
	display.println("✓ SD Card Ready");
	display.println("✓ Keypad Ready");
	display.println("");
	display.println("System Online!");
	display.display();
	delay(2000);
	
	resetActivity();
	setStatus("Welcome to PhantomFox!", 3000);
	
	Serial.println("System initialization complete");
}

void loop() {
	// Turn off screen after period of inactivity
	if (screenOn && (millis() - lastActivity > SCREEN_TIMEOUT)) {
		screenOn = false;
		display.ssd1306_command(SSD1306_DISPLAYOFF);
	}
	
	char key = keypad.getKey();
	if (key) {
		resetActivity();
		
		switch (currentState) {
			case MAIN_MENU:
				handleMainMenuNavigation(key);
				break;
				
			case WIFI_SCANNER:
				performWiFiScan();
				break;
				
			case FILE_EXPLORER:
				showFileExplorer();
				break;
				
			case SYSTEM_INFO:
				showSystemInfo();
				break;
				
			case PAYLOAD_LAUNCHER:
				setStatus("Payload Launcher - Coming Soon!", 3000);
				currentState = MAIN_MENU;
				break;
				
			case LOGS_VIEWER:
				setStatus("Logs Viewer - Coming Soon!", 3000);
				currentState = MAIN_MENU;
				break;
				
			case SETTINGS_MENU:
				setStatus("Settings - Coming Soon!", 3000);
				currentState = MAIN_MENU;
				break;
		}
	}
	
	// Refresh the display based on current menu state
	if (screenOn) {
		switch (currentState) {
			case MAIN_MENU:
				drawMainMenu();
				break;
			default:
				// Other menus handle their own display updates
				break;
		}
	}
	
	delay(50); // Small delay to keep CPU usage reasonable
}