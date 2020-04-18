#ifndef TILE_H
#define TILE_H

#include"texture.h"
#include"windowWrapper.h"
#include"camera.h"
#include<SDL.h>

class Tile {
public:
  Tile(const Texture &texture): texture(texture), x(0), y(0) { }
  Tile(const Texture &texture, int x, int y):
    texture(texture), x(x), y(y) { }
  void render(SDL_Renderer *renderer) const { texture.render(renderer,x,y); }
  void render(SDL_Renderer *renderer, const Camera &camera) const
    { texture.render(renderer,camera.getRelativeX(x),camera.getRelativeY(y)); }
  int getX() const { return x; }
  int getY() const { return y; }
  void setX(int x) { this->x = x; }
  void setY(int y) { this->y = y; }
  int getWidth() const { return texture.getWidth(); }
  int getHeight() const { return texture.getHeight(); }
  SDL_Rect getBoundBox() const { return {x,y,texture.getWidth(), texture.getHeight()}; }
private:
  Texture texture;
  int x;
  int y;
};

#endif
