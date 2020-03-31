#ifndef SELECTIONBOX_H
#define SELECTIONBOX_H

#include<SDL.h>
#include"guiElement.h"
#include"windowWrapper.h"

class SelectionBox : public GuiElement{
public:
  SelectionBox(std::shared_ptr<WindowWrapper> window, const SDL_Color &background, const SDL_Color &border):
  GuiElement(window,0,0,0,0) { setBorderColor(border); setBackgroundColor(background); }
  void setStart(int x, int y);
  void setEnd(int x, int y);
  SDL_Point getOrigin() const { return {frame.x,frame.y}; }
  void unhold() { hold = false; }
  bool isHold() { return hold; }
  void render() const override;
private:
  SDL_Point startPoint;
  bool hold = false;
};

#endif