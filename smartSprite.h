#ifndef SMARTSPRITE_H
#define SMARTSPRITE_H

#include <iostream>
#include "scaledSprite.h"
#include "ioManager.h"

class SmartSprite : public ScaledSprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, const ScaledSprite& p);
  void draw() const;
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  void goUp();
  void goDown();

  static void incrSafeDistance() {++safeDistance; }
  static void decrSafeDistance() { --safeDistance; }
  static float getSafeDistance() { return safeDistance; }

private:	
  enum MODE {NORMAL, EVADE};
  IOManager& io;	
  const ScaledSprite & enemy;
  static float safeDistance;
  MODE currentMode;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
};

#endif


