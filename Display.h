#ifndef Display_h
#define Display_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display {
public:
    Display();
    void init(void);
    void showInstructions(void);
    void showSingleScore(int pointRight);
    void showDoubleScore(int pointRight, int pointLeft);
    void showFinalScore(int pointRight, int pointLeft);
    void setSinglePlayerGame(void);
    void setDoublePlayerGame(void);
private:
    Adafruit_SSD1306 display;
};
#endif
