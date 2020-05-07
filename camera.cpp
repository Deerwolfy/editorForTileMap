#include"camera.h"
#include<iostream>

bool Camera::isVisible(const SDL_Rect &frame) const
{
  if(frame.x+frame.w <= camera.x || frame.x >= camera.x+camera.w)
    return false;
  if(frame.y+frame.h-yOffset <= camera.y || frame.y-yOffset >= camera.y+camera.h)
    return false;
  return true;
}

void Camera::moveBy(int xInc, int yInc)
{
  camera.x += xInc;
  if(xBoundIsSet && camera.x < xLowerBound)
    camera.x = xLowerBound;
  else if(xBoundIsSet && camera.x+camera.w > xUpperBound)
    camera.x = xUpperBound-camera.w;

  camera.y += yInc;
  if(yBoundIsSet && camera.y < yLowerBound)
    camera.y = yLowerBound;
  else if(yBoundIsSet && camera.y+camera.h > yUpperBound)
    camera.y = yUpperBound-camera.h;
}