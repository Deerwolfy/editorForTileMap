#ifndef INTERSECTION_H
#define INTERSECTION_H

#include<SDL.h>
#include<memory>

class Intersection {
public:
  static std::shared_ptr<SDL_Rect> between(const SDL_Rect&, const SDL_Rect&);
};

#endif