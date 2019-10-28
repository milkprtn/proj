#include "animation.h"
#include "samplePattern.h"

int menuBrightness = 255;
unsigned long menuIdleTimer = 0;
unsigned long menuDelay = 10000;
unsigned long previousMillis = 0;

void initMenu() {
  LEDS.addLeds<WS2812B, SCREEN_DATA_PIN, GRB>(leds, SCREEN_PIXELS_COUNT);
  LEDS.showColor(CRGB(0, 0, 0));
  LEDS.setBrightness(menuBrightness);
  LEDS.show();

  menuIdleTimer = millis();
  animation.draw(leds);
}

void runMenu() {
  initMenu();

  do {
    byte state = gamepad.buttons();    

    if (state & NES_A && !buttonPressed) {
      tetris();
      menuIdleTimer = millis();
    }

    if (state & NES_B && !buttonPressed) {
      snake();
      menuIdleTimer = millis();
    }

    if (anyFunctionalButton(state)) {
      if (!buttonPressed) {
        buttonPressed = true;
      }
    } else {
      if (buttonPressed) {
        buttonPressed = false;
      }
    }

    if ( millis () > previousMillis + animation.getFrameDelay()) {
      previousMillis = millis();
      animation.draw(leds);
    }

  } while (millis() < menuIdleTimer + menuDelay);
}

