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
    topPadding(tbPadding), bottomPadding(tbPadding), rightPadding(rlPadding), leftPadding(rlPadding), 
    frame({x,y,rlPadding+rlPadding,tbPadding+tbPadding}) { }
  Button &setText(WindowWrapper&, const Font&, const std::string&);
  Button &setButtonId(int id) { buttonId = id; return *this; }
  Button &setBorderColor(SDL_Color);
  Button &setBackgroundColor(SDL_Color);
  Button &setHoverColor(SDL_Color);
  Button &setLeftClickCallback(std::function<void(const Button&)> callback)
    { leftClickCallback = callback; return *this; }
  Button &setRightClickCallback(std::function<void(const Button&)> callback)
    { rightClickCallback = callback; return *this; }
  Button &setIcon(const Texture&, int width = 32, int height = 32, int iconRightPadding = 0);
  Button &setTextAreaWidth(int width) { textAreaWidth = width; return *this; };
  void render(WindowWrapper&) const;
  void mouseMove(const SDL_Event&);
  int leftClick(const SDL_Event&) const;
  int rightClick(const SDL_Event&) const;
  Button &setX(int x) { frame.x = x; return *this; }
  Button &setY(int y) { frame.y = y; return *this; }
  Button &setTopPadding(int);
  Button &setBottomPadding(int);
  Button &setRightPadding(int);
  Button &setLeftPadding(int);
  int getRightPadding() const { return rightPadding; }
  int getLeftPadding() const { return leftPadding; }
  int getTopPadding() const { return topPadding; }
  int getBottomPadding() const { return bottomPadding; }
  int getX() const { return frame.x; }
  int getY() const { return frame.y; }
  int getWidth() const { return frame.w; }
  int getHeight() const { return frame.h; }
private:
  bool hover = false;
  bool backgroundIsSet = false;
  bool borderIsSet = false;
  bool iconIsSet = false;
  bool textIsSet = false;
  bool hoverIsSet = false;
  int buttonId = 0;
  int textOffsetY = 0;
  int textOffsetX = 0;
  int topPadding;
  int bottomPadding;
  int rightPadding;
  int leftPadding;
  int textAreaWidth = -1;
  std::function<void(const Button&)> leftClickCallback;
  std::function<void(const Button&)> rightClickCallback;
  Texture text;
  Texture icon;
  SDL_Color borderColor;
  SDL_Color backgroundColor;
  SDL_Color hoverColor;
  SDL_Color activeBackground;
  SDL_Rect frame;
};

#endif
