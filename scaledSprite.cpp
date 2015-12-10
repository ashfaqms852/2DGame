#include<iostream>
#include<cmath>
#include "SDL/SDL_rotozoom.h"
#include "scaledSprite.h"
#include "gamedata.h"
#include "frameFactory.h"

ScaledSprite::ScaledSprite(const std::string& name):
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/loc/x"),
             Gamedata::getInstance().getXmlInt(name+"/loc/y")),
           Vector2f((rand()%2?1:-1)*Gamedata::getInstance().getRandFloat(
             Gamedata::getInstance().getXmlInt(name+"/cushion"),
             Gamedata::getInstance().getXmlInt(name+"/speed/x")),
               (rand()%2?1:-1)*Gamedata::getInstance().getRandFloat(
             Gamedata::getInstance().getXmlInt(name+"/cushion"),
             Gamedata::getInstance().getXmlInt(name+"/speed/y"))
           )
          ),
  scale(Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
  Gamedata::getInstance().getXmlFloat(name+"/scale/max"))),
  frame(FrameFactory::getInstance().getFrame(name)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  turn(0),
  yLimit(Gamedata::getInstance().getXmlInt(name+"/yLimit"))
{ }

ScaledSprite::ScaledSprite( const std::string& name, const Vector2f& pos, const Vector2f& vel) :
  Drawable(name, pos, vel),
  scale(Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
  Gamedata::getInstance().getXmlFloat(name+"/scale/max"))),
  frame(FrameFactory::getInstance().getFrame(name)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  turn(0),
  yLimit(Gamedata::getInstance().getXmlInt(name+"/yLimit")) 
{ }

ScaledSprite::ScaledSprite(const ScaledSprite& s) :
  Drawable(s.getName(),s.getPosition(), s.getVelocity()),
  scale(s.scale),
  frame(s.frame),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  turn(s.turn),
  yLimit(s.yLimit)
{ }

ScaledSprite& ScaledSprite::operator=(const ScaledSprite& rhs) {
  setName(rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  scale = rhs.scale;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  turn = rhs.turn;
  yLimit = rhs.yLimit;
  return *this;
}
void ScaledSprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if(!turn) frame->draw(x, y);
  else frame->drawLeft(x,y); 
}

void ScaledSprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > yLimit) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
    turn = 0;
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
    turn = 1;
  }  
}

