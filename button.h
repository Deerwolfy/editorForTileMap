#ifndef BUTTON_H
#define BUTTON_H

#include"texture.h"
#include"windowWrapper.h"
#include"font.h"
#include<SDL.h>
#include<functional>
#include<string>

class Button {
public:
  Button(int x, int y, int tbPadding, int rlPadding):
    tbPadding(tbPadding), rlPadding(rlPadding), border({x,y,rlPadding+rlPadding,tbPadding+tbPadding}) { }
  void setText(WindowWrapper&, const Font&, const std::string&);
  void setActionCode(int code) { actionCode = code; }
  void setBorderColor(SDL_Color);
  void setBackgroundColor(SDL_Color color);
  void setHoverColor(SDL_Color color) { hoverColor = color; }
  void render(WindowWrapper &w) const;
  void mouseMove(SDL_Event&);
  int click(SDL_Event&);
  void setX(int x) { border.x = x; }
  void setY(int y) { border.y = y; }
  int getX() const { return border.x; }
  int getY() const { return border.y; }
  int getWidth() const { return border.w; }
  int getHeight() const { return border.h; }
private:
  Texture text;
  SDL_Color borderColor;
  SDL_Color backgroundColor;
  SDL_Color hoverColor;
  SDL_Color activeBackground;
  bool hover = false;
  bool backgroundIsSet = false;
  bool borderIsSet = false;
  int actionCode = 0;
  int tbPadding;
  int rlPadding;
  SDL_Rect border;
};

#endif
