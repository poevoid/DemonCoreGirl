#include <Arduboy2.h>
Arduboy2 arduboy;
#include "func.h"

void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(60);
}

void loop() {
  if (!arduboy.nextFrame()){return;}
  arduboy.clear();
  arduboy.pollButtons();
  gameloop();
  arduboy.display();
}
