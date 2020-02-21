#include"app.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<stdexcept>
#include"windowWrapper.h"
#include"timer.h"
#include"button.h"
#include"font.h"
#include"spriteLoadCallback.h"
#include"collisionDetector.h"
#include<vector>
#include<map>
#include<iostream>
#include<functional>
#include"listMenu.h"

constexpr double TicksPerFrame = 1000.0/60.0;

struct Callbacks {
  SpriteLoadCallback spriteLoad;
  std::function<void()> blockButtonsWhileListOpen;
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

void App::defineViews(WindowWrapper &w, SDL_Rect &menu, SDL_Rect &editor) const
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

void App::generateButtons(ListMenu &buttons, WindowWrapper &w, Callbacks callbacks) const
{
  Font buttonFont("NotoSans-Regular.ttf", 14, {0xFF,0xFF,0xFF});
  buttons.setTitle(w,buttonFont,"Application");
  buttons.setListButtonClickCallback(callbacks.blockButtonsWhileListOpen);
  buttons.setBackgroundColor({0x1D,0x24,0x30});
  buttons.setHoverColor({0x4F,0x75,0x8A});
  buttons.addEntry(w,buttonFont,"Sprite Folder",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Save level",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Load level",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Editor background",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Save settings",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Load Settings",callbacks.spriteLoad);
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

void App::generateMenu(std::map<int,Texture> &textures, std::map<int, std::string> &names, WindowWrapper &w,
                       std::vector<Button> &buttons, const SDL_Rect &parent) const
{
  int offsetX = 20;
  int currentY = 40;
  int padding = 5;
  int margin = 5;
  int iconWidth = 32;
  int iconHeight = 32;
  int iconSep = 5;
  int buttonWidth = parent.w - offsetX - offsetX;
  int maxTextWidth = buttonWidth-padding-padding-iconWidth-iconSep;
  Font buttonFont("NotoSans-Regular.ttf", 14, {0xFF,0xFF,0xFF});
  for(const auto &t : textures){
    buttons.emplace_back(offsetX,currentY,padding,padding);
    Button &current = buttons.back();
    current.setTextAreaWidth(maxTextWidth);
    current.setText(w,buttonFont,names[t.first]);
    current.setIcon(t.second,iconWidth,iconHeight,iconSep);
    currentY += current.getHeight() + margin;
    current.setBackgroundColor({0x38,0x48,0x61});
    current.setHoverColor({0x4F,0x75,0x8A});
    current.setBorderColor({0x00,0x00,0x00});
    current.setButtonId(t.first);
  }
  for(auto &b : buttons)
    b.setRightPadding(buttonWidth - b.getWidth());
}

void App::run()
{
  WindowWrapper mainWindow(1366,768);
  mainWindow.show();
  SDL_Event e;
  Timer capTimer;
  SDL_Rect menuView;
  SDL_Rect editorView;
  std::map<int,Texture> idToTexture;
  std::map<int,std::string> idToName;
  defineViews(mainWindow,menuView,editorView);
  ListMenu buttonList(5,5,3,5);
  std::vector<Button> menuButtons;
  bool listOpen = false;
  bool regenerateMenu = false;
  generateButtons(buttonList,mainWindow,{
    SpriteLoadCallback(idToTexture,idToName,mainWindow,regenerateMenu),
    [&listOpen] ()->void {if(listOpen) 
                            listOpen = false;
                          else
                            listOpen = true; }
  });
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
              if(!buttonList.leftClick(e)){
                buttonList.hide();
                listOpen = false;
              }
              if(regenerateMenu){
                generateMenu(idToTexture,idToName,mainWindow,menuButtons,menuView);
                regenerateMenu = false;
              }
              if(!listOpen){

              }
              break;
          case SDL_MOUSEMOTION:
            if(isCollide({e.button.x, e.button.y}, menuView)){
              buttonList.mouseMove(e);
              if(!listOpen){
                for(auto &b : menuButtons)
                  b.mouseMove(e);
              }
            }
            break;
        }
      }
    }
    mainWindow.clear();
    drawMenuBackground(mainWindow,menuView);
    for(const auto &b : menuButtons)
      b.render(mainWindow);
    buttonList.render(mainWindow);
    mainWindow.redraw();
    if(capTimer.getTicks() < TicksPerFrame)
      SDL_Delay(TicksPerFrame - capTimer.getTicks());
  }
}
