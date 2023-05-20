#include "sensor.h"

SensorData lastReading; // Last sensor reading
SensorData readings[NUM_READINGS]; // Circular buffer of sensor readings
int nextReading = 0; // Index of next spot to insert in buffer
int stepCount = 0; // Total step count
String motionState = STATE_STANDING; // Current motion state
float calorieBurn = 0.0; // Total calorie burn

bool initMPU6050() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    return false;
  }
  Serial.println("MPU6050 Found!");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  return true;
}

PedoData getPedoData() {

  // Get the latest sensor data
  lastReading = getSensorData();

  // Determine the current motion state
  motionState = determineMotionState();

  // Count steps
  stepCount = countSteps();

  // Compute the calorie burn
  calorieBurn = calculateCaloriesBurned();

  // Get the current timestamp
  unsigned long timestamp = millis();

  // Create a PedoData object
  PedoData pedoData;
  pedoData.sensor_data = lastReading;
  pedoData.motion_state = motionState;
  pedoData.step_count = stepCount;
  pedoData.calorie_burn = calorieBurn;
  pedoData.timestamp = timestamp;

  return pedoData;
}

SensorData getSensorData() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // create new SensorData object
  SensorData sensorData;
  sensorData.ax = a.acceleration.x;
  sensorData.ay = a.acceleration.y;
  sensorData.az = a.acceleration.z;
  sensorData.gx = g.gyro.x;
  sensorData.gy = g.gyro.y;
  sensorData.gz = g.gyro.z;

  // Store the raw sensor readings into the circular buffer
  readings[nextReading] = sensorData;

  // Increment the index for the next reading
  nextReading = (nextReading + 1) % NUM_READINGS;

  return sensorData;
}

String determineMotionState() {
  float a_avg = 0, g_avg = 0;

  // Compute the average magnitudes of acceleration and rotation
  for (int i = 0; i < NUM_READINGS; i++) {
    float a_magnitude = sqrt(readings[i].ax*readings[i].ax + readings[i].ay*readings[i].ay + readings[i].az*readings[i].az) - 9.8;
    float g_magnitude = sqrt(readings[i].gx*readings[i].gx + readings[i].gy*readings[i].gy + readings[i].gz*readings[i].gz);

    a_avg += a_magnitude;
    g_avg += g_magnitude;
  }

  a_avg /= NUM_READINGS;
  g_avg /= NUM_READINGS;

  // Use some thresholds to determine the motion state
  if (a_avg < 0.5 && g_avg < 0.5) {
    Serial.println("Standing still");
    return STATE_STANDING;
  } else if (a_avg < 2.0) {
    Serial.println("Walking");
    return STATE_WALKING;
  } else if (a_avg < 5.0) {
    Serial.println("Running");
    return STATE_RUNNING;
  } else {
    Serial.println("Jumping");
    return STATE_JUMPING;
  }
}

int countSteps() {
    float ax = lastReading.ax, ay = lastReading.ay, az = lastReading.az;
    
    float a_magnitude = sqrt(ax*ax + ay*ay + az*az) - 9.8;
    
    if (a_magnitude > STEP_THRESHOLD) {
        stepCount++;
        Serial.println("Step Count: " + String(stepCount));
    }
    return stepCount;
}

float calculateCaloriesBurned() {
  float MET = 0;
  if (motionState == STATE_STANDING) {
      MET = MET_STANDING;
  } else if (motionState == STATE_WALKING) {
      MET = MET_WALKING;
  } else if (motionState == STATE_RUNNING) {
      MET = MET_RUNNING;
  } else if (motionState == STATE_JUMPING) {
      MET = MET_JUMPING;
  }

  // Estimate Basal Metabolic Rate (BMR) using Harris-Benedict equation
  float BMR;
  if (userData.gender == "Male") {
      BMR = 88.362 + (13.397 * userData.weight) + (4.799 * userData.height) - (5.677 * userData.age);
  } else {
      BMR = 447.593 + (9.247 * userData.weight) + (3.098 * userData.height) - (4.330 * userData.age);
  }

  // Calculate calories burned per minute
  float caloriesPerMinute = MET * 3.9 * userData.weight / 200;

  // Estimate the duration for one step
  float durationOfStepInMinutes = 0.00833; // about 0.5 second

  // Calculate total calories burned (considering the duration of the activity)
  float totalCaloriesBurned = caloriesPerMinute * durationOfStepInMinutes;

  // Calculate active calories burned (total calories - BMR calories)
  float activeCaloriesBurned = totalCaloriesBurned - (BMR / 1440 * durationOfStepInMinutes);

  calorieBurn += activeCaloriesBurned;

  return calorieBurn;
}

