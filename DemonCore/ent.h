#pragma once
#include "arrays.h"
#define CRITICALITY 255

struct Lid {
  int centerX = 80;
  int centerY = 55;
  int radius = 25;
  int startAngle = 215;
  int endAngle = 395;
};
struct Screwdriver {
  float tipX = 60;
  float tipY =55;
  float handleX =32;
  float handleY=40;
};


enum class CurrentScreen : uint8_t {
  Title,
  Gameplay,
  Gameover,
  Instructions,
};

CurrentScreen screen = {CurrentScreen::Title};

float clockwise = 0.055;
float counter = -0.02525;
float tick = geiger.freq(24.5);
int levels= 2;
unsigned char newRads = 0; //dangerscore
unsigned char lastSub =0; //for geiger counter

int rads = 0; //score
int highscore =0;

Lid lid;
Screwdriver screw;
