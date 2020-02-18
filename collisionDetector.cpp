#include"collisionDetector.h"

bool isCollide(const SDL_Point &point, const SDL_Rect &rect)
{
  if(point.x < rect.x || point.x > rect.x+rect.w)
    return false;
  if(point.y < rect.y || point.y > rect.y+rect.h)
    return false;
  return true;
}
