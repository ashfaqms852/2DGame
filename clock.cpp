#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  ticks(0),
  sumOfTicks(0),
  totalTicksAtStart(0),
  totalTicksAtPause(0),
  currentTicks(0),
  previousTicks(0),
  started(false), 
  paused(false), 
  sloMo(false),
  numberOfFrames(0),
  framesAreCapped(Gamedata::getInstance().getXmlBool("clock/framesAreCapped")), 
  frameCap(Gamedata::getInstance().getXmlInt("clock/frameCap")),
  adjustmentFactor(Gamedata::getInstance().getXmlInt("clock/adjustmentFactor"))
{
  start();
}

Clock::Clock(const Clock& c) :
  ticks(c.ticks),
  sumOfTicks(c.sumOfTicks),
  totalTicksAtStart(c.totalTicksAtStart),
  totalTicksAtPause(c.totalTicksAtPause),
  currentTicks(c.currentTicks),
  previousTicks(c.previousTicks),
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo),
  numberOfFrames(c.numberOfFrames),
  framesAreCapped(c.framesAreCapped), 
  frameCap(c.frameCap),
  adjustmentFactor(c.adjustmentFactor)
{ }

void Clock::update() {
  if(!paused) ++numberOfFrames;
  sumOfTicks += ticks;
}

unsigned int Clock::getTicks() const { 
  if (paused) return totalTicksAtPause;
  else return SDL_GetTicks() - totalTicksAtStart; 
}

void Clock::makeAdjustmentsToTicks() {
  currentTicks = getTicks();
  ticks = currentTicks-previousTicks;
  unsigned int lag = capFrameRate();
  previousTicks = currentTicks + lag;
  ticks += lag;  
}

unsigned int Clock::getTicksSinceLastFrame() {
  if (paused) return 0;
  else if ( sloMo ) return 1;
  else{
    makeAdjustmentsToTicks();
    return ticks;
  }
}

void Clock::toggleSloMo() {
  if( started and !sloMo ) {
    totalTicksAtPause = SDL_GetTicks() - totalTicksAtStart;
    sloMo = true;
  }
  else if ( started and sloMo ) {
    totalTicksAtStart = SDL_GetTicks() - totalTicksAtPause;
    sloMo = false;
  }
}

void Clock::toggleCapFrame() {
  framesAreCapped = !framesAreCapped;
  makeAdjustmentsToTicks();
}

int Clock::getFps() const {
  unsigned int ticksToSeconds = getSeconds(); 
  if ( !paused and ticksToSeconds > 0 and numberOfFrames > 0){
    return numberOfFrames/ticksToSeconds;
  }
  else return 0;
}

unsigned int Clock::capFrameRate() const {
  unsigned int lag; 
  if (framesAreCapped && (ticks < (1000.0 / frameCap)) ) {   
  lag = 1000.0/frameCap - ticks + adjustmentFactor;
  SDL_Delay(lag);
  return lag;
  }
  else return 0u;
}

void Clock::start() { 
  started = true; 
  paused = false; 
  numberOfFrames = 0;
  totalTicksAtPause = totalTicksAtStart = SDL_GetTicks(); 
}

void Clock::pause() { 
  if( started and !paused ) {
    totalTicksAtPause = SDL_GetTicks() - totalTicksAtStart;
    paused = true;
  }
}

void Clock::unpause() { 
  if( started and paused ) {
    totalTicksAtStart = SDL_GetTicks() - totalTicksAtPause;
    paused = false;
  }
}

