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
  if(camera.x < 0 || xCapIsSet && camera.x+camera.w > xCap)
    camera.x -= inc;
}
void Camera::incY(int inc)
{
  camera.y += inc;
  if(camera.y < 0 || yCapIsSet && camera.y+camera.y > yCap)
    camera.y -= inc;
}