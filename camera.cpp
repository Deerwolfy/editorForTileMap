#include"camera.h"

bool Camera::isVisible(const SDL_Rect &frame) const
{
  if(frame.x+frame.w <= camera.x || frame.x >= camera.x+camera.w)
    return false;
  if(frame.y+frame.h <= camera.y || frame.y >= camera.y+camera.h)
    return false;
  return true;
}

void Camera::incX(int inc)
{
  camera.x += inc;
  if(xCapIsSet && (camera.x < xLowerCap || camera.x+camera.w > xUpperCap))
    camera.x -= inc;
}
void Camera::incY(int inc)
{
  camera.y += inc;
  if(yCapIsSet && (camera.y < yLowerCap || camera.y+camera.h > yUpperCap))
    camera.y -= inc;
}