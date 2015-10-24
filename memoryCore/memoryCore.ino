// SkullOS
// Skull light show

#include <FastLED.h>
#include "spark.h"
#include "color.h"

//Config
#define OUT_DATA 0
#define NUM_LEDS 12

#define FPS 125
#define NUM_SPARKS 2

#define BG_HUE_WIDTH 12
#define HUE_HAPPY 24
#define HUE_SAD 135

CRGB leds[NUM_LEDS];
Sparkler sparks[NUM_SPARKS];
uint16_t pos;
uint8_t bg_hue = HUE_SAD;
uint16_t mood = 0; // 0=happy, 512=sad

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup() {
  //Serial.begin(57600);
  // Setup LED driver
  FastLED.addLeds<NEOPIXEL, OUT_DATA>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // Get our house in order before we begin
  //doBrightness();
  //FastLED.setDither( 0 );
  FastLED.setBrightness(128);
  random16_add_entropy(analogRead(1) << 4);

  // Init sparks with our LED array
  for (uint8_t i=0; i < NUM_SPARKS; ++i) {
    sparks[i].init(leds, NUM_LEDS);
  }

  
//  sparks[1].start(
//    CRGB::Orange, // color
//    3,           // width
//    4,          // direction/speed
//    255            // lifespan
//  );
//  
  //initSpark(0, 6, CHSV(60, 127, 255)); //Sad: 140
  //initSpark(1, 8, CRGB::Yellow);
  //initSpark(1, 4, CRGB::Orange);
}

void loop() {
  // Call next frame
  doBG();
  //fill_solid(leds, NUM_LEDS, CRGB::Black);
  doSparks();

  EVERY_N_SECONDS( 6 ) { 
    sparks[0].start(
      CRGB::White,  // color
      100,          // strength
      1,            // width
      72,           // direction/speed
      16             // lifespan
    );
  }

  // Update brightness
  //EVERY_N_SECONDS( 1 ) { doBrightness(); }
  //EVERY_N_SECONDS( 10) { switchAnimations(); }
  FastLED.show();
  FastLED.delay(1000 / FPS);
}

////////////////////////////////////////////////////////
// Show functions
//
// Each call should be one frame

#define STEP_SIZE 255 / NUM_LEDS
void doBG() {  
  // Background

  uint8_t hue = pos >> 4;
  for (int i=0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(
      map8(triwave8(hue), bg_hue, bg_hue + BG_HUE_WIDTH), 
      255, 
      255
    );
    hue += STEP_SIZE;
  }
  
  //fill_solid(leds, NUM_LEDS, CHSV(64, 255, 232));
  //fill_solid(leds, NUM_LEDS, CRGB::Black);

  pos += 6;
  if (pos >= 256 << 4 ) { pos -= 256 << 4 ; }
}

void doSparks() {
  for (int i=0; i < NUM_SPARKS; i++) {
    sparks[i].doFrame();
  }
}

void lightSparks() {
  
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
