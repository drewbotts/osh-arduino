#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <OSHClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

using namespace osh;

const char ssid[]     = "Seng";
const char password[] = "earthangel";
WiFiClient client;

Sensor s1;
SOSClient* sos;

static const char ACCEL_URI[] PROGMEM = "http://qudt.org/vocab/quantitykind/LinearAcceleration";

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
int delayMS = 500;

void setup() {
    Serial.begin(9600);

    // set sensor metadata
    s1.setUniqueID("urn:osh:esp8266:adxl:001");
    s1.setName("ADXL345 - Accel");
    s1.addTimeStampOBC("ms");
    s1.addAccel(ACCEL_URI, "Float Acceleration");

    
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
    sos = new SOSClient(client, "bottsgeo.simple-url.com", 9292, "/sensorhub/sos");
    sos->registerDevice(&s1);


    /* Set the range to whatever is appropriate for your project */
    accel.setRange(ADXL345_RANGE_16_G);
    //accel.setRange(ADXL345_RANGE_8_G);
    //accel.setRange(ADXL345_RANGE_4_G);
    //accel.setRange(ADXL345_RANGE_2_G);
    
    //Initialise the sensor
    if(!accel.begin())
    {
      //There was a problem detecting the ADXL345 ... check your connections
      Serial.print("Ooops, no ADXL345 detected ... Check your wiring!");
      while(1);
    }
    
    Serial.println("ADXL345 Unified Sensor Example");
    // Print ADXL345 sensor details.
    sensor_t sensor;
    accel.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
    Serial.println("------------------------------------");
    delay(delayMS);
}

void loop() {
    // Delay between measurements.
    delay(delayMS);

    sos->startMeasurement(&s1);
    sos->pushInt(millis());
    
    // Get accel event and print its values.
    sensors_event_t event;  
    accel.getEvent(&event);
    Serial.print("X: ");
    Serial.print(event.acceleration.x);
    Serial.println(" m/s^2");
    sos->pushFloat(event.acceleration.x);
    Serial.print("Y: ");
    Serial.print(event.acceleration.y);
    Serial.println(" m/s^2");
    sos->pushFloat(event.acceleration.y);
    Serial.print("Z: ");
    Serial.print(event.acceleration.z);
    Serial.println(" m/s^2");
    sos->pushFloat(event.acceleration.z);
    
    sos->sendMeasurement();
}
