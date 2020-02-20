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
  void generateButtons(std::vector<Button>&,WindowWrapper&,Callbacks) const;
  void drawMenuBackground(const WindowWrapper&, const SDL_Rect&) const;
  void generateMenu(std::map<int,Texture>&,std::map<int,std::string>&,WindowWrapper&,std::vector<Button>&) const;
  void throwError(std::string,const char*) const;
};

#endif
