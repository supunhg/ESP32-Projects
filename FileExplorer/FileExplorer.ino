/*
 * FileExplorer.ino
 * 
 * ESP32 SD Card File Browser with OLED Display and Keypad Navigation
 * 
 * Hardware Requirements:
 * - ESP32 Development Board
 * - 128x64 OLED Display (SSD1306, I2C)
 * - 2x2 Keypad Matrix
 * - MicroSD Card Module (SPI)
 * 
 * Navigation:
 * - U/D: Navigate up/down through files
 * - R: Open/view selected file
 * - L: Back to file list (when viewing file)
 * 
 * Wiring:
 * - OLED: SDA=21, SCL=22
 * - Keypad: Rows=26,27 Cols=32,33
 * - SD Card: CS=5, MOSI=23, MISO=19, SCK=18
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <SD.h>
#include <SPI.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Keypad 2x2 matrix configuration
const byte ROWS = 2;
const byte COLS = 2;
char keys[ROWS][COLS] = {
	{'U', 'D'},
	{'L', 'R'}
};

byte rowPins[ROWS] = {26, 27};
byte colPins[COLS] = {32, 33};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// SD Card SPI
#define SD_CS 5
#define MOSI  23
#define MISO  19
#define SCK   18

// File List & Navigation
#define MAX_FILES 50
String files[MAX_FILES];
int fileCount = 0;
int selected = 0;
int offset = 0;
bool viewingFile = false;

// File browser display function
void displayFiles() {
	display.clearDisplay();
	display.setTextSize(1);
	display.setCursor(0, 0);

	for (int i = 0; i < 4 && (offset + i) < fileCount; i++) {
		if ((offset + i) == selected)
			display.print("> ");
		else
			display.print("  ");

		display.println(files[offset + i]);
	}
	display.display();
}

// View file contents on display
void viewFile(String filename) {
	File file = SD.open("/" + filename);
	display.clearDisplay();
	display.setCursor(0, 0);

	if (!file) {
		display.println("Failed to open:");
		display.println(filename);
		display.display();
		delay(2000);
		return;
	}

	display.clearDisplay();
	int line = 0;
	while (file.available() && line < 8) {
		String content = file.readStringUntil('\n');
		display.setCursor(0, line * 8);
		display.println(content);
		line++;
	}
	file.close();
	display.display();
}

// Scan SD card for files
void scanFiles() {
	File root = SD.open("/");
	fileCount = 0;
	while (true) {
		File entry = root.openNextFile();
		if (!entry) break;
		if (!entry.isDirectory() && fileCount < MAX_FILES) {
			files[fileCount++] = String(entry.name());
		}
		entry.close();
	}
	root.close();
}

// Setup function - initialize hardware and SD card
void setup() {
	Serial.begin(115200);

	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println("OLED failed");
		while (true);
	}
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0, 0);
	display.println("Initializing...");
	display.display();

	SPI.begin(SCK, MISO, MOSI, SD_CS);
	if (!SD.begin(SD_CS)) {
		display.println("SD Failed");
		display.display();
		Serial.println("SD fail");
		return;
	}

	display.println("SD OK");
	display.display();
	delay(1000);

	scanFiles();
	displayFiles();
}

// Main loop - handle keypad input and navigation
void loop() {
	char key = keypad.getKey();
	if (key) {
		if (viewingFile && key == 'L') {
			viewingFile = false;
			displayFiles();
			return;
		}

		if (!viewingFile) {
			if (key == 'U') {
				if (selected > 0) selected--;
				if (selected < offset) offset--;
				displayFiles();
			} else if (key == 'D') {
				if (selected < fileCount - 1) selected++;
				if (selected > offset + 3) offset++;
				displayFiles();
			} else if (key == 'R') {
				viewingFile = true;
				viewFile(files[selected]);
			}
		}
	}
}
