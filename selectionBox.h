#ifndef SELECTIONBOX_H
#define SELECTIONBOX_H

#include<SDL.h>
#include"windowWrapper.h"

class SelectionBox {
public:
  SelectionBox(const SDL_Color &background, const SDL_Color &border): selection({0,0,0,0}),
    backgroundColor(background), borderColor(border) { }
  void setStart(int x, int y);
  void setEnd(int x, int y);
  SDL_Point getOrigin() const { return {selection.x,selection.y}; }
  int getWidth() const { return selection.w; }
  int getHeight() const { return selection.h; }
  void unhold() { hold = false; }
  bool isHold() { return hold; }
  void render(WindowWrapper&) const;
private:
  SDL_Point startPoint;
  SDL_Rect selection;
  SDL_Color backgroundColor;
  SDL_Color borderColor;
  bool hold = false;
};

#endif