#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <OSHClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

using namespace osh;

const char ssid[]     = "WiFi SSID";
const char password[] = "WiFi Password";
WiFiClient client;

Sensor s1;
SOSClient* sos;

static const char MAG_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.3/prop.owl#MagneticFieldStrength";

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

int delayMS = 500;

void setup() {
    Serial.begin(9600);

    // set sensor metadata
    s1.setUniqueID("urn:osh:esp8266:hmc:001");
    s1.setName("HMC5883 001");
    s1.addTimeStampOBC("ms");
    s1.addMeasurement("magx", MAG_URI, "uT", "Magnetic Field Strength X");
    s1.addMeasurement("magy", MAG_URI, "uT", "Magnetic Field Strength Y");
    s1.addMeasurement("magz", MAG_URI, "uT", "Magnetic Field Strength Z");

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

    /* Initialise the sensor */
    if(!mag.begin())
    {
      /* There was a problem detecting the TSL2561 ... check your connections */
      Serial.print("Ooops, no HMC5883detected ... Check your wiring or I2C ADDR!");
      while(1);
    }
    
    // Print sensor details.
    sensor_t sensor;
    mag.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
    Serial.println("------------------------------------");
    delay(delayMS);
}

void loop() {
    // Delay between measurements.
    delay(delayMS);

    // send illuminance measurements
    sos->startMeasurement(&s1);
    sos->pushInt(millis());
    
    // Get magnetic event
    sensors_event_t event;  
    mag.getEvent(&event);
    
    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
    
    sos->pushFloat(event.magnetic.x);
    sos->pushFloat(event.magnetic.y);
    sos->pushFloat(event.magnetic.z);
    sos->sendMeasurement();
}
