#pragma once

#include <robotarm/pin.h>

namespace robotarm {

// The default Pico LED.
class LED {
public:
    LED(const Pin &pin = Pin::get_default_led_pin());
    ~LED();

    void turn_on();
    void turn_off();

    void blink(unsigned int ms, bool final_sleep = false);

private:
    Pin m_pin;
};
}  // namespace robotarm