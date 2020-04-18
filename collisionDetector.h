#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include<SDL.h>
#include"tile.h"
#include<vector>
#include<cstddef>

class Collision{
public:
  static bool between(const SDL_Point&, const SDL_Rect&);
  static std::size_t between(const SDL_Point&, const std::vector<Tile>&);
};

#endif
