// SkullOS
// Skull light show

#include <FastLED.h>
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

//Config
#define FPS 100

#define SKULLS 26
#define S_CHNLS 1

#define OUT_DATA 10
#define BRT_PIN 0

#define NUM_LEDS SKULLS * S_CHNLS

CRGB leds[NUM_LEDS];
uint8_t currBrightness = 255;
extern const TProgmemPalette16 mardiGrasPalette_p PROGMEM;
uint8_t mg[3] = {192, 64, 96};

// Shared vars.
// These are initialized when the animation is changed.
// See initVars() for values
int pos;
int8_t dir;
int8_t chroma;

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rotateWash, kitt, rFlash, rStrobe, tStrobe };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

void setup() {
  // Setup LED driver
  FastLED.addLeds<NEOPIXEL, OUT_DATA>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // Get our house in order before we begin
  //doBrightness();
  random16_add_entropy(analogRead(1) << 4);
  switchAnimations();
}

void loop() {
  // Call next frame
  gPatterns[gCurrentPatternNumber]();

  // Update brightness
  //EVERY_N_SECONDS( 1 ) { doBrightness(); }
  EVERY_N_SECONDS( 20 ) { switchAnimations(); }
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

// Wash of colors across all LEDs
void rotateWash() {
  fill_solid(
    leds,
    NUM_LEDS,
    getColor(pos)
   );
   pos += 1;
   if (pos > 255) { pos -= 256; }
   FastLED.show();
   FastLED.delay(25);
}

#define RSTROBE_CHANCE 2
void rStrobe() {
  if (dir == 0) {
    pos = random16(SKULLS + ((RSTROBE_CHANCE - 1) * SKULLS));
    // Dir is the # of strobes before changing
    dir = random8(5) + 1;
  }

  if (pos < SKULLS) {
    setSkull(pos, CRGB::White);
    FastLED.show();
    FastLED.delay(15);
    setSkull(pos, CRGB::Black);
  }
  
  // Fade any leftover LEDs
  fadeToBlackBy(leds, NUM_LEDS, 128);
  FastLED.show();
  FastLED.delay(75);
  
  dir -= 1;
}

// Flash random lights
void rFlash() {
  int l = random16(SKULLS);
  setSkull(l, CHSV(chroma, 192, 255));
  FastLED.show();
  FastLED.delay(35);

  // TODO: set all black or fade all after resetting flash
  setSkull(l, CRGB::Black);
  // Fade any leftover LEDs
  fadeToBlackBy(leds, NUM_LEDS, 32);
  //FastLED.show();
  //FastLED.delay(50);
}


// Twinkle strobe w/color fade
#define TSTROBE_CHANCE 2
void tStrobe() {
  int l = random16(SKULLS + ((TSTROBE_CHANCE) * SKULLS));
  
  fadeToBlackBy(leds, NUM_LEDS, 128);

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

CRGB getColor(uint8_t idx) {
  return ColorFromPalette(mardiGrasPalette_p, idx, 255, LINEARBLEND);
}

uint8_t getChroma(uint8_t idx) {
  return mg[random8(ARRAY_SIZE(mg))];
}

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
  chroma = getChroma(random8());
}

// Switch animations
void switchAnimations() {
  initVars();
  gCurrentPatternNumber = random8(ARRAY_SIZE(gPatterns));
}

const TProgmemPalette16 mardiGrasPalette_p PROGMEM =
{
  CRGB::Purple,
  CRGB::Purple, // 'white' is too bright compared to red and blue
  CRGB::Purple,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::Green,
  CRGB::Green,

  CRGB::Gold,
  CRGB::Gold,
  CRGB::Gold,
  CRGB::Gold,
  CRGB::Gold,
  CRGB::Purple,
  CRGB::Purple,
  CRGB::Purple
};
