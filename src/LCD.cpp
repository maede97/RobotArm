#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <robotarm/LCD.h>

#include <iostream>

namespace robotarm {

LCD::LCD(const Pin& CS, const Pin& RST, const Pin& DC, const Pin& BL, const Pin& CLK, const Pin& DIN) {
    m_SCL = CLK;
    m_TX = DIN;
    m_CS = CS;
    m_RES = RST;
    m_DC = DC;
    m_BL = BL;
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
    spi_init(spi1, 1000000);

    std::cout << "Set SPI init." << std::endl;

    // set pin directions
    m_CS.set_as_output();
    m_RES.set_as_output();
    m_DC.set_as_output();
    m_BL.set_as_output();

    m_BL.set_high();

    gpio_set_function(m_TX.get_pin_number(), gpio_function::GPIO_FUNC_SPI);
    gpio_set_function(m_SCL.get_pin_number(), gpio_function::GPIO_FUNC_SPI);

    std::cout << "Set functions" << std::endl;

    m_CS.set_high();
    m_DC.set_low();
    m_RES.set_low();

    reset();

    std::cout << "Starting commands" << std::endl;

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

void LCD::draw_text(std::string text) {
    m_text = text;
}

void LCD::set_cursor(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end) {
    y_start = y_start + 26;  // Y Offset
    y_end = y_end + 26;
    x_start = x_start + 1;  // X offset
    x_end = x_end + 1;

    write_command(LCD_COMMANDS::CASET);
    write_data(x_start >> 8);
    write_data(x_start & 0xFF);
    write_data(x_end >> 8);
    write_data(x_end & 0xFF);

    write_command(LCD_COMMANDS::RASET);
    write_data(y_start >> 8);
    write_data(y_start & 0xFF);
    write_data(y_end >> 8);
    write_data(y_end & 0xFF);

    write_command(LCD_COMMANDS::RAMWR);
}

void LCD::write_command(LCD_COMMANDS command) {
    // TODO: maybe insert ASM nop here
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
    for (unsigned int i = 0; i < LCD_WIDTH; i++) {
        for (unsigned int j = 0; j < LCD_HEIGHT; j++) {
            m_CS.set_low();
            m_DC.set_high();
            uint8_t low = color >> 8;
            uint8_t high = color & 0xFF;
            spi_write_blocking(spi1, &low, 1);
            spi_write_blocking(spi1, &high, 1);
            m_CS.set_high();
        }
    }
}
void LCD::clear(LCD_COLOR color) {
    clear(static_cast<uint16_t>(color));
}

void LCD::update() {
    // Fill the whole screen with green
}

}  // namespace robotarm