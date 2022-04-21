#include <robotarm/menu.h>

namespace robotarm {

Screen::Screen(std::string title, std::string content) : m_title(title), m_content(content) {
    m_callbacks.resize(4, []() {});  // empty lambdas
    m_callback_strings.resize(4, "");
}

void Screen::register_callback_1(std::string text, std::function<void()> callback) {
    m_callbacks[0] = callback;
    m_callback_strings[0] = text;
}

void Screen::register_callback_2(std::string text, std::function<void()> callback) {
    m_callbacks[1] = callback;
    m_callback_strings[1] = text;
}

void Screen::register_callback_3(std::string text, std::function<void()> callback) {
    m_callbacks[2] = callback;
    m_callback_strings[2] = text;
}

void Screen::register_callback_4(std::string text, std::function<void()> callback) {
    m_callbacks[3] = callback;
    m_callback_strings[3] = text;
}

void Screen::callback_1() {
    m_callbacks[0]();
}

void Screen::callback_2() {
    m_callbacks[1]();
}

void Screen::callback_3() {
    m_callbacks[2]();
}

void Screen::callback_4() {
    m_callbacks[3]();
}

std::string Screen::callback_string_1() const {
    return m_callback_strings[0];
}

std::string Screen::callback_string_2() const {
    return m_callback_strings[1];
}

std::string Screen::callback_string_3() const {
    return m_callback_strings[2];
}

std::string Screen::callback_string_4() const {
    return m_callback_strings[3];
}

std::string Screen::title() const {
    return m_title;
}

std::string Screen::content() const {
    return m_content;
}

Menu::Menu(LCD& lcd, const Pin& button1, const Pin& button2, const Pin& button3, const Pin& button4)
    : m_lcd(lcd), m_button1(button1), m_button2(button2), m_button3(button3), m_button4(button4) {
    init();
}

void Menu::init() {
    m_button1.set_as_input();
    m_button2.set_as_input();
    m_button3.set_as_input();
    m_button4.set_as_input();

    m_lcd.init();

    m_lcd.clear(m_background_color);
}

unsigned Menu::add_screen(std::shared_ptr<Screen> screen) {
    m_screens.push_back(screen);
    return m_screens.size() - 1;
}

void Menu::set_background_color(LCD_COLOR color) {
    m_background_color = color;
    m_lcd.clear(m_background_color);
    m_force_redraw = true;
}

void Menu::set_text_color(LCD_COLOR color) {
    m_text_color = color;
    m_force_redraw = true;
}

void Menu::update() {
    if (m_screens.size() == 0) {
        return;
    }

    if (m_current_screen_id >= m_screens.size()) {
        m_current_screen_id = 0;
        show_screen(m_current_screen_id);
    }

    bool b1 = m_button1.is_high();
    bool b2 = m_button2.is_high();
    bool b3 = m_button3.is_high();
    bool b4 = m_button4.is_high();

    if (b1 && !m_button1_pressed) {
        m_button1_pressed = true;
        m_screens[m_current_screen_id]->callback_1();
    } else if (!b1) {
        m_button1_pressed = false;
    }

    if (b2 && !m_button2_pressed) {
        m_button2_pressed = true;
        m_screens[m_current_screen_id]->callback_2();
    } else if (!b2) {
        m_button2_pressed = false;
    }

    if (b3 && !m_button3_pressed) {
        m_button3_pressed = true;
        m_screens[m_current_screen_id]->callback_3();
    } else if (!b3) {
        m_button3_pressed = false;
    }

    if (b4 && !m_button4_pressed) {
        m_button4_pressed = true;
        m_screens[m_current_screen_id]->callback_4();
    } else if (!b4) {
        m_button4_pressed = false;
    }

    if(m_force_redraw) {
        show_screen(m_current_screen_id);
        m_force_redraw = false;
    }
}

void Menu::show_screen(unsigned screen_id) {
    if (screen_id >= m_screens.size()) {
        return;
    }

    m_current_screen_id = screen_id;

    m_lcd.clear(m_background_color);
    uint8_t spacing = 3;

    m_lcd.draw_text(0, 0, m_screens[m_current_screen_id]->title(), 3, false, m_text_color, m_background_color);
    m_lcd.fill_rect(0, 26, 160, 1, m_text_color);

    m_lcd.draw_text(0, 28, m_screens[m_current_screen_id]->content(), 2, true, m_text_color, m_background_color);

    // draw the button texts
    if (m_screens[m_current_screen_id]->callback_string_1().length() > 0)
        m_lcd.draw_text(0, 70, "1) " + m_screens[m_current_screen_id]->callback_string_1(), 1, false, m_text_color, m_background_color);
    if (m_screens[m_current_screen_id]->callback_string_2().length() > 0)
        m_lcd.draw_text(40, 70, "2) " + m_screens[m_current_screen_id]->callback_string_2(), 1, false, m_text_color, m_background_color);
    if (m_screens[m_current_screen_id]->callback_string_3().length() > 0)
        m_lcd.draw_text(80, 70, "3) " + m_screens[m_current_screen_id]->callback_string_3(), 1, false, m_text_color, m_background_color);
    if (m_screens[m_current_screen_id]->callback_string_4().length() > 0)
        m_lcd.draw_text(120, 70, "4) " + m_screens[m_current_screen_id]->callback_string_4(), 1, false, m_text_color, m_background_color);
}

}  // namespace robotarm
