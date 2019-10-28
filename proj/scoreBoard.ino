void initScore() {
  strip.clear();
  screen.setFont(&TomThumb);
  screen.setTextWrap(false);
  screen.setBrightness(255);
  screen.fillScreen(0);
  screen.setTextColor(screen.Color(255, 0, 0));
}

void showScore(int score) {
  initScore();

  int x;
  int y = 8;

  if (score < 10) {
    x = 5;
  } else if (score >= 10 & score < 100) {
    x = 3;
  } else {
    x = 1;
  }
 
  screen.setCursor(x, y);
  screen.print(score);
  screen.show();
  delay(5000);
}

