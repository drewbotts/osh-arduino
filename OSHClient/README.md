# OSH Client Library for Arduino

This is a client library that can run on µC board compatible with the Arduino framework. It makes it very easy to connect your sensor to an OSH node by registering it and sending its data to the Sensor Observation Service transactional interface (SOS-T).

The source code of the library is in the OSHClient folder. You will have to include this library folder with the Arduino IDE before you can compile any of the scripts in the 'examples' folder. Please refer to the [Arduino documentation](https://www.arduino.cc/en/Guide/Libraries#toc4) to achieve this.

We compile the library itself with [PlatformIO](http://platformio.org/), so we can easily distribute versions for different platforms and boards. You can also compile the examples with PlatformIO.
