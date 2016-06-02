# Arduino support for OSH

It is easy to support Arduino-based sensors and sensor systems in OpenSensorHub (OSH). The sensor description and observations can be sent to any OSH instance via WiFi or serial. This repository supports this by providing libraries and example code to run on µC boards compatible with the Arduino framework.

This includes a client library to connect your sensor to an OSH node by registering the sensor and sending its data to the Sensor Observation Service transactional interface (SOS-T). The source code of the library is in the OSHClient folder. You will have to include this library folder with the Arduino IDE before you can compile any of the scripts in the 'examples' folder. Please refer to the [Arduino documentation](https://www.arduino.cc/en/Guide/Libraries#toc4) to achieve this.
