// Libraries

#include <IRremote.h>
#include <IRremoteInt.h>
#include "Direction.cpp"
#include "Game.cpp"
#include "Remote.cpp"
#include "Display.h"

// defines Pin
#define RIGHTBUTTON   3
#define LEFTBUTTON    18
#define BUZZER        11
#define REMOTE        13

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

Display display;

void setup() {
  Serial.begin(9600);

  display.init();

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
    display.showInstructions();
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
    display.setSinglePlayerGame();
    delay(2000);
    return SINGLEPLAYER;
  } else if (remote != previous && remote == TWO) {
    previous = remote;
    display.setDoublePlayerGame();
    delay(2000);
    return DOUBLEPLAYER;
  }
  return game;
}


// Display Score for each state (single, double, win)
void displayScore() {
  if (game == SINGLEPLAYER) {
    display.showSingleScore(pointRight);
  } else if (game == DOUBLEPLAYER) {
    display.showDoubleScore(pointRight, pointLeft);
  } else if (game == WIN) {
    finish();
    display.showFinalScore(pointRight, pointLeft);
    delay(2000);
    game = FINISHED;
  }
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
