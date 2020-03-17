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
#include"guiElement.h"

class Button : public GuiElement {
public:
  Button(int x, int y, int tbPadding, int rlPadding):
    topPadding(tbPadding), bottomPadding(tbPadding), rightPadding(rlPadding), leftPadding(rlPadding), 
    GuiElement(x,y,rlPadding+rlPadding,tbPadding+tbPadding) { }
  void setText(WindowWrapper&, const Font&, const std::string&);
  void setHoverColor(const SDL_Color&);
  void setBackgroundColor(const SDL_Color&) override;
  std::pair<int,int> setIcon(WindowWrapper&, Texture, int maxDimension = 32, int iconRightPadding = 0);
  void setTextAreaWidth(int width) { textAreaWidth = width; };
  void render(WindowWrapper&) const override;
  void render(WindowWrapper&,SDL_Rect) const;
  void mouseMove(const SDL_Event&);
  void mouseMove(const SDL_Event&, SDL_Rect);
  int click(const SDL_Event&) const;
  int click(const SDL_Event&, const SDL_Rect&) const;
  void setTopPadding(int);
  void setBottomPadding(int);
  void setRightPadding(int);
  void setLeftPadding(int);
  int getRightPadding() const { return rightPadding; }
  int getLeftPadding() const { return leftPadding; }
  int getTopPadding() const { return topPadding; }
  int getBottomPadding() const { return bottomPadding; }
private:
  void updateHover(const SDL_Point&);
  bool hover = false;
  bool iconIsSet = false;
  bool textIsSet = false;
  bool hoverIsSet = false;
  int textOffsetY = 0;
  int textOffsetX = 0;
  int topPadding;
  int bottomPadding;
  int rightPadding;
  int leftPadding;
  int textAreaWidth = -1;
  Texture text;
  Texture icon;
  SDL_Color hoverColor;
  SDL_Color activeBackground;
};

#endif
