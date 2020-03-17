#ifndef LISTMENU_H
#define LISTMENU_H

#include"button.h"
#include"guiElement.h"
#include<SDL.h>
#include"windowWrapper.h"
#include"font.h"
#include<functional>
#include<vector>

class ListMenu : public GuiElement {
public:
  ListMenu(int,int,int,int);
  void setTitle(WindowWrapper&,const Font&,const std::string&);
  void addEntry(WindowWrapper&,const Font&, const std::string&,std::function<void(const GuiElement&)>);
  void setHoverColor(SDL_Color);
  void setBackgroundColor(const SDL_Color &color) override;
  void setBorderColor(const SDL_Color &color) override;
  void setListButtonClickCallback(std::function<void()>);
  void hide() { shown = false; }
  void show() { shown = true; }
  void toggle();
  bool isShown() const { return shown; }
  void render(WindowWrapper&) const override;
  void mouseMove(const SDL_Event&);
  void click(const SDL_Event&);
private:
  Button mainButton;
  std::vector<Button> menuButtons;
  int currentY;
  bool shown = false;
  bool hoverIsSet = false;
  SDL_Color hover;
};

#endif
