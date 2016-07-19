#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include <OSHClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

using namespace osh;

const char ssid[]     = "WiFi SSID";
const char password[] = "WiFi Password";
WiFiClient client;

Sensor s1;
SOSClient* sos;

static const char ILLU_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.3/propEnergyFlux.owl#Illuminance";

// pass in a number for the sensor identifier (for your use later)
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);


int delayMS = 500;

void setup() {
    Serial.begin(9600);

    // set sensor metadata
    s1.setUniqueID("urn:osh:esp8266:tsl:001");
    s1.setName("TSL2591 001");
    s1.addTimeStampOBC("ms");
    s1.addMeasurement("illum", ILLU_URI, "lux", "Illuminance");

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
    sos = new SOSClient(client, "192.168.0.25", 8181, "/sensorhub/sos");
    sos->registerDevice(&s1);

    // You can change the gain on the fly, to adapt to brighter/dimmer light situations
    //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
    tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
    // tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
    // tsl.setGain(TSL2591_GAIN_MAX);   // 9876x gain

    // Changing the integration time gives you a longer time over which to sense light
    // longer timelines are slower, but are good in very low light situtations!
    tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)


    /* Initialise the sensor */
    if(!tsl.begin())
    {
      /* There was a problem detecting the TSL2561 ... check your connections */
      Serial.print("Ooops, no TSL2591 detected ... Check your wiring!");
      while(1);
    }
    
    // Print temperature sensor details.
    sensor_t sensor;
    tsl.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
    Serial.println("------------------------------------");
    delay(delayMS);
}

void loop() {
    // Delay between measurements.
    delay(delayMS);

    // send illuminance measurements
    sos->startMeasurement(&s1);
    sos->pushInt(millis());
    
    // Get illuminance event
    sensors_event_t event;  
    tsl.getEvent(&event);
    
    /* Display the results (light is measured in lux) */
    Serial.print("[ "); Serial.print(event.timestamp); Serial.print(" ms ] ");
    if ((event.light == 0) |
        (event.light > 4294966000.0) | 
        (event.light <-4294966000.0))
    {
      /* If event.light = 0 lux the sensor is probably saturated */
      /* and no reliable data could be generated! */
      /* if event.light is +/- 4294967040 there was a float over/underflow */
      Serial.println("Invalid data (adjust gain or timing)");
    }
    else
    {
      Serial.print(event.light); Serial.println(" lux");
    }

    sos->pushFloat(event.light); // add random temperature value
    sos->sendMeasurement();
}
