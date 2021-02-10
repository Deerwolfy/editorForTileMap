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
#include"selectionBox.h"
#include"tile.h"

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
  const double TicksPerFrame = 1000.0/60.0;
  const int TileMenuScrollSpeed = 30;
  const int TileMenuYOffset = 40;
  const int TilemenuXOffset = 20;
  const double canvasScaleCoef = 0.5;
};

#endif
