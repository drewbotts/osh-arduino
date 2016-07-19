#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <OSHClient.h>
#include <ESP8266WiFi.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 5 
#endif

using namespace osh;

const char ssid[]     = "WiFi SSID";
const char password[] = "WiFi Password";
WiFiClient client;

Sensor s1;
SOSClient* sos;

static const char TEMP_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanTemperature.owl#Temperature";
static const char RELH_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.3/propFraction.owl#RelativeHumidity";


#define DHTPIN            4         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
#define DHTTYPE           DHT11     // DHT 11 
//#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
    // initialize LED digital pin as an output
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);

       // set weather sensor metadata
    s1.setUniqueID("urn:osh:nano:dht:001");
    s1.setName("My DHT Sensor 001");
    s1.addTimeStampOBC("ms");
    s1.addMeasurement("temp", TEMP_URI, "Cel", "Air Temperature");
    s1.addMeasurement("relh", RELH_URI, "%", "Relative Humidity");

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
    
    Serial.println("DHTxx Unified Sensor Example");
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Temperature");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
    Serial.println("------------------------------------");
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Humidity");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
    Serial.println("------------------------------------");
    // Set delay between sensor readings based on sensor details.
    delayMS = sensor.min_delay / 1000;
}

void loop() {

    // blink LED
    digitalWrite(LED_BUILTIN, HIGH);
    delay(600);
    digitalWrite(LED_BUILTIN, LOW);
    delay(600);
    
    // Delay between measurements.
    delay(delayMS);

    // send temp and pressure measurements
    sos->startMeasurement(&s1);
    sos->pushInt(millis());
    
    // Get temperature event and print its value.
    sensors_event_t event;  
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println("Error reading temperature!");
    }
    else {
      Serial.print("Temperature: ");
      Serial.print(event.temperature);
      Serial.println(" *C");
    }

    sos->pushFloat(event.temperature); // add random temperature value
    
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println("Error reading humidity!");
    }
    else {
      Serial.print("Humidity: ");
      Serial.print(event.relative_humidity);
      Serial.println("%");
    }

    sos->pushFloat(event.relative_humidity);  // add random pressure value
    sos->sendMeasurement();
}
