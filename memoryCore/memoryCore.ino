// SkullOS
// Skull light show

#include <FastLED.h>

//Config
#define OUT_DATA 11
#define NUM_LEDS 8

struct Spark {
  uint16_t pos;
  CRGB color;
  int8_t dir;
};

CRGB leds[NUM_LEDS];
struct Spark sparks[5];
uint16_t pos;
uint8_t bg_hue = 26; //Blue: 135, Happy: 20;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup() {
  // Setup LED driver
  FastLED.addLeds<NEOPIXEL, OUT_DATA>(leds, NUM_LEDS); //.setCorrection(TypicalLEDStrip);

  // Get our house in order before we begin
  //doBrightness();
  //FastLED.setDither( 0 );
  //FastLED.setBrightness(128);
  random16_add_entropy(analogRead(1) << 4);
  //initSpark(0, 6, CHSV(60, 127, 255)); //Sad: 140
  //initSpark(1, 8, CRGB::Yellow);
  //initSpark(1, 4, CRGB::Orange);
}

void loop() {
  // Call next frame
  doBG();
  doSparks();

  // Update brightness
  //EVERY_N_SECONDS( 1 ) { doBrightness(); }
  //EVERY_N_SECONDS( 10) { switchAnimations(); }
  FastLED.show();
  FastLED.delay(8);
}

////////////////////////////////////////////////////////
// Show functions
//
// Each call should be one frame

#define HUE_WIDTH 16
#define STEP_SIZE 256 / NUM_LEDS
void doBG() {  
  // Background

  uint8_t hue = pos >> 4;
  for (int i=0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(
      map(triwave8(hue), 0, 255, bg_hue, bg_hue + HUE_WIDTH), 
      255, 
      255
    );
    hue += STEP_SIZE;
  }
  
  //fill_solid(leds, NUM_LEDS, CHSV(64, 255, 232));
  //fill_solid(leds, NUM_LEDS, CRGB::Black);

  pos += 20;
  if (pos >= 256 << 4 ) { pos -= 256 << 4 ; }
}

void initSpark(uint8_t index, int8_t dir, CRGB color) {
  sparks[index].pos = 0;
  sparks[index].dir = dir;
  sparks[index].color = color;
}

void doSparks() {
  for (int i=0; i < ARRAY_SIZE(sparks); i++) {
    if (sparks[i].dir != 0) {
      setSub(leds, sparks[i].pos, sparks[i].color);
      sparks[i].pos += sparks[i].dir;
      if (sparks[i].pos >= NUM_LEDS << 8) {
        sparks[i].pos -= NUM_LEDS << 8;
      }
    }
  }
}

// Set a pixel with subpixel rendering.
void setSub(struct CRGB * leds, uint16_t index, CRGB color) {
  // Calc our base pixel and secondary
  uint16_t curr = index >> 8;
  uint16_t next = curr + 1;
  // Wrap next
  if (next == NUM_LEDS) { next = 0; }
  // Calc our fractional portion
  uint8_t frac = (index & 0xFF);

  leds[curr] = blend(leds[curr], color, (255 - frac));
  leds[next] = blend(leds[next], color, (frac));
}
