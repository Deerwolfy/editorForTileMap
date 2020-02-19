#ifndef BUTTON_H
#define BUTTON_H

#include"texture.h"
#include"windowWrapper.h"
#include"font.h"
#include<SDL.h>
#include<functional>
#include<string>
#include<memory>

class Button {
public:
  Button(int x, int y, int tbPadding, int rlPadding):
    tbPadding(tbPadding), rlPadding(rlPadding), frame({x,y,rlPadding+rlPadding,tbPadding+tbPadding}) { }
  Button &setText(WindowWrapper&, const Font&, const std::string&);
  Button &setButtonId(int id) { buttonId = id; return *this; }
  Button &setBorderColor(SDL_Color);
  Button &setBackgroundColor(SDL_Color color);
  Button &setHoverColor(SDL_Color color) { hoverColor = color; return *this; }
  Button &setLeftClickCallback(std::function<void(const Button&)> callback)
    { leftClickCallback = callback; return *this; }
  Button &setRightClickCallback(std::function<void(const Button&)> callback)
    { rightClickCallback = callback; return *this; }
  void render(WindowWrapper &w) const;
  void mouseMove(SDL_Event&);
  int leftClick(SDL_Event&);
  int rightClick(SDL_Event&);
  Button &setX(int x) { frame.x = x; return *this; }
  Button &setY(int y) { frame.y = y; return *this; }
  int getX() const { return frame.x; }
  int getY() const { return frame.y; }
  int getWidth() const { return frame.w; }
  int getHeight() const { return frame.h; }
private:
  bool hover = false;
  bool backgroundIsSet = false;
  bool borderIsSet = false;
  int buttonId = 0;
  int tbPadding;
  int rlPadding;
  std::function<void(const Button&)> leftClickCallback;
  std::function<void(const Button&)> rightClickCallback;
  Texture text;
  SDL_Color borderColor;
  SDL_Color backgroundColor;
  SDL_Color hoverColor;
  SDL_Color activeBackground;
  SDL_Rect frame;
};

#endif
