#include <robotarm/pin.h>
#include <pico/stdlib.h>

namespace robotarm
{

    Pin::Pin(int pin_number)
        : m_pin_number(pin_number)
    {
        gpio_init(m_pin_number);
        gpio_set_dir(m_pin_number, GPIO_OUT); // default out
    }

    Pin Pin::get_default_led_pin()
    {
        return Pin(PICO_DEFAULT_LED_PIN);
    }

    Pin::~Pin()
    {
    }

    void Pin::set_as_input()
    {
        gpio_set_dir(m_pin_number, GPIO_IN);
    }

    void Pin::set_as_output()
    {
        gpio_set_dir(m_pin_number, GPIO_OUT);
    }

    void Pin::set_high()
    {
        gpio_put(m_pin_number, 1);
    }

    void Pin::set_low()
    {
        gpio_put(m_pin_number, 0);
    }

    bool Pin::is_high()
    {
        return gpio_get(m_pin_number);
    }

    bool Pin::is_low()
    {
        return !is_high();
    }

    void Pin::toggle()
    {
        if (is_high())
            set_low();
        else
            set_high();
    }

    int Pin::get_pin_number()
    {
        return m_pin_number;
    }

}