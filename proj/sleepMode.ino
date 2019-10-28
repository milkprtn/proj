#define ANIMATION_ZIGZAG 0
#define ANIMATION_RAINBOW 1
#define ANIMATION_FACET 2
#define ANIMATION_FADE 3

byte sleepModeActiveAnimation;
byte sleepModeAnimations[] = { ANIMATION_ZIGZAG, ANIMATION_RAINBOW, ANIMATION_FACET, ANIMATION_FADE };

void initSleepMode() {
  strip.clear();
  nextAnimation();
  buttonPressed = false;
}

void runSleepMode() {
  initSleepMode();

  while (true) {
    handleAnimations();
    handleGamepad();
  }
}

void handleAnimations() {
  if (!tickAnimation(sleepModeActiveAnimation)) {
    nextAnimation();
  }
}

void handleGamepad() {
  if (anyFunctionalButton(gamepad.buttons())) {
    buttonPressed = true;
    runMenu();
    initSleepMode();
  }
}

int nextAnimation() {
  setAnimation(randomAnimation());
}

void setAnimation(int animation) {
  sleepModeActiveAnimation = animation;
  initAnimation(animation);
}

int randomAnimation() {
  return sleepModeAnimations[random(sizeof(sleepModeAnimations))];
}

void initAnimation(int animation) {
  switch (sleepModeActiveAnimation) {
    case ANIMATION_ZIGZAG:
      animationZigzagInit();
      break;

    case ANIMATION_RAINBOW:
      animationRainbowInit();
      break;

    case ANIMATION_FACET:
      animationFacetInit();
      break;

    case ANIMATION_FADE:
      animationFadeInit();
      break;
  }
}

bool tickAnimation(int animation) {
  switch (sleepModeActiveAnimation) {
    case ANIMATION_ZIGZAG:
      return animationZigzagTick();
      break;

    case ANIMATION_RAINBOW:
      return animationRainbowTick();
      break;

    case ANIMATION_FACET:
      return animationFacetTick();
      break;

    case ANIMATION_FADE:
      return animationFadeTick();
      break;
  }
}

