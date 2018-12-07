#include "Display.h"

#define OLED_SCL 21
#define OLED_SDA 20
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET 4

Display::Display(void) {
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1, 400000UL, 100000UL);
}

void Display::init() {
// initializing oled display
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Copyright (c) 2018\nChristian\nSeiler\nServices");
    display.setCursor(0, 56);
    display.print("christianseiler.ch");
    display.display();
}

void Display::showInstructions() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Welcome to\nFast Lights!");
    display.setCursor(0, 30);
    display.println("Press:");
    display.println("1 for Single Player");
    display.println("2 for Double Player");
    display.display();
}

// Display Score for each state (single, double, win)
void Display::showSingleScore(int pointRight) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Score");
    display.setCursor(0, 20);
    display.print("Right");
    display.setCursor(100, 20);
    display.print(pointRight);
    display.display();
}
void Display::showDoubleScore(int pointRight, int pointLeft) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Score");
    display.setCursor(0, 20);
    display.print("Left");
    display.setCursor(100, 20);
    display.print(pointLeft);
    display.setCursor(0, 40);
    display.print("Right");
    display.setCursor(100, 40);
    display.print(pointRight);
    display.display();
}

void Display::showFinalScore(int pointRight, int pointLeft) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("The Winner is");
    display.setCursor(0, 20);
    display.print(pointLeft > pointRight ? "LEFT" : "RIGHT");
    display.display();
}

void Display::setSinglePlayerGame(void) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Playing");
    display.setCursor(20, 0);
    display.print("Single Player");
}
void Display::setDoublePlayerGame(void) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Playing");
    display.setCursor(20, 0);
    display.print("Double Player");
}

