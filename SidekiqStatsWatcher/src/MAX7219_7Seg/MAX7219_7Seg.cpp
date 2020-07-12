#include <Arduino.h>
#include <SPI.h>

#include "MAX7219_7Seg.h"

void MAX7219_7Seg::setup(void)
{
  pinMode(MAX7219_7Seg::pinSS, OUTPUT);
  SPI.begin();
  // Set the MAX7219 parameters
  sendData(OP_SHUTDOWN, 1);
  sendData(OP_DECODEMODE, 0xff);
  sendData(OP_SCANLIMIT, NUM_DIGITS-1);
  sendData(OP_INTENSITY, 3);

  // turn on all LED for short time
  sendData(OP_DISPLAYTEST, 1);
  delay(500);
  sendData(OP_DISPLAYTEST, 0);

  // initialize the display to 0
  update(0);
}

void MAX7219_7Seg::sendData(uint16_t cmd, uint8_t data)
{
  uint16_t x = (cmd << 8) | data;
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  digitalWrite(pinSS, LOW);
  SPI.transfer16(x);
  digitalWrite(pinSS, HIGH);
  SPI.endTransaction();
}

void MAX7219_7Seg::update(uint32_t n)
{
  for (uint8_t i = 0; i < NUM_DIGITS; i++)
  {
    uint8_t digit = n % 10;
    sendData(OP_DIGIT0 + i, digit);
    n /= 10;
  }
}
