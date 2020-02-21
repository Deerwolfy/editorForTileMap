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
  ListMenu &setTitle(WindowWrapper&,const Font&,const std::string&);
  ListMenu &addEntry(WindowWrapper&,const Font&, const std::string&,std::function<void(const Button&)>);
  ListMenu &setBackgroundColor(SDL_Color);
  ListMenu &setHoverColor(SDL_Color);
  ListMenu &setBorderColor(SDL_Color);
  ListMenu &setListButtonClickCallback(std::function<void()>);
  void hide() { shown = false; }
  void show() { shown = true; }
  void render(WindowWrapper&) const;
  void mouseMove(const SDL_Event&);
  int leftClick(const SDL_Event&);
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