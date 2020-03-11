#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include<SDL.h>
#include"tile.h"
#include<vector>
#include<cstddef>

bool isCollide(const SDL_Point&, const SDL_Rect&);
std::size_t isCollide(const SDL_Point&, const std::vector<Tile>&);

#endif
