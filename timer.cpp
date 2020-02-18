#include"timer.h"

void Timer::start()
{
  started = true;
  paused = false;
  pausedTicks = 0;
  startTicks = SDL_GetTicks();
}

void Timer::stop()
{
  started = false;
  paused = false;
  pausedTicks = 0;
  startTicks = 0;
}

void Timer::pause()
{
  if(started && !paused){
    paused = true;
    pausedTicks = SDL_GetTicks() - startTicks;
    startTicks = 0; 
  }
}

void Timer::unpause()
{
  if(started && paused){
    paused = false;
    startTicks = SDL_GetTicks() - pausedTicks;
    pausedTicks = 0;
  }
}

Uint32 Timer::getTicks() const
{
  if(started){
    if(paused)
      return pausedTicks;
    else
      return SDL_GetTicks() - startTicks;
  }
  return 0;
}
