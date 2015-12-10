#ifndef MULTIBULLETS_H
#define MULTIBULLETS_H

#include <list>
#include "multiBullet.h"
#include <vector>
#include "collisionStrategy.h"

class MultiBullets {
public:
  MultiBullets(const std::string&);
  MultiBullets(const MultiBullets&);
  MultiBullets& operator=(const MultiBullets&);
  virtual ~MultiBullets() { delete strategy; } 
  void draw() const;
  virtual void update(Uint32);
  void shoot(const Vector2f&, const Vector2f&);
  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount() const { return freeList.size(); }
  bool shooting() const { return !bulletList.empty(); }
  bool collidWith(const Drawable* d) const;  
private:
  std::string name;
  Vector2f myVelocity;
  Frame* bulletFrame;
  mutable std::list<MultiBullet> bulletList;
  mutable std::list<MultiBullet> freeList;
  CollisionStrategy * strategy; 
};
#endif
