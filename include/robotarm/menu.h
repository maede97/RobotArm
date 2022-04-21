#pragma once

#include <robotarm/LCD.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace robotarm {

class Screen {
public:
    Screen(std::string title, std::string content);
    virtual ~Screen() = default;

    void register_callback_1(std::string text, std::function<void()> callback);
    void register_callback_2(std::string text, std::function<void()> callback);
    void register_callback_3(std::string text, std::function<void()> callback);
    void register_callback_4(std::string text, std::function<void()> callback);

    void callback_1();
    void callback_2();
    void callback_3();
    void callback_4();

    std::string callback_string_1() const;
    std::string callback_string_2() const;
    std::string callback_string_3() const;
    std::string callback_string_4() const;

    std::string title() const;
    std::string content() const;

protected:
    std::string m_title;
    std::string m_content;
    std::vector<std::string> m_callback_strings;
    std::vector<std::function<void()>> m_callbacks;
};

class Menu {
public:
    Menu(LCD& lcd, const Pin& button1, const Pin& button2, const Pin& button3, const Pin& button4);
    unsigned add_screen(std::shared_ptr<Screen> screen);

    // performs redraw
    void show_screen(unsigned screen_id);

    void set_background_color(LCD_COLOR color);  // will clear now, but not redraw textssssss
    void set_text_color(LCD_COLOR color);        // will be applied at next update

    void update();

private:
    void init();

private:
    LCD& m_lcd;
    Pin m_button1;
    Pin m_button2;
    Pin m_button3;
    Pin m_button4;

    bool m_button1_pressed;
    bool m_button2_pressed;
    bool m_button3_pressed;
    bool m_button4_pressed;

    unsigned m_current_screen_id = -1;  // will be big and, at first call to update, draw first screen

    std::vector<std::shared_ptr<Screen>> m_screens;

    LCD_COLOR m_background_color = LCD_COLOR::BLACK;
    LCD_COLOR m_text_color = LCD_COLOR::WHITE;

    bool m_force_redraw = false;
};

}  // namespace robotarm