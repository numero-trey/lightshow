// SkullOS
// Skull light show

#include <FastLED.h>

//Config
#define OUT_DATA 0
#define NUM_LEDS 12

#define FPS 100
#define FRAME_DELAY 1000 / FPS
#define NUM_SPARKS 2

#define BG_STEP_SIZE 12
#define BG_HUE_WIDTH 10
#define BG_SAT_WIDTH 48
#define BG_VAL_WIDTH 128

#define HUE_HAPPY 24
#define HUE_SAD 135

CRGB leds[NUM_LEDS];
uint16_t pos;

uint8_t pattern[NUM_LEDS][3]; // H, S, V

uint16_t mood = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#include "spark.h"
#include "color.h"

Sparkler sparks[NUM_SPARKS];

void setup() {
  //Serial.begin(57600);
  // Setup LED driver
  FastLED.addLeds<NEOPIXEL, OUT_DATA>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

//  pinMode(5, INPUT);
//  digitalWrite(5, HIGH);

  // Get our house in order before we begin
  //doBrightness();
  //FastLED.setDither( 0 );
  FastLED.setBrightness(192);
  random16_add_entropy(analogRead(1) << 4);

  // Init sparks with our LED array
  for (uint8_t i=0; i < NUM_SPARKS; ++i) {
    sparks[i].init(leds, NUM_LEDS);
  }

  initPattern();
}

void loop() {
//  if (digitalRead(5)) {
//    mood = 11 << 8;
//  } else {
//    mood = 0;
//  }
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
  FastLED.delay(FRAME_DELAY);
}

////////////////////////////////////////////////////////
// Show functions
//
// Each call should be one frame


void doBG() {  
  // Background
  uint8_t fract = pos & 0xFF;

  for (int i=0; i < NUM_LEDS; ++i) {
    uint8_t curr = wrapPos(i + (pos >> 8));
    uint8_t nxt = wrapPos(curr + 1);
    
    leds[i] = blend(
      bgClr(getMood(i), curr),
      bgClr(getMood(i), nxt),
      fract
    );

    //leds[i] = bgClr(0, nxt);
  } // each led
  
  //fill_solid(leds, NUM_LEDS, CHSV(64, 255, 232));
  //fill_solid(leds, NUM_LEDS, CRGB::Black);

  pos += BG_STEP_SIZE;
  if (pos >= NUM_LEDS << 8 ) { pos -= (NUM_LEDS << 8); }
}

void doSparks() {
  for (int i=0; i < NUM_SPARKS; i++) {
    sparks[i].doFrame();
  }
}

//// Set a pixel with subpixel rendering.
//void setSub(struct CRGB * leds, uint16_t index, CRGB color) {
//  // Calc our base pixel and secondary
//  uint16_t curr = index >> 8;
//  uint16_t next = curr + 1;
//  // Wrap next
//  if (next == NUM_LEDS) { next = 0; }
//  // Calc our fractional portion
//  uint8_t frac = (index & 0xFF);
//
//  leds[curr] = blend(leds[curr], color, (255 - frac));
//  leds[next] = blend(leds[next], color, (frac));
//}

uint8_t wrapPos(uint8_t p) {
  if (p >= NUM_LEDS) {
    p -= NUM_LEDS;
  }
  return p;
}
