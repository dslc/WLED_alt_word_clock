#ifndef _USERMOD_WORD_CLOCK_2_H_
#define _USERMOD_WORD_CLOCK_2_H_

#include "wled.h"

#define MAX_WORD_CLOCK_LEDS 144

/**
 * Word parameters
 */
struct ClockWordParam {
  uint8_t start_led_pos;
  uint8_t letter_count;
};

#define WORD_PARAMS       \
  CW(0, 0, NONE)         \
  CW(1, 7, O_CLOCK)      \
  CW(8, 1, SNOWFLK11)    \
  CW(9, 1, SNOWFLK10)    \
  CW(10, 6, TWELVE)      \
  CW(16, 6, ELEVEN)      \
  CW(22, 3, SIX)         \
  CW(25, 1, SNOWFLK9)    \
  CW(26, 4, NINE)        \
  CW(30, 5, EIGHT)       \
  CW(35, 1, SNOWFLK8)    \
  CW(36, 3, TEN)         \
  CW(39, 5, SEVEN)       \
  CW(44, 5, THREE)       \
  CW(49, 1, SNOWFLK7)    \
  CW(50, 4, FOUR)        \
  CW(54, 4, FIVE)        \
  CW(58, 8, SCHNAPPS)    \
  CW(66, 3, TWO)         \
  CW(69, 1, SNOWFLK6)    \
  CW(70, 3, ONE)         \
  CW(73, 2, TO)          \
  CW(75, 4, PAST)        \
  CW(79, 1, SNOWFLK5)    \
  CW(80, 7, MINUTES)     \
  CW(87, 4, FIVE_MINUTE) \
  CW(91, 1, SNOWFLK4)    \
  CW(93, 6, TWENTY)      \
  CW(99, 7, QUARTER)     \
  CW(106, 1, SNOWFLK3)   \
  CW(107, 3, TEN_MINUTE) \
  CW(110, 1, SNOWFLK2)   \
  CW(111, 4, HALF)       \
  CW(115, 2, IT)         \
  CW(117, 2, IS)         \
  CW(119, 1, SNOWFLK1)

#define CW(POS, LEN, WORD) WORD,
enum ClockWord { WORD_PARAMS };
#undef CW

class Alt_Word_Clock : public Usermod {

  private:
    bool _enabled;
    uint32_t _t_last_update;
    bool _leds_on;
    uint32_t _n_leds = 120;
    uint16_t _pos = 0;
    uint8_t _led_mask[MAX_WORD_CLOCK_LEDS];

    void zero_mask(void);
    void turn_leds_on(void);
    void turn_leds_off(void);

    enum ClockWord hour_to_word(int hour);

    void highlight_clock_word(enum ClockWord clock_word);
    void highlight_clock_words(enum ClockWord *words);
    void show_time(int hour, int minute);

  public:
    void setup() override;

    void loop() override;

    void handleOverlayDraw() override;

    void addToConfig(JsonObject &root) override;
    bool readFromConfig(JsonObject &root) override;

};

#endif
