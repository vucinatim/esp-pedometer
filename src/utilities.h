#pragma once
#include "global_definitions.h"

String getPedoDataJSON(PedoData pedoData);
String getUserDataJSON();

void printAcceleration(sensors_event_t a);
void printGyro(sensors_event_t g);
void printTemp(sensors_event_t temp);
