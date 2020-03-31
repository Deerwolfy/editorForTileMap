#ifndef APP_H
#define APP_H

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<stdexcept>
#include<string>
#include"windowWrapper.h"
#include"timer.h"
#include"button.h"
#include"font.h"
#include"spriteLoadCallback.h"
#include"changeTileIdCallback.h"
#include"collisionDetector.h"
#include<vector>
#include<map>
#include"textureName.h"
#include<iostream>
#include<functional>
#include"listMenu.h"
#include"texture.h"
#include"tile.h"
#include"selectionBox.h"
#include"textField.h"
#include<memory>

struct Callbacks;

class App {
public:
  App() { init(); }
  void run();
  ~App() { quit(); }
private:
  void init() const;
  void quit() const;
  void defineViews(std::shared_ptr<WindowWrapper>,SDL_Rect&, SDL_Rect&) const;
  void generateButtons(ListMenu&,Callbacks) const;
  void drawMenuBackground(std::shared_ptr<WindowWrapper>, const SDL_Rect&) const;
  int generateMenu(std::map<int,TextureName>&,std::shared_ptr<WindowWrapper>,
                    std::vector<Button>&,const SDL_Rect&,std::function<void(const GuiElement&)>,std::function<void(const GuiElement&)>) const;
  void throwError(std::string,const char*) const;
  const double TicksPerFrame = 1000.0/60.0;
  const int TileMenuScrollSpeed = 30;
  const int TileMenuItemsMargin = 5;
  const int TileMenuYOffset = 40;
  const int TilemenuXOffset = 20;
};

#endif
