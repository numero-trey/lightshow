// SkullOS
// Skull light show

#include <FastLED.h>

//Config
#define OUT_DATA 11
#define SKULLS 8
#define S_CHNLS 1
#define BRT_PIN 0

#define NUM_LEDS SKULLS * S_CHNLS

CRGB leds[NUM_LEDS];
uint8_t currBrightness = 255;

// Shared vars.
// These are initialized when the animation is changed
int pos = 0;
int8_t dir = 1;

void setup() {
  FastLED.addLeds<NEOPIXEL, OUT_DATA>(leds, NUM_LEDS);
  doBrightness();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0; i<4; i++) {
     kitt();
  }

  EVERY_N_SECONDS( 1 ) { doBrightness(); }
}

////////////////////////////////////////////////////////
// Show functions
//
// Each call should be one frame and contain it's own delay

// Knight Rider KITT effect
void kitt() {
  fadeToBlackBy(leds, NUM_LEDS, 128);
  setSkull(pos, CRGB::Red);
  FastLED.show();

  if ((pos == SKULLS - 1) || (pos == 0 && dir == -1)) {
    dir *= -1;
  }
  pos += dir;
  
  FastLED.delay(100);
}

// Flash random lights
void rStrobe() {
  int l = random16(SKULLS);
  setSkull(l, CRGB::Red);
  FastLED.show();
  FastLED.delay(5);

  setSkull(l, CRGB::Black);
  FastLED.show();
  FastLED.delay(75);
}

#define TSTROBE_CHANCE 3
// Twinkle strobe w/color fade
void tStrobe() {
  int l = random16(SKULLS + ((TSTROBE_CHANCE - 1) * SKULLS));
  
  fadeToBlackBy(leds, NUM_LEDS, 24);

  if (l < SKULLS) {
    setSkull(l, CRGB::White);
    FastLED.show();
    delay(10);
    setSkull(l, CRGB(82, 0, 0));
  }
  
  FastLED.show();
  
  delay(50);
}

////////////////////////////////////////////////
// Util Functions

void setSkull(uint16_t ch, CRGB c) {
  for(int i=0; i < S_CHNLS; i++) {
    leds[ch + i] = c;
  }
}

// Poll A0 and update brightness if needed
void doBrightness() {
  int val = analogRead(BRT_PIN) >> 2;
  if (val != currBrightness) {
    FastLED.setBrightness(val);
    currBrightness = val;
  }
}
