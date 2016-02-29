/*
 * Crystalist
 * (c)2016 Trey Chandler
 * 
*/

#include <FastLED.h>

#define OUTPIN 10
#define NUM_LEDS 12
#define BRT_PIN 1
#define FPS 120

CRGB leds[NUM_LEDS];
int lastBrightness = 255;

uint16_t chroma = 0;
uint16_t pos = 0;

void setup() {
  FastLED.addLeds<NEOPIXEL, OUTPIN>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
  //doBrightness();
  //fill_solid(leds, NUM_LEDS, CRGB::White); 
  //FastLED.show();
}



void loop() {
  // put your main code here, to run repeatedly:
  //spinFade();
  spin();
  
  FastLED.show();
  delay(1000 / FPS);

  //chroma += 4;
  
}

void strobeAll() {
  setAll(CRGB::Indigo); 
  delay(6);
  
  setAll(CRGB::Black);
  delay(100);
}

void rStrobe() {
  int l = random16(NUM_LEDS);
  leds[l] = CRGB::Red;
  FastLED.show();
  delay(5);

  leds[l] = CRGB::Black;
  FastLED.show();
  delay(75);
}

void spin() {
  fill_solid(leds, NUM_LEDS, CRGB::Black); 
  setSub(pos, CHSV(chroma >> 8, 255, 255));
  FastLED.show();
  pos += 16;
  pos = (pos >= NUM_LEDS << 8) ? pos - (NUM_LEDS << 8): pos;
  chroma += 8;
}

void spinFade() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t frac = brighten8_video(pos & 0xFF);
  uint8_t whole = pos >> 8;
  uint8_t last = (whole == 0) ? NUM_LEDS: whole - 1;

  leds[whole] = CHSV(chroma >> 8, 255, frac);
  leds[last] = CHSV(chroma >> 8, 255, 255);
  
  pos += 32;
  pos = (pos >= NUM_LEDS << 8) ? pos - (NUM_LEDS << 8): pos;

  chroma += 8;
}

void setAll(CRGB c) {
  for(int i=0;i < NUM_LEDS; i++) {
    leds[i] = c;
  }
  FastLED.show(); 
}

// Poll A0 and update brightness if needed
void doBrightness() {
  int val = analogRead(BRT_PIN) >> 2;
  if (val != lastBrightness) {
    FastLED.setBrightness(val);
    lastBrightness = val;
  }
}

void setSub(uint16_t pos, CRGB c) {
  uint8_t b; // blend scratch
  uint8_t frac = (pos & 0xFF);
  uint8_t whole = pos >> 8;
  uint8_t next = whole + 1;
  if (next >= NUM_LEDS) {
    next -= NUM_LEDS;
  }
  b = dim8_video(frac);
  leds[whole] = blend(leds[whole], c, 255 - b);
  //b = dim8_video(frac);
  leds[next] = blend(leds[next], c, b);
}
