#include <OSHClient.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 5 
#endif

using namespace osh;

Sensor s1;
static const char TEMP_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanTemperature.owl#Temperature";
static const char PRESS_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanPress.owl#Pressure";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
    
  s1.setUniqueID("urn:osh:nano:temp:001");
  s1.setName("My Temp Sensor");
  s1.addTimeStampOBC("ms");
  s1.addMeasurement("temp", TEMP_URI, "Cel", "Air Temperature");
  s1.addMeasurement("press", PRESS_URI, "hPa", "Atmospheric Pressure");
  s1.toXML(Serial);
}

void loop() {
  // blink LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(600);
  digitalWrite(LED_BUILTIN, LOW);
  delay(600);
}
