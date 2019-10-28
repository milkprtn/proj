unsigned long animationFacetStartTime = 0;
unsigned long animationFacetDuration = 5 * 1000;
int animationFacetSwitchProbability = 80;

struct MyAnimationState {
  RgbColor StartingColor;
  RgbColor EndingColor;
};

MyAnimationState animationState[SCREEN_PIXELS_COUNT];

void fadeBlendAnimUpdate(const AnimationParam& param) {
  RgbColor updatedColor = RgbColor::LinearBlend(
                            animationState[param.index].StartingColor,
                            animationState[param.index].EndingColor,
                            param.progress);

  stripBus.SetPixelColor(param.index, updatedColor);
}

void PickRandom() {
  float luminance = 0.4f;
  uint16_t count = random(SCREEN_PIXELS_COUNT);

  while (count > 0) {
    uint16_t pixel = random(SCREEN_PIXELS_COUNT);
    uint16_t time = random(100, 400);
    animationState[pixel].StartingColor = stripBus.GetPixelColor(pixel);
    animationState[pixel].EndingColor = HslColor(random(360) / 360.0f, 1.0f, luminance);

    animations.StartAnimation(pixel, time, fadeBlendAnimUpdate);

    count--;
  }
}

bool animationFacetInit() {
  animationFacetStartTime = millis();
}

bool animationFacetTick() {
  bool inProgress = true;

  if (animations.IsAnimating()) {
    animations.UpdateAnimations();
    stripBus.Show();
  } else {
    PickRandom();
  }

  if (animationFacetStartTime + animationFacetDuration < millis()) {
    if (random(100) > animationFacetSwitchProbability) {
      inProgress = false;
    } else {
      animationFacetStartTime = millis();
    }
  }

  return inProgress;
}

