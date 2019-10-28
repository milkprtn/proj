#define SNAKE_SPEED 5

Snake snakeGame(SCREEN_WIDTH, SCREEN_HEIGHT, SNAKE_SPEED);

bool snakeGameover;
int snakeScore;

void initSnake() {
  snakeGameover = false;
  snakeScore = 0;

  snakeGame.resetGame();
  snakeGame.wasGameReset();
}

byte setPixel(byte x, byte y, byte r, byte g, byte b) {
  y += 3;

  if (y % 2 != 0) {
    x = SCREEN_WIDTH - x - 1;
  }

  byte ledID = SCREEN_PIXELS_COUNT - (y * SCREEN_WIDTH) - x - 1 ;
  leds[ledID].setRGB(r, g, b);
  return ledID;
}

void changeRGBtoGBR() {
  for (int whiteLed = 0; whiteLed < SCREEN_PIXELS_COUNT; whiteLed++) {
    leds[whiteLed].setRGB(leds[whiteLed].g , leds[whiteLed].b, leds[whiteLed].r);
  }
}

void clearScreen() {
  for (int whiteLed = 0; whiteLed < SCREEN_PIXELS_COUNT; whiteLed++) {
    leds[whiteLed].setRGB(2, 2, 2);
  }
}

void snake() {
  initSnake();

  do {
    byte state = gamepad.buttons();

    if (state & NES_DOWN) {
      snakeGame.goLeft();
    }

    if (state & NES_UP) {
      snakeGame.goRight();
    }

    if (state & NES_LEFT) {
      snakeGame.goDown();
    }

    if (state & NES_RIGHT) {
      snakeGame.goUp();
    }

    snakeScore = snakeGame.getSnakeLenght() - 1;

    Snake::pixel* snakeLimbs = snakeGame.getSnakeLimbs();
    Snake::pixel* snakeFood = snakeGame.getFoodPositions();

    clearScreen();
    setPixel(snakeFood[0].posX, 8 - snakeFood[0].posY, 255, 0, 0);
    setPixel(snakeLimbs[0].posX, 8 - snakeLimbs[0].posY, 0, 255, 0);
    for (int i = 1; i < snakeGame.getSnakeLenght(); i++) {
      setPixel(snakeLimbs[i].posX, 8 - snakeLimbs[i].posY, 0, 100, 0);
    }

    FastLED.show();
    snakeGame.tick();

    delay(30);
  } while (!snakeGame.wasGameReset());

  for (int i = 0; i < 30; i++) {
    changeRGBtoGBR();
    FastLED.show();
    delay(40);
  }

  showScore(snakeScore);
}

