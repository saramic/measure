#pragma once

#include "SPI.h" // SPI Serial Peripheral Interface

/*
 * Core object for MAZ7219_7Seg library
 */
class MAX7219_7Seg
{
  public:
    /*
     * pin out
     *   7-seg board  function  ESP func   VSPI pin  HSPI pin  Meaning
     *   DIN          MOSI      HSPI MOSI  GPIO23    GPIO13    Master Output Slave Input (data output from master)
     *   CS           SS        HSPI CS    GPIO5     GPIO15    Slave Select / Chip Select
     *   CLK          SCLK      HSPI CLK   GPIO18    GPIO14    Serial CLocK
     */
    // Define the number of 7-seg digits
    // in the display. Valid range [1..8]
    const uint8_t NUM_DIGITS = 8;

    // Define the SS pin
    const uint8_t pinSS = 5; // GPIO5

    // Opcodes for the MAX7221 and MAX7219
    const uint16_t OP_NOOP = 0;
    const uint16_t OP_DIGIT0 = 1;
    // note: all OP_DIGITn are +n offsets from OP_DIGIT0
    const uint16_t OP_DIGIT1 = 2;
    const uint16_t OP_DIGIT2 = 3;
    const uint16_t OP_DIGIT3 = 4;
    const uint16_t OP_DIGIT4 = 5;
    const uint16_t OP_DIGIT5 = 6;
    const uint16_t OP_DIGIT6 = 7;
    const uint16_t OP_DIGIT7 = 8;
    const uint16_t OP_DECODEMODE = 9;
    const uint16_t OP_INTENSITY = 10;
    const uint16_t OP_SCANLIMIT = 11;
    const uint16_t OP_SHUTDOWN = 12;
    const uint16_t OP_DISPLAYTEST = 15;

    /*
     * setup to reset display to 0
     */
    void setup(void);

    /*
     * Work out the individual digits to be displayed and show them.
     */
    void update(uint32_t n);

  private:

    /*
     * send a simple command to the MAX7219 using the hardware SPI interface.
     */
     void sendData(uint16_t cmd, uint8_t data);
};
