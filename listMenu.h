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
  ListMenu(std::shared_ptr<WindowWrapper>,int,int,int,int);
  void setTitle(const Font&,const std::string&);
  void addEntry(const Font&, const std::string&,std::function<void(const GuiElement&)>);
  void setHoverColor(SDL_Color);
  void setBackgroundColor(const SDL_Color &color) override;
  void setBorderColor(const SDL_Color &color) override;
  void setListButtonClickCallback(std::function<void()>);
  void doOpen() { open = true; }
  void doClose() { open = false; }
  void toggle();
  bool isOpen() const { return open; }
  void render() const override;
  void mouseMove(const SDL_Event&);
  void click(const SDL_Event&);
private:
  Button mainButton;
  std::vector<Button> menuButtons;
  int currentY;
  bool hoverIsSet = false;
  bool open = false;
  SDL_Color hover;
};

#endif
