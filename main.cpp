#include <math.h>
#include <pico/stdlib.h>
#include <robotarm/LCD.h>
#include <robotarm/LED.h>
#include <robotarm/io.h>

#include <iostream>

int main(int argc, char const *argv[]) {
    robotarm::IO::init_io();

    uint16_t colors[] = {0xFFFF, 0x0000, 0x001F, 0XF81F, 0XFFE0, 0X07FF, 0xF800, 0xF81F, 0x07E0, 0x7FFF, 0xFFE0,
                         0XBC40, 0XFC07, 0X8430, 0X01CF, 0X7D7C, 0X5458, 0X841F, 0XC618, 0XA651, 0X2B12};

    robotarm::LED led;

    robotarm::Pin lcd_cs(9), lcd_res(10), lcd_dc(11), lcd_bl(4);
    robotarm::Pin lcd_clk(14), lcd_din(15);

    robotarm::Pin button1(5);
    robotarm::Pin button2(6);

    button1.set_as_input();
    button2.set_as_input();

    robotarm::LCD screen(lcd_cs, lcd_res, lcd_dc, lcd_bl, lcd_clk, lcd_din);
    screen.init();

    int curr_col = 0;

    screen.draw_text("Hello World!");

    screen.clear(robotarm::LCD_COLOR::WHITE);

    while (true) {
        bool b1 = button1.is_high();
        bool b2 = button2.is_high();

        if (b1) {
            curr_col++;
            if (curr_col >= sizeof(colors) / sizeof(uint16_t)) {
                curr_col = 0;
            }
            screen.clear(colors[curr_col]);
        } else if (b2) {
            curr_col--;
            if (curr_col < 0) {
                curr_col = sizeof(colors) / sizeof(uint16_t) - 1;
            }
            screen.clear(colors[curr_col]);
        }

        //double s = 300. * (sin(loop) + 1.01);  // in [0, 2]
        //led.blink(100, true);

        //std::cout << "Blinking for " << s << " seconds" << std::endl;

        //loop += 0.1;

        screen.update();  // is this a problem that this is not called at a high frequency?
    }

    /*
    robotarm::Pin led1(20), led2(21), led3(22), led4(26);
    robotarm::Pin buzzer(27);
    robotarm::Pin button1(16), button2(17), button3(2), button4(19);

    button1.set_as_input();
    button2.set_as_input();
    button3.set_as_input();
    button4.set_as_input();

    while (true) {
        if (button1.is_high()) {
            led1.set_high();
        } else {
            led1.set_low();
        }

        if (button2.is_high()) {
            led2.set_high();
        } else {
            led2.set_low();
        }

        if (button3.is_high()) {
            led3.set_high();
        } else {
            led3.set_low();
        }

        if (button4.is_high()) {
            led4.set_high();
        } else {
            led4.set_low();
        }

        // sum up all buttons
        // if >= 2, buzzer.set_high();
        // else buzzer.set_low();
        if (button1.is_high() + button2.is_high() + button3.is_high() + button4.is_high() >= 2) {
            buzzer.set_high();
        } else {
            buzzer.set_low();
        }

        sleep_ms(50); // TODO: add to API somewhere
    }
    */

    return 0;
}
