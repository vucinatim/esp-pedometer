#include "web_handlers.h"
#include "FS.h"
#include "LittleFS.h"
#include <ArduinoJson.h>
#include "utilities.h"

File workoutFile;
bool isWorkoutActive = false;

void startWorkout() {
  String fileName = "/workout_" + String(millis()) + ".txt";
  workoutFile = LittleFS.open(fileName, "w");
  
  if(workoutFile) {
    // Add user data at the start of the file
    String userDataJSON = getUserDataJSON();
    workoutFile.write(userDataJSON.c_str());
    workoutFile.write("\n");
  }
  
  isWorkoutActive = true;
}

void stopWorkout() {
  isWorkoutActive = false;
  if (workoutFile) {
    workoutFile.close();
  }
}

void pauseWorkout() {
  isWorkoutActive = false;
}

void resumeWorkout() {
  isWorkoutActive = true;
}

void writePedoDataToFile(String pedoDataJSON) {
  if (workoutFile && isWorkoutActive) {
    workoutFile.write(pedoDataJSON.c_str());
  }
}

