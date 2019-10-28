#define MATRIX_WIDTH SCREEN_WIDTH
#define MATRIX_HEIGHT SCREEN_HEIGHT
#define DEFAULT_FALL_SPEED 80
#define DEBOUNCE_VALUE 15
#define DOTS_IN_BRICK 4

#define X 0
#define Y 1

int bricks[7][DOTS_IN_BRICK][2] = {
  {{0, 1}, { -1, 0}, {0, 0}, {1, 0}},
  {{ -1, 1}, { -1, 0}, {0, 0}, {1, 0}},
  {{1, 1}, { -1, 0}, {0, 0}, {1, 0}},
  {{ -1, 0}, {0, 0}, {0, 1}, {1, 1}},
  {{ -1, 1}, {0, 1}, {0, 0}, {1, 0}},
  {{1, 0}, {1, 1}, {0, 1}, {0, 0}},
  {{ -1, 0}, {0, 0}, {1, 0}, {2, 0}}
};

int colors[7][3] = {
  {255, 0, 0},
  {100, 255, 0},
  {0, 0, 255},
  {255, 255, 0},
  {0, 255, 255},
  {255, 100, 0},
  {255, 255, 255}
};

int pos[2];
int matrix[MATRIX_WIDTH][MATRIX_HEIGHT] = {0};
int current[DOTS_IN_BRICK][2] = {{1, 1}, { -1, 0}, {0, 0}, {1, 0}};
int currentnum = 1;
int truepos[2] = {0, 0};
int fallspeed;
int fallcount;
int debounce;
int rowtodelete;
int score;
bool gameover;

void initTetris() {
  pos[X] = MATRIX_WIDTH / 2;
  pos[Y] = 0;
  score = 0;
  gameover = false;
  buttonPressed = false;
  fallspeed = DEFAULT_FALL_SPEED;
  fallcount = 0;
  debounce = 0;
  rowtodelete = 0;

  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
      matrix[x][y] = 0;
    }
  }
}

void tetris() {
  initTetris();

  do {
    byte state = gamepad.buttons();

    if (fallcount == fallspeed) {
      fallcount = 0;
      down();
    }

    if (state & NES_DOWN && debounce == 0) {
      debounce = DEBOUNCE_VALUE;
      down();
    }

    if (state & NES_RIGHT && debounce == 0) {
      debounce = DEBOUNCE_VALUE;
      right();
    }

    if (state & NES_LEFT && debounce == 0) {
      debounce = DEBOUNCE_VALUE;
      left();
    }

    if (anyFunctionalButton(state)) {
      if (!buttonPressed) {
        debounce = DEBOUNCE_VALUE;
        buttonPressed = true;
        rotate();
      }
    } else {
      if (buttonPressed) {
        buttonPressed = false;
      }
    }

    if (debounce != 0) {
      debounce--;
    }

    fallcount++;
    checkrows();
    checkend();
    show();
    delay(1);
  } while (!gameover);

  showScore(score);
}

void checkend() {
  gameover = false;
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    if (matrix[x][0] != 0) {
      gameover = true;
    }
  }
}

void checkrows() {
  bool fullrow;

  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    fullrow = true;
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      if (matrix[x][y] == 0) {
        fullrow = false;
      }
    }
    if (fullrow) {
      for (int row = y; row > 0; row--) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
          matrix[x][row] = matrix[x][row - 1];
        }
      }
      score++;
    }
  }
}

void down() {
  bool moveable = true;
  for (int e = 0; e < DOTS_IN_BRICK; e++) {
    if (matrix[current[e][X] + pos[X]][current[e][Y] + pos[Y] + 1] != 0) {
      moveable = false;
    }
    if (current[e][Y] + pos[Y] + 1 == MATRIX_HEIGHT ) {
      moveable = false;
    }
  }
  if (moveable) {
    pos[Y]++;
  }
  else {
    add(random(7));
  }
}

void right() {
  bool moveable = true;
  for (int e = 0; e < DOTS_IN_BRICK; e++) {
    if (matrix[current[e][X] + pos[X] + 1][current[e][Y] + pos[Y]] != 0) {
      moveable = false;
    }
    if (current[e][X] + pos[X] + 1 == MATRIX_HEIGHT) {
      moveable = false;
    }
  }
  if (moveable) {
    pos[X]++;
  }
}

void left() {
  bool moveable = true;
  for (int e = 0; e < DOTS_IN_BRICK; e++) {
    if (matrix[current[e][X] + pos[X] - 1][current[e][Y] + pos[Y]] != 0) {
      moveable = false;
    }
    if (current[e][X] + pos[X] - 1 == -1) {
      moveable = false;
    }
  }
  if (moveable) {
    pos[X]--;
  }
}

void rotate() {
  int currentCache[DOTS_IN_BRICK][2] = {0};

  for (int e = 0; e < DOTS_IN_BRICK; e++) {
    currentCache[e][X] = current[e][X];
    currentCache[e][Y] = current[e][Y];
  }
  for (int e = 0; e < DOTS_IN_BRICK; e++) {
    current[e][Y] = currentCache[e][X];
    current[e][X] = -currentCache[e][Y];
  }
  bool moveable = true;
  for (int e = 0; e < DOTS_IN_BRICK; e++) {
    if (matrix[current[e][X] + pos[X]][current[e][Y] + pos[Y]] != 0 ||
        current[e][X] + pos[X] < 0 ||
        current[e][X] + pos[X] > SCREEN_WIDTH - 1 ||
        matrix[current[e][Y] + pos[Y]][current[e][Y] + pos[Y]] != 0 ||
        current[e][Y] + pos[Y] < 0 ||
        current[e][Y] + pos[Y] > SCREEN_HEIGHT - 1) {
      moveable = false;
    }
  }
  if (!moveable) {
    for (int e = 0; e < DOTS_IN_BRICK; e++) {
      current[e][X] = currentCache[e][X];
      current[e][Y] = currentCache[e][Y];
    }
  }
}

void add(int b) {
  for (int e = 0; e < DOTS_IN_BRICK; e++) {
    matrix[current[e][X] + pos[X]][current[e][Y] + pos[Y]] = currentnum + 1;
    current[e][X] = bricks[b][e][X];
    current[e][Y] = bricks[b][e][Y];
  }
  currentnum = b;
  pos[X] = MATRIX_WIDTH / 2;
  pos[Y] = 0;
}

void show() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 2, 2, 2);
  }
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 0; y < MATRIX_HEIGHT; y ++) {
      if (matrix[x][y] != 0) {
        if (x % 2 == 1) {
          strip.setPixelColor((x * MATRIX_HEIGHT) + y, colors[matrix[x][y] - 1][0], colors[matrix[x][y] - 1][1], colors[matrix[x][y] - 1][2]);
        }
        else {
          strip.setPixelColor((x * MATRIX_HEIGHT) + (MATRIX_HEIGHT - y - 1), colors[matrix[x][y] - 1][0], colors[matrix[x][y] - 1][1], colors[matrix[x][y] - 1][2]);
        }
      }
    }
  }
  for (int e = 0; e < DOTS_IN_BRICK; e++) {
    truepos[X] = current[e][X] + pos[X];
    truepos[Y] = current[e][Y] + pos[Y];
    if (truepos[X] % 2 == 1) {
      strip.setPixelColor((truepos[X]*MATRIX_HEIGHT) + truepos[Y], colors[currentnum][0], colors[currentnum][1], colors[currentnum][2]);
    }
    else {
      strip.setPixelColor((truepos[X]*MATRIX_HEIGHT) + (MATRIX_HEIGHT - truepos[Y] - 1), colors[currentnum][0], colors[currentnum][1], colors[currentnum][2]);
    }
  }
  strip.show();
}
