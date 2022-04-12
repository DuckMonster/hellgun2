#pragma once

void time_init();
void time_update();

float time_delta();
float time_delta_raw();
float time_elapsed();
float time_elapsed_raw();

float time_since(float time_point);
bool time_has_reached(float time_point);

void time_dilate(float value);
