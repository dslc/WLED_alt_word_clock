#ifndef _USERMOD_WORD_CLOCK_2_H_
#define _USERMOD_WORD_CLOCK_2_H_

#include "wled.h"

class Alt_Word_Clock : public Usermod {

  private:
    bool _enabled;
    uint32_t _t_last_update;
    bool _leds_on;
    uint32_t _n_leds = 20;

    void turn_leds_on(void);
    void turn_leds_off(void);

  public:
    void setup() override;

    void loop() override;

    void handleOverlayDraw() override;

    void addToConfig(JsonObject &root) override;
    bool readFromConfig(JsonObject &root) override;

};

#endif
