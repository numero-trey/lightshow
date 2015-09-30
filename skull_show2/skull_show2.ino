#include <FastLED.h>

#define OUTPIN 11
#define SKULLS 8
#define S_CHNLS 1
#define NUM_LEDS SKULLS * S_CHNLS
#define BRT_PIN 0

CRGB leds[NUM_LEDS];
int lastBrightness = 255;

void setup() {
  FastLED.addLeds<NEOPIXEL, OUTPIN>(leds, NUM_LEDS);
  doBrightness();
}

void loop() {
  // put your main code here, to run repeatedly:
  rStrobe();
}

void wiper() {
  
}

// Strobe all lights
void strobeAll() {
  setAll(CRGB::Indigo); 
  delay(5);
  
  setAll(CRGB::Black);
  delay(50);
}

// Flash random lights
void rStrobe() {
  int l = random16(SKULLS);
  setSkull(l, CRGB::Red);
  FastLED.show();
  delay(5);

  setSkull(l, CRGB::Black);
  FastLED.show();
  delay(75);
}

void setSkull(uint16_t ch, CRGB c) {
  for(int i=0; i < S_CHNLS; i++) {
    leds[ch + i] = c;
  }
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
