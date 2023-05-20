#include "web_handlers.h"
#include "FS.h"
#include "LittleFS.h"
#include <ArduinoJson.h>
#include "utilities.h"

File workoutFile;
bool isWorkoutActive = false;

String startWorkout()
{
    String workoutName = "workout_" + String(millis());
    String fileName = "/" + workoutName + ".txt";
    workoutFile = LittleFS.open(fileName, "w");

    if (workoutFile)
    {
        // Add user data at the start of the file
        String userDataJSON = getUserDataJSON();
        workoutFile.write("{\n");
        workoutFile.write("\"user_data\":");
        workoutFile.write(userDataJSON.c_str());
        workoutFile.write(",\n");
        workoutFile.write("\"workout_data\": [\n");
    }

    isWorkoutActive = true;
    return workoutName;
}

void stopWorkout()
{
    isWorkoutActive = false;
    if (workoutFile)
    {
        workoutFile.write("{ \"end_time\": ");
        workoutFile.write(String(millis()).c_str());
        workoutFile.write("}\n");
        workoutFile.write("\n]\n");
        workoutFile.write("}");
        workoutFile.close();
    }
}

void pauseWorkout()
{
    isWorkoutActive = false;
}

void resumeWorkout()
{
    isWorkoutActive = true;
}

void writePedoDataToFile(String pedoDataJSON)
{
    if (workoutFile && isWorkoutActive)
    {
        workoutFile.write(pedoDataJSON.c_str());
        workoutFile.write(",\n");
    }
}
