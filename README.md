A simple miniature DIY project (toy for my son) using 4 LEDs and ATTiny85

Notes:
  * Uses interrupts and timers instead of delays
  * Optimizes power consumption
  * My first C project in the MCU world (I prefer Assembly)
  * Battery holder is surface-mount

TODO in the next project:
  * Start using 'power-down' mode to preserve even more power
  * Try to add a [Joule thief](https://en.wikipedia.org/wiki/Joule_thief) or something similar

Mistakes to fix in future projects:
  * It's not a problem to use INT0 even if the pin is reserved for LED (that's why no deep sleep and battery draw)
  * CR1220 is not designed for this type application

Picture:  
![Photo](/_pictures/photo.jpg)

Schematic:  
![Schematic](/_pictures/schema.png)
