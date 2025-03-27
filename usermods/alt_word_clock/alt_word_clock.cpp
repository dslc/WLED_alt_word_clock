#include "alt_word_clock.h"

void Alt_Word_Clock::setup() {

}

void Alt_Word_Clock::loop() {
  if (!_enabled) {
    return;
  }

  uint32_t t_now = millis();
  if (t_now - _t_last_update < 5000) {
    return;
  }
  _t_last_update = t_now;

  if (_leds_on) {
    _leds_on = false;
  }
  else {
    _leds_on = true;
  }
}

void Alt_Word_Clock::turn_leds_on(void) {
  for (int i=0; i<_n_leds; i++) {
    strip.setPixelColor(i, RGBW32(0, 0, 255, 255));
  }
}

void Alt_Word_Clock::turn_leds_off(void) {
  for (int i=0; i<_n_leds; i++) {
    strip.setPixelColor(i, RGBW32(0, 0, 0, 0));
  }
}

void Alt_Word_Clock::handleOverlayDraw() {
  if (!_enabled) {
    return;
  }
  if (_leds_on) {
    turn_leds_on();
  }
  else {
    turn_leds_off();
  }
}

void Alt_Word_Clock::addToConfig(JsonObject &root) {
  JsonObject top = root.createNestedObject(F("AltWordClock"));
  top[F("enabled")] = _enabled;
}

bool Alt_Word_Clock::readFromConfig(JsonObject &root) {
  JsonObject top = root[F("AltWordClock")];

  bool configComplete = !top.isNull();

  configComplete &= getJsonValue(top[F("enabled")], _enabled);

  return configComplete;
}

static Alt_Word_Clock alt_word_clock;
REGISTER_USERMOD(alt_word_clock);

