#include "utilities.h"

String getPedoDataJSON(PedoData pedoData)
{
  String json = "{";
  json += "\"acceleration_x\": " + String(pedoData.sensor_data.ax) + ",";
  json += "\"acceleration_y\": " + String(pedoData.sensor_data.ay) + ",";
  json += "\"acceleration_z\": " + String(pedoData.sensor_data.az) + ",";
  json += "\"gyro_x\": " + String(pedoData.sensor_data.gx) + ",";
  json += "\"gyro_y\": " + String(pedoData.sensor_data.gy) + ",";
  json += "\"gyro_z\": " + String(pedoData.sensor_data.gz) + ",";
  json += "\"motion_state\": \"" + String(pedoData.motion_state) + "\",";
  json += "\"calorie_burn\": " + String(pedoData.calorie_burn) + ",";
  json += "\"step_count\": " + String(pedoData.step_count) + ",";
  json += "\"timestamp\": " + String(pedoData.timestamp);
  json += "}";
  return json;
}

String getUserDataJSON()
{
  String json = "{";
  json += "\"weight\": " + String(userData.weight) + ",";
  json += "\"height\": " + String(userData.height) + ",";
  json += "\"age\": " + String(userData.age) + ",";
  json += "\"gender\": \"" + String(userData.gender) + "\",";
  json += "\"calorie_goal\": " + String(userData.calorieGoal);
  json += "}";
  return json;
}

void printAcceleration(sensors_event_t a) {
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.println(a.acceleration.z);
}

void printGyro(sensors_event_t g) {
  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.println(g.gyro.z);
}

void printTemp(sensors_event_t temp) {
  Serial.print("Temperature: ");
  Serial.println(temp.temperature);
}
