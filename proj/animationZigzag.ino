unsigned long animationZigzagLastTickTime = 0;
unsigned long animationZigzagDelay = 15;
int animationZigzagSwitchProbability = 20;
RgbColor animationZigzagColor;
int animationZigzagFrame = 0;

void animationZigzagInit() {
  animationZigzagFrame = 0;
  animationZigzagLastTickTime = 0;
  animationZigzagColor = randomColor();
}

bool animationZigzagTick() {
  bool inProgress = true;

  if (millis() > animationZigzagLastTickTime + animationZigzagDelay) {
    animationZigzagLastTickTime = millis();

    stripBus.SetPixelColor(animationZigzagFrame++, animationZigzagColor);
    stripBus.Show();

    if (animationZigzagFrame == strip.numPixels()) {
      animationZigzagColor = randomColor();
      animationZigzagFrame = 0;

      if (random(100) < animationZigzagSwitchProbability) {
        inProgress = false;
      }
    }
  }

  return inProgress;
}

RgbColor randomColor() {
  return HslColor(random(360) / 360.0f, 1.0f, 0.4f);
}

