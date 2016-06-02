#include <OSHClient.h>
#include <ESP8266WiFi.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 5 
#endif

using namespace osh;

const char ssid[]     = "MySSID";
const char password[] = "MyPWD";
WiFiClient client;

Sensor s1;
SOSClient* sos;

static const char TEMP_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanTemperature.owl#Temperature";
static const char PRESS_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanPress.owl#Pressure";


void setup()
{
    // initialize LED digital pin as an output
    pinMode(LED_BUILTIN, OUTPUT);

    // init serial for status
    Serial.begin(115200);

    // set weather sensor metadata
    s1.setUniqueID("urn:osh:nano:temp:001");
    s1.setName("My Temp Sensor");
    s1.addTimeStampOBC("ms");
    s1.addMeasurement("temp", TEMP_URI, "Cel", "Air Temperature");
    s1.addMeasurement("press", PRESS_URI, "hPa", "Atmospheric Pressure");
    
    // connect to WiFi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // print IP address 
    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // register to OSH node using SOS-T protocol
    sos = new SOSClient(client, "192.168.0.16", 8181, "/sensorhub/sos");
    sos->registerDevice(&s1);
}


void loop()
{
    // blink LED
    digitalWrite(LED_BUILTIN, HIGH);
    delay(600);
    digitalWrite(LED_BUILTIN, LOW);
    delay(600);

    // send temp and pressure measurements
    sos->startMeasurement(&s1);
    sos->pushInt(millis());
    sos->pushFloat(random(15.0, 20.0)); // add random temperature value
    sos->pushFloat(random(950, 1120));  // add random pressure value
    sos->sendMeasurement();
}

