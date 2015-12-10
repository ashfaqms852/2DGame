#include "headsUpDisplay.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "aaline.h"

HeadsUpDisplay::HeadsUpDisplay() :
  HUD_WIDTH(Gamedata::getInstance().getXmlInt("hudDetails/hudDimensions/x")),
  HUD_HEIGHT(Gamedata::getInstance().getXmlInt("hudDetails/hudDimensions/y")),
  x(Gamedata::getInstance().getXmlInt("hudDetails/hudLocation/x")),
  y(Gamedata::getInstance().getXmlInt("hudDetails/hudLocation/y")),
  secondsLocation(Gamedata::getInstance().getXmlInt("textPositioning/secondsLocation/x"),
         Gamedata::getInstance().getXmlInt("textPositioning/secondsLocation/y")),
  fpsLocation(Gamedata::getInstance().getXmlInt("textPositioning/fpsLocation/x"),
         Gamedata::getInstance().getXmlInt("textPositioning/fpsLocation/y")),
  playerMovementLocation(Gamedata::getInstance().getXmlInt("textPositioning/playerMovementLocation/x"),
         Gamedata::getInstance().getXmlInt("textPositioning/playerMovementLocation/y")),
  toggleHUDLocation(Gamedata::getInstance().getXmlInt("textPositioning/toggleHUDLocation/x"),
         Gamedata::getInstance().getXmlInt("textPositioning/toggleHUDLocation/y")),
  restartGameLocation(Gamedata::getInstance().getXmlInt("textPositioning/restartGameLocation/x"),
         Gamedata::getInstance().getXmlInt("textPositioning/restartGameLocation/y")),
  godModeLocation(Gamedata::getInstance().getXmlInt("textPositioning/godModeLocation/x"),
         Gamedata::getInstance().getXmlInt("textPositioning/godModeLocation/y")),
  bulletListLocation(Gamedata::getInstance().getXmlInt("textPositioning/bulletListLocation/x"),
         Gamedata::getInstance().getXmlInt("textPositioning/bulletListLocation/y")),
  freeListLocation(Gamedata::getInstance().getXmlInt("textPositioning/freeListLocation/x"),
         Gamedata::getInstance().getXmlInt("textPositioning/freeListLocation/y"))  
{}
void HeadsUpDisplay::drawHUD(SDL_Surface * const& screen, const IOManager& io,int seconds,int fps , unsigned int bulletCount , unsigned freeCount) const{
 
  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, x, y+HUD_HEIGHT/2, x+HUD_WIDTH, y+HUD_HEIGHT/2, HUD_HEIGHT, 0xff, 0xff, 0xff, 0xff/2);
    io.printMessageValueAt("Seconds: ", seconds, secondsLocation[0], secondsLocation[1]);
    io.printMessageValueAt("fps: ", fps, fpsLocation[0], fpsLocation[1]);
    io.printMessageAt("Use WASD to move the player.", playerMovementLocation[0], playerMovementLocation[1]);
    io.printMessageAt("Press F1 to toggle HUD.", toggleHUDLocation[0], toggleHUDLocation[1]);
    io.printMessageAt("Press R to restart the game.", restartGameLocation[0], restartGameLocation[1]);
    io.printMessageAt("Press G to toggle God mode.", godModeLocation[0], godModeLocation[1]);
    io.printMessageValueAt("Bullet List Count: ", bulletCount, bulletListLocation[0], bulletListLocation[1]);
    io.printMessageValueAt("Free List Count: ", freeCount, freeListLocation[0], freeListLocation[1]);
  Draw_AALine(screen, x,y,x+HUD_WIDTH,y, RED);

}
