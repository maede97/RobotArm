#pragma once

#include <robotarm/pin.h>

#include <string>

namespace robotarm {

enum class LCD_COMMANDS {
    NOP = 0x00,
    SWRESET = 0x01,

    SLPIN = 0x10,
    SLPOUT = 0x11,  // Sleep out
    NORON = 0x13,   // Partial Off (Normal)

    INVOFF = 0x20,   // Display Inversion Off
    INVON = 0x21,    // Display Inversion On
    DISPOFF = 0x28,  // Display Off
    DISPON = 0x29,   // Display On

    CASET = 0x2A,  // Column Address set
    RASET = 0x2B,  // Row Address set
    RAMWR = 0x2C,  // Memory Write

    MADCTL = 0x36,  // Memory data access control
    COLMOD = 0x3A,  // Interface pixel format

    FRMCTR1 = 0xB1,  // Frame rate control?
    FRMCTR2 = 0xB2,  // Frame rate control?
    FRMCTR3 = 0xB3,  // Frame rate control?
    INVCTR = 0xB4,   // Display inversion control
    DISSET5 = 0xB6,  // ???

    PWCTR1 = 0xC0,  // Power control
    PWCTR2 = 0xC1,  // Power control
    PWCTR3 = 0xC2,  // Power control
    PWCTR4 = 0xC3,  // Power control
    PWCTR5 = 0xC4,  // Power control
    VMCTR1 = 0xC5,  // VCOM control

    GMCTRP1 = 0xE0,  // Gamma control
    GMCTRN1 = 0xE1,  // Gamma control

    PWCTR6 = 0xFC,  // Power control
};

enum class LCD_COLOR {
    WHITE = 0xFFFF,
    BLACK = 0x0000,
    BLUE = 0x001F,
    BRED = 0XF81F,
    GRED = 0XFFE0,
    GBLUE = 0X07FF,
    RED = 0xF800,
    MAGENTA = 0xF81F,
    GREEN = 0x07E0,
    CYAN = 0x7FFF,
    YELLOW = 0xFFE0,
    BROWN = 0XBC40,
    BRRED = 0XFC07,
    GRAY = 0X8430,
    DARKBLUE = 0X01CF,
    LIGHTBLUE = 0X7D7C,
    GRAYBLUE = 0X5458,
    LIGHTGREEN = 0X841F,
    LGRAY = 0XC618,
    LGRAYBLUE = 0XA651,
    LBBLUE = 0X2B12
};

/**
 * @brief Interface for ST7735S,
 * https://www.waveshare.com/0.96inch-lcd-module.htm
 * with 160x80 pixels, RGB (65k colors) with backlight
 * 
 * this code is heavily inspired by https://github.com/bablokb/pico-st7735/
 * 
 * we use SPI1 for communication with the LCD! --> select pins that work for SPI1!
 */
class LCD {
public:
    const uint8_t LCD_WIDTH = 160;
    const uint8_t LCD_HEIGHT = 80;

    LCD(const Pin& CS /* chip select */, const Pin& RST /* reset */, const Pin& DC /* data / command */, const Pin& BL /* backlight */, const Pin& CLK = Pin(14) /* SPI clock */, const Pin& DIN = Pin(15) /* SPI TX */);

    void init();

    void draw_text(std::string text);

    void clear(LCD_COLOR color);
    void clear(uint16_t color);

    void update();

private:
    void write_command(LCD_COMMANDS command);
    void write_data(uint8_t data);

    void delay(uint32_t ms);
    void reset();
    void set_cursor(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);

private:
    Pin m_SCL;
    Pin m_RX;
    Pin m_TX;
    Pin m_CS;
    Pin m_RES;
    Pin m_DC;
    Pin m_BL;

    std::string m_text;

    bool m_use_spi_1;
};

}  // namespace robotarm