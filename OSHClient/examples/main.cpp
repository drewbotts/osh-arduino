#include <Arduino.h>
#include "OSHClient.h"

using namespace osh;

#ifdef ARDUINO_ARCH_AVR
static int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif

#ifdef ARDUINO_ARCH_ESP8266
static int freeRam()
{
}
#endif

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
const char* ssid     = "SSID";
const char* password = "***";
WiFiClient client;
#endif

Sensor s1;
Sensor s2;
Sensor s3;
System sys;
SOSClient* sos;

static const char TEMP_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanTemperature.owl#Temperature";
static const char PRESS_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanPress.owl#Pressure";
static const char WINDSPEED_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanWind.owl#WindSpeed";
static const char WINDDIR_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanWind.owl#WindDirection";
static const char SENSORLOC_URI[] PROGMEM = "property/OGC/0/SensorLocation";

int counter = 0;

void setup()
{
    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.println(freeRam());

    // temp sensor
    s1.setUniqueID("urn:osh:nano:temp:001");
    s1.setName("My Temp Sensor");
    s1.addTimeStampOBC("ms");
    s1.addMeasurement("temp", TEMP_URI, "Cel", "Air Temperature");
    s1.addMeasurement("press", PRESS_URI, "hPa", "Atmospheric Pressure");
    s1.toXML(Serial);
    //Serial.println(freeRam());

    // wind sensor
    s2.setUniqueID("urn:osh:nano:wind:001");
    s2.setName("My Wind Sensor");
    s2.addTimeStampUTC();
    s2.addMeasurement("speed", WINDSPEED_URI, "m/s", "Wind Speed");
    s2.addMeasurement("dir", WINDDIR_URI, "deg", "Wind Direction");
    s2.toXML(Serial);
    //Serial.println(freeRam());

    // GPS sensor
    s3.setUniqueID("urn:osh:nano:loc:001");
    s3.setName("My GPS");
    s3.addTimeStampUTC();
    s3.addLocationLLA(SENSORLOC_URI, "Sensor Location");
    s3.toXML(Serial);
    //Serial.println(freeRam());

    // sensor system
    sys.setUniqueID("urn:osh:nano:weatherStation:001");
    sys.setName("My Weather Station");
    sys.addSensor(&s1);
    sys.addSensor(&s2);
    sys.addSensor(&s3);
    sys.toXML(Serial);
    //Serial.println(freeRam());

#ifdef ARDUINO_ARCH_ESP8266
    // connect to WiFi
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    sos = new SOSClient(client, "192.168.0.16", 8181, "/sensorhub/sos");
    sos->registerDevice(&sys);
#endif
}


void loop()
{
    // blink LED
    digitalWrite(LED_BUILTIN, HIGH);
    delay(600);
    digitalWrite(LED_BUILTIN, LOW);
    delay(600);

    sos->startMeasurement(&s1);
    sos->pushInt(millis());
    sos->pushFloat(random(15.0, 20.0));
    sos->pushFloat(random(950, 1120));
    sos->sendMeasurement();

    char utcTime[30];
    sprintf(utcTime, "2016-02-23T%02d:%02d:%02dZ", counter/3600, (counter/60)%60, counter%60);

    sos->startMeasurement(&s2);
    sos->pushString(utcTime);
    sos->pushFloat(random(0.0, 200.0)/10.);
    sos->pushFloat(random(0, 359));
    sos->sendMeasurement();

    sos->startMeasurement(&s3);
    sos->pushString(utcTime);
    sos->pushFloat(random(4300, 4400)/100.);
    sos->pushFloat(random(100, 200)/100.);
    sos->pushFloat(random(350, 360));
    sos->sendMeasurement();

    counter++;
}
