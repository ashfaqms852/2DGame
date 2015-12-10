#ifndef PlAYER__H
#define PlAYER__H

#include <string>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "multiBullets.h"
#include "collisionStrategy.h"

class Player : public Drawable {
public:
  Player(const std::string& name);
  Player(const Player&);
  MultiBullets* getBullets() {
    return &bullets;
  }
  bool collidWith(const Drawable* d) const {
    return bullets.collidWith(d);
  } 
  bool playerCollidWith(const Drawable* d) const {
    return strategy->execute(*this, *d);
  }
  virtual ~Player();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  
  virtual const Frame* getFrame() const { 
    return frame; 
  }
  virtual void shoot(bool down);
  void stop();
  void right();
  void left();
  void up();
  void down();
  unsigned int getBulletListSize() const { return bullets.bulletCount(); }
  unsigned int getFreeListSize() const { return bullets.freeCount(); }
  
private:
  Player& operator=(const Player&);
protected:
  const Frame * frame;
  int playerWidth;
  int playerHeight;
  int worldWidth;
  int worldHeight;
  const int speedX;
  const int speedY;
  bool keyPressedX;
  bool keyPressedY;
  std::string bulletName;
  MultiBullets bullets;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  CollisionStrategy * strategy;
};
#endif

