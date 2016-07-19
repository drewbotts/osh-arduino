#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <OSHClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

using namespace osh;

const char ssid[]     = "WiFi SSID";
const char password[] = "WiFi Password";
WiFiClient client;

Sensor s1;
SOSClient* sos;

static const char ORIEN_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.3/reprMath.owl#Orientation";

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

/* Assign a unique ID to this sensor at the same time */
Adafruit_BNO055 bno = Adafruit_BNO055(55);

int delayMS = 500;

void setup() {
    Serial.begin(9600);

    // set sensor metadata
    s1.setUniqueID("urn:osh:esp8266:bno:001");
    s1.setName("BNO055 001");
    s1.addTimeStampOBC("ms");
    s1.addMeasurement("roll", ORIEN_URI, "deg", "Roll");
    s1.addMeasurement("pitch", ORIEN_URI, "deg", "Pitch");
    s1.addMeasurement("heading", ORIEN_URI, "deg", "Heading");

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

    bno.setExtCrystalUse(true);
    
    //Initialise the sensor
    if(!bno.begin())
    {
      //There was a problem detecting the BNO055 ... check your connections
      Serial.print("Ooops, no BNO055 detected ... Check your wiring!");
      while(1);
    }
    
    Serial.println("BNO055 Unified Sensor Example");
    // Print BNO055 sensor details.
    sensor_t sensor;
    bno.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");  
    Serial.println("------------------------------------");
    delay(delayMS);
}

void loop() {
    // Delay between measurements.
    delay(BNO055_SAMPLERATE_DELAY_MS);

    sos->startMeasurement(&s1);
    sos->pushInt(millis());
    
    // Get orientation event and print its values.
    sensors_event_t event;  
    bno.getEvent(&event);
    Serial.print("Roll: ");
    Serial.print(event.orientation.x);
    Serial.println(" deg");
    sos->pushFloat(event.orientation.x);
    Serial.print("Pitch: ");
    Serial.print(event.orientation.y);
    Serial.println(" deg");
    sos->pushFloat(event.orientation.y);
    Serial.print("Heading: ");
    Serial.print(event.orientation.z);
    Serial.println(" deg");
    sos->pushFloat(event.orientation.z);
    
    sos->sendMeasurement();
}
