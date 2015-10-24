#define NUM_LEDS 12
uint8_t bgHue(uint8_t mood, uint8_t offset) {
  
}

#define BASE_LED 2
#define MOOD_STEPS = NUM_LEDS / BASE_LED
void doSad(uint16_t step) {
  uint8_t start_led;
  uint8_t end_led;

  if (step > BASE_LED) {
    start_led = BASE_LED - step;
  } else {
    start_led = NUM_LEDS - step + BASE_LED;
  }

  if (step < NUM_LEDS - BASE_LED) {
    end_led = NUM_LEDS - step;
  } else {
    end_led = step - (NUM_LEDS - BASE_LED);
  }
}

/*
Start at selected LED.


*/
