// Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include "Direction.cpp"
#include "Game.cpp"
#include "Remote.cpp"

// defines Pin
#define RIGHTBUTTON   3
#define LEFTBUTTON    18
#define BUZZER        11
#define REMOTE        13

// oled stuff
#define OLED_SCL      21
#define OLED_SDA      20
#define SCREEN_WIDTH  128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET    4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1, 400000UL, 100000UL);

int pointRight = 0;
int pointLeft = 0;

// Lamp things
const int LAMPS [] = {22, 25, 26, 29, 30, 33, 34, 37, 38, 41, 42};
const int COUNT = (sizeof(LAMPS) / sizeof(LAMPS[0]));
int lamp;
const int maxPoints = 10;

// Timing
int delayMS = 200;
const int buzzMS = 300;

bool pressed = false;

// IR stuff
IRrecv ir(REMOTE);
decode_results results;

Game game = FINISHED;
Direction dir = DIR_LEFT;
Remote previous;

void setup() {
  Serial.begin(9600);

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

  // IR Remote setup
  ir.enableIRIn();
  ir.blink13(false);

  // light up each led at startup
  for (int i : LAMPS) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
    delay(200);
    digitalWrite(i, LOW);
  }

  // Pushbutton setup
  pinMode(LEFTBUTTON, INPUT);
  pinMode(RIGHTBUTTON, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFTBUTTON), leftButtonPressed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHTBUTTON), rightButtonPressed, CHANGE);

  // give me a break
  delay(2000);
}

void loop() {
  // get the current remote button
  if (ir.decode(&results)) {
    ir.resume();
  }

  // 
  if (game == FINISHED) {
    displayInstructions();
    LED(game);
    game = setGame();
  } else if (game == WIN) {
    displayScore();
  } else {
    LED(game);
    displayScore();
  }
}




// Interrupt routine for the left pushbutton (aka player two)
void leftButtonPressed() {
  if (lamp == COUNT/2 && pressed == false) {
    pressed = true;
    tone(BUZZER, 391.9954, buzzMS);
    pointLeft += 1;
    if (pointLeft >= maxPoints) {
      game = WIN;
    }
  }
}


// Interrupt routine for the right pushbutton (aka player one)
void rightButtonPressed() {
  if (lamp == COUNT/2 && pressed == false) {
    pressed = true;
    tone(BUZZER, 440, buzzMS);
    pointRight += 1;
    if (pointRight >= maxPoints) {
      game = WIN;
    }
  }
}


// reset score and play win melody
void finish() {
  pointRight = 0;
  pointLeft = 0;
  tone(BUZZER, 415.3047, 300);
  delay(350);
  tone(BUZZER, 349.2282, 500);
  delay(350);
  tone(BUZZER, 466.1638, 700);
  delay(700);
}


// start game if remote has been pressed
Game setGame() {
  Remote remote = (Remote) results.value;
  if (remote != previous && remote == ONE) {
    previous = remote;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Playing");
    display.setCursor(20, 0);
    display.print("Single Player");
    delay(2000);
    return SINGLEPLAYER;
  } else if (remote != previous && remote == TWO) {
    previous = remote;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Playing");
    display.setCursor(20, 0);
    display.print("Double Player");
    delay(2000);
    return DOUBLEPLAYER;
  }
  return game;
}


// Display Score for each state (single, double, win)
void displayScore() {
  if (game == SINGLEPLAYER) {
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Score");
    display.setCursor(0, 20);
    display.print("Right");
    display.setCursor(100, 20);
    display.print(pointRight);
    display.display();
    
  } else if (game == DOUBLEPLAYER) {
    
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
    
  } else if (game == WIN) {
    
    finish();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("The Winner is");
    display.setCursor(0, 20);
    display.print(pointLeft > pointRight ? "LEFT" : "RIGHT");
    display.display();
    delay(2000);
    game = FINISHED;
  }
}


void displayInstructions() {
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Welcome to\nFast Lights!");
  display.setCursor(0, 30);
  display.println("Press:");
  display.println("1 for Single Player");
  display.println("2 for Double Player");
  display.display();
}


// determine which led to light
void LED(Game game) {
  // turn off the lamp that is on
  digitalWrite(LAMPS[lamp], LOW);
  
  if (game == FINISHED) {
    // light up led left to right to left
    if (dir == DIR_LEFT) {
      if (lamp == COUNT-1) {
        dir = DIR_RIGHT;
        lamp -= 1;
      } else {
        lamp += 1;
      }
    } else {
      if (lamp == 0) {
        dir = DIR_LEFT;
        lamp += 1;
      } else {
        lamp -= 1;
      }
    }
  } else {

    // game is on
    // get a random number between 0 and the number of lamps
    int n = random(COUNT);
    // if the random lamp was the previous lamp, choose another
    lamp = n == lamp ? n + random(-1, 2) : n;
    pressed = false;
  }

  // turn on lamp
  digitalWrite(LAMPS[lamp], HIGH);
  delay(delayMS);
}
