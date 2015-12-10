#ifndef CLOCK_H
#define CLOCK_H
#include <SDL.h>
#include <string>

class Manager;

class Clock {
public:
  static Clock& getInstance();  // This class is a Singleton
  unsigned int getTicksSinceLastFrame();
  unsigned int getTicksSinceInit() const { return sumOfTicks; }
  unsigned int capFrameRate() const;
  void toggleSloMo();
  void toggleCapFrame();
  void makeAdjustmentsToTicks();
  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  int getFps() const;
  unsigned int getTicks() const;
  unsigned int getSeconds() const { return getTicks()/1000;  }
  void start();
  void pause();
  void unpause();
  void update();  // Increments time/ticks

private:
  unsigned int ticks;
  unsigned int sumOfTicks;
  unsigned int totalTicksAtStart;
  unsigned int totalTicksAtPause;
  unsigned int currentTicks;
  unsigned int previousTicks;
  bool started;
  bool paused;
  bool sloMo;
  unsigned int numberOfFrames;
  bool framesAreCapped;
  const int frameCap;
  const unsigned int adjustmentFactor;
  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
#endif
