#ifndef REMOTECONTROL_OLED_H
#define REMOTECONTROL_OLED_H

#include <U8g2lib.h>
#include <Wire.h>

class Oled : public U8G2_SSD1306_128X64_NONAME_F_SW_I2C {
   private:
    // Pin definetion of OLED
    const uint8_t sda = 4;
    const uint8_t scl = 15;
    const uint8_t rst = 16;

   public:
    Oled();
    ~Oled();
};

Oled::Oled() : U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, Oled::scl, Oled::sda, Oled::rst) {
    this->begin();
    this->setFont(u8g2_font_5x8_mf);
}

Oled::~Oled() {}

#endif