 #include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <NESpad.h>
#include "TomThumb.h"
#include "Snake.h"

#define SCREEN_DATA_PIN 9
#define SCREEN_WIDTH 12
#define SCREEN_HEIGHT 12
#define SCREEN_PIXELS_COUNT SCREEN_WIDTH * SCREEN_HEIGHT

NESpad gamepad = NESpad(5, 4, 6);

CRGB leds[SCREEN_PIXELS_COUNT];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(SCREEN_PIXELS_COUNT, SCREEN_DATA_PIN, NEO_GRB + NEO_KHZ800);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripBus(SCREEN_PIXELS_COUNT, SCREEN_DATA_PIN);
Adafruit_NeoMatrix screen = Adafruit_NeoMatrix(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DATA_PIN,
                            NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB + NEO_KHZ800);

NeoPixelAnimator animations(SCREEN_PIXELS_COUNT);

bool buttonPressed;
int vccPin = 3;
int gndPin = 7;

void setup() {
  Serial.begin(57600);
  initGamepad();
  strip.begin();
  randomize();
}

void initGamepad() {
  pinMode (vccPin, OUTPUT);
  pinMode (gndPin, OUTPUT);
  digitalWrite (vccPin, HIGH);
  digitalWrite (gndPin, LOW);
}

bool anyFunctionalButton(byte state) {
  return
    state & NES_A ||
    state & NES_B ||
    state & NES_SELECT ||
    state & NES_START;
}

bool anyButton(byte state) {
  return
    state & NES_A ||
    state & NES_B ||
    state & NES_UP ||
    state & NES_DOWN ||
    state & NES_LEFT ||
    state & NES_RIGHT ||
    state & NES_SELECT ||
    state & NES_START;
}

void loop() {
  runSleepMode();
}

void randomize() {
  uint32_t seed = analogRead(0);
  delay(1);

  for (int shifts = 3; shifts < 31; shifts += 3) {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

  randomSeed(seed);
}

