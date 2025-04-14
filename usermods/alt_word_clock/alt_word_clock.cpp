#include "alt_word_clock.h"

#define CW(POS, LEN, WORD) {POS, LEN},
static ClockWordParam clock_words[] = {WORD_PARAMS};
#undef CW

void Alt_Word_Clock::setup() {
  _brightness = 32;
}

void Alt_Word_Clock::loop() {
  if (!_enabled) {
    return;
  }

  uint32_t t_now = millis();
  if (t_now - _t_last_update < 1000) {
    return;
  }
  _t_last_update = t_now;

  if (_ldr_brightness_control) {
    uint8_t ambient_light_level = get_ambient_light_level();

    // The higher the ambient lighting, the lower the clock brightness.
    _brightness = 255 - ambient_light_level;
  }
  /*
  _brightness += 8;
  if (_brightness >= 240) {
    _brightness = 1;
  }
  */

  show_time(hourFormat12(localTime), minute(localTime));
}

void Alt_Word_Clock::highlight_clock_word(enum ClockWord clock_word) {
  ClockWordParam *w = &clock_words[clock_word];
  for (int i = 0; i < w->letter_count; i++) {
    _led_mask[w->start_led_pos + i] = 1;
  }
}

void Alt_Word_Clock::highlight_clock_words(enum ClockWord *words) {
  zero_mask();

  enum ClockWord clock_word = words[0];

  int i = 0;
  while (clock_word != NONE) {
    highlight_clock_word(clock_word);
    i++;
    clock_word = words[i];
  }
}

enum ClockWord Alt_Word_Clock::hour_to_word(int hour) {
  switch (hour) {
    case 1: return ONE;
    case 2: return TWO;
    case 3: return THREE;
    case 4: return FOUR;
    case 5: return FIVE;
    case 6: return SIX;
    case 7: return SEVEN;
    case 8: return EIGHT;
    case 9: return NINE;
    case 10: return TEN;
    case 11: return ELEVEN;
    case 12: return TWELVE;
    default: return ONE;
  }
}

void Alt_Word_Clock::show_time(int hour, int minute) {
  enum ClockWord sentence[9] = {IT, IS};
  int i = 2;
  bool after_half = false;
  bool on_the_hour = false;

  // 24-hour to 12-hour format
  if (hour > 12) {
    hour = hour - 12;
  }

  if (minute >= 0 && minute <= 3) {
    on_the_hour = true;
  }
  else if (minute >= 4 && minute <= 7) {
    sentence[i++] = FIVE_MINUTE;
    sentence[i++] = MINUTES;
    sentence[i++] = PAST;
  }
  else if (minute >= 8 && minute <= 12) {
    sentence[i++] = TEN_MINUTE;
    sentence[i++] = MINUTES;
    sentence[i++] = PAST;
  }
  else if (minute >= 13 && minute <= 17) {
    sentence[i++] = QUARTER;
    sentence[i++] = PAST;
  }
  else if (minute >= 18 && minute <= 22) {
    sentence[i++] = TWENTY;
    sentence[i++] = MINUTES;
    sentence[i++] = PAST;
  }
  else if (minute >= 23 && minute <= 27) {
    sentence[i++] = TWENTY;
    sentence[i++] = FIVE_MINUTE;
    sentence[i++] = MINUTES;
    sentence[i++] = PAST;
  }
  else if (minute >= 28 && minute <= 32) {
    sentence[i++] = HALF;
    sentence[i++] = PAST;
  }
  else if (minute >= 33 && minute <= 37) {
    after_half = true;
    sentence[i++] = TWENTY;
    sentence[i++] = FIVE_MINUTE;
    sentence[i++] = MINUTES;
    sentence[i++] = TO;
  }
  else if (minute >= 38 && minute <= 42) {
    after_half = true;
    sentence[i++] = TWENTY;
    sentence[i++] = MINUTES;
    sentence[i++] = TO;
  }
  else if (minute >= 43 && minute <= 47) {
    after_half = true;
    sentence[i++] = QUARTER;
    sentence[i++] = TO;
  }
  else if (minute >= 48 && minute <= 52) {
    after_half = true;
    sentence[i++] = TEN_MINUTE;
    sentence[i++] = MINUTES;
    sentence[i++] = TO;
  }
  else if (minute >= 53 && minute <= 57) {
    after_half = true;
    sentence[i++] = FIVE_MINUTE;
    sentence[i++] = MINUTES;
    sentence[i++] = TO;
  }
  else if (minute >= 58) {
    after_half = true;
    on_the_hour = true;
  }

  ClockWord hour_word;
  if (after_half)  {
    hour_word = hour_to_word(hour + 1);
  }
  else {
    hour_word = hour_to_word(hour);
  }
  sentence[i++] = hour_word;
  if (on_the_hour) {
    sentence[i++] = O_CLOCK;
  }
  sentence[i++] = NONE;
  highlight_clock_words(sentence);
}

void Alt_Word_Clock::zero_mask(void) {
  for (int i=0; i<_n_leds; i++) {
    _led_mask[i] = 0;
  }
}

void Alt_Word_Clock::handleOverlayDraw() {
  if (!_enabled) {
    return;
  }
  for (int i=0; i<_n_leds; i++) {
    if (_led_mask[i] == 0) {
      strip.setPixelColor(i, RGBW32(0, 0, 0, 0));
    }
  }
  if (_ldr_brightness_control) {
    strip.setBrightness(_brightness);
  }
}

void Alt_Word_Clock::addToConfig(JsonObject &root) {
  JsonObject top = root.createNestedObject(F("AltWordClock"));
  top[F("enabled")] = _enabled;
  top[F("LDRBrightnessControl")] = _ldr_brightness_control;
  top[F("LDRADCpin")] = _LDR_ADC_pin;
}

bool Alt_Word_Clock::readFromConfig(JsonObject &root) {
  JsonObject top = root[F("AltWordClock")];

  bool configComplete = !top.isNull();

  configComplete &= getJsonValue(top[F("enabled")], _enabled);
  configComplete &= getJsonValue(top[F("LDRBrightnessControl")], _ldr_brightness_control);
  configComplete &= getJsonValue(top[F("LDRADCpin")], _LDR_ADC_pin);

  return configComplete;
}

uint8_t Alt_Word_Clock::get_ambient_light_level(void) {
  uint16_t adc_millivolts = analogReadMilliVolts(_LDR_ADC_pin);

  // 0 - 3300 mV -> 0 - 255

  uint8_t level = (adc_millivolts * 255) / 3300;
  return level;
}

static Alt_Word_Clock alt_word_clock;
REGISTER_USERMOD(alt_word_clock);

