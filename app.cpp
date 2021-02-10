#include"app.h"
#include<SDL_image.h>
#include<SDL_ttf.h>
#include"timer.h"
#include"font.h"
#include"texture.h"
#include"textField.h"
#include"spriteLoadCallback.h"
#include"changeTileIdCallback.h"
#include"collisionDetector.h"
#include"popupInputBox.h"
#include"fileSystemExplorer.h"
#include"camera.h"
#include"errorHandler.h"
#include"buttonMenu.h"
#include"tileCanvas.h"

struct Callbacks {
  SpriteLoadCallback spriteLoad;
  std::function<void(const GuiElement&)> quitCallback;
  std::function<void(const GuiElement&)> setCanvasBackground;
};

struct KeyFlags {
  bool shift = false;
  bool leftMouse = false;
};

void App::init() const
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    ErrorHandler::raise("Unable to init sdl subsystem SDL_Error: ", SDL_GetError());
  }
  int flags = IMG_INIT_PNG | IMG_INIT_JPG;
  if(!(IMG_Init(flags) & flags)){
    ErrorHandler::raise("Unable to init SDL_image SDL_Error: ", IMG_GetError());
  }
  if(TTF_Init() == -1){
    ErrorHandler::raise("Unable to init SDL_ttf SDL_Error: ", TTF_GetError());
  }
}

void App::quit() const
{
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
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
  buttons.addEntry(buttonFont,"Load Sprites",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Save level",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Load level",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Canvas background",callbacks.setCanvasBackground);
  buttons.addEntry(buttonFont,"Save settings",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Load Settings",callbacks.spriteLoad);
  buttons.addEntry(buttonFont,"Quit",callbacks.quitCallback);
}

void App::run()
{
  const int winWidth = 1366;
  const int winHeight = 768;
  const int canvasWidth = 512;
  const int canvasHeight = 512;
  AppColors colors;
  std::shared_ptr<WindowWrapper> mainWindow = std::make_shared<WindowWrapper>(winWidth,winHeight);
  std::shared_ptr<PopupInputBox> popup;
  KeyFlags keys;
  SDL_Point cameraMoveLastPos; // Camera position for calculating view move difference
  int tileSize = 32;
  mainWindow->show();
  int currentTile = 1;
  SDL_Rect menuView;
  SDL_Rect editorView;
  defineViews(mainWindow,menuView,editorView);
  TileCanvas canvas(mainWindow,(editorView.w-canvasWidth)/2,(editorView.h-canvasHeight)/2,canvasWidth,canvasHeight,tileSize);
  canvas.setBackgroundColor(colors.canvasBackground);
  ButtonMenu tileMenu(mainWindow, TilemenuXOffset,TileMenuYOffset,menuView.w-TilemenuXOffset*2,menuView.h-TileMenuYOffset*2);
  tileMenu.setCameraScrollSpeed(TileMenuScrollSpeed);
  Camera editorCamera(0,0,editorView.w,editorView.h);
  std::map<int,TextureName> idToTextureName;
  ListMenu buttonList(mainWindow,5,5,3,5);
  SelectionBox leftMouseBox(mainWindow,colors.leftSelection,colors.leftSelectionBorder);
  SelectionBox rightMouseBox(mainWindow,colors.rightSelection,colors.rightSelectionBorder);
  bool regenerateMenu = false;
  tileMenu.setButtonsLeftCallback([&currentTile](const GuiElement &b)->void{ currentTile = b.getElementId(); }); // Callback to select tile
  tileMenu.setButtonsRightCallback([&popup,&colors,&mainWindow,&idToTextureName,&canvas,&regenerateMenu](const GuiElement &e)->void{ // Callback to create popup
    popup = std::make_shared<PopupInputBox>(mainWindow,300,125,"NotoSans-Regular.ttf",colors);
    popup->setBackgroundColor(colors.popupBackground);
    popup->setBorderColor(colors.popupBorder);
    popup->setActionButtonLabel("Set");
    popup->setTitle("Set tile id");
    popup->setElementId(e.getElementId());
    popup->setActionCallback([&regenerateMenu,&popup,&mainWindow,&idToTextureName,&canvas] (const GuiElement&)->void{ // Callback to change tile id
      ChangeTileIdCallback callback(mainWindow,idToTextureName,canvas);
      callback(*popup.get());
      popup->markDone();
      regenerateMenu = true;
    });
    popup->setCloseCallback([&popup](const GuiElement&){popup->markDone();}); // Callback to mark popup as done
  });
  bool tilesLoad = false;
  bool quit = false;
  generateButtons(buttonList,{
    SpriteLoadCallback(idToTextureName,mainWindow,regenerateMenu),
    [&quit](const GuiElement&){ quit = true; },
    [&canvas](const GuiElement&){
      std::wstring path;
      if(!(path = FileSystemExplorer::openFile(L"Select canvas background")).empty())
        canvas.setBackgroundTexture(path);
    }
  }, colors);
  Timer capTimer;
  SDL_Event e;
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
              tileMenu.click(e);
              if(e.button.button == SDL_BUTTON_LEFT){
                if(Collision::between({e.button.x,e.button.y},editorView) && !keys.shift){
                  leftMouseBox.setStart(e.button.x - editorView.x,e.button.y - editorView.y);
                }
                keys.leftMouse = true;
              }
              else if(e.button.button == SDL_BUTTON_RIGHT){
                if(Collision::between({e.button.x,e.button.y},editorView) && !keys.shift){
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
              if(Collision::between({e.button.x,e.button.y},editorView) && tilesLoad && !keys.shift){
                  canvas.placeTiles(leftMouseBox,editorCamera,idToTextureName[currentTile].texture,currentTile);
              }
              keys.leftMouse = false;
            }
            else if(e.button.button == SDL_BUTTON_RIGHT){
              if(Collision::between({e.button.x,e.button.y},editorView) && !keys.shift){
                canvas.removeTiles(rightMouseBox,editorCamera);
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
            tileMenu.mouseMove(e);
            if(keys.shift){ // Move canvas camera
              if(keys.leftMouse)
                editorCamera.moveBy(cameraMoveLastPos.x-e.motion.x,cameraMoveLastPos.y-e.motion.y);
              cameraMoveLastPos.x = e.motion.x;
              cameraMoveLastPos.y = e.motion.y;
            } else { // Resize selection area
              if(leftMouseBox.isHold()){
                leftMouseBox.setEnd(e.motion.x - editorView.x,e.motion.y - editorView.y);
              }
              else if(rightMouseBox.isHold()){
                rightMouseBox.setEnd(e.motion.x - editorView.x,e.motion.y - editorView.y);
              }
            }
          }
        break;
        case SDL_MOUSEWHEEL:
          if(!buttonList.isOpen() && !popup){
            int mousePosX = 0, mousePosY = 0;
            SDL_GetMouseState(&mousePosX,&mousePosY);
            if(Collision::between({mousePosX,mousePosY},menuView)){
              tileMenu.scroll(e);
            }
            else if(Collision::between({mousePosX,mousePosY},editorView)){
              if(e.wheel.y > 0){
                canvas.scaleUp(canvasScaleCoef);
              }
              else if(e.wheel.y < 0){
                canvas.scaleDown(canvasScaleCoef);
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
                popup->markDone(); // Mark popup for close
            break;
            case SDLK_RETURN:
              if(popup)
                popup->confirm(); // Execute popup action
            break;
            case SDLK_LSHIFT: case SDLK_RSHIFT:
              keys.shift = true;
              int currentMouseX, currentMouseY;
              SDL_GetMouseState(&currentMouseX,&currentMouseY);
              if(Collision::between({currentMouseX, currentMouseY}, editorView)){ // Record comera position for calculating distance to move
                cameraMoveLastPos.x = currentMouseX;
                cameraMoveLastPos.y = currentMouseY;
              }
            break;
          }
        break;
        case SDL_KEYUP:
          switch(e.key.keysym.sym){
            case SDLK_LSHIFT: case SDLK_RSHIFT:
              keys.shift = false;
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
      tileMenu.generate(idToTextureName,colors);
      regenerateMenu = false;
      tilesLoad = true;
    }
    if(popup && popup->isDone()){
      popup.reset();
    }
    mainWindow->clear();
    SDL_Color prev = mainWindow->getColor();
    mainWindow->setColor(colors.menuBackground);
    mainWindow->fillRect(menuView);
    mainWindow->setColor(colors.menuBorder);
    mainWindow->drawRect(menuView);
    mainWindow->setColor(colors.editorBackground);
    mainWindow->fillRect(editorView);
    mainWindow->setColor(prev);
    tileMenu.render();
    mainWindow->setViewport(&editorView);
    canvas.render(editorCamera);
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
