#include "web_handlers.h"
#include "FS.h"
#include "LittleFS.h"
#include <ArduinoJson.h>
#include "workout.h"
#include "utilities.h"

const char *ssid = "Booba";
const char *password = "kkjegeslo";

ESP8266WebServer server(80);

void setupWiFi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupWebServer()
{
  if (!LittleFS.begin())
  {
    Serial.println("An error has occurred while mounting LittleFS");
    return;
  }

  server.on("/", HTTP_GET, []()
            {
    File file = LittleFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
    file.close(); });

  server.on("/api/data", HTTP_GET, []()
            {
    // Get the data from the sensor
    PedoData pedoData = getPedoData();
    // Convert the data to JSON
    String pedoDataJSON = getPedoDataJSON(pedoData);
    // Write the data to the file
    writePedoDataToFile(pedoDataJSON);
    // Send the data to the client
    server.send(200, "application/json", pedoDataJSON); });

  server.on("/api/user", HTTP_GET, []()
            {
    String data = getUserDataJSON();
    server.send(200, "application/json", data); });

  server.on("/api/user", HTTP_POST, handleUserData);

  server.on("/runner_boy.riv", HTTP_GET, []()
            {
    File file = LittleFS.open("/runner_boy.riv", "r");
    server.streamFile(file, "application/javascript"); });

  server.on("/api/start_workout", HTTP_POST, []()
            {
  String workoutName = startWorkout();
  server.send(200, "application/json", "{\"name\":\"" + workoutName + "\",\"message\":\"Workout session started\"}"); });

  server.on("/api/stop_workout", HTTP_POST, []()
            {
  stopWorkout();
  server.send(200, "application/json", "{\"message\":\"Workout session stopped\"}"); });

  server.on("/api/pause_workout", HTTP_POST, []()
            {
  pauseWorkout();
  server.send(200, "application/json", "{\"message\":\"Workout session paused\"}"); });

  server.on("/api/resume_workout", HTTP_POST, []()
            {
  resumeWorkout();
  server.send(200, "application/json", "{\"message\":\"Workout session resumed\"}"); });

  server.on("/api/workouts", HTTP_GET, []()
            {
  Dir dir = LittleFS.openDir("/");
  String fileList = "[";
  while (dir.next()) {
    String fileName = dir.fileName();
    if (fileName.startsWith("workout")) {
      if (fileList.length() > 1) {
        fileList += ",";
      }
      fileList += "\"" + fileName + "\"";
    }
  }
  fileList += "]";
  server.send(200, "application/json", fileList); });

  server.on("/api/workout", HTTP_GET, []()
            {
  if (!server.hasArg("file")) {
    server.send(400, "application/json", "{\"message\":\"File not specified\"}");
    return;
  }

  String fileName = server.arg("file");
  if (!LittleFS.exists(fileName)) {
    server.send(404, "application/json", "{\"message\":\"File not found\"}");
    return;
  }

  File file = LittleFS.open(fileName, "r");
  server.streamFile(file, "application/json");
  file.close(); });

  server.on("/api/files", HTTP_GET, []() {
    Dir dir = LittleFS.openDir("/");
    String fileList = "[";
    while (dir.next()) {
      String fileName = dir.fileName();
      if (fileList.length() > 1) {
        fileList += ",";
      }
      fileList += "\"" + fileName + "\"";
    }
    fileList += "]";
    server.send(200, "application/json", fileList);
  });


  server.begin();
  Serial.println("HTTP server started");
}

void handleUserData()
{
  if (server.hasArg("plain") == false)
  { // Check if body received
    server.send(422, "application/json", "{\"message\":\"Body not found\"}");
    return;
  }

  String messageBody = server.arg("plain"); // Get the body of the request

  // Create the JSON document
  DynamicJsonDocument doc(1024);

  // Parse the JSON from the request body
  deserializeJson(doc, messageBody);

  // Extract values and assign them to the global variables
  userData.weight = doc["weight"];
  userData.height = doc["height"];
  userData.age = doc["age"];
  userData.gender = doc["gender"].as<String>();
  userData.calorieGoal = doc["calorieGoal"];

  // Send a response
  server.send(200, "application/json", "{\"message\":\"Successfully updated user details\"}");
}
