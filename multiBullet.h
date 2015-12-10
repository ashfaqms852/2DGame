#ifndef MULTIBULLET__H
#define MULTIBULLET__H

#include "sprite.h"


class MultiBullet : public Sprite{
public:
  MultiBullet(const std::string&, const Vector2f&, const Vector2f&);
  MultiBullet(const MultiBullet&);
  MultiBullet& operator=(const MultiBullet&);
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset();

protected:
  float distance;
  float maxDistance;
  bool tooFar;
};

#endif
