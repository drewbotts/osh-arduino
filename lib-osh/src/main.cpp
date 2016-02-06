#define LibOSH_XML // to enable XML output
//#define LibOSH_JSON // to enable JSON output

#include "Arduino.h"
#include <LibOSH.h>
//#include <PubSubClient.h>

Sensor s1;
Sensor s2;
System sys;

void setup()
{
    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(57600 * 2);

    // set minimal sensor metadata
    s1.setUniqueID("urn:osh:nano:temp:001");
    s1.setDescription("My Temp Sensor");
    s1.addMeasurement("temp", "http://sweet.jpl.nasa.gov/2.2/quanTemperature.owl#Temperature", "Cel", "Air Temperature");

    s2.setUniqueID("urn:osh:nano:wind:001");
    s2.setDescription("My Wind Sensor");
    s2.addMeasurement("speed", "http://sweet.jpl.nasa.gov/2.2/quanWind.owl#WindSpeed", "m/s", "Wind Speed");
    s2.addMeasurement("dir", "http://sweet.jpl.nasa.gov/2.2/quanWind.owl#WindDirection", "deg", "Wind Direction");

    sys.setUniqueID("urn:osh:nano:weatherStation:001");
    sys.setDescription("My Weather Station");
    sys.addSensor(&s1);
    sys.addSensor(&s2);

    s1.toXML(Serial);
    s2.toXML(Serial);
    sys.toXML(Serial);
}

void loop()
{
    // blink LED
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);

    float data[1] = {2.0};
    s1.sendData(data, Serial);
}
