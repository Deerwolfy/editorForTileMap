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
#include"changeTileIdCallback.h"
#include"collisionDetector.h"
#include<vector>
#include<map>
#include"textureName.h"
#include<iostream>
#include<functional>
#include"listMenu.h"
#include"tile.h"
#include"selectionBox.h"

constexpr double TicksPerFrame = 1000.0/60.0;
constexpr int TileMenuScrollSpeed = 20;
constexpr int TileMenuItemsMargin = 5;
constexpr int TileMenuYOffset = 40;
constexpr int TilemenuXOffset = 20;

struct Callbacks {
  SpriteLoadCallback spriteLoad;
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
  Font buttonFont("NotoSans-Regular.ttf", 14, {0xFF,0xFF,0xFF,0xFF});
  buttons.setTitle(w,buttonFont,"Editor");
  buttons.setBackgroundColor({0x1D,0x24,0x30,0xFF});
  buttons.setHoverColor({0x4F,0x75,0x8A,0xFF});
  buttons.addEntry(w,buttonFont,"Sprite Folder",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Save level",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Load level",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Editor background",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Save settings",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"Load Settings",callbacks.spriteLoad);
  buttons.addEntry(w,buttonFont,"View log",callbacks.spriteLoad);
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

int App::generateMenu(std::map<int,TextureName> &textureNames, WindowWrapper &w,
                       std::vector<Button> &buttons, const SDL_Rect &parent, std::function<void(const Button&)> leftCallback,
                       std::function<void(const Button&)> rightCallback) const
{
  int offsetX = TilemenuXOffset;
  int currentY = TileMenuYOffset;
  int padding = 5;
  int iconMaxSide = 32;
  int iconSep = 5;
  int buttonWidth = parent.w - offsetX - offsetX;
  int maxTextWidth;
  Font buttonFont("NotoSans-Regular.ttf", 14, {0xFF,0xFF,0xFF,0xFF});
  buttons.clear();
  for(const auto &t : textureNames){
    buttons.emplace_back(offsetX,currentY,padding,padding);
    Button &current = buttons.back();
    auto iconDimensions = current.setIcon(w,t.second.texture,iconMaxSide,iconSep);
    maxTextWidth = buttonWidth-padding-padding-iconDimensions.first-iconSep;
    current.setTextAreaWidth(maxTextWidth);
    current.setText(w,buttonFont,t.second.name);
    currentY += current.getHeight() + TileMenuItemsMargin;
    current.setBackgroundColor({0x38,0x48,0x61,0xFF});
    current.setHoverColor({0x4F,0x75,0x8A,0xFF});
    current.setBorderColor({0x00,0x00,0x00,0xFF});
    current.setElementId(t.first);
    current.setRightPadding(buttonWidth - current.getWidth());
  }
  return currentY;
}

void App::run()
{
  WindowWrapper mainWindow(1366,768);
  int tileSize = 32;
  mainWindow.show();
  SDL_Event e;
  
  int currentTile = 1;
  Timer capTimer;
  SDL_Rect menuView;
  SDL_Rect editorView;
  defineViews(mainWindow,menuView,editorView);
  SDL_Rect tileMenu = {TilemenuXOffset,TileMenuYOffset,menuView.w-TilemenuXOffset*2,menuView.h-TileMenuYOffset*2};
  SDL_Rect tileMenuCamera = {0,0,menuView.w-TilemenuXOffset,menuView.h-TileMenuYOffset};
  std::map<int,TextureName> idToTextureName;
  std::vector<Tile> tiles;
  ListMenu buttonList(5,5,3,5);
  std::vector<Button> menuButtons;
  SelectionBox leftMouseBox({0x00,0xFF,0x00,0x44},{0x00,0xFF,0x00,0x88});
  SelectionBox rightMouseBox({0xFF,0x00,0x00,0x44},{0xFF,0x00,0x00,0x88});
  bool regenerateMenu = false;
  bool tilesLoad = false;
  int menuButtonsHeight = 0;
  generateButtons(buttonList,mainWindow,{
    SpriteLoadCallback(idToTextureName,mainWindow,regenerateMenu),
  });
  bool quit = false;
  while(!quit){
    capTimer.start();
    while(SDL_PollEvent(&e) != 0){
      switch(e.type){
        case SDL_QUIT:
          quit = true;
        break;
        case SDL_MOUSEBUTTONDOWN:
            buttonList.click(e);
            if(!buttonList.isShown()){
              if(e.button.button == SDL_BUTTON_LEFT){
                if(isCollide({e.button.x,e.button.y},editorView)){
                  leftMouseBox.setStart(e.button.x - editorView.x,e.button.y - editorView.y);
                }
              }
              else if(e.button.button == SDL_BUTTON_RIGHT){
                if(isCollide({e.button.x,e.button.y},editorView)){
                  rightMouseBox.setStart(e.button.x - editorView.x,e.button.y - editorView.y);
                }
              }
            }
            if(regenerateMenu){
              menuButtonsHeight = generateMenu(idToTextureName,mainWindow,menuButtons,menuView,
                [&currentTile](const Button &b)->void{ currentTile = b.getElementId();},
                ChangeTileIdCallback(mainWindow,idToTextureName)
              );
              regenerateMenu = false;
              tilesLoad = true;
            }
        break;
        case SDL_MOUSEBUTTONUP:
          if(!buttonList.isShown()){
              if(e.button.button == SDL_BUTTON_LEFT){
                if(isCollide({e.button.x,e.button.y},editorView) && tilesLoad){
                  SDL_Point origin = leftMouseBox.getOrigin();
                  int xOffset = origin.x%tileSize;
                  int yOffset = origin.y%tileSize;
                  int startX = origin.x - xOffset;
                  int countX = (leftMouseBox.getWidth()+xOffset)/tileSize + 1;
                  int w = leftMouseBox.getWidth();
                  int currentY = origin.y - yOffset;
                  int countY = (leftMouseBox.getHeight()+yOffset)/tileSize + 1;
                  while(countY--){
                    int currentX = startX;
                    int count = countX;
                    while(count--){
                      if(isCollide({currentX,currentY},tiles) == tiles.size())
                        tiles.emplace_back(idToTextureName[currentTile].texture,currentX,currentY);
                      currentX += tileSize;
                    }
                    currentY += tileSize;
                  }
                }
                leftMouseBox.unhold();
              }
              else if(e.button.button == SDL_BUTTON_RIGHT){
                if(isCollide({e.button.x,e.button.y},editorView)){
                  SDL_Point origin = rightMouseBox.getOrigin();
                  int xOffset = origin.x%tileSize;
                  int yOffset = origin.y%tileSize;
                  int startX = origin.x - xOffset;
                  int countX = (rightMouseBox.getWidth()+xOffset)/tileSize + 1;
                  int w = rightMouseBox.getWidth();
                  int currentY = origin.y - yOffset;
                  int countY = (rightMouseBox.getHeight()+yOffset)/tileSize + 1;
                  while(countY--){
                    int currentX = startX;
                    int count = countX;
                    while(count--){
                      std::size_t index = isCollide({currentX,currentY},tiles);
                      if(index != tiles.size())
                        tiles.erase(std::begin(tiles) + index);
                      currentX += tileSize;
                    }
                    currentY += tileSize;
                  }
                }
                rightMouseBox.unhold();
              }
            }
        break;
        case SDL_MOUSEMOTION:
          buttonList.mouseMove(e);
          if(!buttonList.isShown()){
            for(auto &b : menuButtons)
              b.mouseMove(e,tileMenuCamera);
            if(leftMouseBox.isHold()){
              leftMouseBox.setEnd(e.motion.x - editorView.x,e.motion.y - editorView.y);
            }
            else if(rightMouseBox.isHold()){
              rightMouseBox.setEnd(e.motion.x - editorView.x,e.motion.y - editorView.y);
            }
          }
        break;
        case SDL_MOUSEWHEEL:
          int currentMouseX, currentMouseY;
          SDL_GetMouseState(&currentMouseX,&currentMouseY);
          if(e.wheel.y > 0){
            if(isCollide({currentMouseX, currentMouseY}, menuView)){
              if(tileMenuCamera.y >= TileMenuScrollSpeed)
                tileMenuCamera.y -= TileMenuScrollSpeed;
            }
          }
          else if(e.wheel.y < 0){
            if(isCollide({currentMouseX, currentMouseY}, menuView)){
              if(tileMenuCamera.y + tileMenuCamera.h < menuButtonsHeight)
                tileMenuCamera.y += TileMenuScrollSpeed;
            }
          }
        break;
      }
    }
    mainWindow.clear();
    drawMenuBackground(mainWindow,menuView);
    mainWindow.setClip(&tileMenu);
    for(const auto &b : menuButtons)
      b.render(mainWindow,tileMenuCamera);
    mainWindow.setClip();
    mainWindow.setViewport(&editorView);
    for(auto &tile : tiles)
      tile.render(mainWindow);
    if(leftMouseBox.isHold())
      leftMouseBox.render(mainWindow);
    if(rightMouseBox.isHold())
      rightMouseBox.render(mainWindow);
    mainWindow.setViewport();
    buttonList.render(mainWindow);
    mainWindow.redraw();
    if(capTimer.getTicks() < TicksPerFrame)
      SDL_Delay(TicksPerFrame - capTimer.getTicks());
  }
}
