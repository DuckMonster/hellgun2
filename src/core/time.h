#pragma once

void time_init();
void time_update();

float time_delta();
float time_delta_raw();
float time_elapsed();

void time_dilate(float value);
