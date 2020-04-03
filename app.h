#ifndef APP_H
#define APP_H

#include<SDL.h>
#include"windowWrapper.h"
#include"guiElement.h"
#include<string>
#include<vector>
#include<map>
#include"textureName.h"
#include"listMenu.h"
#include"button.h"
#include"appColors.h"
#include<functional>
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
  void generateButtons(ListMenu&,const Callbacks&, const AppColors&) const;
  void drawMenuBackground(std::shared_ptr<WindowWrapper>, const SDL_Rect&, const AppColors&) const;
  int generateMenu(std::map<int,TextureName>&,std::shared_ptr<WindowWrapper>,
                    std::vector<Button>&,const SDL_Rect&,std::function<void(const GuiElement&)>,std::function<void(const GuiElement&)>, const AppColors&) const;
  void throwError(std::string,const char*) const;
  const double TicksPerFrame = 1000.0/60.0;
  const int TileMenuScrollSpeed = 30;
  const int TileMenuItemsMargin = 5;
  const int TileMenuYOffset = 40;
  const int TilemenuXOffset = 20;
};

#endif
