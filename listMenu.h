#ifndef LISTMENU_H
#define LISTMENU_H

#include"button.h"
#include<SDL.h>
#include"windowWrapper.h"
#include"font.h"
#include<functional>
#include<vector>

class ListMenu {
public:
  ListMenu(int,int,int,int);
  void setTitle(WindowWrapper&,const Font&,const std::string&);
  void addEntry(WindowWrapper&,const Font&, const std::string&,std::function<void(const Button&)>);
  void setBackgroundColor(SDL_Color);
  void setHoverColor(SDL_Color);
  void setBorderColor(SDL_Color);
  void setListButtonClickCallback(std::function<void()>);
  void hide() { shown = false; }
  void show() { shown = true; }
  void toggle();
  bool isShown() const { return shown; }
  void render(WindowWrapper&) const;
  void mouseMove(const SDL_Event&);
  void click(const SDL_Event&);
private:
  Button mainButton;
  std::vector<Button> menuButtons;
  int currentY;
  SDL_Rect frame;
  bool shown = false;
  bool backgroundIsSet = false;
  bool hoverIsSet = false;
  bool borderIsSet = false;
  SDL_Color border;
  SDL_Color background;
  SDL_Color hover;
};

#endif
