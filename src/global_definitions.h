#pragma once

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Constants
const uint32_t SERIAL_BAUD_RATE = 9600;
const uint32_t SHORT_DELAY = 10;  // ms
const uint32_t LONG_DELAY = 500;  // ms

// Motion states
const String STATE_STANDING = "Standing";
const String STATE_WALKING = "Walking";
const String STATE_RUNNING = "Running";
const String STATE_JUMPING = "Jumping";

// Global instances
extern Adafruit_MPU6050 mpu;

// Sensor data
struct SensorData {
  float ax, ay, az;
  float gx, gy, gz;
};

struct UserData {
  float weight;
  float age;
  String gender;
  float height;
  float calorieGoal;
};

extern UserData userData;

struct PedoData {
  SensorData sensor_data;
  String motion_state;
  int step_count;
  float calorie_burn;
};

#define NUM_READINGS 10
#define STEP_THRESHOLD 1.5

// Calorie burn constants
#define MET_STANDING 1.3
#define MET_WALKING 3.9
#define MET_RUNNING 7.0
#define MET_JUMPING 8.0
