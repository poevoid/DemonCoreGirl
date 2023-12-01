#include <Arduboy2.h>
Arduboy2 arduboy;
BeepPin1 geiger;
#include "func.h"

void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  geiger.begin();
  arduboy.setFrameRate(60);
}

void loop() {
  if (!arduboy.nextFrame()){return;}
  geiger.timer();
  arduboy.clear();
  arduboy.pollButtons();
  gameloop();
  arduboy.display();
}
