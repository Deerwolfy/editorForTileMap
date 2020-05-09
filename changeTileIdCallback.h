#ifndef CHANGETILEIDCALLBACK_H
#define CHANGETILEIDCALLBACK_H

#include<map>
#include<vector>
#include"guiElement.h"
#include"windowWrapper.h"
#include"texture.h"
#include"textureName.h"
#include"popupInputBox.h"
#include"tileCanvas.h"
#include"tile.h"

class ChangeTileIdCallback {
public:
  ChangeTileIdCallback(std::shared_ptr<WindowWrapper> window,std::map<int,TextureName> &idToTextureName, TileCanvas &canvas):
    window(window), idToTextureName(idToTextureName), canvas(canvas) { }
  void operator()(const PopupInputBox&);
private:
  std::shared_ptr<WindowWrapper> window;
  TileCanvas &canvas;
  std::map<int,TextureName> &idToTextureName;
};

#endif