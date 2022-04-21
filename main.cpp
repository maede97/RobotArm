#include <math.h>
#include <pico/stdlib.h>
#include <robotarm/LCD.h>
#include <robotarm/LED.h>
#include <robotarm/io.h>
#include <robotarm/menu.h>

#include <iostream>

int main(int argc, char const *argv[]) {
    using ScreenPtr = std::shared_ptr<robotarm::Screen>;

    robotarm::IO::init_io();

    robotarm::LED led;

    robotarm::Pin lcd_cs(9), lcd_res(10), lcd_dc(11), lcd_bl(4), lcd_clk(14), lcd_din(15);

    robotarm::Pin button1(5);
    robotarm::Pin button2(6);
    robotarm::Pin button3(7);
    robotarm::Pin button4(8);

    robotarm::LCD lcd(lcd_cs, lcd_res, lcd_dc, lcd_bl, lcd_clk, lcd_din);

    robotarm::Menu menu(lcd, button1, button2, button3, button4);

    ScreenPtr screen_1 = std::make_shared<robotarm::Screen>("Main Menu", "What do you want to do?");
    screen_1->register_callback_1("Blink", [&led]() { led.blink(500, true); });
    screen_1->register_callback_2("FG", [&menu]() { menu.show_screen(1); });
    screen_1->register_callback_3("BG", [&menu]() { menu.show_screen(2); });
    screen_1->register_callback_4("Reset", [&menu]() {
        menu.set_background_color(robotarm::LCD_COLOR::BLACK);
        menu.set_text_color(robotarm::LCD_COLOR::WHITE);
    });

    ScreenPtr screen_2 = std::make_shared<robotarm::Screen>("Foreground", "Choose Text Color");
    screen_2->register_callback_1("Green", [&menu]() { menu.set_text_color(robotarm::LCD_COLOR::GREEN); });
    screen_2->register_callback_2("Yellow", [&menu]() { menu.set_text_color(robotarm::LCD_COLOR::YELLOW); });
    screen_2->register_callback_3("MAIN", [&menu]() { menu.show_screen(0); });
    screen_2->register_callback_4("BG", [&menu]() { menu.show_screen(2); });

    ScreenPtr screen_3 = std::make_shared<robotarm::Screen>("Background", "Choose BG Color");
    screen_3->register_callback_1("Red", [&menu]() { menu.set_background_color(robotarm::LCD_COLOR::RED); });
    screen_3->register_callback_2("Blue", [&menu]() { menu.set_background_color(robotarm::LCD_COLOR::BLUE); });
    screen_3->register_callback_3("MAIN", [&menu]() { menu.show_screen(0); });
    screen_3->register_callback_4("FG", [&menu]() { menu.show_screen(1); });

    menu.add_screen(screen_1);
    menu.add_screen(screen_2);
    menu.add_screen(screen_3);

    while (true) {
        menu.update();
    }

    return 0;
}
