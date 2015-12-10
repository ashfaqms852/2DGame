#include <iostream>
#include "multiBullet.h"
#include <cmath>
#include "gamedata.h"
#include "frameFactory.h"

MultiBullet::MultiBullet( const std::string& name, const Vector2f& pos, const Vector2f& vel) :
  Sprite(name, pos, vel, FrameFactory::getInstance().getFrame(name)),
  distance(0),
  maxDistance(Gamedata::getInstance().getXmlInt(name+"/maxDistance")),
  tooFar(false)
{}

MultiBullet::MultiBullet(const MultiBullet& mb) :
  Sprite(mb),
  distance(mb.distance),
  maxDistance(mb.maxDistance),
  tooFar(mb.tooFar)
{}


MultiBullet& MultiBullet::operator=(const MultiBullet& rhs) {
  Sprite::operator=( rhs );
  distance = rhs.distance;
  maxDistance = rhs.maxDistance;
  tooFar = rhs.tooFar;
  return *this;
}


void MultiBullet::reset(){
  tooFar = false;
  distance = 0;
}

void MultiBullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr); 

  if(Y()+frameHeight < 0 || Y() > worldHeight) {
    tooFar = true;
  }
  if(X() < 0 || X() > worldWidth){
    tooFar  = true;
  }
  distance += (hypot(X()-pos[0], Y()-pos[1]));
  if(distance > maxDistance)
    tooFar = true; 
}
