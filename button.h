#ifndef BUTTON_H
#define BUTTON_H

#include"texture.h"
#include"windowWrapper.h"
#include"font.h"
#include<SDL.h>
#include<functional>
#include<string>
#include<memory>
#include<utility>

class Button {
public:
  Button(int x, int y, int tbPadding, int rlPadding):
    topPadding(tbPadding), bottomPadding(tbPadding), rightPadding(rlPadding), leftPadding(rlPadding), 
    frame({x,y,rlPadding+rlPadding,tbPadding+tbPadding}) { }
  void setText(WindowWrapper&, const Font&, const std::string&);
  void setButtonId(int id) { buttonId = id; }
  void setBorderColor(SDL_Color);
  void setBackgroundColor(SDL_Color);
  void setHoverColor(SDL_Color);
  void setLeftClickCallback(std::function<void(const Button&)> callback)
    { leftClickCallback = callback; }
  void setRightClickCallback(std::function<void(const Button&)> callback)
    { rightClickCallback = callback; }
  std::pair<int,int> setIcon(WindowWrapper&, Texture, int maxDimension = 32, int iconRightPadding = 0);
  void setTextAreaWidth(int width) { textAreaWidth = width; };
  void render(WindowWrapper&) const;
  void render(WindowWrapper&,SDL_Rect) const;
  void mouseMove(const SDL_Event&);
  void mouseMove(const SDL_Event&, SDL_Rect);
  int leftClick(const SDL_Event&) const;
  int leftClick(const SDL_Event&, SDL_Rect) const;
  int rightClick(const SDL_Event&) const;
  int rightClick(const SDL_Event&, SDL_Rect) const;
  void setX(int x) { frame.x = x; }
  void setY(int y) { frame.y = y; }
  void setTopPadding(int);
  void setBottomPadding(int);
  void setRightPadding(int);
  void setLeftPadding(int);
  int getRightPadding() const { return rightPadding; }
  int getLeftPadding() const { return leftPadding; }
  int getTopPadding() const { return topPadding; }
  int getBottomPadding() const { return bottomPadding; }
  int getX() const { return frame.x; }
  int getY() const { return frame.y; }
  int getWidth() const { return frame.w; }
  int getHeight() const { return frame.h; }
  int getId() const { return buttonId; }
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
