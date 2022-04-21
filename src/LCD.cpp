#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <robotarm/LCD.h>

/**
 * Font stuff is from https://github.com/gavinlyonsrepo/pic_16F18346_projects
 */

static const char Font_Tiny[] = {0x00, 0x00, 0x00,  //
                                 0x00, 0x2e, 0x00,  // !
                                 0x06, 0x00, 0x06,  // "
                                 0x3e, 0x14, 0x3e,  // #
                                 0x0c, 0x36, 0x18,  // $
                                 0x32, 0x08, 0x26,  // %
                                 0x14, 0x2a, 0x34,  // &
                                 0x00, 0x06, 0x00,  // '
                                 0x00, 0x1c, 0x22,  // (
                                 0x22, 0x1c, 0x00,  // )
                                 0x14, 0x08, 0x14,  // *
                                 0x08, 0x1c, 0x08,  // +
                                 0x40, 0x60, 0x00,  // ,
                                 0x08, 0x08, 0x08,  // -
                                 0x00, 0x20, 0x00,  // .
                                 0x60, 0x1c, 0x02,  // /
                                 0x3e, 0x22, 0x3e,  // 0
                                 0x24, 0x3e, 0x20,  // 1
                                 0x32, 0x2a, 0x26,  // 2
                                 0x22, 0x2a, 0x3e,  // 3
                                 0x0e, 0x08, 0x3e,  // 4
                                 0x2e, 0x2a, 0x3a,  // 5
                                 0x3e, 0x2a, 0x3a,  // 6
                                 0x02, 0x3a, 0x06,  // 7
                                 0x3e, 0x2a, 0x3e,  // 8
                                 0x2e, 0x2a, 0x3e,  // 9
                                 0x00, 0x14, 0x00,  // :
                                 0x40, 0x64, 0x00,  // ;
                                 0x08, 0x14, 0x22,  // <
                                 0x14, 0x14, 0x14,  // =
                                 0x22, 0x14, 0x08,  // >
                                 0x02, 0x2a, 0x06,  // ?
                                 0x3e, 0x22, 0x2e,  // @
                                 0x3e, 0x0a, 0x3e,  // A
                                 0x3e, 0x2a, 0x36,  // B
                                 0x3e, 0x22, 0x22,  // C
                                 0x3e, 0x22, 0x1c,  // D
                                 0x3e, 0x2a, 0x22,  // E
                                 0x3e, 0x0a, 0x02,  // F
                                 0x3e, 0x22, 0x3a,  // G
                                 0x3e, 0x08, 0x3e,  // H
                                 0x22, 0x3e, 0x22,  // I
                                 0x30, 0x20, 0x3e,  // J
                                 0x3e, 0x08, 0x36,  // K
                                 0x3e, 0x20, 0x20,  // L
                                 0x3e, 0x04, 0x3e,  // M
                                 0x3c, 0x08, 0x1e,  // N
                                 0x3e, 0x22, 0x3e,  // O
                                 0x3e, 0x0a, 0x0e,  // P
                                 0x3e, 0x22, 0x7e,  // Q
                                 0x3e, 0x0a, 0x34,  // R
                                 0x2e, 0x2a, 0x3a,  // S
                                 0x02, 0x3e, 0x02,  // T
                                 0x3e, 0x20, 0x3e,  // U
                                 0x1e, 0x20, 0x1e,  // V
                                 0x3e, 0x10, 0x3e,  // W
                                 0x36, 0x08, 0x36,  // X
                                 0x06, 0x38, 0x06,  // Y
                                 0x32, 0x2a, 0x26,  // Z
                                 0x00, 0x3e, 0x22,  // [
                                 0x02, 0x1c, 0x60,  // "\"
                                 0x22, 0x3e, 0x00,  // ]
                                 0x04, 0x02, 0x04,  // ^
                                 0x40, 0x40, 0x40,  // _
                                 0x00, 0x02, 0x00,  // `
                                 0x3e, 0x0a, 0x3e,  // a
                                 0x3e, 0x2a, 0x36,  // b
                                 0x3e, 0x22, 0x22,  // c
                                 0x3e, 0x22, 0x1c,  // d
                                 0x3e, 0x2a, 0x22,  // e
                                 0x3e, 0x0a, 0x02,  // f
                                 0x3e, 0x22, 0x3a,  // g
                                 0x3e, 0x08, 0x3e,  // h
                                 0x22, 0x3e, 0x22,  // i
                                 0x30, 0x20, 0x3e,  // j
                                 0x3e, 0x08, 0x36,  // k
                                 0x3e, 0x20, 0x20,  // l
                                 0x3e, 0x04, 0x3e,  // m
                                 0x3c, 0x08, 0x1e,  // n
                                 0x3e, 0x22, 0x3e,  // o
                                 0x3e, 0x0a, 0x0e,  // p
                                 0x3e, 0x22, 0x7e,  // q
                                 0x3e, 0x0a, 0x34,  // r
                                 0x2e, 0x2a, 0x3a,  // s
                                 0x02, 0x3e, 0x02,  // t
                                 0x3e, 0x20, 0x3e,  // u
                                 0x1e, 0x20, 0x1e,  // v
                                 0x3e, 0x10, 0x3e,  // w
                                 0x36, 0x08, 0x36,  // x
                                 0x06, 0x38, 0x06,  // y
                                 0x32, 0x2a, 0x26,  // z
                                 0x08, 0x3e, 0x22,  // {
                                 0x00, 0x7e, 0x00,  // |
                                 0x22, 0x3e, 0x08,  // }
                                 0x02, 0x06, 0x04,  // ~
                                 0x00, 0x00, 0x00};

namespace robotarm {

LCD::LCD(const Pin& CS, const Pin& RST, const Pin& DC, const Pin& BL, const Pin& CLK, const Pin& DIN) {
    m_SCL = CLK;
    m_TX = DIN;
    m_CS = CS;
    m_RES = RST;
    m_DC = DC;
    m_BL = BL;
}

uint8_t LCD::col_to_x(uint8_t col, uint8_t size) {
    return col * (size * 4);
}

uint8_t LCD::row_to_y(uint8_t row, uint8_t size) {
    return row * (size * 7 + 3);
}

void LCD::delay(uint32_t ms) {
    sleep_ms(ms);
}

void LCD::reset() {
    m_CS.set_low();
    delay(20);
    m_RES.set_low();
    delay(20);
    m_RES.set_high();
    delay(20);
}

void LCD::init() {
    spi_init(spi1, 32000000);

    // set pin directions
    m_CS.set_as_output();
    m_RES.set_as_output();
    m_DC.set_as_output();
    m_BL.set_as_output();

    m_BL.set_high();

    gpio_set_function(m_TX.get_pin_number(), gpio_function::GPIO_FUNC_SPI);
    gpio_set_function(m_SCL.get_pin_number(), gpio_function::GPIO_FUNC_SPI);

    m_CS.set_high();
    m_DC.set_low();
    m_RES.set_low();

    reset();

    write_command(LCD_COMMANDS::SWRESET);
    delay(150);

    write_command(LCD_COMMANDS::SLPOUT);
    delay(500);

    write_command(LCD_COMMANDS::INVON);

    write_command(LCD_COMMANDS::FRMCTR1);
    write_data(0x05);
    write_data(0x3a);
    write_data(0x3a);

    write_command(LCD_COMMANDS::FRMCTR2);
    write_data(0x05);
    write_data(0x3a);
    write_data(0x3a);

    write_command(LCD_COMMANDS::FRMCTR3);
    write_data(0x05);
    write_data(0x3a);
    write_data(0x3a);
    write_data(0x05);
    write_data(0x3a);
    write_data(0x3a);

    write_command(LCD_COMMANDS::INVCTR);
    write_data(0x03);

    write_command(LCD_COMMANDS::PWCTR1);
    write_data(0x62);
    write_data(0x02);  // 0x02 = -4.6V, 0x1c = -3.3V?
    write_data(0x04);

    write_command(LCD_COMMANDS::PWCTR2);
    write_data(0xc0);

    write_command(LCD_COMMANDS::PWCTR3);
    write_data(0x0d);
    write_data(0x00);

    write_command(LCD_COMMANDS::PWCTR4);
    write_data(0x8d);
    write_data(0x6a);

    write_command(LCD_COMMANDS::PWCTR5);
    write_data(0x8d);
    write_data(0xee);

    write_command(LCD_COMMANDS::VMCTR1);
    write_data(0x0e);

    write_command(LCD_COMMANDS::GMCTRP1);
    write_data(0x10);
    write_data(0x0E);
    write_data(0x02);
    write_data(0x03);
    write_data(0x0E);
    write_data(0x07);
    write_data(0x02);
    write_data(0x07);
    write_data(0x0A);
    write_data(0x12);
    write_data(0x27);
    write_data(0x37);
    write_data(0x00);
    write_data(0x0D);
    write_data(0x0E);
    write_data(0x10);

    write_command(LCD_COMMANDS::GMCTRN1);
    write_data(0x10);
    write_data(0x0E);
    write_data(0x03);
    write_data(0x03);
    write_data(0x0F);
    write_data(0x06);
    write_data(0x02);
    write_data(0x08);
    write_data(0x0A);
    write_data(0x13);
    write_data(0x26);
    write_data(0x36);
    write_data(0x00);
    write_data(0x0D);
    write_data(0x0E);
    write_data(0x10);

    write_command(LCD_COMMANDS::COLMOD);
    write_data(0x05);  // 16-bit color

    write_command(LCD_COMMANDS::MADCTL);
    write_data(0xa8);

    write_command(LCD_COMMANDS::DISPON);
    delay(100);
}

void LCD::draw_circle(uint8_t x0, uint8_t y0, uint8_t r, LCD_COLOR color) {
    int16_t f, ddF_x, ddF_y, x, y;
    f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    draw_pixel(x0, y0 + r, color);
    draw_pixel(x0, y0 - r, color);
    draw_pixel(x0 + r, y0, color);
    draw_pixel(x0 - r, y0, color);
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        draw_pixel(x0 + x, y0 + y, color);
        draw_pixel(x0 - x, y0 + y, color);
        draw_pixel(x0 + x, y0 - y, color);
        draw_pixel(x0 - x, y0 - y, color);
        draw_pixel(x0 + y, y0 + x, color);
        draw_pixel(x0 - y, y0 + x, color);
        draw_pixel(x0 + y, y0 - x, color);
        draw_pixel(x0 - y, y0 - x, color);
    }
}

void LCD::draw_text(uint8_t x, uint8_t y, std::string text, uint8_t size, bool wrap, LCD_COLOR fg_color, LCD_COLOR bg_color) {
    uint8_t cursor_x = x;
    uint8_t cursor_y = y;

    //cursor_y += size * 7 + 3;  // initial move down

    uint16_t textlen = text.length();

    for (uint16_t i = 0; i < textlen; i++) {
        if (wrap && ((cursor_x + size * 3) >= LCD_WIDTH)) {
            cursor_x = 0;
            cursor_y += size * 7 + 3;
            if (cursor_y > LCD_HEIGHT) {
                cursor_y = LCD_HEIGHT;
            }
            if (text[i] == 0x20)
                goto skip;
        }

        draw_char(cursor_x, cursor_y, text[i], fg_color, bg_color, size);
        cursor_x += size * 4;
        if (cursor_x > LCD_WIDTH)
            cursor_x = LCD_WIDTH;
    skip:;
    }
}

void LCD::draw_char(uint8_t x, uint8_t y, uint8_t c, LCD_COLOR color, LCD_COLOR bg_color, uint8_t size) {
    const uint8_t ascii_offset = 0x20;
    if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
        return;

    if (size < 1)
        size = 1;
    if (c < ' ' || c > '~')
        c = '?';
    for (uint8_t i = 0; i < 3; i++) {
        uint8_t line = Font_Tiny[(c - ascii_offset) * 3 + i];
        for (uint8_t j = 0; j < 8; j++) {
            if (line & 0x1) {
                if (size == 1) {
                    draw_pixel(x + i, y + j, color);
                } else {
                    fill_rect(x + (i * size), y + (j * size), size, size, color);
                }
            } else if (bg_color != color) {
                if (size == 1) {
                    draw_pixel(x + i, y + j, bg_color);
                } else {
                    fill_rect(x + (i * size), y + (j * size), size, size, bg_color);
                }
            }
            line >>= 1;
        }
    }
}

void LCD::draw_pixel(uint8_t x, uint8_t y, LCD_COLOR color) {
    if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) {
        return;
    }

    set_cursor(x, y, x + 1, y + 1);
    uint16_t color16 = static_cast<uint16_t>(color);

    m_DC.set_high();
    m_CS.set_low();

    uint8_t hi = color16 >> 8;
    uint8_t lo = color16 & 0xFF;

    spi_write_blocking(spi1, &hi, 1);
    spi_write_blocking(spi1, &lo, 1);

    m_CS.set_high();
}

void LCD::fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, LCD_COLOR color) {
    uint8_t hi, lo;
    uint16_t col = static_cast<uint16_t>(color);
    hi = col >> 8;
    lo = col & 0xFF;

    if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
        return;
    if ((x + w - 1) >= LCD_WIDTH)
        w = LCD_WIDTH - x;
    if ((y + h - 1) >= LCD_HEIGHT)
        h = LCD_HEIGHT - y;

    set_cursor(x, y, x + w - 1, y + h - 1);

    m_DC.set_high();
    m_CS.set_low();

    for (uint8_t i = 0; i < h; i++) {
        for (uint8_t j = 0; j < w; j++) {
            spi_write_blocking(spi1, &hi, 1);
            spi_write_blocking(spi1, &lo, 1);
        }
    }
    m_CS.set_high();
}

void LCD::set_cursor(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end) {
    y_start = y_start + 26;  // Y Offset
    y_end = y_end + 26;
    x_start = x_start + 1;  // X offset
    x_end = x_end + 1;

    write_command(LCD_COMMANDS::CASET);
    write_data(0);
    write_data(x_start);
    write_data(0);
    write_data(x_end);

    write_command(LCD_COMMANDS::RASET);
    write_data(0);
    write_data(y_start);
    write_data(0);
    write_data(y_end);

    write_command(LCD_COMMANDS::RAMWR);
}

void LCD::write_command(LCD_COMMANDS command) {
    m_DC.set_low();
    m_CS.set_low();

    // convert to uint8_t
    uint8_t dat = static_cast<uint8_t>(command);

    spi_write_blocking(spi1, &dat, 1);

    m_CS.set_high();
}

void LCD::write_data(uint8_t data) {
    m_DC.set_high();
    m_CS.set_low();

    spi_write_blocking(spi1, &data, 1);

    m_CS.set_high();
}

void LCD::clear(uint16_t color) {
    set_cursor(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    m_CS.set_low();
    m_DC.set_high();
    for (unsigned int i = 0; i < LCD_WIDTH; i++) {
        for (unsigned int j = 0; j < LCD_HEIGHT; j++) {
            uint8_t low = color >> 8;
            uint8_t high = color & 0xFF;
            spi_write_blocking(spi1, &low, 1);
            spi_write_blocking(spi1, &high, 1);
        }
    }
    m_CS.set_high();
}
void LCD::clear(LCD_COLOR color) {
    clear(static_cast<uint16_t>(color));
}

}  // namespace robotarm