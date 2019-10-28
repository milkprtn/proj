unsigned long animationFadeStartTime = 0;
unsigned long animationFadeDuration = 5 * 1000;
int animationFadeSwitchProbability = 20;

const uint8_t AnimationChannels = 1;
uint16_t effectState = 0;

MyAnimationState fadeAnimationState[AnimationChannels];

void BlendAnimUpdate(const AnimationParam& param) {
  RgbColor updatedColor = RgbColor::LinearBlend(
                            fadeAnimationState[param.index].StartingColor,
                            fadeAnimationState[param.index].EndingColor,
                            param.progress);

  for (uint16_t pixel = 0; pixel < SCREEN_PIXELS_COUNT; pixel++) {
    stripBus.SetPixelColor(pixel, updatedColor);
  }
}

void FadeInFadeOutRinseRepeat(float luminance) {
  if (effectState == 0) {
    RgbColor target = HslColor(random(360) / 360.0f, 1.0f, luminance);
    uint16_t time = random(1000, 2000);

    fadeAnimationState[0].StartingColor = stripBus.GetPixelColor(0);
    fadeAnimationState[0].EndingColor = target;

    animations.StartAnimation(0, time, BlendAnimUpdate);
  } else if (effectState == 1) {
    uint16_t time = random(1000, 2000);

    fadeAnimationState[0].StartingColor = stripBus.GetPixelColor(0);
    fadeAnimationState[0].EndingColor = RgbColor(0);

    animations.StartAnimation(0, time, BlendAnimUpdate);
  }

  effectState = (effectState + 1) % 2;
}

void animationFadeInit() {
  animationFadeStartTime = millis();
}

bool animationFadeTick() {
  bool inProgress = true;

  if (animations.IsAnimating()) {
    animations.UpdateAnimations();
    stripBus.Show();
  } else {
    FadeInFadeOutRinseRepeat(0.5f);
  }

  if (animationFadeStartTime + animationFadeDuration > millis()) {
    if (random(100) < animationFadeSwitchProbability) {
      inProgress = false;
    } else {
      animationFadeStartTime = millis();
    }
  }

     return inProgress;
}

