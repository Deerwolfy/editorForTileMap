#include"app.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<stdexcept>
#include"windowWrapper.h"
#include"timer.h"
#include"button.h"
#include"font.h"
#include<iostream>
#include<vector>

constexpr double TicksPerFrame = 1000.0/60.0;

enum {
  NO_ACTION,
  LOAD_LEVEL,
  SAVE_LEVEL,
  SPRITE_FOLDER,
  LOAD_BACKGROUND,
  SAVE_SETTINGS,
  LOAD_SETTINGS
};

void App::init() const
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    throwError("Unable to init sdl subsystem SDL_Error: ", SDL_GetError());
  }
  int flags = IMG_INIT_PNG | IMG_INIT_JPG;
  if(!(IMG_Init(flags) & flags)){
    throwError("Unable to init SDL_image SDL_Error: ", IMG_GetError());
  }
  if(TTF_Init() == -1){
    throwError("Unable to init SDL_ttf SDL_Error: ", TTF_GetError());
  }
}

void App::quit() const
{
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void App::throwError(std::string msg, const char *sdlMsg) const
{
  throw std::runtime_error(msg.append(sdlMsg)); 
}

void App::defineViews(WindowWrapper &w, SDL_Rect &menu, SDL_Rect &editor)
{
  menu.x = 0;
  menu.y = 0;
  menu.w = w.getWidth()*0.27;
  menu.h = w.getHeight();
  editor.x = menu.w;
  editor.y = 0;
  editor.w = w.getWidth()-menu.w;
  editor.h = w.getHeight();

}

void App::generateButtons(std::vector<Button> &buttons, WindowWrapper &w)
{
  Font buttonFont("NotoSans-Regular.ttf", 14, {0xFF,0xFF,0xFF,0xFF});
  int hOffset = 5;
  int vOffset = 5;
  int padding = 5;
  buttons.emplace_back(hOffset,vOffset,padding,padding);
  buttons.emplace_back(0,vOffset,padding,padding);
  buttons.emplace_back(0,vOffset,padding,padding);
  buttons.emplace_back(0,vOffset,padding,padding);
  buttons.emplace_back(hOffset,0,padding,padding);
  buttons.emplace_back(0,0,padding,padding);
  buttons[0].setText(w,buttonFont,"save level");
  buttons[1].setText(w,buttonFont,"load level");
  buttons[2].setText(w,buttonFont,"sprite folder");
  buttons[3].setText(w,buttonFont,"background");
  buttons[4].setText(w,buttonFont,"load settings");
  buttons[5].setText(w,buttonFont,"save settings");
  buttons[0].setActionCode(SAVE_LEVEL);
  buttons[1].setActionCode(LOAD_LEVEL);
  buttons[2].setActionCode(SPRITE_FOLDER);
  buttons[3].setActionCode(LOAD_BACKGROUND);
  buttons[4].setActionCode(LOAD_SETTINGS);
  buttons[5].setActionCode(SAVE_SETTINGS);
  buttons[1].setX(buttons[0].getX()+buttons[0].getWidth()+hOffset);
  buttons[2].setX(buttons[1].getX()+buttons[1].getWidth()+hOffset);
  buttons[3].setX(buttons[2].getX()+buttons[2].getWidth()+hOffset);
  buttons[5].setX(buttons[4].getX()+buttons[4].getWidth()+hOffset);
  buttons[4].setY(w.getHeight()-buttons[4].getHeight()-vOffset);
  buttons[5].setY(w.getHeight()-buttons[5].getHeight()-vOffset);
  for(auto &b : buttons){
    b.setBackgroundColor({0x38,0x48,0x61,0xFF});
    b.setHoverColor({0x4F,0x75,0x8A,0xFF});
  }
}

void App::drawMenuBackground(const WindowWrapper &w, const SDL_Rect &menu) const
{
  SDL_Color prev = w.getColor();
  w.setColor(0x2B,0x37,0x4A);
  SDL_RenderFillRect(w.getRenderer(),&menu);
  w.setColor(0x00,0x00,0x00);
  SDL_RenderDrawRect(w.getRenderer(),&menu);
  w.setColor(prev);
}

void App::run()
{
  WindowWrapper mainWindow(1366,768);
  mainWindow.show();
  SDL_Event e;
  Timer capTimer;
  SDL_Rect menuView;
  SDL_Rect editorView;
  defineViews(mainWindow,menuView,editorView);
  std::vector<Button> buttons;
  generateButtons(buttons,mainWindow);
  bool quit = false;
  while(!quit){
    capTimer.start();
    while(SDL_PollEvent(&e) != 0){
      if(e.type == SDL_QUIT){
        quit = true;
      }
      else {
        switch(e.type){
          case SDL_MOUSEBUTTONDOWN:
            for(auto &b : buttons)
              b.click(e);
              break;
          case SDL_MOUSEMOTION:
            for(auto &b : buttons)
              b.mouseMove(e);
            break;
        }
      }
    }
    mainWindow.clear();
    drawMenuBackground(mainWindow,menuView);
    for(auto &b : buttons)
      b.render(mainWindow);
    mainWindow.redraw();
    if(capTimer.getTicks() < TicksPerFrame)
      SDL_Delay(TicksPerFrame - capTimer.getTicks());
  }
}
