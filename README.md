ESP8266 Pedometer
=================

This repository contains the code for an ESP8266 based pedometer which makes use of the MPU6050 sensor to measure the motion and provides HTTP endpoints for the data. It also includes various utility functions to manipulate the data gathered from the sensor.

Prerequisites
-------------

1.  PlatformIO
2.  ESP8266 Development Board
3.  MPU6050 Sensor

Libraries used
--------------

1.  Adafruit MPU6050 v2.0.3
2.  Adafruit Unified Sensor v1.1.4
3.  ArduinoJson v6.21.2

![Alt text](https://github.com/vucinatim/esp-pedometer/blob/8f3f74629b24570afe5bdcb84c8dcce368680e52/Schematic.png)

Setup
-----

Firstly, clone the repository to your local machine.

To install the required libraries, follow these steps:

1.  Open the PlatformIO home page in your IDE.
2.  Go to `Libraries`.
3.  Search for `Adafruit MPU6050`, `Adafruit Unified Sensor` and `ArduinoJson`.
4.  Click on each library and click `Add to Project`. Choose this project to add the library to it.

After installing the required libraries, build the project using the PlatformIO `Build` option.

Deployment
----------

Connect the ESP8266 and upload the code using the PlatformIO `Upload` option. The sensor data can be viewed from the serial monitor.

API
---

The firmware provides the following HTTP endpoints:

1.  `GET /api/data` - Returns the sensor data.
2.  `GET /api/user` - Returns the user data.
3.  `POST /api/user` - Accepts the user data in the form of JSON. The format of the data should be: `{"weight": <value>, "height": <value>, "age": <value>, "gender": <value>, "calorieGoal": <value>}`.
4.  `POST /api/start_workout` - Starts a workout session.
5.  `POST /api/stop_workout` - Stops a workout session.
6.  `POST /api/pause_workout` - Pauses a workout session.
7.  `POST /api/resume_workout` - Resumes a paused workout session.
8.  `GET /api/workouts` - Returns a list of all workouts.
9.  `GET /api/workout?file=<workout file>` - Returns the details of a specific workout.

Data Structure
--------------

The data from the `/api/data` endpoint is returned in the following format:



```json
{
    "acceleration_x": "<float>",
    "acceleration_y": "<float>",
    "acceleration_z": "<float>",
    "gyro_x": "<float>",
    "gyro_y": "<float>",
    "gyro_z": "<float>",
    "motion_state": "<string>",
    "calorie_burn": "<float>",
    "step_count": "<int>",
    "timestamp": "<unsigned_long>"
}
```

The user data from the `/api/user` endpoint is returned in the following format:



```json
{
    "weight": "<float>",
    "height": "<float>",
    "age": "<int>",
    "gender": "<string>",
    "calorie_goal": "<float>"
}
```

Troubleshooting
---------------

Ensure that the sensor is connected correctly. If the device fails to find the MPU6050 sensor, check the connections.

If the device does not connect to WiFi, ensure that the correct SSID and password are entered in the `ssid` and `password` variables respectively in the `web_handlers.h` file.

For further issues or assistance, you can raise an issue on this repository.

Contributors
------------

*   Tim Vučina & Andrej Kronovšek
