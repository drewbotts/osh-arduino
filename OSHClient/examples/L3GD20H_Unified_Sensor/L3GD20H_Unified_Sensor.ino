#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <OSHClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

using namespace osh;

const char ssid[]     = "WiFi SSID";
const char password[] = "WiFi Password";
WiFiClient client;

Sensor s1;
SOSClient* sos;

static const char ANGVEL_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.3/propRotation.owl#AngularVelocity";

/* Assign a unique ID to this sensor at the same time */
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
int delayMS = 500;

void setup() {
    Serial.begin(9600);

    // set sensor metadata
    s1.setUniqueID("urn:osh:esp8266:gyro:001");
    s1.setName("L3GD20H 001");
    s1.addTimeStampOBC("ms");
    s1.addMeasurement("angvx", ANGVEL_URI, "rad/s", "Angular Velocity X");
    s1.addMeasurement("angvy", ANGVEL_URI, "rad/s", "Angular Velocity Y");
    s1.addMeasurement("angvz", ANGVEL_URI, "rad/s", "Angular Velocity Z");

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


    /* Enable AutoRange */
    gyro.enableAutoRange(true);
    
    //Initialise the sensor
    if(!gyro.begin())
    {
      //There was a problem detecting the L3GD20H ... check your connections
      Serial.print("Ooops, no L3GD20H detected ... Check your wiring!");
      while(1);
    }
    
    Serial.println("L3GD20H Unified Sensor Example");
    // Print L3GD20H sensor details.
    sensor_t sensor;
    gyro.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");  
    Serial.println("------------------------------------");
    delay(delayMS);
}

void loop() {
    // Delay between measurements.
    delay(delayMS);

    sos->startMeasurement(&s1);
    sos->pushInt(millis());
    
    // Get event and print its values.
    sensors_event_t event;  
    gyro.getEvent(&event);
    Serial.print("X: ");
    Serial.print(event.gyro.x);
    Serial.println(" rad/s");
    sos->pushFloat(event.gyro.x);
    Serial.print("Y: ");
    Serial.print(event.gyro.y);
    Serial.println(" rad/s");
    sos->pushFloat(event.gyro.y);
    Serial.print("Z: ");
    Serial.print(event.gyro.z);
    Serial.println(" rad/s");
    sos->pushFloat(event.gyro.z);
    
    sos->sendMeasurement();
}
