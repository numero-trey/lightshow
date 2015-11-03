
CHSV bgClr(uint8_t moo, uint8_t index) {
  uint8_t base_hue = HUE_HAPPY;
  uint8_t sat = 255 - pattern[index][1];
  uint8_t val = 255 - pattern[index][2];
  

  if (moo > 191) {
    base_hue = HUE_SAD; //map(moo, 192, 255, HUE_HAPPY, HUE_SAD);
  }
  
  return CHSV(
    base_hue + pattern[index][0],
    sat,
    val
  );
}

#define BASE_LED 6
uint8_t getMood(uint8_t led) {
  if (mood >= NUM_LEDS << 8) { return 255; }

  uint8_t start_led;
  uint8_t end_led;
  uint8_t stp = mood >> 8;

  if (stp <= BASE_LED) {
    start_led = BASE_LED - stp;
  } else {
    start_led = NUM_LEDS - stp + BASE_LED;
  }

  if (stp < NUM_LEDS - BASE_LED) {
    end_led = BASE_LED + stp;
  } else {
    end_led = stp - (NUM_LEDS - BASE_LED);
  }

  if (led > start_led && led < end_led) {
    return 255;
  } else if (led == start_led || led == end_led) {
    return mood & 0xFF;
  }

  return 0;
}


void initPattern() {
  for (int i=0; i < NUM_LEDS; ++i) {
      pattern[i][0] = random8(BG_HUE_WIDTH);
      pattern[i][1] = random8(BG_SAT_WIDTH);
      pattern[i][2] = random8(BG_VAL_WIDTH);
  }
}

