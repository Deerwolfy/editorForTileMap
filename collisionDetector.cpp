#include"collisionDetector.h"

bool Collision::between(const SDL_Point &point, const SDL_Rect &rect)
{
  if(point.x < rect.x || point.x >= rect.x+rect.w)
    return false;
  if(point.y < rect.y || point.y >= rect.y+rect.h)
    return false;
  return true;
}

std::size_t Collision::between(const SDL_Point &point, const std::vector<Tile> &collection)
{
  for(std::size_t i = 0; i != collection.size(); ++i)
    if(between(point,collection[i].getBoundBox()))
      return i;
  return collection.size();
}