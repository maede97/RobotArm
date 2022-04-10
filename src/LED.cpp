#include <pico/stdlib.h>
#include <robotarm/LED.h>

namespace robotarm {

LED::LED(const Pin &pin) : m_pin(pin) {
    m_pin.set_as_output();
}

LED::~LED() {}

void LED::turn_on() {
    m_pin.set_high();
}

void LED::turn_off() {
    m_pin.set_low();
}

void LED::blink(unsigned int ms, bool final_sleep) {
    turn_on();
    sleep_ms(ms);
    turn_off();
    if (final_sleep)
        sleep_ms(ms);
}
}  // namespace robotarm