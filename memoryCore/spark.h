#define FADE_STEPS 4

class Sparkler {
  public:
    CRGB *outputs;
    uint16_t num_leds;
    
    CRGB color;
    uint8_t strength = 255;
    uint16_t life;
    uint16_t pos;
    int8_t dir;
    uint8_t width;
    
    void doFrame(void);
    boolean isActive(void);
    void start(CRGB clr, uint8_t str, uint8_t wdth, int8_t dr, uint16_t lfe);
    void init(CRGB *leds, uint16_t num);
    
  private:
    uint8_t last_pos;
    uint16_t age;
    void draw(void);
    uint16_t wrapPos(uint16_t p);
};

boolean Sparkler::isActive(void) {
  return life > 0;
}

void Sparkler::doFrame(void) {
  // Bail if we are not active
  if (!this->isActive()) {
    return;
  }
  
  // Draw segment
  this->draw();
  
  // Update vars
  pos += dir;
  if (pos >= num_leds << 8) { pos -= num_leds << 8; }
  // Update life & age on whole pixel movements
  if (pos >> 8 != last_pos) {
    --life;
    ++age;
    last_pos = pos >> 8;
  }
  
}

void Sparkler::start(CRGB clr, uint8_t str, uint8_t wdth, int8_t dr, uint16_t lfe) {
  color = clr;
  width = wdth;
  dir = dr;
  life = lfe;
  strength = str;
  age = 0;
  pos = 0;
}

// Wraps around LED indices that are past the end of the string
uint16_t Sparkler::wrapPos(uint16_t p) {
  if (p >= num_leds) {
    p -= num_leds;
  }
  return p;
}

void Sparkler::draw(void) {
  uint16_t s; // Scratch pad
  uint8_t b; // blend scratch
  
  // First LED
  uint16_t curr = pos >> 8;
  
  // Calc fade in/out
  uint8_t fade = strength;
  if (age < FADE_STEPS) {
    fade = scale8(
      ease8InOutApprox(pos / FADE_STEPS),
      strength
    );
  }
  if (life < FADE_STEPS + 1) {
    fade = scale8(
      ease8InOutApprox(((life << 8) - FADE_STEPS - (pos & 0xFF)) / FADE_STEPS),
      strength
    );
  }

  // Calc our fractional portion
  uint8_t frac = (pos & 0xFF);
  
  // Set first LEDs
  b = scale8(fade, 255 - frac);
  outputs[curr] = blend(outputs[curr], color, b);
  // Set last LED
  s = this->wrapPos(curr + width);
  b = scale8(fade, frac);
  outputs[s] = blend(outputs[s], color, b);
  
  // Fill gap
  for (uint8_t i = 1; i < width; ++i) {
    s = this->wrapPos(curr + i);
    outputs[s] = blend(outputs[s], color, fade);
  }
}

void Sparkler::init(CRGB *leds, uint16_t num) {
  outputs = leds;
  num_leds = num;
}
