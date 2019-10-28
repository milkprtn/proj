unsigned long animationRainbowLastTickTime = 0;
unsigned long animationRainbowDelay = 15;
int animationRainbowSwitchProbability = 20;
int animationRainbowFrame;
bool initialRainbowAnimation;

void animationRainbowInit() {
  animationRainbowFrame = 0;
  initialRainbowAnimation = true;
}

void initialRainbowTick() {
  stripBus.SetPixelColor(animationRainbowFrame, Wheel(((animationRainbowFrame * 256 / strip.numPixels())) & 255));
  stripBus.Show();

  if (++animationRainbowFrame >= strip.numPixels()) {
    initialRainbowAnimation = false;
    animationRainbowFrame = 0;
  }
}

bool regularRainbowTick() {
  for (int i = 0; i < strip.numPixels(); i++) {
    stripBus.SetPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + animationRainbowFrame) & 255));
  }
  stripBus.Show();

  return ++animationRainbowFrame < 256;
}

bool animationRainbowTick() {
  bool inProgress = true;

  if (millis() > animationRainbowLastTickTime + animationRainbowDelay) {
    animationRainbowLastTickTime = millis();

    if (initialRainbowAnimation) {
      initialRainbowTick();
    } else {
      if (!regularRainbowTick()) {
        animationRainbowFrame = 0;

        if (random(100) < animationRainbowSwitchProbability) {
          inProgress = false;
        }
      }
    }
  }

  return inProgress;
}

RgbColor Wheel(byte WheelPos) {
  RgbColor color;
  WheelPos = 255 - WheelPos;

  if (WheelPos < 85) {
    color = RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    color = RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    color = RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  }

  return color;
}

