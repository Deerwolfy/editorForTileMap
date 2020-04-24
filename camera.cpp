#include"camera.h"

bool Camera::isVisible(const SDL_Rect &frame) const
{
  if(frame.x+frame.w <= camera.x || frame.x >= camera.x+camera.w)
    return false;
  if(frame.y+frame.h <= camera.y || frame.y >= camera.y+camera.h)
    return false;
  return true;
}

void Camera::moveBy(int xInc, int yInc)
{
  camera.x += xInc;
  if(xBoundIsSet && (camera.x < xLowerBound || camera.x+camera.w > xUpperBound))
    camera.x -= xInc;

  camera.y += yInc;
  if(yBoundIsSet && (camera.y < yLowerBound || camera.y+camera.h > yUpperBound))
    camera.y -= yInc;
}