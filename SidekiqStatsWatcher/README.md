SidekiqStatsWatcher

---

SidekiqStatsWatcher is an ESP32 WROOM board that pings sidekiq stats, parses
the JSON and prints the total number of processed jobs on an 8 digit 7-segment
display driven by an MAX7219 driver.

## TODO

- [ ] hardware and wiring diagram
- [ ] minimum demo to get a sidekiq status page up
- [ ] other library options, why did I have to go down to the SPI programming
  layer as related in [using MAX7219 part
  1](https://arduinoplusplus.wordpress.com/2019/06/08/using-the-max7219-in-your-projects-part-1/)
  and [using MAX7219 part
  2](https://arduinoplusplus.wordpress.com/2019/06/22/using-the-max7219-in-your-projects-part-2/)
  - [DigitLedDisplay](https://github.com/ozhantr/DigitLedDisplay/)
  - [MD_MAX72xx](https://github.com/MajicDesigns/MD_MAX72XX)
  - [LedControl[(http://wayoda.github.io/LedControl/)
- [ ] better document my understanding of SPI
- [ ] change to WifiMulti
- [ ] configure Wifi setup via bluetooth

