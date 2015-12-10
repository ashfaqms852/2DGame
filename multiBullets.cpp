#include <iostream>
#include "multiBullets.h"
#include "gamedata.h"
#include "frameFactory.h"

MultiBullets::MultiBullets( const std::string& name) :
  name(name),
  myVelocity( Gamedata::getInstance().getXmlInt(name+"/speed/x"),
    	      Gamedata::getInstance().getXmlInt(name+"/speed/y")),
  bulletFrame(FrameFactory::getInstance().getFrame(name)),
  bulletList(),
  freeList(),
  strategy(new PerPixelCollisionStrategy())
{}

MultiBullets::MultiBullets(const MultiBullets& b) :
  name(b.name),
  myVelocity(b.myVelocity),
  bulletFrame(b.bulletFrame),
  bulletList(b.bulletList),
  freeList(b.freeList),
  strategy(b.strategy)
{}

MultiBullets& MultiBullets::operator=(const MultiBullets& rhs) {
  name = rhs.name;
  myVelocity = rhs.myVelocity;
  bulletFrame = rhs.bulletFrame;
  bulletList = rhs.bulletList;
  freeList = rhs.freeList;
  strategy = rhs.strategy;
  return *this;
}

bool MultiBullets::collidWith(const Drawable* obj) const {
  std::list<MultiBullet>::iterator ptr = bulletList.begin();
  while(ptr != bulletList.end()){
    if(strategy->execute(*ptr, *obj)){
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}


void MultiBullets::shoot(const Vector2f& pos, const Vector2f& objVel){
  if(freeList.empty()){
    MultiBullet b(name, pos, objVel);
    bulletList.push_back(b);
  }
  else{
    MultiBullet b = freeList.front();
    freeList.pop_front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    bulletList.push_back(b);
  }
}

void MultiBullets::draw() const { 
  std::list<MultiBullet>::const_iterator ptr = bulletList.begin();
  while(ptr != bulletList.end()){
    ptr->draw();
    ++ptr;
  }
}

void MultiBullets::update(Uint32 ticks) {
  std::list<MultiBullet>::iterator ptr = bulletList.begin();
  while(ptr != bulletList.end()){
    ptr->update(ticks);
    if(ptr->goneTooFar()){
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }
    else ++ptr;
  }
}
