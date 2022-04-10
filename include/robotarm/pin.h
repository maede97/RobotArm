#pragma once

namespace robotarm {

class Pin {
public:
    Pin(int pin_number);
    static Pin get_default_led_pin();

    ~Pin();

    void set_as_output();
    void set_as_input();

    void set_high();
    void set_low();

    bool is_high();
    bool is_low();

    void toggle();

    int get_pin_number();

private:
    int m_pin_number;
};

}  // namespace robotarm