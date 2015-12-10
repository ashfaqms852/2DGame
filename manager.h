#ifndef MANAGER_H
#define MANAGER_H
#include <SDL.h>
#include <vector>
#include <list>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "scaledSprite.h"
#include "headsUpDisplay.h"
#include "health.h"
#include "explodingSprite.h"
#include "smartSprite.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void setHUD(bool showHUD) {
    hudFlag = showHUD;  
  }

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  World backBuilding;
  World frontBuilding;
  World poles;
  World road;
  Viewport& viewport;
  bool hudFlag;
  HeadsUpDisplay hud;
  Health healthBar;
  std::vector<Drawable*> sprites;
  std::list<ExplodingSprite*> explodingScaledSprite;
  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  unsigned int numberOfRockets;
  int score;
  bool healthRemaining;
  SDLSound sound;
  bool godMode;
  void draw() const;
  void update();
  void makeScaledRockets();
  void drawHUD() const;
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
#endif
