/**
 * 
*/

// Arduino / ESP32 Libraries
#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <BlindControl.h>

/* *** PROJECT SETTINGS *** */

// WiFi related settings
#define AP_NAME "dd-wrt"
#define AP_PSK ""

// 

/* (end of important settings) */

// TimeClient related settings
#define TIME_OFFSET -5

// LED pin for status
#define LED_PIN 4

// Runtime settings

// This value controls how often the time is retrieved and compared to the defined
// schedule. Value is in seconds. Increase this value if you are running on battery
// and are concerned about how long it will last.
#define UPDATE_FREQ 60

/* *** SYSTEM METHODS *** */

/**
 * - SetupWiFi
 * 
 * Sets up any parameters related to the wireless connection, and runs the WiFi.begin()
 * method to start the connection process.
 * 
*/
void setupWiFi() {

  Serial.println("Starting WiFi...");

  // Set wifi mode to station (client)
  WiFi.mode(WIFI_STA);

  // Start the connection
  WiFi.begin(AP_NAME, AP_PSK);

  // Wait until connected to the network, while displaying a loading message.

  Serial.print("Connecting");

  bool statusLEDOn = false;
  while(WiFi.status() != WL_CONNECTED){

    delay(500);

    // Toggle LED on and off
    digitalWrite(LED_PIN, statusLEDOn);
    statusLEDOn = !statusLEDOn;

    // Print to serial
    Serial.print(".");

  }

  // Network has connected, turn LED off and print to serial
  Serial.println(" Connected!");
  digitalWrite(LED_PIN, LOW);

}

/* *** SCHEDULING METHODS *** */

// TimeClient ibrary instantiation stuff, not important
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", TIME_OFFSET * 3600);

TimeData getCurrentTime(){
  return TimeData(
    timeClient.getHours(),
    timeClient.getMinutes()
  );
}

/* *** ACTUATION CODE *** */

class CustomizedBlinds : public Blinds {
  public:
    void open() {
      Serial.println("Blinds are open!");
    }
    void close() {
      Serial.println("Blinds are closed!");
    }
};

/* *** RUNTIME METHODS *** */

Schedule schedule;
CustomizedBlinds blinds;

void configureSchedule(){
  
  schedule = Schedule(CLOSED);

  schedule.addEntry(
    ScheduleEntry(
      TimeData(13, 15),
      OPEN
    )
  );

  schedule.addEntry(
    ScheduleEntry(
      TimeData(13, 16),
      CLOSED
    )
  );

}

void setup() {

  Serial.begin(115200);
  pinMode (4, OUTPUT);

  // Setup methods
  setupWiFi();
  timeClient.begin();

  // Configure schedule
  configureSchedule();

  blinds = CustomizedBlinds();

}

void loop() {
  

  if(WiFi.status() != WL_CONNECTED) {
    setupWiFi();
  }

  timeClient.update();

  switch(schedule.getScheduledBlindState(getCurrentTime())){
    case OPEN:
      blinds.open();
      break;
    case CLOSED:
      blinds.close();
      break;
  }

  // Wait the predefined time period before checking again.
  delay(UPDATE_FREQ * 1000);
}