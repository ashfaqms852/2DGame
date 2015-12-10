#ifndef HEADSUPDISPLAY__H
#define HEADSUPDISPLAY__H
#include <string>
#include <SDL.h>
#include "ioManager.h"
#include "vector2f.h"

class HeadsUpDisplay {
public:
  HeadsUpDisplay();
  void drawHUD(SDL_Surface * const&, const IOManager&, int seconds, int fps , unsigned int b , unsigned int f) const;

private:
  const int HUD_WIDTH;
  const int HUD_HEIGHT;
  int x;
  int y;
  Vector2f secondsLocation;
  Vector2f fpsLocation;
  Vector2f playerMovementLocation;
  Vector2f toggleHUDLocation;
  Vector2f restartGameLocation;
  Vector2f godModeLocation;
  Vector2f bulletListLocation;
  Vector2f freeListLocation;   
};
#endif
