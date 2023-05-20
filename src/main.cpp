#include "global_definitions.h"
#include "sensor.h"
#include "utilities.h"
#include "web_handlers.h"

// Global instance
Adafruit_MPU6050 mpu;

UserData userData = {
  .weight = 70,
  .age = 30,
  .gender = "Male",
  .height = 170,
  .calorieGoal = 500
};

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial)
    delay(SHORT_DELAY); 

  Serial.println("Adafruit MPU6050 test!");

  if(!initMPU6050()) {
    while(1) {
      delay(SHORT_DELAY);
    }
  }

  setupWiFi();
  setupWebServer();

  delay(SHORT_DELAY);
}

void loop() {
  server.handleClient();
}
