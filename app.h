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

class App {
public:
  App() { init(); }
  void run();
  ~App() { quit(); }
private:
  void init() const;
  void quit() const;
  void defineViews(WindowWrapper&,SDL_Rect&, SDL_Rect&);
  void generateButtons(std::vector<Button>&,WindowWrapper&);
  void drawMenuBackground(const WindowWrapper&, const SDL_Rect&) const;
  void loadSprites(std::map<int,Texture>&, std::map<int,std::string>&);
  void throwError(std::string,const char*) const;
};

#endif
