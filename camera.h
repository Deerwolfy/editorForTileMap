#ifndef CAMERA_H
#define CAMERA_H

#include<SDL.h>

class Camera {
public:
  Camera(int x, int y, int w, int h):
  camera({x,y,w,h}) { }
  int getRelativeX(int x) const { return x - camera.x; }
  int getRelativeY(int y) const { return y - camera.y; }
  int mapX(int x) const { return x + camera.x; }
  int mapY(int y) const { return y + camera.y; }
  SDL_Rect getRelativeRect(const SDL_Rect &frame) const
    { return {frame.x-camera.x,frame.y-camera.y,frame.w,frame.h}; };
  SDL_Point getRelativePoint(const SDL_Point &point) const
    { return {point.x-camera.x,point.y-camera.y}; }
  SDL_Rect mapRectInside(const SDL_Rect &frame) const
    { return {frame.x+camera.x,frame.y+camera.y,frame.w,frame.h}; }
  SDL_Point mapPointInside(const SDL_Point &point) const
    { return {point.x+camera.x,point.y+camera.y}; };
  bool isVisible(const SDL_Rect&) const;
  void moveBy(int xInc, int yInc);
  void moveTo(int x, int y) { camera.x = x; camera.y = y;}
  void setXBound(int lower, int upper) { xLowerBound = lower; xUpperBound = upper; xBoundIsSet = true; }
  void setYBound(int lower, int upper) { yLowerBound = lower; yUpperBound = upper; yBoundIsSet = true; }
  void setYMovementSpeed(int speed) { yMovementSpeed = speed; }
  void setXMovementSpeed(int speed) { xMovementSpeed = speed; }
  void moveUp() { moveBy(0,-yMovementSpeed); }
  void moveDown() { moveBy(0,yMovementSpeed); }
  void moveLeft() { moveBy(-xMovementSpeed,0); }
  void moveRight() { moveBy(xMovementSpeed,0); }
private:
  SDL_Rect camera;
  int xLowerBound;
  int xUpperBound;
  bool xBoundIsSet = false;
  int yLowerBound;
  int yUpperBound;
  bool yBoundIsSet = false;
  int yMovementSpeed = 0;
  int xMovementSpeed = 0;
};

#endif