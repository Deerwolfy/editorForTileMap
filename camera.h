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
  void incX(int inc);
  void incY(int inc);
  void setX(int x) { camera.x = x; }
  void setY(int y) { camera.y = y; }
  void setXScrollCap(int lowerCap, int upperCap) { xLowerCap = lowerCap; xUpperCap = upperCap; xCapIsSet = true; }
  void setYScrollCap(int lowerCap, int upperCap) { yLowerCap = lowerCap; yUpperCap = upperCap; yCapIsSet = true; }
  void setYScrollSpeed(int speed) { yScrollSpeed = speed; }
  void setXScrollSpeed(int speed) { xScrollSpeed = speed; }
  void scrollUp() { incY(-yScrollSpeed); }
  void scrollDown() { incY(yScrollSpeed); }
  void scrollLeft() { incX(-xScrollSpeed); }
  void scrollRight() { incX(xScrollSpeed); }
private:
  SDL_Rect camera;
  int xLowerCap;
  int xUpperCap;
  bool xCapIsSet = false;
  int yLowerCap;
  int yUpperCap;
  bool yCapIsSet = false;
  int yScrollSpeed = 0;
  int xScrollSpeed = 0;
};

#endif