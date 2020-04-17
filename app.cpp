#include"app.h"
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<stdexcept>
#include"timer.h"
#include"font.h"
#include"texture.h"
#include"tile.h"
#include"selectionBox.h"
#include"textField.h"
#include"spriteLoadCallback.h"
#include"changeTileIdCallback.h"
#include"collisionDetector.h"
#include"popupInputBox.h"
#include"fileSystemExplorer.h"
#include"camera.h"

struct Callbacks {
  SpriteLoadCallback spriteLoad;
  std::function<void(const GuiElement&)> quitCallback;
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

void App::defineViews(std::shared_ptr<WindowWrapper> w, SDL_Rect &menu, SDL_Rect &editor) const
{
  menu.x = 0;
  menu.y = 0;
  menu.w = w->getWidth()*0.27;
  menu.h = w->getHeight();
  editor.x = menu.w;
  editor.y = 0;
  editor.w = w->getWidth()-menu.w;
  editor.h = w->getHeight();
}

void App::generateButtons(ListMenu &buttons, const Callbacks &callbacks, const AppColors &colors) const
{
  Font buttonFont("NotoSans-Regular.ttf", 14, colors.buttonText);
  buttons.setTitle(buttonFont,"Editor");
  buttons.setBackgroundColor(colors.buttonBackground);
  buttons.setHoverColor(colors.buttonHover);
  buttons.addEntry(buttonFont,"Sprite Folder",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Save level",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Load level",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Editor background",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Save settings",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Load Settings",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"View log",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Quit",callbacks.quitCallback);
}

void App::drawMenuBackground(std::shared_ptr<WindowWrapper> w, const SDL_Rect &menu, const AppColors &colors) const
{
  SDL_Color prev = w->getColor();
  w->setColor(colors.menuBackground);
  w->fillRect(menu);
  w->setColor(colors.menuBorder);
  w->drawRect(menu);
  w->setColor(prev);
}

int App::generateMenu(std::map<int,TextureName> &textureNames, std::shared_ptr<WindowWrapper> w,
                       std::vector<Button> &buttons, const SDL_Rect &parent, std::function<void(const GuiElement&)> leftCallback,
                       std::function<void(const GuiElement&)> rightCallback, const AppColors &colors) const
{
  int offsetX = TilemenuXOffset;
  int currentY = TileMenuYOffset;
  int padding = 5;
  int iconMaxSide = 32;
  int iconSep = 5;
  int buttonWidth = parent.w - offsetX - offsetX;
  int maxTextWidth;
  Font buttonFont("NotoSans-Regular.ttf", 14, colors.buttonText);
  buttons.clear();
  for(const auto &t : textureNames){
    buttons.emplace_back(w,offsetX,currentY,padding,padding);
    Button &current = buttons.back();
    auto iconDimensions = current.setIcon(t.second.texture,iconMaxSide,iconSep);
    maxTextWidth = buttonWidth-padding-padding-iconDimensions.first-iconSep;
    current.setTextAreaWidth(maxTextWidth);
    current.setText(buttonFont,t.second.name);
    currentY += current.getHeight() + TileMenuItemsMargin;
    current.setBackgroundColor(colors.buttonBackground);
    current.setHoverColor(colors.buttonHover);
    current.setBorderColor(colors.buttonBorder);
    current.setElementId(t.first);
    current.setLeftClickCallback(leftCallback);
    current.setRightClickCallback(rightCallback);
    current.setRightPadding(buttonWidth - current.getWidth());
  }
  return currentY;
}

void App::run()
{
  AppColors colors;
  std::shared_ptr<WindowWrapper> mainWindow = std::make_shared<WindowWrapper>(1366,768);
  std::shared_ptr<PopupInputBox> popup;
  bool popupClose = false;
  int tileSize = 32;
  mainWindow->show();
  SDL_Event e;
  int currentTile = 1;
  Timer capTimer;
  SDL_Rect menuView;
  SDL_Rect editorView;
  defineViews(mainWindow,menuView,editorView);
  SDL_Rect tileMenu = {TilemenuXOffset,TileMenuYOffset,menuView.w-TilemenuXOffset*2,menuView.h-TileMenuYOffset*2};
  Camera tileMenuCamera(0,0,menuView.w-TilemenuXOffset,menuView.h-TileMenuYOffset);
  tileMenuCamera.setYScrollSpeed(TileMenuScrollSpeed);
  std::map<int,TextureName> idToTextureName;
  std::vector<Tile> tiles;
  ListMenu buttonList(mainWindow,5,5,3,5);
  std::vector<Button> menuButtons;
  SelectionBox leftMouseBox(mainWindow,colors.leftSelection,colors.leftSelectionBorder);
  SelectionBox rightMouseBox(mainWindow,colors.rightSelection,colors.rightSelectionBorder);
  bool regenerateMenu = false;
  bool tilesLoad = false;
  int menuButtonsHeight = 0;
  bool quit = false;
  generateButtons(buttonList,{
    SpriteLoadCallback(idToTextureName,mainWindow,regenerateMenu),
    [&quit](const GuiElement&){ quit = true; }
  }, colors);
  while(!quit){
    capTimer.start();
    while(SDL_PollEvent(&e) != 0){
      switch(e.type){
        case SDL_QUIT:
          quit = true;
        break;
        case SDL_MOUSEBUTTONDOWN:
            buttonList.click(e);
            if(!buttonList.isOpen() && !popup){
              for(auto &b : menuButtons)
                b.click(e,tileMenuCamera);
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
        break;
        case SDL_MOUSEBUTTONUP:
          if(popup){
            popup->click(e);
          }
          else if(!buttonList.isOpen()){
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
            }
          }
          leftMouseBox.unhold();
          rightMouseBox.unhold();
        break;
        case SDL_MOUSEMOTION:
          buttonList.mouseMove(e);
          if(popup){
            popup->mouseMove(e);
          }
          else if(!buttonList.isOpen()){
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
          if(!buttonList.isOpen() && !popup){
            int currentMouseX, currentMouseY;
            SDL_GetMouseState(&currentMouseX,&currentMouseY);
            if(e.wheel.y > 0){
              if(isCollide({currentMouseX, currentMouseY}, menuView)){
                tileMenuCamera.scrollUp();
              }
            }
            else if(e.wheel.y < 0){
              if(isCollide({currentMouseX, currentMouseY}, menuView)){
                tileMenuCamera.scrollDown();
              }
            }
          }
        break;
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym){
            case SDLK_BACKSPACE:
              if(popup)
                popup->backspace();
            break;
            case SDLK_ESCAPE:
              if(popup)
                popupClose = true;
            break;
            case SDLK_RETURN:
              if(popup)
                popup->confirm();
            break;
          }
        break;
        case SDL_TEXTINPUT:
          if(popup)
            popup->textInput(e);
        break;
      }
    }
    if(regenerateMenu){
      menuButtonsHeight = generateMenu(idToTextureName,mainWindow,menuButtons,menuView,
        [&currentTile](const GuiElement &b)->void{ currentTile = b.getElementId();},
        [&popup,&popupClose,&colors,&mainWindow,&idToTextureName,&tiles,&regenerateMenu](const GuiElement &e)->void{
          popup = std::make_shared<PopupInputBox>(mainWindow,300,125,"NotoSans-Regular.ttf",colors);
          popup->setBackgroundColor(colors.popupBackground);
          popup->setBorderColor(colors.popupBorder);
          popup->setActionButtonLabel("Change");
          popup->setTitle("Change tile id");
          popup->setElementId(e.getElementId());
          popup->setActionCallback([&regenerateMenu,&popup,&popupClose,&mainWindow,&idToTextureName,&tiles] (const GuiElement&)->void
          {
            ChangeTileIdCallback callback(mainWindow,idToTextureName,tiles);
            callback(*popup.get());
            popupClose = true;
            regenerateMenu = true;
          });
          popup->setCloseCallback([&popupClose](const GuiElement&){popupClose = true;});
        }, colors
      );
      tileMenuCamera.setYScrollCap(menuButtonsHeight);
      regenerateMenu = false;
      tilesLoad = true;
    }
    if(popupClose){
      popup.reset();
      popupClose = false;
    }
    mainWindow->clear();
    drawMenuBackground(mainWindow,menuView,colors);
    mainWindow->setClip(&tileMenu);
    for(const auto &b : menuButtons)
      b.render(tileMenuCamera);
    mainWindow->setClip();
    mainWindow->setViewport(&editorView);
    for(auto &tile : tiles)
      tile.render(mainWindow->getRenderer());
    if(leftMouseBox.isHold())
      leftMouseBox.render();
    if(rightMouseBox.isHold())
      rightMouseBox.render();
    mainWindow->setViewport();
    buttonList.render();
    if(popup)
      popup->render();
    mainWindow->redraw();
    if(capTimer.getTicks() < TicksPerFrame)
      SDL_Delay(TicksPerFrame - capTimer.getTicks());
  }
}
