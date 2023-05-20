#pragma once
#include "global_definitions.h"

bool initMPU6050();
PedoData getPedoData();
SensorData getSensorData();
String determineMotionState();
int countSteps();
float calculateCaloriesBurned();
