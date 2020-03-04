#ifndef APP_H
#define APP_H

#include<string>
#include<SDL.h>
#include"windowWrapper.h"
#include"button.h"
#include"texture.h"
#include<vector>
#include<string>
#include<map>
#include"listMenu.h"

struct Callbacks;

class App {
public:
  App() { init(); }
  void run();
  ~App() { quit(); }
private:
  void init() const;
  void quit() const;
  void defineViews(WindowWrapper&,SDL_Rect&, SDL_Rect&) const;
  void generateButtons(ListMenu&,WindowWrapper&,Callbacks) const;
  void drawMenuBackground(const WindowWrapper&, const SDL_Rect&) const;
  void generateMenu(std::map<int,std::pair<Texture,std::string>>&,WindowWrapper&,
                    std::vector<Button>&,const SDL_Rect&,std::function<void(const Button&)>,std::function<void(const Button&)>) const;
  void throwError(std::string,const char*) const;
};

#endif
