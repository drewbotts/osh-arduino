#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <OSHClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <Wire.h>


using namespace osh;

//--------------------------------------------- //
//------------- Network Constants ------------- //
//--------------------------------------------- //
const char ssid[]     = "SSID";
const char password[] = "PASSWORD";
const char oshNodeIp[] = "OSH-NODE-IP";
//--------------------------------------------- //
//--------------------------------------------- //
//--------------------------------------------- //

//----------------- BOUY ID ------------------- //
//--------------------------------------------- //
char bouyID[] = "BOUY-ID";
//--------------------------------------------- //
//--------------------------------------------- //

static const char ntpServerName[] = "time.nist.gov"; // NTP Server
const int timeZone = 0;     // UTC
//const int timeZone = 1;     // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)

WiFiClient client;
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets


Sensor s1; // *Sensor
SOSClient* sos1;


static const char BOUYID_URI[] PROGMEM = "http://dbpedia.org/resource/IMEI";
static const char SEQNUM_URI[] PROGMEM = "http://sensorml.com/ont/swe/property/SequenceNumber";
static const char MAG_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.3/prop.owl#MagneticFieldStrength";

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified();


int momsn = 0;
unsigned long delayTime;
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);
char timeUTC[40];

void setup() {
  Serial.begin(9600);

   // Create UID for BouyID and Sensor Module ID
  // if in need of space remove this segment and manually update BouyID and UniqueID
  // -------------------------------------------//
    String wifiID = WiFi.macAddress();
    char bouyUID[wifiID.length()];
    int i; 
    for (i = 0; i < sizeof(bouyUID); i++) { 
        bouyUID[i] = wifiID[i]; 
      } 
    Serial.print(bouyUID);

    String hmcID = wifiID + ":HMC5883";
    char hmcUID[hmcID.length()];
    for (i=0; i < sizeof(hmcUID); i++) {
      hmcUID[i] = hmcID[i];
    }
  //------------------------------------------ //

  // set s1 sensor metadata
  s1.setUniqueID(hmcUID);
  s1.setName("HMC5883");
  s1.setDataRecordDef("urn:darpa:oot:message:magnetometer");
  s1.setLabel("Magnetometer Message");
  s1.addTimeStampUTC();
  s1.addMeasurement("Bouy_ID", BOUYID_URI, NULL, "Bouy ID", TEXT);
  s1.addMeasurement("MOMSN", SEQNUM_URI, NULL, "MO Message Sequence Number", COUNT);
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
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(86400); // Once per day = 86,400

  //Initialise the sensor
  if(!mag.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no HMC5883detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  // register to OSH node using SOS-T protocol
  sos1 = new SOSClient(client, oshNodeIp, 9292, "/sensorhub/sos");
  sos1->registerDevice(&s1); 

  Serial.println("HMC5883 Example");

  // set delay time
  delayTime = 60000;
  
}

time_t prevTime = 0; // initialize

void loop(void) {
  if (timeStatus() != timeNotSet) {
    if (now() != prevTime) { //update only if time has changed
      prevTime = now();

      ++momsn;
  
      sos1->startMeasurement(&s1);
      sos1->pushString(getTimeISO8601());
      sos1->pushString(bouyID);
      sos1->pushInt(momsn);

      // Get magnetic event
      sensors_event_t event;  
      mag.getEvent(&event);
    
      /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
      Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
      Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
      Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
    
      sos1->pushFloat(event.magnetic.x);
      sos1->pushFloat(event.magnetic.y);
      sos1->pushFloat(event.magnetic.z);
    
      sos1->sendMeasurement();
      
    }
  }

  // Delay between measurements
  delay(delayTime);
}


char * getTimeISO8601() {
  
  char yearBuff [5];
  itoa(year(), yearBuff, 10);

  char monthBuff [3];
  itoa(month(), monthBuff, 10);
  if (strlen(monthBuff) == 1) {
    monthBuff[1] = monthBuff[0]; // Shift [x _ _] -> [_ x _]
    monthBuff[0] = '0'; // Pad with zero ['0' x _]
    monthBuff[2] = 0; // String terminator ['0' x 0]
  }
  
  char dayBuff [3];
  itoa(day(), dayBuff, 10);
  if (strlen(dayBuff) == 1) {
    dayBuff[1] = dayBuff[0]; // Shift [x _ _] -> [_ x _]
    dayBuff[0] = '0'; // Pad with zero ['0' x _]
    dayBuff[2] = 0; // String terminator ['0' x 0]
  }
  
  char hourBuff [3];
  itoa(hour(), hourBuff, 10);
  if (strlen(hourBuff) == 1) {
    hourBuff[1] = hourBuff[0]; // Shift [x _ _] -> [_ x _]
    hourBuff[0] = '0'; // Pad with zero ['0' x _]
    hourBuff[2] = 0; // String terminator ['0' x 0]
  }

  char minuteBuff [3];
  itoa(minute(), minuteBuff, 10);
  if (strlen(minuteBuff) == 1) {
    minuteBuff[1] = minuteBuff[0]; // Shift [x _ _] -> [_ x _]
    minuteBuff[0] = '0'; // Pad with zero ['0' x _]
    minuteBuff[2] = 0; // String terminator ['0' x 0]
  }

  char secondBuff [3];
  itoa(second(), secondBuff, 10);
  if (strlen(secondBuff) == 1) {
    secondBuff[1] = secondBuff[0]; // Shift [x _ _] -> [_ x _]
    secondBuff[0] = '0'; // Pad with zero ['0' x _]
    secondBuff[2] = 0; // String terminator ['0' x 0]
  }

  strcpy (timeUTC, yearBuff);
  strcat (timeUTC, "-");
  strcat (timeUTC, monthBuff);
  strcat (timeUTC, "-");
  strcat (timeUTC, dayBuff);
  strcat (timeUTC, "T");
  strcat (timeUTC, hourBuff);
  strcat (timeUTC, ":");
  strcat (timeUTC, minuteBuff);
  strcat (timeUTC, ":");
  strcat (timeUTC, secondBuff);
  strcat (timeUTC, "Z");
  return timeUTC;
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
