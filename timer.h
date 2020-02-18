#ifndef TIMER_H
#define TIMER_H

#include<SDL.h>

class Timer {
public:
  Timer():
    startTicks(0), pausedTicks(0), paused(false), started(false) { }
  void start();
  void stop();
  void pause();
  void unpause();
  Uint32 getTicks() const;
private:
  Uint32 startTicks;
  Uint32 pausedTicks;
  bool paused;
  bool started;
};

#endif
