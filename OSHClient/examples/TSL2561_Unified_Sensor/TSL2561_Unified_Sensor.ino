#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
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

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

int delayMS = 500;

void setup() {
    Serial.begin(9600);

    // set sensor metadata
    s1.setUniqueID("urn:osh:esp8266:tsl:001");
    s1.setName("TSL2561 001");
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

    tsl.enableAutoRange(true);
    tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
    //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
    //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);

    /* Initialise the sensor */
    if(!tsl.begin())
    {
      /* There was a problem detecting the TSL2561 ... check your connections */
      Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
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
    if (event.light)
    {
      Serial.print(event.light); Serial.println(" lux");
    }
    else
    {
      /* If event.light = 0 lux the sensor is probably saturated
         and no reliable data could be generated! */
      Serial.println("Sensor overload");
    }

    sos->pushFloat(event.light); // add random temperature value
    sos->sendMeasurement();
}
