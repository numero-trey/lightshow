// SkullOS
// Skull light show

#include <FastLED.h>

//Config
#define OUT_DATA 11
#define SKULLS 8
#define S_CHNLS 4
#define BRT_PIN 0

#define NUM_LEDS SKULLS * S_CHNLS

CRGB leds[NUM_LEDS];
uint8_t currBrightness = 255;

// Shared vars.
// These are initialized when the animation is changed.
// See initVars() for values
int pos;
int8_t dir;
int8_t chroma;

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { kitt, rStrobe, tStrobe };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

void setup() {
  // Setup LED driver
  FastLED.addLeds<NEOPIXEL, OUT_DATA>(leds, NUM_LEDS); //.setCorrection(TypicalLEDStrip);

  // Get our house in order before we begin
  doBrightness();
  random16_add_entropy(analogRead(1) << 4);
  switchAnimations();
}

void loop() {
  // Call next frame
  gPatterns[gCurrentPatternNumber]();
  //kitt();
  //rStrobe();

  // Update brightness
  EVERY_N_SECONDS( 1 ) { doBrightness(); }
  EVERY_N_SECONDS( 10) { switchAnimations(); }
}

////////////////////////////////////////////////////////
// Show functions
//
// Each call should be one frame and contain it's own delay

// Knight Rider KITT effect
#define KITTDELAY 1500 / (SKULLS * 2 - 1)
void kitt() {
  fadeToBlackBy(leds, NUM_LEDS, 640/SKULLS);
  setSkull(pos, CHSV(chroma, 255, 255));
  FastLED.show();
  setSkull(pos, CHSV(chroma, 255, 192));

  if ((pos == SKULLS - 1) || (pos == 0 && dir == -1)) {
    dir *= -1;
  }
  pos += dir;
  
  FastLED.delay(KITTDELAY);
}

// Flash random lights
void rStrobe() {
  int l = random16(SKULLS);
  setSkull(l, CHSV(chroma, 255, 255));
  FastLED.show();
  FastLED.delay(35);

  // TODO: set all black or fade all after resetting flash
  setSkull(l, CRGB::Black);
  // Fade any leftover LEDs
  fadeToBlackBy(leds, NUM_LEDS, 128);
  //FastLED.show();
  //FastLED.delay(50);
}


// Twinkle strobe w/color fade
#define TSTROBE_CHANCE 3
void tStrobe() {
  int l = random16(SKULLS + ((TSTROBE_CHANCE - 1) * SKULLS));
  
  fadeToBlackBy(leds, NUM_LEDS, 128/SKULLS);

  if (l < SKULLS) {
    setSkull(l, CRGB::White);
    FastLED.show();
    FastLED.delay(15);
    setSkull(l, CHSV(chroma, 255, 255)); //(82, 0, 0)
  }
  
  FastLED.show();
  
  FastLED.delay(50);
}

////////////////////////////////////////////////
// Util Functions

void setSkull(uint16_t ch, CRGB c) {
  for(int i=0; i < S_CHNLS; i++) {
    leds[ch * S_CHNLS + i] = c;
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

// Initialize shared vars for animations
void initVars() {
  dir = 1;
  pos = 0;
  chroma = random8();
}

// Switch animations
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
void switchAnimations() {
  initVars();
  gCurrentPatternNumber = random8(ARRAY_SIZE(gPatterns));
}

