#include <pico/stdlib.h>
#include <robotarm/pin.h>

namespace robotarm {

Pin::Pin(int pin_number) : m_pin_number(pin_number) {
    if (!valid())
        return;
    
    gpio_init(m_pin_number);
    gpio_set_dir(m_pin_number, GPIO_OUT);  // default out
}

Pin Pin::get_default_led_pin() {
    return Pin(PICO_DEFAULT_LED_PIN);
}

Pin Pin::NONE() {
    return Pin(-1);
}

Pin::~Pin() {}

bool Pin::valid() const {
    return m_pin_number >= 0;
}

void Pin::set_as_input() {
    if (!valid())
        return;
    gpio_set_dir(m_pin_number, GPIO_IN);
}

void Pin::set_as_output() {
    if (!valid())
        return;
    gpio_set_dir(m_pin_number, GPIO_OUT);
}

void Pin::set_high() {
    if (!valid())
        return;
    gpio_put(m_pin_number, 1);
}

void Pin::set_low() {
    if (!valid())
        return;
    gpio_put(m_pin_number, 0);
}

bool Pin::is_high() {
    if (!valid())
        return false;
    return gpio_get(m_pin_number);
}

bool Pin::is_low() {
    if (!valid())
        return false;
    return !is_high();
}

void Pin::toggle() {
    if (!valid())
        return;
    if (is_high())
        set_low();
    else
        set_high();
}

int Pin::get_pin_number() {
    return m_pin_number;
}

}  // namespace robotarm