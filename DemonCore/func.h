#include "Arduboy2Core.h"
#include "ent.h"

void rotateScrewdriver(float angle) {
  float s = sin(angle);
  float C = cos(angle);
  screw.handleX -= screw.tipX;
  screw.handleY -= screw.tipY;

  float newx = (screw.handleX * C) - (screw.handleY * s);
  float newy = (screw.handleX * s) + (screw.handleY * C);

  screw.handleX = newx + screw.tipX;
  screw.handleY = newy + screw.tipY;
}
void handleControls() {



  if (arduboy.pressed(LEFT_BUTTON) && lid.startAngle < 235) {
    lid.startAngle += 2;
    lid.endAngle += 2;
    if (screw.handleY > 39 && arduboy.everyXFrames(2)) {
      rotateScrewdriver(clockwise);
    }
  }
  /*if (arduboy.pressed(RIGHT_BUTTON)) {
    lid.startAngle--;
    lid.endAngle--;
    rotateScrewdriver(counter);
  }*/
}

void handleBounds() {

  if (screw.handleY < 50) {
    //lid.startAngle--;
    // lid.endAngle--;
    if (arduboy.everyXFrames(2)) { rotateScrewdriver(counter); }
  }
  if (screw.handleY > 50) {
    screw.handleY = 50;
  }
  if (lid.startAngle > 180 && arduboy.notPressed(LEFT_BUTTON)) {
    lid.startAngle--;
    lid.endAngle--;
  }
  if (lid.startAngle < 180) {
    lid.startAngle = 180;
    lid.endAngle = 360;
  }
}
void handleLED() {
  if (newRads > 0) {
    arduboy.setRGBled(0, 0, newRads * 3);
  } else {
    arduboy.setRGBled(0, 0, 0);
  }
}
void handleScoring() {
  if (lid.startAngle > 180) {
    if (lid.startAngle > 186 && newRads > 0) {
      newRads--;
    }
    if (lid.startAngle < 186 && lid.startAngle > 183) {
      newRads++;
      rads++;
    }
    if (lid.startAngle < 183) {
      rads += 2;
      newRads += 2;
    }
  }
  if (lid.startAngle == 180) {
    newRads = CRITICALITY;

    screen = CurrentScreen::Gameover;
  }
}
void testfunction() {
  if (arduboy.pressed(A_BUTTON)) {
    if (newRads < CRITICALITY) {
      newRads++;
    }
  } else {
    newRads = 0;
  }
}
void drawFilledSector(int x, int y, int r, int startAngle, int endAngle) {
  for (int angle = startAngle; angle <= endAngle; ++angle) {
    float radian = radians(angle);
    int px = x + r * cos(radian);
    int py = y + r * sin(radian);
    arduboy.drawLine(px, py, lid.centerX, lid.centerY);
  }
}
void handleGraphics() {
  arduboy.drawRect(45, 20, levels, 5);
  Sprites::drawOverwrite(7, 0, animegirltop, 0);
  Sprites::drawOverwrite(0, 32, animegirlbttm, 0);
  drawFilledSector(lid.centerX, lid.centerY, lid.radius, lid.startAngle, lid.endAngle);
  arduboy.drawRoundRect(42, 55, 76, 15, 3);  //bottom
  arduboy.drawLine(screw.tipX, screw.tipY, screw.handleX, screw.handleY);
  //arduboy.println(lid.startAngle);
  arduboy.setCursor(45, 10);
  arduboy.print("RADs : ");
  arduboy.print(rads);
  if (screen == CurrentScreen::Gameover) {
    arduboy.setCursor(10, 25);
    arduboy.print("!!!CORE CRITICAL!!!");
    if (arduboy.everyXFrames(5)) {
      arduboy.invert(true);
    } else {
      arduboy.invert(false);
    }
  }
}

void resetGame() {
  lid.radius = 25;
  lid.startAngle = 215;
  lid.endAngle = 395;
  rads = 0;
  newRads = 0;
  levels = 0;
  arduboy.setRGBled(0, 0, 0);
  //arduboy.invert(true); //invert title screen
}

void geigerCounter() {
  if (newRads > 0) {
    if (newRads != lastSub) {
      geiger.tone(newRads / tick, 1);
      if (newRads > lastSub) {
        levels++;
      } else {
        if (levels>1){
        levels--;
      }}
    } else {
      geiger.noTone();
    }
    lastSub = newRads;
  }
  if (levels >= WIDTH - 45) {
    levels = 1;
  }
}


void geiger2() {
  geiger.tone(tick, 10);
}
void gameloop() {
  switch (screen) {
    case CurrentScreen::Title:
      resetGame();
      Sprites::drawOverwrite(50, 15, radiation3232, 0);
      Sprites::drawOverwrite(90, 16, halfsphere, 0);
      Sprites::drawOverwrite(15, 0, halfsphereleft, 0);
      arduboy.setTextWrap(true);
      arduboy.setCursor(36, 47);
      arduboy.println("Demon Core");
      arduboy.setCursor(25, 0);
      arduboy.print("HI-SCORE : ");
      EEPROM.get(EEPROM_STORAGE_SPACE_START, highscore);
      arduboy.print(highscore);
      arduboy.setCursor(0, 55);
      arduboy.print("</A:Start     B:Help");
      if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(LEFT_BUTTON)) {
        screen = CurrentScreen::Gameplay;
      }
      if (arduboy.justPressed(B_BUTTON)) {
        screen = CurrentScreen::Instructions;
      }
      break;

    case CurrentScreen::Gameplay:
      arduboy.invert(true);
      handleControls();
      handleBounds();
      handleLED();
      geigerCounter();
      handleScoring();
      handleGraphics();
      break;

    case CurrentScreen::Gameover:
      if (rads > highscore) {
        highscore = rads;
        EEPROM.put(EEPROM_STORAGE_SPACE_START, highscore);
      }
      handleGraphics();
      handleLED();
      if (lid.radius < 150) {
        lid.radius++;
        if (lid.radius < 64) {
          geiger2();
        }
      } else {
        geiger.noTone();
      }
      if (lid.startAngle > 160) {
        lid.startAngle--;
      }
      if (lid.endAngle < 380) {
        lid.endAngle++;
      }
      if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(RIGHT_BUTTON)) {
        screen = CurrentScreen::Title;
      }

      break;

    case CurrentScreen::Instructions:
      arduboy.setCursor(0, 0);
      arduboy.println("Use the LEFT button  to raise screwdriver. Try to get the core close to criticality, but be careful: If  it goes critical you will be consumed by  nuclear fire!!");
      if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON) || arduboy.justPressed(RIGHT_BUTTON)) {
        screen = CurrentScreen::Title;
      }
      break;
  }
}
